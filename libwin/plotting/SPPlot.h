//Declares components used to generate S-Parameter & polar plots.

#ifndef SPPLOT_H
#define SPPLOT_H

#include <tools/GDIExtensions.h>
#include <tools/IOExtensions.h>
#include <iostream>
#include <vector>

namespace SParamPlots
{
/******************************************************************************/
/*                          Constants, typedefs, etc.                         */
/******************************************************************************/
   using namespace GDIExt;
   using std::istream;
   using std::string;

   typedef std::vector<double> RealVector;

/******************************************************************************/
/*                             Class Declarations                             */
/******************************************************************************/
/*---------------------------Plot Class Declaration---------------------------*/
   class Plot
   {
   private:
      GDIExt::EnhMetafile metafile;
      Font defaultFont, symbolFont;
      double thetaCorrect; //Used to compensate for StarOffice Bug
      double maxMagCircle; //Constant used to identify the magnitude of the outermost power circle 

      void DrawSmith();
      void DrawPolar(const RealVector &pwrCircles);
      void DrawConstR(double rL);
      void DrawConstX(double xL);
      void LabelR(double rL, double xOffset=0);
      void LabelX(double xL);
      void DrawMagCircle(double mag_dB);
      void DrawAngleLine(double angle);
      void LabelMagCircle(double mag_dB);
      void LabelAngleLine(double angle);
      void DrawDataSet(istream &dataStream);

   public:
      Plot(bool SOThetaCorrect = false); //Set to true to compensate for StarOffice's theta problem
      void DrawFromStream(istream &dataStream);
      void Open(const string &filePath);
      void CreateEmptySmith();
      void CreateEmptyPolar();

      void Save(const string &filePath) const;
      void Draw(HDC hDC, const Rect &bounds) const; //Draw onto a GDI device

      friend std::ostream& operator<<(std::ostream& os, const Plot &p);
   }; //class Plot

} //namespace SParamPlots
#endif //#ifndef SPPLOT_H

