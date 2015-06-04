/*Creates S-Parameter & polar plots.

NOTE:
   Uses deprecated library GDIExtensions.h.
   Should be modified to use the GDI+ "Metafile" class instead.
*/

#include <plotting/SPPlot.h>
#include <math.h>
#include <string>
#include <sstream>
#include <fstream>

namespace SParamPlots
{
/******************************************************************************/
/*                          Constants, typedefs, etc.                         */
/******************************************************************************/
   using std::ostringstream;
   using std::ifstream;

/******************************************************************************/
/*                              Helper Functions                              */
/******************************************************************************/
   COLORREF ReadColor(istream &dataStream)
   {
      string strToken;
      int R, G, B;

      dataStream >> strToken;
      if (strToken!="RGB") throw
         std::invalid_argument("Invalid color format.  Must be: 'RGB [R] [G] [B]',\nex: 'RGB 0 0 255' for blue'");

      dataStream >> R >> G >> B;
      return RGB(R,G,B);
   } //COLORREF ReadColor(istream &dataStream)

   double MaxPowerCircle(const RealVector &v)
   {
      double max = 0; //Smallest displayed power circle WILL be 0dB
      for (unsigned int i=0; i<v.size(); ++i)
         if (v.at(i)>max) max = v.at(i);
      return max;
   } //double MaxPowerCircle(const RealVector &v)

   RealVector ReadConstPowerCircles(istream &dataStream)
   {
      string strToken;
      int nCircles;

      dataStream >> strToken;
      if (strToken!="ConstPowerCircles") throw
         std::invalid_argument("Missing 'ConstPowerCircles'");

      dataStream >> nCircles;

      {
         RealVector cPCs(nCircles); //List of constant power circles
         double curCircle;

         for (int i=0; i<nCircles; ++i)
         {
            dataStream >> curCircle;
            if (curCircle==0) throw std::invalid_argument("Do not specify 0dB circle - included by default");
            cPCs.at(i) = curCircle;
         }

         return cPCs;
      }
   }//RealVector ReadConstPowerCircles(istream &dataStream)

/******************************************************************************/
/*                             Module-Level Classes                           */
/******************************************************************************/
   class LineStyle
   {
      public:
      enum SymbolType{Circle, Square, Triangle, NoSymbol};

      bool traceLine;
      double lineWidth;
      COLORREF lineColor;
      SymbolType symbol;
      double symbolSize;
      bool symbolFill;
      COLORREF fillColor;
      int symbolSkip;
   }; //class LineStyle

   //operator>> - Reads a 'LineStyle' from an input stream:
   istream &operator>>(istream &dataStream, LineStyle &lStyle)
   {
      string strToken;

      dataStream >> strToken;
      if (strToken!="TraceLine") throw std::invalid_argument("Missing 'TraceLine'");
      dataStream >> strToken;
      if (strToken=="Yes")
         lStyle.traceLine = true;
      else if (strToken=="No")
         lStyle.traceLine = false;
      else
         throw std::invalid_argument("'TraceLine' must be either 'Yes' or 'No'");

      dataStream >> strToken;
      if (strToken!="LineWidth") throw std::invalid_argument("Missing 'LineWidth'");
      dataStream >> lStyle.lineWidth;

      dataStream >> strToken;
      if (strToken!="LineColor") throw std::invalid_argument("Missing 'LineColor'");
      lStyle.lineColor = ReadColor(dataStream);

      dataStream >> strToken;
      if (strToken!="Symbol") throw std::invalid_argument("Missing 'Symbol'");
      dataStream >> strToken;
      if (strToken=="Circle")
         lStyle.symbol = LineStyle::Circle;
      else if (strToken=="Square")
         lStyle.symbol = LineStyle::Square;
      else if (strToken=="Triangle")
         lStyle.symbol = LineStyle::Triangle;
      else if (strToken=="None")
      {
         lStyle.symbol = LineStyle::NoSymbol;
         return dataStream; //All information has been collected
      }
      else
         throw std::invalid_argument("Unrecognized symbol: "+strToken);

      dataStream >> strToken;
      if (strToken!="SymbolSize") throw std::invalid_argument("Missing 'SymbolSize'");
      dataStream >> lStyle.symbolSize;
      lStyle.symbolSize = lStyle.symbolSize/100; //Defined in % of a graph unit (approx 1/200 of the graph)

      dataStream >> strToken;
      if (strToken!="SymbolFill") throw std::invalid_argument("Missing 'SymbolFill'");
      dataStream >> strToken;
      if (strToken=="Yes")
      {
         lStyle.symbolFill = true;
         dataStream >> strToken;
         if (strToken!="FillColor") throw std::invalid_argument("Missing 'FillColor'");
         lStyle.fillColor = ReadColor(dataStream);
      }
      else if (strToken=="No")
         lStyle.symbolFill = false;
      else
         throw std::invalid_argument("'SymbolFill' must be either 'Yes' or 'No'");

      dataStream >> strToken;
      if (strToken!="SymbolSkip") throw std::invalid_argument("Missing 'SymbolSkip'");
      dataStream >> lStyle.symbolSkip;
      lStyle.symbolSkip = abs(lStyle.symbolSkip);

      return dataStream;
   } //istream &operator>>(istream &dataStream, LineStyle &lStyle)

/******************************************************************************/
/*                                Main Classes                                */
/******************************************************************************/
/*------------------Plot Class: Member Function Definitions-------------------*/
   Plot::Plot(bool SOThetaCorrect): metafile(Rect(100,100)),
      defaultFont("Times New Roman", 7), symbolFont("Symbol", 9),
      thetaCorrect(SOThetaCorrect?-1:1) {}

