#pragma once
#include <plotting\SPPlot.h>

namespace WinSPP {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class WinSPPFrm : public System::Windows::Forms::Form
	{
	public:
		WinSPPFrm(void) : plot(0)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~WinSPPFrm()
		{
			if (components)
			{
				delete components;
			}
			if (plot) { delete plot; plot = 0; };
		}
	private: System::Windows::Forms::GroupBox^  gbPlot;
	private: System::Windows::Forms::Panel^  panel2;
	private: System::Windows::Forms::CheckBox^  chkSOComp;
	private: System::Windows::Forms::RadioButton^  rbEmptySmith;
	private: System::Windows::Forms::RadioButton^  rbEmptyPolar;
	private: System::Windows::Forms::TextBox^  tbFileName;
	private: System::Windows::Forms::RadioButton^  rbFromFile;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Button^  btnSave;
	private: System::Windows::Forms::Button^  btnPreview;
	private: System::Windows::Forms::GroupBox^  gbPreview;
	private: System::Windows::Forms::PictureBox^  pbPreview;
	private: System::Windows::Forms::Panel^  pnlPreview;
	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(WinSPPFrm::typeid));
			this->gbPlot = (gcnew System::Windows::Forms::GroupBox());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->chkSOComp = (gcnew System::Windows::Forms::CheckBox());
			this->rbEmptySmith = (gcnew System::Windows::Forms::RadioButton());
			this->rbEmptyPolar = (gcnew System::Windows::Forms::RadioButton());
			this->tbFileName = (gcnew System::Windows::Forms::TextBox());
			this->rbFromFile = (gcnew System::Windows::Forms::RadioButton());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->btnSave = (gcnew System::Windows::Forms::Button());
			this->btnPreview = (gcnew System::Windows::Forms::Button());
			this->gbPreview = (gcnew System::Windows::Forms::GroupBox());
			this->pnlPreview = (gcnew System::Windows::Forms::Panel());
			this->pbPreview = (gcnew System::Windows::Forms::PictureBox());
			this->gbPlot->SuspendLayout();
			this->panel2->SuspendLayout();
			this->panel1->SuspendLayout();
			this->gbPreview->SuspendLayout();
			this->pnlPreview->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbPreview))->BeginInit();
			this->SuspendLayout();
			// 
			// gbPlot
			// 
			this->gbPlot->Controls->Add(this->panel2);
			this->gbPlot->Controls->Add(this->panel1);
			this->gbPlot->Dock = System::Windows::Forms::DockStyle::Top;
			this->gbPlot->Location = System::Drawing::Point(0, 0);
			this->gbPlot->Name = L"gbPlot";
			this->gbPlot->Size = System::Drawing::Size(632, 124);
			this->gbPlot->TabIndex = 6;
			this->gbPlot->TabStop = false;
			this->gbPlot->Text = L"Plot:";
			// 
			// panel2
			// 
			this->panel2->Controls->Add(this->chkSOComp);
			this->panel2->Controls->Add(this->rbEmptySmith);
			this->panel2->Controls->Add(this->rbEmptyPolar);
			this->panel2->Controls->Add(this->tbFileName);
			this->panel2->Controls->Add(this->rbFromFile);
			this->panel2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panel2->Location = System::Drawing::Point(3, 16);
			this->panel2->Name = L"panel2";
			this->panel2->Padding = System::Windows::Forms::Padding(10);
			this->panel2->Size = System::Drawing::Size(531, 105);
			this->panel2->TabIndex = 3;
			// 
			// chkSOComp
			// 
			this->chkSOComp->AutoSize = true;
			this->chkSOComp->Location = System::Drawing::Point(193, 27);
			this->chkSOComp->Name = L"chkSOComp";
			this->chkSOComp->Size = System::Drawing::Size(171, 17);
			this->chkSOComp->TabIndex = 4;
			this->chkSOComp->Text = L"Compensate for StarOffice bug";
			this->chkSOComp->UseVisualStyleBackColor = true;
			this->chkSOComp->Click += gcnew System::EventHandler(this, &WinSPPFrm::chkSOComp_Click);
			// 
			// rbEmptySmith
			// 
			this->rbEmptySmith->AutoSize = true;
			this->rbEmptySmith->Checked = true;
			this->rbEmptySmith->Location = System::Drawing::Point(10, 3);
			this->rbEmptySmith->Name = L"rbEmptySmith";
			this->rbEmptySmith->Size = System::Drawing::Size(104, 17);
			this->rbEmptySmith->TabIndex = 0;
			this->rbEmptySmith->TabStop = true;
			this->rbEmptySmith->Text = L"Empty Smith Plot";
			this->rbEmptySmith->UseVisualStyleBackColor = true;
			this->rbEmptySmith->Click += gcnew System::EventHandler(this, &WinSPPFrm::rbEmptySmith_Click);
			// 
			// rbEmptyPolar
			// 
			this->rbEmptyPolar->AutoSize = true;
			this->rbEmptyPolar->Location = System::Drawing::Point(10, 26);
			this->rbEmptyPolar->Name = L"rbEmptyPolar";
			this->rbEmptyPolar->Size = System::Drawing::Size(102, 17);
			this->rbEmptyPolar->TabIndex = 0;
			this->rbEmptyPolar->TabStop = true;
			this->rbEmptyPolar->Text = L"Empty Polar Plot";
			this->rbEmptyPolar->UseVisualStyleBackColor = true;
			this->rbEmptyPolar->Click += gcnew System::EventHandler(this, &WinSPPFrm::rbEmptyPolar_Click);
			// 
			// tbFileName
			// 
			this->tbFileName->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->tbFileName->Location = System::Drawing::Point(10, 75);
			this->tbFileName->Name = L"tbFileName";
			this->tbFileName->Size = System::Drawing::Size(511, 20);
			this->tbFileName->TabIndex = 2;
			// 
			// rbFromFile
			// 
			this->rbFromFile->AutoSize = true;
			this->rbFromFile->Location = System::Drawing::Point(10, 49);
			this->rbFromFile->Name = L"rbFromFile";
			this->rbFromFile->Size = System::Drawing::Size(67, 17);
			this->rbFromFile->TabIndex = 1;
			this->rbFromFile->TabStop = true;
			this->rbFromFile->Text = L"From file:";
			this->rbFromFile->UseVisualStyleBackColor = true;
			this->rbFromFile->CheckedChanged += gcnew System::EventHandler(this, &WinSPPFrm::rbFromFile_CheckedChanged);
			this->rbFromFile->Click += gcnew System::EventHandler(this, &WinSPPFrm::rbFromFile_Click);
			// 
			// panel1
			// 
			this->panel1->Controls->Add(this->btnSave);
			this->panel1->Controls->Add(this->btnPreview);
			this->panel1->Dock = System::Windows::Forms::DockStyle::Right;
			this->panel1->Location = System::Drawing::Point(534, 16);
			this->panel1->Name = L"panel1";
			this->panel1->Padding = System::Windows::Forms::Padding(10);
			this->panel1->Size = System::Drawing::Size(95, 105);
			this->panel1->TabIndex = 2;
			// 
			// btnSave
			// 
			this->btnSave->Location = System::Drawing::Point(10, 72);
			this->btnSave->Name = L"btnSave";
			this->btnSave->Size = System::Drawing::Size(75, 23);
			this->btnSave->TabIndex = 3;
			this->btnSave->Text = L"Save .emf";
			this->btnSave->UseVisualStyleBackColor = true;
			this->btnSave->Click += gcnew System::EventHandler(this, &WinSPPFrm::btnSave_Click);
			// 
			// btnPreview
			// 
			this->btnPreview->Location = System::Drawing::Point(10, 10);
			this->btnPreview->Name = L"btnPreview";
			this->btnPreview->Size = System::Drawing::Size(75, 23);
			this->btnPreview->TabIndex = 3;
			this->btnPreview->Text = L"Preview";
			this->btnPreview->UseVisualStyleBackColor = true;
			this->btnPreview->Click += gcnew System::EventHandler(this, &WinSPPFrm::btnPreview_Click);
			// 
			// gbPreview
			// 
			this->gbPreview->Controls->Add(this->pnlPreview);
			this->gbPreview->Dock = System::Windows::Forms::DockStyle::Fill;
			this->gbPreview->Location = System::Drawing::Point(0, 124);
			this->gbPreview->Name = L"gbPreview";
			this->gbPreview->Size = System::Drawing::Size(632, 329);
			this->gbPreview->TabIndex = 7;
			this->gbPreview->TabStop = false;
			this->gbPreview->Text = L"Preview:";
			// 
			// pnlPreview
			// 
			this->pnlPreview->Controls->Add(this->pbPreview);
			this->pnlPreview->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pnlPreview->Location = System::Drawing::Point(3, 16);
			this->pnlPreview->Name = L"pnlPreview";
			this->pnlPreview->Size = System::Drawing::Size(626, 310);
			this->pnlPreview->TabIndex = 1;
			this->pnlPreview->Resize += gcnew System::EventHandler(this, &WinSPPFrm::pnlPreview_Resize);
			// 
			// pbPreview
			// 
			this->pbPreview->Location = System::Drawing::Point(131, 29);
			this->pbPreview->Name = L"pbPreview";
			this->pbPreview->Size = System::Drawing::Size(359, 182);
			this->pbPreview->TabIndex = 0;
			this->pbPreview->TabStop = false;
			// 
			// WinSPPFrm
			// 
			this->AcceptButton = this->btnPreview;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(632, 453);
			this->Controls->Add(this->gbPreview);
			this->Controls->Add(this->gbPlot);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Name = L"WinSPPFrm";
			this->Text = L"WinSPP";
			this->Shown += gcnew System::EventHandler(this, &WinSPPFrm::WinSPPFrm_Shown);
			this->gbPlot->ResumeLayout(false);
			this->panel2->ResumeLayout(false);
			this->panel2->PerformLayout();
			this->panel1->ResumeLayout(false);
			this->gbPreview->ResumeLayout(false);
			this->pnlPreview->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pbPreview))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion

//Cached plot:
private: SParamPlots::Plot *plot;

//Custom functions:
private: void UpdateMetafile(bool displayErrors);
private: void UpdateMetafile() { UpdateMetafile(true); };
private: void RefreshPreview();
private: void UpdateControlState();
private: void SaveMetafile();

//Yuk... VS seems to want to put EH code in the header file... gross....
private: System::Void WinSPPFrm_Shown(System::Object^  sender, System::EventArgs^  e) {
	UpdateMetafile();
	RefreshPreview();
	UpdateControlState();
}
private: System::Void btnPreview_Click(System::Object^  sender, System::EventArgs^  e) {
	UpdateMetafile();
	RefreshPreview();
}
private: System::Void rbEmptySmith_Click(System::Object^  sender, System::EventArgs^  e) {
	UpdateMetafile();
	RefreshPreview();
}
private: System::Void rbEmptyPolar_Click(System::Object^  sender, System::EventArgs^  e) {
	UpdateMetafile();
	RefreshPreview();
}
private: System::Void rbFromFile_Click(System::Object^  sender, System::EventArgs^  e) {
	UpdateMetafile(false);
	RefreshPreview();
}
private: System::Void chkSOComp_Click(System::Object^  sender, System::EventArgs^  e) {
	UpdateMetafile();
	RefreshPreview();
}
private: System::Void pnlPreview_Resize(System::Object^  sender, System::EventArgs^  e) {
	RefreshPreview();
}
private: System::Void rbFromFile_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	UpdateControlState();
}
private: System::Void btnSave_Click(System::Object^  sender, System::EventArgs^  e) {
	SaveMetafile();
}
};
}
