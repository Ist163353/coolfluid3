// Copyright (C) 2010 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.


#include "RDM/RotationAdv2D.hpp"

/////////////////////////////////////////////////////////////////////////////////////

namespace CF {
namespace RDM {

///////////////////////////////////////////////////////////////////////////////////////

RotationAdv2D::RotationAdv2D()
{
}

/////////////////////////////////////////////////////////////////////////////////////

RotationAdv2D::~RotationAdv2D()
{
}

/////////////////////////////////////////////////////////////////////////////////////

Real RotationAdv2D::Lu(const RealVector2& point, const RealVector2& grad, const Real& u )
{
   return point[YY]*grad[XX] - point[XX]*grad[YY];
}


} // RDM
} // CF

/////////////////////////////////////////////////////////////////////////////////////
