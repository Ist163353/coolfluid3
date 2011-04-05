// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef CF_RDM_SysLDA_hpp
#define CF_RDM_SysLDA_hpp

#include "RDM/DomainTerm.hpp"

/////////////////////////////////////////////////////////////////////////////////////

namespace CF {
namespace RDM {

/////////////////////////////////////////////////////////////////////////////////////

class RDM_API SysLDA : public RDM::DomainTerm {

  template < typename PHYS > struct ElementLoop;
  template < typename PHYS > struct ElementLoopSys;

public: // typedefs

  typedef boost::shared_ptr< SysLDA > Ptr;
  typedef boost::shared_ptr< SysLDA const > ConstPtr;

public: // functions

  /// Contructor
  /// @param name of the component
  SysLDA ( const std::string& name );

  /// Virtual destructor
  virtual ~SysLDA();

  /// Get the class name
  static std::string type_name () { return "SysLDA"; }

  /// Execute the loop for all elements
  virtual void execute();

};

/////////////////////////////////////////////////////////////////////////////////////

} // RDM
} // CF

/////////////////////////////////////////////////////////////////////////////////////

#endif // CF_Mesh_SysLDA_hpp