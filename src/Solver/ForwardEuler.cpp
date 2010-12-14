// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include <iomanip>

#include "Common/CBuilder.hpp"
#include "Common/OptionArray.hpp"
#include "Common/ComponentPredicates.hpp"
#include "Common/Log.hpp"
#include "Common/Foreach.hpp"

#include "Solver/ForwardEuler.hpp"
#include "Solver/CDiscretization.hpp"

#include "Mesh/CMesh.hpp"
#include "Mesh/CRegion.hpp"
#include "Mesh/CField.hpp"
#include "Mesh/CTable.hpp"

#include "Actions/CLoop.hpp"
#include "Actions/CForAllNodes.hpp"

namespace CF {
namespace Solver {

using namespace Common;
using namespace Mesh;
using namespace Actions;

Common::ComponentBuilder < ForwardEuler, CIterativeSolver, LibSolver > ForwardEuler_Builder;

////////////////////////////////////////////////////////////////////////////////

ForwardEuler::ForwardEuler ( const std::string& name  ) :
  CIterativeSolver ( name )
{
  properties()["brief"] = std::string("Iterative Solver component");
  properties()["description"] = std::string("Forward Euler Time Stepper");
  
  m_properties["Domain"].as_option().attach_trigger ( boost::bind ( &ForwardEuler::trigger_Domain,   this ) );
  
  
  this->regist_signal ( "solve" , "Solve", "Solve" )->connect ( boost::bind ( &ForwardEuler::solve, this ) );
  // signal("solve").signature
  //     .insert<URI>("Domain", "Domain to load mesh into" )
  //     .insert_array<URI>( "Files" , "Files to read" );
  
  m_take_step = allocate_component<CForAllNodes>("take_step");
  m_take_step->create_action("CF.Solver.CTakeStep");  
  add_static_component(m_take_step);
  
  m_solution_field = allocate_component<CLink>("solution_field");
  add_static_component(m_solution_field);
  
  m_residual_field = allocate_component<CLink>("residual_field");
  add_static_component(m_residual_field);
  
  m_update_coeff_field = allocate_component<CLink>("update_coeff_field");
  add_static_component(m_update_coeff_field);
}

////////////////////////////////////////////////////////////////////////////////

ForwardEuler::~ForwardEuler()
{
}

////////////////////////////////////////////////////////////////////////////////

void ForwardEuler::trigger_Domain()
{
  URI domain; property("Domain").put_value(domain);
  
  const CMesh::Ptr& mesh = find_component_ptr_recursively<CMesh>(*look_component(domain));
  if (is_not_null(mesh))
  {
    std::vector<URI> volume_regions;
    boost_foreach( const CRegion& region, find_components_recursively_with_name<CRegion>(*mesh,"fluid"))
      volume_regions.push_back(URI(region.full_path()));

    std::vector<URI> all_regions;
    boost_foreach( const CRegion& region, find_components<CRegion>(*mesh))
      all_regions.push_back(URI(region.full_path()));
    
    m_take_step->configure_property( "Regions" , volume_regions );
    discretization_method().configure_property( "Regions" , volume_regions );
  }
  else
  {
    CFinfo << "domain has no mesh " << CFendl;
    return;
  }
  CFinfo << "domain has mesh" << CFendl;
}
//////////////////////////////////////////////////////////////////////////////

CDiscretization& ForwardEuler::discretization_method()
{
  return find_component<CDiscretization>(*this);
}

//////////////////////////////////////////////////////////////////////////////

void ForwardEuler::solve()
{
    CFinfo << "Setting up links" << CFendl;
    CMesh& mesh = find_component_recursively<CMesh>(*Core::instance().root());
    
    CField::Ptr solution = find_component_ptr_with_name<CField>(mesh,"solution");
    if ( is_null(solution) )
      solution = mesh.create_field("solution",1,CField::NODE_BASED).as_type<CField>();
    m_solution_field->link_to(solution);

    CField::Ptr residual = find_component_ptr_with_name<CField>(mesh,"residual");
    if ( is_null(residual) )
      residual = mesh.create_field("residual",1,CField::NODE_BASED).as_type<CField>();
    m_residual_field->link_to(residual);

    CField::Ptr inverse_updatecoeff = find_component_ptr_with_name<CField>(mesh,"inverse_updatecoeff");
    if ( is_null(inverse_updatecoeff) )
      inverse_updatecoeff = mesh.create_field("inverse_updatecoeff",1,CField::NODE_BASED).as_type<CField>();
    m_update_coeff_field->link_to(inverse_updatecoeff);


    // initial condition
    boost_foreach (CTable<Real>& node_data, find_components_recursively_with_tag<CTable<Real> >(*m_solution_field->get(), "node_data"))
    {
      CFLogVar(node_data.size());
       for (Uint i=0; i<node_data.size(); ++i)
    			node_data[i][0]=0;
    } 

    CFinfo << "Starting Iterative loop" << CFendl;
    for ( Uint iter = 0; iter < m_nb_iter;  ++iter)
    {
      CFinfo << "reset data" << CFendl;
      // update coefficient and residual to zero
      // Set the field data of the source field
      boost_foreach (CTable<Real>& node_data, find_components_recursively_with_tag<CTable<Real> >(*m_residual_field->get(), "node_data"))
        for (Uint i=0; i<node_data.size(); ++i)
    			node_data[i][0]=0;
      boost_foreach (CTable<Real>& node_data, find_components_recursively_with_tag<CTable<Real> >(*m_update_coeff_field->get(),"node_data"))
        for (Uint i=0; i<node_data.size(); ++i)
    			node_data[i][0]=0;
        
        CFinfo << "compute rhs" << CFendl;
      // compute RHS
      discretization_method().compute_rhs();

      CFinfo << "time march" << CFendl;
      // explicit update
      //m_take_step->execute();

      CFinfo << "norm compute" << CFendl;
      // compute norm
      Real rhs_L2=0;
      Uint dof=0;
      boost_foreach (CTable<Real>& node_data, find_components_recursively_with_tag<CTable<Real> >(*m_residual_field->get(),"node_data"))
      {    
        for (Uint i=0; i<node_data.size(); ++i)
    		{
          rhs_L2 += node_data[i][0]*node_data[i][0];
          dof++;
    		}
      }
      rhs_L2 = sqrt(rhs_L2)/dof;
      
      // output convergence info
      CFinfo << "Iter [" << std::setw(4) << iter << "] L2(rhs) [" << std::setw(12) << rhs_L2 << "]" << CFendl;
    }
}

////////////////////////////////////////////////////////////////////////////////

} // Solver
} // CF
