#include <windows.h>
#include "WinSPPFrm.h"
#include <msclr/marshal_cppstd.h>

namespace WinSPP {

//In-elegant refresh all function to update control state:
void WinSPPFrm::UpdateControlState() {
	tbFileName->Enabled = rbFromFile->Checked;
} //UpdateControlState

void WinSPPFrm::UpdateMetafile(bool displayErrors) {
	using namespace Runtime::InteropServices;
	using namespace GDIExt;
	bool compensateSObug = chkSOComp->Checked;
	std::string fileName = msclr::interop::marshal_as<std::string>(tbFileName->Text);
	if (plot) { delete plot; plot = 0; }

	try	{
		plot = new SParamPlots::Plot(compensateSObug);
		if (rbEmptySmith->Checked)
			plot->CreateEmptySmith();
		else if (rbEmptyPolar->Checked)
			plot->CreateEmptyPolar();
		else
			plot->Open(fileName);
	} catch (std::exception &e) {
		if (plot) { delete plot; plot = 0; }
		RefreshPreview();
		if (displayErrors) {
			System::String ^msg = gcnew System::String(e.what());
			MessageBox::Show(msg, "ERROR", MessageBoxButtons::OK);
		}
	}
} //UpdateMetafile

void WinSPPFrm::RefreshPreview() {
	Drawing::Rectangle rect = pnlPreview->ClientRectangle;
	int maxdim = min(rect.Width, rect.Height);
	if (maxdim > 0) {
		pbPreview->SetBounds((rect.Width - maxdim) / 2, (rect.Height - maxdim) / 2, maxdim, maxdim);
	}

	try	{
		using namespace Drawing;
		using Drawing::Pen;
		using Drawing::Rectangle;
		using Drawing::SolidBrush;

		Rectangle rect = pbPreview->ClientRectangle;
		SolidBrush ^brush = gcnew SolidBrush(Color::White);
		Bitmap ^bmp = gcnew Bitmap(rect.Width, rect.Height);
		Graphics ^g = Graphics::FromImage(bmp);
		//Use a white background:
		g->FillRectangle(brush, rect);

		if (plot) {
			HDC hdc = static_cast<HDC>(g->GetHdc().ToPointer());
			plot->Draw(hdc, GDIExt::Rect(pbPreview->Width, pbPreview->Height));
			g->ReleaseHdc();
		}

		pbPreview->Image = bmp;
	} catch (...) {}
} //RefreshPreview

void WinSPPFrm::SaveMetafile() {
	try	{
		System::String ^msg;
		std::string fileName = msclr::interop::marshal_as<std::string>(tbFileName->Text);
		fileName += ".emf";
		if (!plot) throw(std::runtime_error("Nothing to save."));

		if (rbEmptySmith->Checked)
			fileName = "Smith.emf";
		else if (rbEmptyPolar->Checked)
			fileName = "Polar.emf";

		plot->Save(fileName);
		msg = gcnew System::String(fileName.c_str());
		MessageBox::Show(msg, "Saved to file:", MessageBoxButtons::OK);
	} catch (std::exception &e) {
		System::String ^msg = gcnew System::String(e.what());
		MessageBox::Show(msg, "ERROR", MessageBoxButtons::OK);
	}
} //SaveMetafile
} //namespace WinSPP
