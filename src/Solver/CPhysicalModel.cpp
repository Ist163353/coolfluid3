// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

////////////////////////////////////////////////////////////////////////////////

#include "Common/OptionT.hpp"
#include "Common/CBuilder.hpp"

#include "CPhysicalModel.hpp"

using namespace CF::Common;

namespace CF {
namespace Solver {

Common::ComponentBuilder < CPhysicalModel, Component, LibSolver > CPhysicalModel_Builder;
  
////////////////////////////////////////////////////////////////////////////////

CPhysicalModel::CPhysicalModel(const std::string& name) : Component(name),
  m_type("null"),
  m_dim(0u),
  m_nbdofs(0u)
{
  // options

  /// @todo later this will be removed when the physical model stops beign so generic

  m_properties.add_option<OptionT <std::string> >("Type",
                                           "Type of the physical model (serves to identify the model)",
                                           "null")
      ->mark_basic()
      ->link_to(&m_type);

  m_properties.add_option<OptionT <Uint> >("Dimensions",
                                           "Dimensionality of the problem, i.e. the number of components for the spatial coordinates",
                                           0u)
      ->mark_basic()
      ->link_to(&m_dim);

  m_properties.add_option<OptionT <Uint> >("DOFs",
                                           "Degrees of freedom",
                                           0u)
      ->mark_basic()
      ->link_to(&m_nbdofs);
}

CPhysicalModel::~CPhysicalModel()
{
}

////////////////////////////////////////////////////////////////////////////////

} // Solver
} // CF
