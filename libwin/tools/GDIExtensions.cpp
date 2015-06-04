#include <tools/GDIExtensions.h>
#include <fstream> 

namespace GDIExt
{
/******************************************************************************/
/*                          Constants, typedefs, etc.                         */
/******************************************************************************/
   using StdExt::Math::Round;
   using StdExt::Math::Width;
   using StdExt::Math::Height;

/******************************************************************************/
/*                              Helper Functions                              */
/******************************************************************************/
   //GetDefaultPrinterDC: Code adapted to C++ from "Programming Windows 95" by
   //Charles Petzold, Microsoft Press, 1996.  ISBN: 1-55615-676-6.
   HDC GetDefaultPrinterDC()
   {
      char szPrinterInfo[256], *szPrinterName, *szDriver, *szOutput;
      HDC hDC;
      char *nextTkn = NULL;

      GetProfileString("windows", "device", ",,,", szPrinterInfo, 256);

      //Separate the comma separated expression into 3 strings:
      if ((szPrinterName = strtok_s(szPrinterInfo, ",", &nextTkn)) == 0 ||
          (szDriver = strtok_s(0, ",", &nextTkn)) == 0 ||
          (szOutput = strtok_s(0, ",", &nextTkn)) == 0)
         throw std::domain_error("No default printer available!");

      hDC = CreateDC(szDriver, szPrinterName, 0, 0);
      if (!hDC) throw std::domain_error("Could not get a handle to the default printer");
      return hDC;
   }


   //mm2HIMetric() - Converts the coordinates of a Rect structure from mm to
   //a RECT structure in units of 0.01mm (MM_HIMETRIC):
   RECT mm2HIMetric(const Rect& src)
   {
      RECT dest;
      dest.left = Round(src.p1.x*100);
      dest.top = Round(src.p1.y*100);
      dest.right = Round(src.p2.x*100);
      dest.bottom = Round(src.p2.y*100);
      return dest;
   } //RECT mm2HIMetric(const Rect& src)

   RECT GetRECT(const Rect& src)
   {
      RECT dest;
      dest.left = Round(src.p1.x);
      dest.top = Round(src.p1.y);
      dest.right = Round(src.p2.x);
      dest.bottom = Round(src.p2.y);
      return dest;
   } //RECT GetRECT(const Rect& src)

/******************************************************************************/
/*                               Main Functions                               */
/******************************************************************************/
   //CreateFont: Code adapted to C++ (and modified) from 'EzCreateFont' in
   //"Programming Windows 95" by Charles Petzold, Microsoft Press, 1996.
   //ISBN: 1-55615-676-6.
   HFONT CreateFont(HDC hDC, const Font &font)
   {
      const double MM_PER_PT = 25.4/72.0; //mm per point
      double lHeight, lWidth; //Logical height & width of font
      HFONT hFont; //Return value
      LOGFONT lf;

      SaveDC(hDC); //Will be modifying the device context temporarily

      SetMapMode(hDC, MM_HIMETRIC);

      lHeight = font.ptHeight*(100*MM_PER_PT); //in 0.01mm (MM_HIMETRIC)
      lWidth = font.ptWidth*(100*MM_PER_PT); //in 0.01mm (MM_HIMETRIC)

      lf.lfHeight = -Round(lHeight);
      lf.lfWidth = 0;
      lf.lfEscapement = -Round(font.angle*10);
      lf.lfOrientation = 0;
      lf.lfWeight = font.style & Font::Bold ? FW_BOLD : 0;
      lf.lfItalic = font.style & Font::Italic ? 1 : 0;
      lf.lfUnderline = font.style & Font::Underline ? 1 : 0;
      lf.lfStrikeOut = font.style & Font::StrikeOut ? 1 : 0;
      lf.lfCharSet = font.charSet;
      lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
      lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
      lf.lfQuality = DEFAULT_QUALITY;
      lf.lfPitchAndFamily = DEFAULT_PITCH;

      if (font.name.size() >= LF_FACESIZE) //Account for the terminating character
         throw std::invalid_argument("Font name must be less than 31 characters");

      strcpy_s(lf.lfFaceName, font.name.c_str());
      hFont = CreateFontIndirect(&lf);

      if (font.ptWidth >= 0.1) //Anything less than .1 is equivalent to 0.
      {
         TEXTMETRIC tm;
         //NOTE: A value of 0 automatically selects the default aspect ratio
         hFont = (HFONT)SelectObject(hDC, hFont);
         GetTextMetrics(hDC, &tm);
         DeleteObject(SelectObject(hDC, hFont));
         lf.lfWidth = Round(tm.tmAveCharWidth*lWidth/lHeight);
         hFont = CreateFontIndirect(&lf);
      }

      RestoreDC(hDC, -1); //Restore devic context to most recently saved state
      return hFont;
   }

/******************************************************************************/
/*                                Main Classes                                */
/******************************************************************************/
/*------------------------EnhMetafile Class Definition------------------------*/
   EnhMetafile::EnhMetafile(const Rect &bounds, const string &description,
      HDC hDCReference): hEMF(0), hCanvas(0), bounds(bounds), description(description),
      hPen(0), hFont(0)
   {
      Create(hDCReference);
      SetMapMode(hCanvas, MM_HIMETRIC);
      SelectPen(pen);
      SelectBrush(brush);
      SelectFont(font);
      SetBkMode(TRANSPARENT);
      SetUserCoordSystem(Point(0, 0), 1, 1);
   }