   void Plot::DrawSmith()
   {
      const int nConstRCurves = 11;
      const double constRCurves[nConstRCurves] = {0.2, 0.4, 0.6, 0.8, 1.5, 2, 3, 4, 6, 10, 20};
      const int nConstXCurves = 12;
      const double constXCurves[nConstXCurves] = {0.2, 0.4, 0.6, 0.8, 1, 1.5, 2, 3, 4, 6, 10, 20};
      Pen pen(PS_SOLID|PS_GEOMETRIC|PS_ENDCAP_FLAT, 0.2);

      metafile.SetUserCoordSystem(Point(50,50), 2.2, 2.2);
      pen.bColor = RGB(170,170,170); //Use light gray pen for most circles
      metafile.SelectPen(pen);

      for (int i=0; i<nConstRCurves; ++i)
         DrawConstR(constRCurves[i]);

      for (int i=0; i<nConstXCurves; ++i)
         DrawConstX(constXCurves[i]);

      metafile.MoveTo(Point(-1.1,0));
      metafile.LineTo(Point(1.1,0));
      pen.bColor = RGB(0,0,0); //Use black pen for remaining shapes
      metafile.SelectPen(pen);
      metafile.Circle(Point(0, 0), 1);
      DrawConstR(1);

      //Label constant reactance circles:
      //BUG: Only TA_TOP is supported by StarOffice when rotating text!?!?!:
      metafile.SetTextAlign(TA_TOP|TA_CENTER|TA_NOUPDATECP);
      for (int i=0; i<nConstXCurves; ++i)
         LabelX(constXCurves[i]);
      LabelX(0);  //Label imag(gamma)=0 "circle"

      //Label constant resistance circles:
      defaultFont.angle = 0;
      metafile.SelectFont(defaultFont);
      for (int i=0; i<nConstRCurves; ++i)
         LabelR(constRCurves[i]);
      LabelR(1, 0.02); //Label real(gamma)=1 circle

      //Draw symbols:
      metafile.SelectFont(symbolFont);
      {
         const char inf[2] = {0xA5,0}; //Null-terminated infinity symbol
         metafile.TextOut(Point(-0.96, 0.025), inf);
      }
   } //void Plot::DrawSmith()

   void Plot::DrawPolar(const RealVector &pwrCircles)
   {
      const int nPwrCircles = pwrCircles.size(); //Other than 0dB
      const int nAngleLines = 4;
      const double angleLines[nAngleLines] = {0, 45, 90, 135};
      Pen pen(PS_SOLID|PS_GEOMETRIC|PS_ENDCAP_FLAT, 0.2);

      maxMagCircle = pow(10, MaxPowerCircle(pwrCircles)/20.0);
      metafile.SetUserCoordSystem(Point(50,50), maxMagCircle*2.2, maxMagCircle*2.2);
      pen.bColor = RGB(170,170,170); //Use light gray pen for most lines & circles
      metafile.SelectPen(pen);

      for (int i=0; i<nAngleLines; ++i)
         DrawAngleLine(angleLines[i]);

      for (int i=0; i<nPwrCircles; ++i)
         DrawMagCircle(pwrCircles.at(i));

      pen.bColor = RGB(0,0,0); //Use black pen for remaining shapes
      metafile.SelectPen(pen);
      metafile.Circle(Point(0, 0), 1);

      //Label angle lines:
      //BUG: Only TA_TOP is supported by StarOffice when rotating text!?!?!:
      metafile.SetTextAlign(TA_TOP|TA_CENTER|TA_NOUPDATECP);
      for (int i=0; i<nAngleLines; ++i)
         LabelAngleLine(angleLines[i]);

      //Label magnitude lines:
      defaultFont.angle = 0;
      metafile.SelectFont(defaultFont);
      metafile.SetTextAlign(TA_TOP|TA_RIGHT|TA_NOUPDATECP);
      for (int i=0; i<nPwrCircles; ++i)
         LabelMagCircle(pwrCircles.at(i));
      LabelMagCircle(0); //Label 0dB circle
   } //void Plot::DrawPolar()

