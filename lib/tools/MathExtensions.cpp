#include <tools/MathExtensions.h>

//The StdExt namespace contains various tools that extend the standard library:
namespace StdExt
{
namespace Math
{
/******************************************************************************/
/*                          Constants, typedefs, etc.                         */
/******************************************************************************/

/******************************************************************************/
/*                                  Functions                                 */
/******************************************************************************/
   //Round() - Returns x rounded to the nearest integer
   int Round(double x)
   {
      if (x>=0)
         return int(x+.5);
      else
         return int(x-.5);
      //NOTE:
      //   C++ performs double-to-integer conversions by TRUNCATING the
      //   fractionnal portion of the double.  By adding/subtracting .5, you
      //   force it to round numbers up/down to the nearest integer value.
   } //int Round(double x)
} //namespace Math
} //namespace StdExt

/*------------------------------Global Namespace------------------------------*/
   //Acosh() - Returns the hyperbolic arccosine of x:
   double Acosh(double x)
      {return StdExt::Math::ln(x+sqrt(x*x-1));}

   //Asinh() - Returns the hyperbolic arcsine of x:
   double Asinh(double x)
      {return StdExt::Math::ln(x+sqrt(x*x+1));}