   //SetUserCoordSystem - Defines the user coordinate system to be used for drawing.
   //   Inputs:
   //   -origin     Locates the origin of the coordinate system w.r.t. the
   //               bottom-left corner of the metafile (in mm).
   //   -extentsX   The width of the metafile in user coordinates.
   //   -extentsY   The height of the metafile in user coordinates.
   void EnhMetafile::SetUserCoordSystem(const Point &origin, double extentsX,
      double extentsY)
   {
      double phWidth, phHeight; //Physical width & height of the metafile
      phWidth = Width(bounds)*100; //in 0.01mm (MM_HIMETRIC)
      phHeight = Height(bounds)*100; //in 0.01mm (MM_HIMETRIC)
      offsetX = origin.x*100; //in 0.01mm (MM_HIMETRIC)
      offsetY = origin.y*100 - phHeight; //in 0.01mm (MM_HIMETRIC)
      scaleX = phWidth/extentsX;
      scaleY = phHeight/extentsY;
      //If we let (x, y) be the physical coordinates of the metafile and
      //(x', y') be the user coordinates of the metafile, then:
      //   x = origin.x + phWidth
      //                  -------- * x'     and
      //                  extentsX
      //   y = (origin.y-phHeight) + phHeight
      //                             -------- * y'
      //                             extentsY
   } //EnhMetafile::SetUserCoordSystem(...)

   RECT EnhMetafile::User2Phy(const Rect &src) const
   {
      RECT dest;
      dest.left = LONG(src.p1.x*scaleX + offsetX);
      dest.top = LONG(src.p1.y*scaleY + offsetY);
      dest.right = LONG(src.p2.x*scaleX + offsetX);
      dest.bottom = LONG(src.p2.y*scaleY + offsetY);
      return dest;
   } //RECT EnhMetafile::User2Phy(const Rect &src) const

   POINT EnhMetafile::User2Phy(const Point &src) const
   {
      POINT dest;
      dest.x = LONG(src.x*scaleX + offsetX);
      dest.y = LONG(src.y*scaleY + offsetY);
      return dest;
   } //POINT EnhMetafile::User2Phy(const Point &src) const

   void EnhMetafile::Create(HDC hDCReference)
   {
      RECT rect = mm2HIMetric(bounds);
      bool useDefaultPrinter = !hDCReference;

      try //Try to get a handle to the default printer:
         {if (useDefaultPrinter) hDCReference = GetDefaultPrinterDC();}
      catch (...) //Indicate that it was not possible:
         {useDefaultPrinter = false;}

      hCanvas = CreateEnhMetaFile(hDCReference, 0, &rect, description.c_str());
      if (useDefaultPrinter) DeleteDC(hDCReference);
   } //void EnhMetafile::Create(HDC hDCReference)

   void EnhMetafile::MoveTo(const Point &pt)
   {
      POINT lastPos, phPt = User2Phy(pt);
      if (!hCanvas) throw std::domain_error("Cannot edit while locked.");

      MoveToEx(hCanvas, phPt.x, phPt.y, &lastPos);
   } //void EnhMetafile::MoveTo(const Point &pt)

   void EnhMetafile::LineTo(const Point &pt)
   {
      POINT phPt = User2Phy(pt);
      if (!hCanvas) throw std::domain_error("Cannot edit while locked.");

      ::LineTo(hCanvas, phPt.x, phPt.y);
   } //void EnhMetafile::LineTo(const Point &pt)

