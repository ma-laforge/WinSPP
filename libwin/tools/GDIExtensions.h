/*Extensions/object wrappers for the old Windows GDI C-interface.

NOTE:
   Please deprecate.  One should really use the GDI+ "Metafile" class instead.
*/

#ifndef GDIEXTENSIONS_H
#define GDIEXTENSIONS_H

#include <tools/MathExtensions.h>
#include <windows.h>
#include <string>
#include <vector>

namespace GDIExt
{
/******************************************************************************/
/*                          Constants, typedefs, etc.                         */
/******************************************************************************/
   using std::string;
   typedef StdExt::Math::Point2D<double> Point; //An (x,y) point in 2D space
   typedef StdExt::Math::Rect<double> Rect; //A rectangle (in 2D space)
   typedef std::vector<Point> PointVector; //A vector of "Point"s

/******************************************************************************/
/*                                   Classes                                  */
/******************************************************************************/
/*----------------------------Pen Class Declaration---------------------------*/
   class Pen
   {
   public:
      DWORD style;
      double width; //in mm
      UINT bStyle; //Brush style
      COLORREF bColor; //Brush style
      LONG hatch; //Hatch

      Pen(DWORD style=PS_SOLID|PS_GEOMETRIC|PS_ENDCAP_ROUND, double width=0.5,
         UINT bStyle=PS_SOLID, COLORREF bColor=0, LONG hatch=HS_BDIAGONAL):
         style(style), width(width), bStyle(bStyle), bColor(bColor),
         hatch(hatch) {};
   }; //class Pen

/*---------------------------Brush Class Declaration--------------------------*/
   class Brush
   {
   public:
      UINT lbStyle;
      COLORREF lbColor;
      LONG lbHatch;

      Brush(UINT lbStyle=BS_NULL, COLORREF lbColor=RGB(255,255,255),
         LONG lbHatch=HS_BDIAGONAL):lbStyle(lbStyle), lbColor(lbColor),
         lbHatch(lbHatch) {};
   }; //class Brush
/*---------------------------Font Class Declaration---------------------------*/
   class Font
   {
   public:
      enum Styles {Normal=0x0, Bold=0x1, Italic=0x2, Underline=0x4, StrikeOut=0x8};

      string name;
      double ptHeight, ptWidth; //Height, width in pts
      Styles style;
      double angle; //Text angle in degrees
      BYTE charSet;

      Font(const string &name="Times New Roman", double ptHeight=12,
         Styles style=Normal, double angle=0, double ptWidth=0,
         BYTE charSet=ANSI_CHARSET): name(name), ptHeight(ptHeight),
         style(style), angle(angle), ptWidth(ptWidth), charSet(charSet) {};
   }; //class Font

/*------------------------EnhMetafile Class Declaration-----------------------*/
   class EnhMetafile
   {
   private:
      HENHMETAFILE hEMF; //Handle to the enhanced metafile
      HDC hCanvas; //Handle to the metafile's device context
      Rect bounds; //Specifies the physical dimensions of the metafile (in mm)
      string description; //Describes the metafile
      Pen pen; HPEN hPen;
      Font font; HFONT hFont;
      Brush brush; HBRUSH hBrush;

      //Variables used to map between user coordinates and those of the metafile:
      double scaleX, scaleY, offsetX, offsetY;

      EnhMetafile(); //Undefined - Must explicitly supply parameters
      EnhMetafile(const EnhMetafile& src); //Prevent copying
      EnhMetafile &operator=(const EnhMetafile &src); //Prevent copying

      void Create(HDC hDCReference);
      RECT User2Phy(const Rect &src) const;
      POINT User2Phy(const Point &src) const;

   public:
      EnhMetafile(const Rect &bounds, const string &description="", HDC hDCReference=0);
      ~EnhMetafile();

      //SetUserCoordSystem - Defines the user coordinate system to be used for drawing.
      //   Inputs:
      //   -origin     Locates the origin of the coordinate system w.r.t. the
      //               bottom-left corner of the metafile (in mm).
      //   -extentsX   The width of the metafile in user coordinates.
      //   -extentsY   The height of the metafile in user coordinates.
      void SetUserCoordSystem(const Point &origin, double extentsX, double extentsY);

      void SelectPen(const Pen &pen);
      void SelectBrush(const Brush &brush);
      void SelectFont(const Font &font);

      void MoveTo(const Point &pt);
      void LineTo(const Point &pt);
      void Polyline(const PointVector &ptV); //Makes a single, continuous line (not Polyline from GDI)
      void Arc(const Rect &bounds, const Point &start, const Point &end);
      void Ellipse(const Rect &bounds);
      void Rectangle(const Rect &bounds);
      void Polygon(const PointVector &ptV);
      void DrawText(const Rect &bounds, const string &text, UINT uFormat);
      void TextOut(const Point &pt, const string &text);

      //Mathematical representations:
      void Circle(const Point &c, double r); //Form: (x-c.x)^2+(y-c.y)^2=r^2

      //Applies to DrawText:
      COLORREF SetBkColor(COLORREF crColor);
      UINT SetBkMode(UINT iBkMode);
      UINT SetTextAlign(UINT fMode);

      void Lock();
      void Unlock(HDC hDCReference=0);

      void Draw(HDC hDC, const Rect &bounds) const;
      void Save(const string &filePath) const;

      //<< Cannot be used to write to a .emf file (use save instead).
      //Underlying GetEnhMetaFileBits outputs to different bit boundaries.
      friend std::ostream& operator<<(std::ostream& os, const EnhMetafile &m);
   }; //class Plot

/******************************************************************************/
/*                                  Functions                                 */
/******************************************************************************/
   HFONT CreateFont(HDC hDC, const Font &font);
} //namespace GDIExt
#endif //#ifndef GDIEXTENSIONS_H