   void Plot::DrawConstR(double rL)
      {metafile.Circle(Point(rL/(1+rL), 0), 1/(1+rL));}

   void Plot::DrawConstX(double xL)
   {
      double one_xL = 1/xL;
      metafile.Circle(Point(1, one_xL), one_xL);
      metafile.Circle(Point(1, -one_xL), one_xL);
   }

   void Plot::LabelR(double rL, double xOffset)
   {
      const double xi = (rL-1)/(1+rL) + xOffset; //x-intercept of circle + offset
      ostringstream label; label << rL;
      metafile.TextOut(Point(xi, 0.005), label.str().c_str());
   }

   void Plot::LabelX(double xL)
   {
      using StdExt::Math::PI;
      const double shiftFactor = 1.06; //Factor used to shift labels slightly outside the unit circle
      ostringstream label; label << xL << 'j';

      if (xL==0)
      {
         defaultFont.angle = thetaCorrect*90;
         metafile.SelectFont(defaultFont); //Rotate text
         metafile.TextOut(Point(-shiftFactor, 0), label.str().c_str());
      }
      else
      {
         const double b = 1/xL; //y-coordinate of const. reactance circle's centre
         const double yi = 2*b/(1+b*b); //y-coord. of intersection with inf. impedance circle
         const double xi = 1-yi*b; //x-coord. of intersection with inf. impedance circle
         const double x = xi*shiftFactor;
         const double y = yi*shiftFactor;
         const double angle = atan2(yi,xi)*(180/PI)-90;

         defaultFont.angle = thetaCorrect*angle;
         metafile.SelectFont(defaultFont); //Rotate text
         metafile.TextOut(Point(x, y), label.str().c_str());
         defaultFont.angle = 180-thetaCorrect*angle;
         metafile.SelectFont(defaultFont); //Rotate text
         metafile.TextOut(Point(x, -y), string("-")+label.str().c_str());
      }
   } //void Plot::LabelX(double xL)

   void Plot::DrawMagCircle(double mag_dB)
      {metafile.Circle(Point(0, 0), pow(10, mag_dB/20));}

   void Plot::DrawAngleLine(double angle)
   {
      using StdExt::Math::PI;
      const double radius = maxMagCircle; //Radius to end angle lines
      const double x = radius*cos(angle*(PI/180));
      const double y = radius*sin(angle*(PI/180));

      metafile.MoveTo(Point(-x,-y));
      metafile.LineTo(Point(x,y));
   } //void Plot::DrawAngleLines(double angle)

   void Plot::LabelMagCircle(double mag_dB)
   {
      ostringstream label; label << mag_dB << "dB";
      metafile.TextOut(Point(pow(10, mag_dB/20)-0.01, 0.005), label.str().c_str());
   }

   void Plot::LabelAngleLine(double angle)
   {
      using StdExt::Math::PI;
      const double radius = maxMagCircle; //Radius to end angle lines (largest circle)
      const double shiftFactor = radius*1.06; //Factor used to shift labels slightly outside the largest circle
      const double x = shiftFactor*cos(angle*(PI/180));
      const double y = shiftFactor*sin(angle*(PI/180));
      const char degSymbol = 0xB0;

      defaultFont.angle = thetaCorrect*(angle-90);
      metafile.SelectFont(defaultFont); //Rotate text
      {
         ostringstream label; label << angle << degSymbol;
         metafile.TextOut(Point(x, y), label.str().c_str());
      }
      defaultFont.angle = 180+defaultFont.angle;
      metafile.SelectFont(defaultFont); //Rotate text
      {
         ostringstream label;
         if (angle!=0) label << '-'; //Add a negative sign if we are not displaying 180deg
         label << (180-angle) << degSymbol;
         metafile.TextOut(Point(-x, -y), label.str().c_str());
      }
   } //void Plot::LabelAngleLine(double angle)

