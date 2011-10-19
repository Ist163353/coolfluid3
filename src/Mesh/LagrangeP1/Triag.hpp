// Copyright (C) 2010-2011 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef cf3_Mesh_LagrangeP1_Triag_hpp
#define cf3_Mesh_LagrangeP1_Triag_hpp

#include "Mesh/ShapeFunctionBase.hpp"
#include "Mesh/LagrangeP1/API.hpp"

namespace cf3 {
namespace Mesh {
namespace LagrangeP1 {

////////////////////////////////////////////////////////////////////////////////

struct Mesh_LagrangeP1_API Triag_traits
{
  enum { nb_nodes       = 3               };
  enum { dimensionality = 2               };
  enum { order          = 1               };
  enum { shape          = GeoShape::TRIAG };
};

////////////////////////////////////////////////////////////////////////////////

/// @class Triag
/// @verbatim
/// Local connectivity:
///             2
///             | .
///             |   .
///             |     .
///             |       .
///             |         .
///             0-----------1
/// Reference domain: <0,1> x <0,1>
/// @endverbatim
/// @see ShapeFunction for documentation on undocumented static functions
struct Mesh_LagrangeP1_API Triag : public ShapeFunctionBase<Triag,Triag_traits>
{
  static const RealMatrix& local_coordinates();
  static void compute_value(const MappedCoordsT& mapped_coord, ValueT& result);
  static void compute_gradient(const MappedCoordsT& mapped_coord, GradientT& result);
};


////////////////////////////////////////////////////////////////////////////////

} // LagrangeP1
} // Mesh
} // cf3

#endif // CF3_Mesh_LagrangeP1_Triag_hpp
