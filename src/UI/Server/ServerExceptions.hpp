// Copyright (C) 2010-2011 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef cf3_GUI_Server_ServerExceptions_hpp
#define cf3_GUI_Server_ServerExceptions_hpp

////////////////////////////////////////////////////////////////////////////////

#include "Common/Exception.hpp"

//////////////////////////////////////////////////////////////////////////////

namespace cf3 {
namespace UI {
namespace Server {

//////////////////////////////////////////////////////////////////////////////

/// @brief Exception thrown when a given client id could not be associated to
/// any existing client.

/// @author Quentin Gasper.
class UnknownClientId : public cf3::common::Exception
{
public:

    /// Constructor
    UnknownClientId(const cf3::common::CodeLocation& where,
                             const std::string& what);

    virtual ~UnknownClientId() throw ();

}; // class UnknownClientId

//////////////////////////////////////////////////////////////////////////////

/// @brief Exception thrown when the server can not open its socket.

/// @author Quentin Gasper.

class NetworkError : public cf3::common::Exception
{
  public:

  /// Constructor
  NetworkError(const cf3::common::CodeLocation& where, const std::string& what);

  /// Copy constructor
  virtual ~NetworkError() throw ();

}; // class NetworkError

//////////////////////////////////////////////////////////////////////////////

} // Server
} // UI
} // cf3

//////////////////////////////////////////////////////////////////////////////

#endif // CF3_GUI_Server_ServerExceptions_hpp
