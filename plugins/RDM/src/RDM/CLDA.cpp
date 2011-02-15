// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include "RDM/CLDA.hpp"

using namespace CF::Common;

namespace CF {
namespace RDM {

Common::ComponentBuilder < CLDA, CAction, LibRDM > CLDA_Builder;

////////////////////////////////////////////////////////////////////////////////

CLDA::CLDA ( const std::string& name  ): CLoop(name)
{
  regist_typeinfo(this);
}

CLDA::~CLDA() {}

void CLDA::execute()
{
  // access the fields



  boost_foreach(Mesh::CRegion::Ptr& region, m_loop_regions)
  {
    CFinfo << region->full_path().string() << CFendl;

    CLDA::ElementLoop loop( *region );

    boost::mpl::for_each< RDM::CellTypes >( loop );
  }
}

//////////////////////////////////////////////////////////////////////////////

} // RDM
} // CF