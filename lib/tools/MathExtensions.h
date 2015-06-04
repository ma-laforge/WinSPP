#ifndef MATHEXTENSIONS_H
#define MATHEXTENSIONS_H

#include <cmath>
#include <complex>

//The StdExt namespace contains various tools that extend the standard library:
namespace StdExt
{
namespace Math
{
/******************************************************************************/
/*                          Constants, typedefs, etc.                         */
/******************************************************************************/
   const double PI     = 3.1415926535897932384626433832795;
   const double TWO_PI = 6.283185307179586476925286766559;

/******************************************************************************/
/*                              Template Classes                              */
/******************************************************************************/
//#define T double //To debug templates
   template <class T>
   class Point2D
   {
   public:
      T x, y; //Coordinates of a point

      Point2D(): x(0), y(0) {} //Point (0,0)
      Point2D(T x, T y): x(x), y(y) {}

      Point2D<T> operator+(const Point2D<T> &p2) {return Point2D<T>(x+p2.x, y+p2.y);}
   };
//#undef T //If debugging templates

//#define T double //To debug templates
   template <class T>
   class Rect
   {
   public:
      Point2D<T> p1, p2; //Points defining the extents of the rectangle

      Rect(): p1(0,0), p2(1,1) {} //Default rectangle
      Rect(const Point2D<T> &p1, const Point2D<T> &p2): p1(p1), p2(p2) {}
      Rect(T width, T height): p1(0, 0), p2(width, height) {}
      Rect(const Point2D<T> &c, T width, T height): p1(c.x-width/2, c.y+height/2), p2(c.x+width/2, c.y-height/2) {}
      Rect(T left, T top, T right, T bottom): p1(left, top), p2(right, bottom) {}
   };
//#undef T //If debugging templates

//#define T double //To debug templates
   template <class T>
   class Line2D
   {
   public:
      T m, b; //Parameters of line equation: y=mx+b

      Line2D(): m(1), b(0) {} //line x=y
      Line2D(T m, T b): m(m), b(b) {}
      Line2D(T x1, T y1, T x2, T y2)
      {
         m = (y2-y1)/(x2-x1);
         b = y1-m*x1;
         //JUSTIFICATION:
         //If we substitute point (x1,y1) in y=mx+b, we get: y1=m*x1+b.  Thus,
         //b=y1-m*x1
      }

      inline T getX(T y) const {return (y-b)/m;}
      inline T getY(T x) const {return m*x+b;}
   };
//#undef T //If debugging templates

/******************************************************************************/
/*                                  Functions                                 */
/******************************************************************************/
   template <class T> inline std::complex<T> ln(std::complex<T> z) {return log(z);}
   inline double ln(double x) {return log(x);}

   int Round(double x); //Returns x rounded to the nearest integer

   inline double Width(const Rect<double> &rect)
      {return std::abs(rect.p1.x-rect.p2.x);}

   inline double Height(const Rect<double> &rect)
      {return std::abs(rect.p1.y-rect.p2.y);}
} //namespace Math
} //namespace StdExt

/*------------------------------Global Namespace------------------------------*/
//NOTE:
//   The following functions are found in the global namespace in order to match
//   the location of the others provided in C/C++.

   double Acosh(double x); //Returns the hyperbolic arccosine of x
   double Asinh(double x); //Returns the hyperbolic arcsine of x

/******************************************************************************/
/*                   Complex Arithmetic (Function Templates)                  */
/******************************************************************************/
   //Modulus() - Returns the modulus of z:
   template <class T> T Modulus(std::complex<T> z)
      {return std::abs(z);}

   //Modulus2() - Returns the modulus^2 of z:
   template <class T> T Modulus2(std::complex<T> z)
      {return real(z*conj(z));}

   //Acos() - Returns the arccosine of z:
   template <class T> std::complex<T> Acos(std::complex<T> z)
      {const std::complex<T> i(0,1); return -i*StdExt::Math::ln(z+i*sqrt(1.0-z*z));}

   //Acosh() - Returns the hyperbolic arccosine of z:
   template <class T> std::complex<T> Acosh(std::complex<T> z)
      {const std::complex<T> one(1,0); return StdExt::Math::ln(z+sqrt(z*z-one));}

   //Asinh() - Returns the hyperbolic arccosine of z:
   template <class T> std::complex<T> Asinh(std::complex<T> z)
      {return StdExt::Math::ln(z+sqrt(z*z+1.0));}
#endif //#ifndef MATHEXTENSIONS_H