   void EnhMetafile::Polyline(const PointVector &ptV)
   {
      POINT lastPos, phPt = User2Phy(ptV.at(0));
      if (!hCanvas) throw std::domain_error("Cannot edit while locked.");

      BeginPath(hCanvas);
      MoveToEx(hCanvas, phPt.x, phPt.y, &lastPos); //Move to first point

      for (unsigned int i=1; i<ptV.size(); ++i)
      {
         phPt = User2Phy(ptV.at(i));
         ::LineTo(hCanvas, phPt.x, phPt.y);
      }

      EndPath(hCanvas);
      StrokePath(hCanvas);
   } //void EnhMetafile::Polyline(const PointVector &ptV)

   void EnhMetafile::Arc(const Rect &bounds, const Point &start, const Point &end)
   {
      RECT phBounds = User2Phy(bounds);
      POINT phStart = User2Phy(start), phEnd = User2Phy(end);
      if (!hCanvas) throw std::domain_error("Cannot edit while locked.");

      ::Arc(hCanvas, phBounds.left, phBounds.top, phBounds.right, phBounds.bottom,
         phStart.x, phStart.y, phEnd.x, phEnd.y);
   } //void EnhMetafile::Arc(...)

   void EnhMetafile::Ellipse(const Rect &bounds)
   {
      RECT phBounds = User2Phy(bounds);
      if (!hCanvas) throw std::domain_error("Cannot edit while locked.");

      ::Ellipse(hCanvas, phBounds.left, phBounds.top, phBounds.right, phBounds.bottom);
   } //void EnhMetafile::Ellipse(const Rect &bounds)

   void EnhMetafile::Rectangle(const Rect &bounds)
   {
      RECT phBounds = User2Phy(bounds);
      if (!hCanvas) throw std::domain_error("Cannot edit while locked.");

      ::Rectangle(hCanvas, phBounds.left, phBounds.top, phBounds.right, phBounds.bottom);
   } //void EnhMetafile::Rectangle(const Rect &bounds)

   void EnhMetafile::Polygon(const PointVector &ptV)
   {
      if (!hCanvas) throw std::domain_error("Cannot edit while locked.");
      POINT *phPts = new POINT[ptV.size()];

      //Copy over points
      for (unsigned int i=0; i<ptV.size(); ++i)
         phPts[i] = User2Phy(ptV.at(i));

      ::Polygon(hCanvas, phPts, ptV.size());
      delete [] phPts; //Release memory
   } //void EnhMetafile::Polygon(const PointVector &ptV)

   //Form: (x-c.x)^2+(y-c.y)^2=r^2
   void EnhMetafile::Circle(const Point &c, double r)
      {Ellipse(Rect(c.x-r, c.y+r, c.x+r, c.y-r));}

   void EnhMetafile::DrawText(const Rect &bounds, const string &text, UINT uFormat)
   {
      RECT phBounds = User2Phy(bounds);
      if (!hCanvas) throw std::domain_error("Cannot edit while locked.");

      ::DrawText(hCanvas, text.c_str(), text.size(), &phBounds, uFormat);
   } //void EnhMetafile::DrawText(...)

   void EnhMetafile::TextOut(const Point &pt, const string &text)
   {
      POINT phPt = User2Phy(pt);
      if (!hCanvas) throw std::domain_error("Cannot edit while locked.");

      ::TextOut(hCanvas, phPt.x, phPt.y, text.c_str(), text.size());
   }

   void EnhMetafile::SelectPen(const Pen &pen)
   {
      HPEN hPenOld = hPen;
      LOGBRUSH lBrush;
      if (!hCanvas) throw std::domain_error("Cannot select new pen while locked.");

      this->pen = pen;
      lBrush.lbStyle = pen.bStyle;
      lBrush.lbColor = pen.bColor;
      lBrush.lbHatch = pen.hatch;

      hPen = ExtCreatePen(pen.style, int(100*pen.width), &lBrush, 0, 0);
      SelectObject(hCanvas, hPen);
      if (hPenOld) DeleteObject(hPenOld);
   } //void EnhMetafile::SelectPen(const Pen &pen)

