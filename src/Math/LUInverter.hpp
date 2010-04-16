#ifndef CF_Math_LUInverter_hh
#define CF_Math_LUInverter_hh

////////////////////////////////////////////////////////////////////////////////

#include <valarray>

#include "Math/RealVector.hpp"
#include "Math/MatrixInverter.hpp"
#include "Math/MathChecks.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace CF {

  namespace Math {

////////////////////////////////////////////////////////////////////////////////

  /// This class represents a solver that uses the LU decomposition
  /// @author Andrea Lani
  /// @author Tiago Quintino
class Math_API LUInverter : public MatrixInverter {
public:

  /// Default constructor without arguments
  LUInverter(const Uint& n);

  /// Default destructor
  ~LUInverter();

  /// Invert the given matrix a and put the result in x
  void invert(const RealMatrix& a, RealMatrix& x);

private: //helper functions

  /// Factorize the matrix
  void factLU();

  /// Solve with forward and backward substitution
  void solveForwBack();

private: //data

  Uint                _n;

  std::valarray<Uint> _indx;

  RealVector             _tempcol;

  RealVector             _vv;

  RealMatrix             _a;

}; // end of class LUInverter

////////////////////////////////////////////////////////////////////////////////

  } // namespace Math

} // namespace CF

////////////////////////////////////////////////////////////////////////////////

#endif // CF_Math_LUInverter_hh