   void Plot::DrawFromStream(istream &dataStream)
   {
      string strToken;
      int nDataSets;

      //Find out what type of plot to generate:
      dataStream >> strToken;
      if (strToken=="Smith")
         DrawSmith();
      else if (strToken=="Polar")
         DrawPolar(ReadConstPowerCircles(dataStream));
      else
         throw std::invalid_argument("Missing plot type ('Smith' or 'Polar')");

      //Find out number of datasets to plot & proceed:
      dataStream >> strToken;
      if (strToken!="DataSets")
         throw std::invalid_argument("Missing 'DataSets'");

      dataStream >> nDataSets;
      for (int i=0; i<nDataSets; ++i)
         DrawDataSet(dataStream);

      metafile.Lock();
   } //void Plot::DrawFromStream(istream &dataStream)

   void Plot::DrawDataSet(istream &dataStream)
   {
      using StdExt::SkipWhiteSpaces;
      using StdExt::GetLine;
      Pen pen(PS_SOLID|PS_GEOMETRIC|PS_ENDCAP_ROUND);
      LineStyle lStyle;
      string strToken;
      int nDataPoints;

      dataStream >> strToken;
      if (strToken!="Description") throw std::invalid_argument("Missing 'Description'");
      dataStream >> SkipWhiteSpaces; //Find the first non-whitespace character
      GetLine(dataStream); //Remaining text on the line is the description of the dataset (ignored)

      dataStream >> lStyle; //Read in all the information about the line style

      dataStream >> strToken;
      if (strToken!="DataPoints") throw std::invalid_argument("Missing 'DataPoints'");
      dataStream >> nDataPoints;

      {
         PointVector data(nDataPoints);

         //Read in data points:
         for (int i=0; i<nDataPoints; ++i)
            dataStream >> data.at(i).x >> data.at(i).y; //real imag

         //Select proper pen:
         pen.width = lStyle.lineWidth;
         pen.bColor = lStyle.lineColor;
         metafile.SelectPen(pen);

         //Plot curve, if requested:
         if (lStyle.traceLine) metafile.Polyline(data);

         if (lStyle.symbol!=LineStyle::NoSymbol) //Plot symbols:
         {
            const int increment = 1+abs(lStyle.symbolSkip);
            metafile.SelectBrush(Brush(lStyle.symbolFill?BS_SOLID:BS_NULL, lStyle.fillColor));

            if (lStyle.symbol==LineStyle::Circle)
               for (int i=0; i<nDataPoints; i+=increment)
                  metafile.Ellipse(Rect(data.at(i), lStyle.symbolSize, lStyle.symbolSize));
            else if (lStyle.symbol==LineStyle::Square)
               for (int i=0; i<nDataPoints; i+=increment)
                  metafile.Rectangle(Rect(data.at(i), lStyle.symbolSize, lStyle.symbolSize));
            else if (lStyle.symbol==LineStyle::Triangle)
            {
               const double half_ss = lStyle.symbolSize/2;
               PointVector symb_orig(3), symb(3); //Symbol at origin, actual symbol

               //Compute coordinates of symbol around the origin (0,0):
               symb_orig.at(0) = Point(-half_ss, -half_ss);
               symb_orig.at(1) = Point(half_ss, -half_ss);
               symb_orig.at(2) = Point(0, half_ss);

               for (int i=0; i<nDataPoints; i+=increment)
               {
                  //Compute the coordinates of the symbol for the current data point:
                  symb.at(0) = symb_orig.at(0)+data.at(i);
                  symb.at(1) = symb_orig.at(1)+data.at(i);
                  symb.at(2) = symb_orig.at(2)+data.at(i);

                  metafile.Polygon(symb); //Plot symbol
               }
            }
         }
      }
   } //void Plot::DrawDataSet(istream &dataStream)

   void Plot::CreateEmptySmith()
   {
      DrawSmith();
      metafile.Lock();
   } //void Plot::CreateEmptySmith()

   void Plot::CreateEmptyPolar()
   {
      RealVector defaultCircles(4); //Other than 0dB
      defaultCircles.at(0) = -3;
      defaultCircles.at(1) = -6;
      defaultCircles.at(2) = -9;
      defaultCircles.at(3) = -15;
      DrawPolar(defaultCircles);
      metafile.Lock();
   } //void Plot::CreateEmptyPolar()

   void Plot::Draw(HDC hDC, const Rect &bounds) const
      {metafile.Draw(hDC, bounds);}

   void Plot::Open(const string &filePath)
   {
      ifstream fileStream(filePath.c_str()); //Open the desired file
      if (!fileStream.is_open()) throw std::invalid_argument(
         "Cannot open file: " + filePath);
      DrawFromStream(fileStream);
      fileStream.close();
   } //void Plot::Open(const string &filePath)

   void Plot::Save(const string &filePath) const
      {metafile.Save(filePath);}

   std::ostream& operator<<(std::ostream& os, const Plot &p)
      {return os << p.metafile;}
} //namespace SParamPlots