   void EnhMetafile::SelectBrush(const Brush &brush)
   {
      HBRUSH hBrushOld = hBrush;
      LOGBRUSH lBrush;
      if (!hCanvas) throw std::domain_error("Cannot select new pen while locked.");

      this->brush = brush;
      lBrush.lbStyle = brush.lbStyle;
      lBrush.lbColor = brush.lbColor;
      lBrush.lbHatch = brush.lbHatch;

      hBrush = CreateBrushIndirect(&lBrush);
      SelectObject(hCanvas, hBrush);
      if (hBrushOld) DeleteObject(hBrushOld);
   } //void EnhMetafile::SelectBrush(const Brush &brush)


   void EnhMetafile::SelectFont(const Font &font)
   {
      HFONT hFontOld = hFont;
      if (!hCanvas) throw std::domain_error("Cannot select new font while locked.");
      this->font = font;
      hFont = CreateFont(hCanvas, font);
      SelectObject(hCanvas, hFont);
      if (hFontOld) DeleteObject(hFontOld);
   } //void EnhMetafile::SelectFont(const Font &font)

   COLORREF EnhMetafile::SetBkColor(COLORREF crColor)
   {
      if (!hCanvas) throw std::domain_error("Cannot set bakground color while locked.");
      return ::SetBkColor(hCanvas, crColor);
   } //COLORREF EnhMetafile::SetBkColor(COLORREF crColor)

   UINT EnhMetafile::SetBkMode(UINT iBkMode)
   {
      if (!hCanvas) throw std::domain_error("Cannot set bakground mode while locked.");
      return ::SetBkMode(hCanvas, iBkMode);
   } //UINT EnhMetafile::SetBkMode(UINT iBkMode)

   UINT EnhMetafile::SetTextAlign(UINT fMode)
   {
      if (!hCanvas) throw std::domain_error("Cannot set text alignment while locked.");
      return ::SetTextAlign(hCanvas, fMode);
   } //UINT EnhMetafile::SetTextAlign(UINT fMode)

   void EnhMetafile::Lock()
   {
      if (hCanvas)
      {
         hEMF = CloseEnhMetaFile(hCanvas);
         hCanvas = 0; //Canvas no longer exists
      }
   }

   void EnhMetafile::Unlock(HDC hDCReference)
   {
      if (hEMF)
      {
         RECT rect = mm2HIMetric(bounds);
         if (hCanvas) throw std::domain_error("hCanvas & hEMF should not coexist!");

         Create(hDCReference); //Create a new canvas
         PlayEnhMetaFile(hCanvas, hEMF, &rect); //Re-play current metafile on new canvas
         DeleteEnhMetaFile(hEMF); //Delete old metafile
         hEMF = 0; //Metafile no longer exists

         //Reset configuration (don't think this is required):
//         SelectObject(hCanvas, hPen);
      }
   }

   void EnhMetafile::Draw(HDC hDC, const Rect &bounds) const
   {
      RECT rect = GetRECT(bounds);
      if (!hEMF) throw std::domain_error("Metafile is not locked: cannot be played.");

      PlayEnhMetaFile(hDC, hEMF, &rect);
   }

   void EnhMetafile::Save(const string &filePath) const
   {
      HENHMETAFILE hEMFSaved;
      if (!hEMF) throw std::domain_error("Metafile is not locked: cannot be saved.");
      hEMFSaved = CopyEnhMetaFile(hEMF, filePath.c_str());
      if (!hEMFSaved) throw std::runtime_error("Error saving file: " + filePath);
      DeleteEnhMetaFile(hEMFSaved); //Release memory
   }

   EnhMetafile::~EnhMetafile()
   {
      Lock();
      if (hEMF) DeleteEnhMetaFile(hEMF);
      if (hPen) DeleteObject(hPen);
      if (hBrush) DeleteObject(hBrush);
      if (hFont) DeleteObject(hFont);
   }

   //Write to an output stream
   std::ostream& operator<<(std::ostream& os, const EnhMetafile &m)
   {
      if (!m.hEMF) throw std::domain_error("Metafile is not locked: cannot be saved.");
      {
         UINT sz = GetEnhMetaFileBits(m.hEMF, 0, 0);
         BYTE *buf = new BYTE[sz];
         try {
            GetEnhMetaFileBits(m.hEMF, sz, buf);
            os.write(reinterpret_cast<char *>(buf), sz);
         } catch (...) {}
         delete[] buf;
      }
      return os;
   }
} //namespace GDIExt
