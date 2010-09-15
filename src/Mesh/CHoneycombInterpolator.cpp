// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include <boost/foreach.hpp>
#include <boost/algorithm/string/erase.hpp>
#include "Common/ObjectProvider.hpp"
#include "Common/ComponentPredicates.hpp"
#include "Common/PropertyT.hpp"
#include "Common/PropertyArray.hpp"


#include "Mesh/CHoneycombInterpolator.hpp"
#include "Mesh/CMesh.hpp"
#include "Mesh/CArray.hpp"
#include "Mesh/CTable.hpp"
#include "Mesh/CRegion.hpp"
#include "Mesh/CElements.hpp"

//////////////////////////////////////////////////////////////////////////////

namespace CF {
namespace Mesh {

  using namespace Common;
  
////////////////////////////////////////////////////////////////////////////////

CF::Common::ObjectProvider < Mesh::CHoneycombInterpolator,
                             Mesh::CInterpolator,
                             MeshLib,
                             NB_ARGS_1 >
aHoneyCombInterpolatorProvider ( "Honeycomb" );

//////////////////////////////////////////////////////////////////////////////

CHoneycombInterpolator::CHoneycombInterpolator( const CName& name )
  : CInterpolator(name), m_dim(0), m_ranges(3), m_N(3), m_D(3), m_comb_idx(3), m_sufficient_nb_points(0)
{
  BUILD_COMPONENT;
}
  
/////////////////////////////////////////////////////////////////////////////

void CHoneycombInterpolator::defineConfigProperties ( CF::Common::PropertyList& options )
{
  options.add_option< PropertyT<Uint> >
  ( "ApproximateNbElementsPerCell",
    "The approximate amount of elements that are stored in a structured" ,
    1 );
  
  std::vector<Uint> dummy;
  options.add_option< PropertyArrayT<Uint> >
  ( "Divisions",
    "The number of divisions in each direction of the comb. Takes precedence over \"ApproximateNbElementsPerCell\". " ,
    dummy);   
}  

//////////////////////////////////////////////////////////////////////////////

void CHoneycombInterpolator::construct_internal_storage(const CMesh::Ptr& source, const CMesh::Ptr& target)
{
  if (m_source_mesh != source)
  {
    m_source_mesh = source;
    create_honeycomb();
  }
  m_target_mesh = target;
  
  find_comb_idx(RealVector(5.5,3));
  find_pointcloud();

}
  
/////////////////////////////////////////////////////////////////////////////
  
void CHoneycombInterpolator::interpolate_field_from_to(const CField& source, CField& target)
{  
}
  
//////////////////////////////////////////////////////////////////////

void CHoneycombInterpolator::create_honeycomb()
{
  m_dim=0;
  std::set<const CArray*> all_coordinates;
  BOOST_FOREACH(CElements& elements, recursive_range_typed<CElements>(*m_source_mesh))
  { 
    m_dim = std::max(elements.element_type().dimensionality() , m_dim);
    all_coordinates.insert(&elements.coordinates());
  }
  
  std::vector<Real> L(3);
  for (Uint d=0; d<m_dim; ++d)
  {
    m_ranges[d].resize(2,0.0);
  }
      
  Real V=1;
  BOOST_FOREACH(const CArray* coordinates , all_coordinates)
    BOOST_FOREACH(const CArray::ConstRow& node, coordinates->array())
      for (Uint d=0; d<m_dim; ++d)
      {
        m_ranges[d][0] = std::min(m_ranges[d][0],  node[d]);
        m_ranges[d][1] = std::max(m_ranges[d][1],  node[d]);
      }
  for (Uint d=0; d<m_dim; ++d)
  {
    L[d] = m_ranges[d][1] - m_ranges[d][0];
    V*=L[d];
  }
  
  m_nb_elems = get_component_typed<CRegion>(*m_source_mesh).recursive_filtered_elements_count(IsElementsVolume());
  
  
  if (property("Divisions")->value<std::vector<Uint> >().size() > 0)
  {
    m_N = property("Divisions")->value<std::vector<Uint> >();
    for (Uint d=0; d<m_dim; ++d)
      m_D[d] = (L[d])/static_cast<Real>(m_N[d]);
  }
  else
  {
    Real V1 = V/m_nb_elems;
    Real D1 = std::pow(V1,1./m_dim)*property("ApproximateNbElementsPerCell")->value<Uint>();
    
    for (Uint d=0; d<m_dim; ++d)
    {
      m_N[d] = (Uint) std::ceil(L[d]/D1);
      m_D[d] = (L[d])/static_cast<Real>(m_N[d]);
    }
  }
  

  
  for (Uint d=0; d<m_dim; ++d)
  {
    CFinfo << "range["<<d<<"] :   L = " << L[d] << "    N = " << m_N[d] << "    D = " << m_D[d] << CFendl;
  }
  CFinfo << "V = " << V << CFendl;
  
  // initialize the honeycomb
  m_honeycomb.resize(boost::extents[std::max(Uint(1),m_N[0])][std::max(Uint(1),m_N[1])][std::max(Uint(1),m_N[2])]);
  
  
  Uint total_nb_elems=0;
  BOOST_FOREACH(const CElements& elements, recursive_filtered_range_typed<CElements>(*m_source_mesh,IsElementsVolume()))
  {
    const CArray& coordinates = elements.coordinates();
    Uint nb_nodes_per_element = elements.connectivity_table().array().shape()[1];
    Uint elem_idx=0;
    BOOST_FOREACH(const CTable::ConstRow& elem, elements.connectivity_table().array())
    {
      RealVector centroid(0.0,m_dim);
      BOOST_FOREACH(const Uint node_idx, elem)
        centroid += RealVector(coordinates[node_idx]);        

      centroid /= static_cast<Real>(nb_nodes_per_element);
      for (Uint d=0; d<m_dim; ++d)
        m_comb_idx[d]=std::min((Uint) std::floor( (centroid[d] - m_ranges[d][0])/m_D[d]), m_N[d]-1 );
      m_honeycomb[m_comb_idx[0]][m_comb_idx[1]][m_comb_idx[2]].push_back(std::make_pair<const CElements*,Uint>(&elements,elem_idx));
      elem_idx++;
    }
    total_nb_elems += elem_idx;
  }
  
  
  Uint total=0;
 
  switch (m_dim)
  {
    case DIM_2D:
      for (Uint i=0; i<m_N[0]; ++i)
        for (Uint j=0; j<m_N[1]; ++j)
        {
          Uint k=0;
          CFinfo << "("<<i<<","<<j<<") has " << m_honeycomb[i][j][k].size() << " elems" << CFendl;
          total += m_honeycomb[i][j][k].size();
        }
      break;
    case DIM_3D:
      for (Uint i=0; i<m_N[0]; ++i)
        for (Uint j=0; j<m_N[1]; ++j)
          for (Uint k=0; k<m_N[2]; ++k)
          {
            CFinfo << "("<<i<<","<<j<<","<<k<<") has " << m_honeycomb[i][j][k].size() << " elems" << CFendl;
            total += m_honeycomb[i][j][k].size();
          }
      break;
    default:
      break;
  }

  CFinfo << "total = " << total << " of " << m_nb_elems << CFendl;
  
  m_sufficient_nb_points = static_cast<Uint>(std::pow(3.,(int)m_dim));

}


//////////////////////////////////////////////////////////////////////////////
  
void CHoneycombInterpolator::find_comb_idx(const RealVector& coordinate)
{
  CFinfo << "point " << coordinate << CFflush;
  cf_assert(coordinate.size() == m_dim);
  
  for (Uint d=0; d<m_dim; ++d)
    m_comb_idx[d] = std::min((Uint) std::floor( (coordinate[d] - m_ranges[d][0])/m_D[d]), m_N[d]-1 );
  
  CFinfo << " should be in box ("<<m_comb_idx[0]<<","<<m_comb_idx[1]<<","<<m_comb_idx[2]<<")" << CFendl;

}
  
//////////////////////////////////////////////////////////////////////////////

void CHoneycombInterpolator::find_pointcloud()
{
  m_pointcloud.resize(0);
  Uint r=1;
  int i(0), j(0), k(0);
  int imin, jmin, kmin;
  int imax, jmax, kmax;
  
  if (m_honeycomb[m_comb_idx[0]][m_comb_idx[1]][m_comb_idx[2]].size() <= m_sufficient_nb_points)
    r=0;
  
  while (m_pointcloud.size() < m_sufficient_nb_points && m_pointcloud.size() < m_nb_elems)
  {
    ++r;
    m_pointcloud.resize(0);

    switch (m_dim)
    {
      case DIM_3D:
        imin = std::max(int(m_comb_idx[0])-int(r), 0);  imax = std::min(int(m_comb_idx[0])+int(r),int(m_N[0])-1);
        jmin = std::max(int(m_comb_idx[1])-int(r), 0);  jmax = std::min(int(m_comb_idx[1])+int(r),int(m_N[1])-1);
        kmin = std::max(int(m_comb_idx[2])-int(r), 0);  kmax = std::min(int(m_comb_idx[2])+int(r),int(m_N[2])-1);
        for (i = imin; i <= imax; ++i)
          for (j = jmin; j <= jmax; ++j)
            for (k = kmin; k <= kmax; ++k)
            {
              BOOST_FOREACH(const Point& point, m_honeycomb[i][j][k])
              m_pointcloud.push_back(&point);
              CFinfo << "   ("<<i<<","<<j<<","<<k<<")" <<  CFendl;
            }
        break;
      case DIM_2D:
        imin = std::max(int(m_comb_idx[0])-int(r), 0);  imax = std::min(int(m_comb_idx[0])+int(r),int(m_N[0])-1);
        jmin = std::max(int(m_comb_idx[1])-int(r), 0);  jmax = std::min(int(m_comb_idx[1])+int(r),int(m_N[1])-1);
        for (i = imin; i <= imax; ++i)
          for (j = jmin; j <= jmax; ++j)
          {
            BOOST_FOREACH(const Point& point, m_honeycomb[i][j][k])
            m_pointcloud.push_back(&point);
            CFinfo << "   ("<<i<<","<<j<<","<<k<<")" <<  CFendl;
          }
        break;
      case DIM_1D:
        imin = std::max(int(m_comb_idx[0])-int(r), 0);  imax = std::min(int(m_comb_idx[0])+int(r),int(m_N[0])-1);
        for (i = imin; i <= imax; ++i)
        {
          BOOST_FOREACH(const Point& point, m_honeycomb[i][j][k])
          m_pointcloud.push_back(&point);
          CFinfo << "   ("<<i<<","<<j<<","<<k<<")" <<  CFendl;
        }
        break;
    }    
  }
    
  CFinfo << m_pointcloud.size() << " points in the pointcloud " << CFendl;
 
}
  
//////////////////////////////////////////////////////////////////////////////

} // Mesh
} // CF
