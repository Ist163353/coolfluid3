// Copyright (C) 2010-2011 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include "Common/RegistLibrary.hpp"

#include "UI/UICommon/LibUICommon.hpp"

namespace cf3 {
namespace UI {
namespace UICommon {

cf3::common::RegistLibrary<LibUICommon> LibUICommon;

////////////////////////////////////////////////////////////////////////////////

void LibUICommon::initiate_impl()
{
}

void LibUICommon::terminate_impl()
{
}

////////////////////////////////////////////////////////////////////////////////

} // Network
} // UI
} // cf3
