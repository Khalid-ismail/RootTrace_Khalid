//#pragma once
#include "highgui.h"

#include "../Header_Files/About.h"
#include "MainCode.h"



namespace RootTrace2 {
	
/*	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices; */ //K_I

	const double processX = 1.0;//0.9
	const double processY = 1.0;//0.9

	// Do not change here. Use brightness scroll bar.
	const int idealr = 255;
	const int idealg = 255;
	const int idealb = 255;
	


	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{

			InitializeComponent();
		
			
			

		}
	private: System::Windows::Forms::TrackBar^  slider_BGprior;
	public: 

	private: System::Windows::Forms::Label^  stDevDisplay;
	public: 

	public: 

	static bool advancedOptionsVisible=false;


	private: System::Windows::Forms::GroupBox^  groupBox3;

	private: System::Windows::Forms::CheckBox^  autoEnhanceCheck;





	private: System::Windows::Forms::ToolTip^  toolTip1;
	private: System::Windows::Forms::CheckBox^  cb_selectBG;
	private: System::Windows::Forms::Button^  b_loadBG;
	private: System::Windows::Forms::Button^  b_loadPrevious;
	private: System::Windows::Forms::Button^  b_saveBG;
	private: System::Windows::Forms::SaveFileDialog^  saveFileDialog_bgmask;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialogBMP;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TrackBar^  tb_lowerHyst;
	private: System::Windows::Forms::TrackBar^  tb_upperHyst;


	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  lowerHystLabel;
	private: System::Windows::Forms::Label^  upperHystLabel;

	private: System::String^ workingFolder;



	public: 

	public: 
		static double slider_BGprior_value = 0.99;
		static double upperHyst = 0.97;
		//Asad's
		static double motionMixture = 100;
	private: System::Windows::Forms::GroupBox^  groupBox5;
	private: System::Windows::Forms::Button^  b_SetCalib;

	public: 

	private: System::Windows::Forms::Label^  pixLabel;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::TextBox^  textBox2;

	private: System::Windows::Forms::Button^  b_crop;
	private: System::Windows::Forms::GroupBox^  groupBox6;
	private: System::Windows::Forms::Label^  m_message;
	private: System::Windows::Forms::ProgressBar^  m_progress;
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::PictureBox^  pictureBox2;
	private: System::Windows::Forms::Button^  b_go;
	private: System::Windows::Forms::GroupBox^  Flow;
	private: System::Windows::Forms::PictureBox^  pictureBox4;
	private: System::Windows::Forms::PictureBox^  pictureBox3;
	private: System::Windows::Forms::PictureBox^  pictureBox5;
	private: System::Windows::Forms::GroupBox^  groupBox7;
	private: System::Windows::Forms::Button^  b_quickLoad;
	private: System::Windows::Forms::Button^  b_quickSave;
	private: System::Windows::Forms::TextBox^  tb_outputFolder;

	private: System::Windows::Forms::Button^  b_setOutputFolder;
	private: System::Windows::Forms::FolderBrowserDialog^  outputFolderBrowser;
	private: System::Windows::Forms::GroupBox^  groupBox8;

	private: String^outputFolderString;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::TextBox^  tb_minsPerFile;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::PictureBox^  pictureBox6;
	private: System::Windows::Forms::PictureBox^  pictureBox7;
	private: System::Windows::Forms::CheckBox^  cb_gravitropic;
	private: System::Windows::Forms::ToolStripMenuItem^  helpToolStripMenuItem1;
	private: System::Windows::Forms::CheckBox^  cb_defaultToRoot;
	private: System::Windows::Forms::TextBox^  tb_radius;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::CheckBox^  cb_tipAngles;
private: System::Windows::Forms::GroupBox^  groupBox9;
private: System::Windows::Forms::CheckBox^  cb_rot_CCW;
private: System::Windows::Forms::CheckBox^  cb_rot_CW;
private: System::Windows::Forms::CheckBox^  cb_flip_hor;
private: System::Windows::Forms::CheckBox^  autoSmoothCheck;
private: System::Windows::Forms::Label^  label13;
private: System::Windows::Forms::TrackBar^  motionModelTrackerBar;
private: System::Windows::Forms::Label^  label14;
private: System::Windows::Forms::ToolStripMenuItem^  toolsToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  scanForBarcodeToolStripMenuItem;
private: System::Windows::Forms::CheckBox^  tabooPointChkBox;
private: System::Windows::Forms::Button^  b_fitImage;
private: System::Windows::Forms::TabPage^  tabPage3;
private: System::Windows::Forms::NumericUpDown^  numeric_tipPointsNum;
private: System::Windows::Forms::Label^  label15;
private: System::Windows::Forms::CheckBox^  cb_EvolveTipPoints;
private: System::Windows::Forms::GroupBox^  groupBox2;
private: System::Windows::Forms::TrackBar^  tb_blowHighlights;

private: System::Windows::Forms::CheckBox^  cb_EnableEnhancing;
private: System::Windows::Forms::Label^  label1;
private: System::Windows::Forms::CheckBox^  cb_blowHighlights;
private: System::Windows::Forms::CheckBox^  cb_localmaxfilter;
private: System::Windows::Forms::CheckBox^  cb_suppressHor;
private: System::Windows::Forms::Label^  brightness_text;
private: System::Windows::Forms::Label^  label16;
private: System::Windows::Forms::TrackBar^  tb_brightness;

private: System::Windows::Forms::GroupBox^  groupBox10;
private: System::Windows::Forms::CheckBox^  cb_gaus_filter;
private: System::Windows::Forms::CheckBox^  cb_med_filter;
private: System::Windows::Forms::CheckBox^  cb_bilat_filter;
private: System::Windows::Forms::CheckBox^  cb_countLaterals;
private: System::Windows::Forms::TabPage^  Comments;
private: System::Windows::Forms::RichTextBox^  rtext_comments;
private: System::Windows::Forms::Button^  b_saveComments;
private: System::Windows::Forms::RadioButton^  radioButton1;
private: System::Windows::Forms::RadioButton^  radioButton2;
private: System::Windows::Forms::GroupBox^  groupBox4;
private: System::Windows::Forms::CheckBox^  cb_invert;
private: System::Windows::Forms::CheckBox^  cb_bg_removal;









			 static double lowerHyst = 0.009;

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			delete mainprog;
			if (components)
			{
				delete components;
			}
		}
	

	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;

	private: System::Windows::Forms::MenuStrip^  Mainwindowmenus;

	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openImageToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  helpToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  aboutToolStripMenuItem;
	private: System::Windows::Forms::ToolStrip^  toolStrip1;
	private: System::Windows::Forms::ToolStripButton^  toolStripButton1;
	private: System::Windows::Forms::ToolStripButton^  toolStripButton2;







	private: System::Windows::Forms::OpenFileDialog^  openFileDialogStartPoints;
	private: System::Windows::Forms::SaveFileDialog^  saveFileDialogStartPoints;
private: System::Windows::Forms::TabControl^  tabControl;





	private: System::Windows::Forms::TabPage^  tabPage1;

	private: System::Windows::Forms::Button^  savePoints;
	private: System::Windows::Forms::Button^  loadPoints;

	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Button^  button2;
	public: System::Windows::Forms::TextBox^  textBox1;
	private: 
	private: System::Windows::Forms::Button^  button1;
	public: 
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::CheckBox^  autoResizeCheck;
	private: System::Windows::Forms::CheckBox^  advancedOptions;
	private: System::Windows::Forms::HelpProvider^ help;




	private: System::ComponentModel::IContainer^  components;

	

	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->Mainwindowmenus = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openImageToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->helpToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->aboutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->scanForBarcodeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
			this->toolStripButton1 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton2 = (gcnew System::Windows::Forms::ToolStripButton());
			this->openFileDialogStartPoints = (gcnew System::Windows::Forms::OpenFileDialog());
			this->saveFileDialogStartPoints = (gcnew System::Windows::Forms::SaveFileDialog());
			this->tabControl = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->cb_countLaterals = (gcnew System::Windows::Forms::CheckBox());
			this->cb_tipAngles = (gcnew System::Windows::Forms::CheckBox());
			this->b_fitImage = (gcnew System::Windows::Forms::Button());
			this->cb_gravitropic = (gcnew System::Windows::Forms::CheckBox());
			this->tabooPointChkBox = (gcnew System::Windows::Forms::CheckBox());
			this->groupBox8 = (gcnew System::Windows::Forms::GroupBox());
			this->b_setOutputFolder = (gcnew System::Windows::Forms::Button());
			this->tb_outputFolder = (gcnew System::Windows::Forms::TextBox());
			this->b_quickLoad = (gcnew System::Windows::Forms::Button());
			this->b_quickSave = (gcnew System::Windows::Forms::Button());
			this->Flow = (gcnew System::Windows::Forms::GroupBox());
			this->pictureBox7 = (gcnew System::Windows::Forms::PictureBox());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->pictureBox6 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox5 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox3 = (gcnew System::Windows::Forms::PictureBox());
			this->b_go = (gcnew System::Windows::Forms::Button());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->b_crop = (gcnew System::Windows::Forms::Button());
			this->pictureBox4 = (gcnew System::Windows::Forms::PictureBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->m_message = (gcnew System::Windows::Forms::Label());
			this->m_progress = (gcnew System::Windows::Forms::ProgressBar());
			this->groupBox5 = (gcnew System::Windows::Forms::GroupBox());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->tb_minsPerFile = (gcnew System::Windows::Forms::TextBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->b_SetCalib = (gcnew System::Windows::Forms::Button());
			this->pixLabel = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->b_loadPrevious = (gcnew System::Windows::Forms::Button());
			this->autoResizeCheck = (gcnew System::Windows::Forms::CheckBox());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->cb_bg_removal = (gcnew System::Windows::Forms::CheckBox());
			this->cb_bilat_filter = (gcnew System::Windows::Forms::CheckBox());
			this->cb_med_filter = (gcnew System::Windows::Forms::CheckBox());
			this->cb_gaus_filter = (gcnew System::Windows::Forms::CheckBox());
			this->cb_suppressHor = (gcnew System::Windows::Forms::CheckBox());
			this->cb_localmaxfilter = (gcnew System::Windows::Forms::CheckBox());
			this->cb_blowHighlights = (gcnew System::Windows::Forms::CheckBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->tb_blowHighlights = (gcnew System::Windows::Forms::TrackBar());
			this->cb_EnableEnhancing = (gcnew System::Windows::Forms::CheckBox());
			this->tabPage3 = (gcnew System::Windows::Forms::TabPage());
			this->brightness_text = (gcnew System::Windows::Forms::Label());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->tb_brightness = (gcnew System::Windows::Forms::TrackBar());
			this->groupBox10 = (gcnew System::Windows::Forms::GroupBox());
			this->cb_EvolveTipPoints = (gcnew System::Windows::Forms::CheckBox());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->numeric_tipPointsNum = (gcnew System::Windows::Forms::NumericUpDown());
			this->Comments = (gcnew System::Windows::Forms::TabPage());
			this->b_saveComments = (gcnew System::Windows::Forms::Button());
			this->rtext_comments = (gcnew System::Windows::Forms::RichTextBox());
			this->groupBox6 = (gcnew System::Windows::Forms::GroupBox());
			this->savePoints = (gcnew System::Windows::Forms::Button());
			this->loadPoints = (gcnew System::Windows::Forms::Button());
			this->b_saveBG = (gcnew System::Windows::Forms::Button());
			this->cb_selectBG = (gcnew System::Windows::Forms::CheckBox());
			this->b_loadBG = (gcnew System::Windows::Forms::Button());
			this->advancedOptions = (gcnew System::Windows::Forms::CheckBox());
			this->slider_BGprior = (gcnew System::Windows::Forms::TrackBar());
			this->stDevDisplay = (gcnew System::Windows::Forms::Label());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->cb_defaultToRoot = (gcnew System::Windows::Forms::CheckBox());
			this->lowerHystLabel = (gcnew System::Windows::Forms::Label());
			this->upperHystLabel = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->tb_lowerHyst = (gcnew System::Windows::Forms::TrackBar());
			this->tb_upperHyst = (gcnew System::Windows::Forms::TrackBar());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->motionModelTrackerBar = (gcnew System::Windows::Forms::TrackBar());
			this->autoSmoothCheck = (gcnew System::Windows::Forms::CheckBox());
			this->autoEnhanceCheck = (gcnew System::Windows::Forms::CheckBox());
			this->toolTip1 = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->cb_invert = (gcnew System::Windows::Forms::CheckBox());
			this->radioButton2 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton1 = (gcnew System::Windows::Forms::RadioButton());
			this->saveFileDialog_bgmask = (gcnew System::Windows::Forms::SaveFileDialog());
			this->openFileDialogBMP = (gcnew System::Windows::Forms::OpenFileDialog());
			this->groupBox7 = (gcnew System::Windows::Forms::GroupBox());
			this->outputFolderBrowser = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->tb_radius = (gcnew System::Windows::Forms::TextBox());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->groupBox9 = (gcnew System::Windows::Forms::GroupBox());
			this->cb_rot_CCW = (gcnew System::Windows::Forms::CheckBox());
			this->cb_rot_CW = (gcnew System::Windows::Forms::CheckBox());
			this->cb_flip_hor = (gcnew System::Windows::Forms::CheckBox());
			this->Mainwindowmenus->SuspendLayout();
			this->toolStrip1->SuspendLayout();
			this->tabControl->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->groupBox8->SuspendLayout();
			this->Flow->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox7))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox6))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox5))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox4))->BeginInit();
			this->groupBox5->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->tabPage2->SuspendLayout();
			this->groupBox2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->tb_blowHighlights))->BeginInit();
			this->tabPage3->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->tb_brightness))->BeginInit();
			this->groupBox10->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numeric_tipPointsNum))->BeginInit();
			this->Comments->SuspendLayout();
			this->groupBox6->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->slider_BGprior))->BeginInit();
			this->groupBox3->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->tb_lowerHyst))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->tb_upperHyst))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->motionModelTrackerBar))->BeginInit();
			this->groupBox4->SuspendLayout();
			this->groupBox7->SuspendLayout();
			this->groupBox9->SuspendLayout();
			this->SuspendLayout();
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			this->openFileDialog1->Filter = L"Windows Bitmap files|*.bmp|TIFF files|*.tif|JPEG files|*.jpg|All file types|*.*";
			this->openFileDialog1->FilterIndex = 3;
			this->openFileDialog1->Multiselect = true;
			this->openFileDialog1->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &Form1::openFileDialog1_FileOk);
			// 
			// Mainwindowmenus
			// 
			this->Mainwindowmenus->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->fileToolStripMenuItem, 
				this->helpToolStripMenuItem, this->toolsToolStripMenuItem});
			this->Mainwindowmenus->Location = System::Drawing::Point(0, 0);
			this->Mainwindowmenus->Name = L"Mainwindowmenus";
			this->Mainwindowmenus->RenderMode = System::Windows::Forms::ToolStripRenderMode::System;
			this->Mainwindowmenus->Size = System::Drawing::Size(565, 24);
			this->Mainwindowmenus->TabIndex = 2;
			this->Mainwindowmenus->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->openImageToolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// openImageToolStripMenuItem
			// 
			this->openImageToolStripMenuItem->Name = L"openImageToolStripMenuItem";
			this->openImageToolStripMenuItem->Size = System::Drawing::Size(148, 22);
			this->openImageToolStripMenuItem->Text = L"&Open Image...";
			this->openImageToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::openImageToolStripMenuItem_Click);
			// 
			// helpToolStripMenuItem
			// 
			this->helpToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->helpToolStripMenuItem1, 
				this->aboutToolStripMenuItem});
			this->helpToolStripMenuItem->Name = L"helpToolStripMenuItem";
			this->helpToolStripMenuItem->Size = System::Drawing::Size(44, 20);
			this->helpToolStripMenuItem->Text = L"Help";
			// 
			// helpToolStripMenuItem1
			// 
			this->helpToolStripMenuItem1->Name = L"helpToolStripMenuItem1";
			this->helpToolStripMenuItem1->ShortcutKeyDisplayString = L"";
			this->helpToolStripMenuItem1->ShortcutKeys = System::Windows::Forms::Keys::F1;
			this->helpToolStripMenuItem1->Size = System::Drawing::Size(121, 22);
			this->helpToolStripMenuItem1->Text = L"Help!";
			this->helpToolStripMenuItem1->Click += gcnew System::EventHandler(this, &Form1::helpToolStripMenuItem1_Click);
			// 
			// aboutToolStripMenuItem
			// 
			this->aboutToolStripMenuItem->Name = L"aboutToolStripMenuItem";
			this->aboutToolStripMenuItem->Size = System::Drawing::Size(121, 22);
			this->aboutToolStripMenuItem->Text = L"About...";
			this->aboutToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::aboutToolStripMenuItem_Click);
			// 
			// toolsToolStripMenuItem
			// 
			this->toolsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->scanForBarcodeToolStripMenuItem});
			this->toolsToolStripMenuItem->Name = L"toolsToolStripMenuItem";
			this->toolsToolStripMenuItem->Size = System::Drawing::Size(48, 20);
			this->toolsToolStripMenuItem->Text = L"Tools";
			// 
			// scanForBarcodeToolStripMenuItem
			// 
			this->scanForBarcodeToolStripMenuItem->Name = L"scanForBarcodeToolStripMenuItem";
			this->scanForBarcodeToolStripMenuItem->Size = System::Drawing::Size(163, 22);
			this->scanForBarcodeToolStripMenuItem->Text = L"Scan for barcode";
			this->scanForBarcodeToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::scanForBarcodeToolStripMenuItem_Click);
			// 
			// toolStrip1
			// 
			this->toolStrip1->AllowItemReorder = true;
			this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->toolStripButton1, 
				this->toolStripButton2});
			this->toolStrip1->Location = System::Drawing::Point(0, 24);
			this->toolStrip1->Name = L"toolStrip1";
			this->toolStrip1->RenderMode = System::Windows::Forms::ToolStripRenderMode::System;
			this->toolStrip1->Size = System::Drawing::Size(565, 25);
			this->toolStrip1->TabIndex = 3;
			this->toolStrip1->Text = L"toolStrip1";
			// 
			// toolStripButton1
			// 
			this->toolStripButton1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
			this->toolStripButton1->Enabled = false;
			this->toolStripButton1->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton1.Image")));
			this->toolStripButton1->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton1->Name = L"toolStripButton1";
			this->toolStripButton1->Size = System::Drawing::Size(56, 22);
			this->toolStripButton1->Text = L"Enhance";
			this->toolStripButton1->ToolTipText = L"Enhance contrast";
			this->toolStripButton1->Click += gcnew System::EventHandler(this, &Form1::toolStripButton1_Click);
			// 
			// toolStripButton2
			// 
			this->toolStripButton2->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
			this->toolStripButton2->Enabled = false;
			this->toolStripButton2->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton2.Image")));
			this->toolStripButton2->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton2->Name = L"toolStripButton2";
			this->toolStripButton2->Size = System::Drawing::Size(53, 22);
			this->toolStripButton2->Text = L"Smooth";
			this->toolStripButton2->Click += gcnew System::EventHandler(this, &Form1::toolStripButton2_Click);
			// 
			// openFileDialogStartPoints
			// 
			this->openFileDialogStartPoints->FileName = L"openFileDialog2";
			this->openFileDialogStartPoints->Filter = L"XML files|*.xml";
			// 
			// tabControl
			// 
			this->tabControl->Controls->Add(this->tabPage1);
			this->tabControl->Controls->Add(this->tabPage2);
			this->tabControl->Controls->Add(this->tabPage3);
			this->tabControl->Controls->Add(this->Comments);
			this->tabControl->Location = System::Drawing::Point(12, 52);
			this->tabControl->Name = L"tabControl";
			this->tabControl->SelectedIndex = 0;
			this->tabControl->Size = System::Drawing::Size(541, 371);
			this->tabControl->TabIndex = 9;
			// 
			// tabPage1
			// 
			this->tabPage1->BackColor = System::Drawing::Color::Transparent;
			this->tabPage1->Controls->Add(this->cb_countLaterals);
			this->tabPage1->Controls->Add(this->cb_tipAngles);
			this->tabPage1->Controls->Add(this->b_fitImage);
			this->tabPage1->Controls->Add(this->cb_gravitropic);
			this->tabPage1->Controls->Add(this->tabooPointChkBox);
			this->tabPage1->Controls->Add(this->groupBox8);
			this->tabPage1->Controls->Add(this->b_quickLoad);
			this->tabPage1->Controls->Add(this->b_quickSave);
			this->tabPage1->Controls->Add(this->Flow);
			this->tabPage1->Controls->Add(this->m_message);
			this->tabPage1->Controls->Add(this->m_progress);
			this->tabPage1->Controls->Add(this->groupBox5);
			this->tabPage1->Controls->Add(this->b_loadPrevious);
			this->tabPage1->Controls->Add(this->autoResizeCheck);
			this->tabPage1->Controls->Add(this->groupBox1);
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(533, 345);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"General";
			// 
			// cb_countLaterals
			// 
			this->cb_countLaterals->AutoSize = true;
			this->cb_countLaterals->Location = System::Drawing::Point(399, 132);
			this->cb_countLaterals->Name = L"cb_countLaterals";
			this->cb_countLaterals->Size = System::Drawing::Size(96, 17);
			this->cb_countLaterals->TabIndex = 36;
			this->cb_countLaterals->Text = L"Count laterals\?";
			this->toolTip1->SetToolTip(this->cb_countLaterals, L"Enable lateral root counting. Works best with typically straighter roots (versus " 
				L"highly bendy)");
			this->cb_countLaterals->UseVisualStyleBackColor = true;
			this->cb_countLaterals->CheckedChanged += gcnew System::EventHandler(this, &Form1::cb_countLaterals_CheckedChanged);
			// 
			// cb_tipAngles
			// 
			this->cb_tipAngles->AutoSize = true;
			this->cb_tipAngles->Location = System::Drawing::Point(399, 115);
			this->cb_tipAngles->Name = L"cb_tipAngles";
			this->cb_tipAngles->Size = System::Drawing::Size(81, 17);
			this->cb_tipAngles->TabIndex = 35;
			this->cb_tipAngles->Text = L"Tip angles\?";
			this->cb_tipAngles->UseVisualStyleBackColor = true;
			this->cb_tipAngles->CheckedChanged += gcnew System::EventHandler(this, &Form1::cb_tipAngles_CheckedChanged);
			// 
			// b_fitImage
			// 
			this->b_fitImage->Location = System::Drawing::Point(399, 150);
			this->b_fitImage->Name = L"b_fitImage";
			this->b_fitImage->Size = System::Drawing::Size(90, 27);
			this->b_fitImage->TabIndex = 35;
			this->b_fitImage->Text = L"Fit Image";
			this->toolTip1->SetToolTip(this->b_fitImage, L"Fits the window to the image");
			this->b_fitImage->UseVisualStyleBackColor = true;
			this->b_fitImage->Click += gcnew System::EventHandler(this, &Form1::b_fitImage_Click);
			// 
			// cb_gravitropic
			// 
			this->cb_gravitropic->AutoSize = true;
			this->cb_gravitropic->Location = System::Drawing::Point(399, 99);
			this->cb_gravitropic->Name = L"cb_gravitropic";
			this->cb_gravitropic->Size = System::Drawing::Size(83, 17);
			this->cb_gravitropic->TabIndex = 34;
			this->cb_gravitropic->Text = L"Gravitropic\?";
			this->toolTip1->SetToolTip(this->cb_gravitropic, L"Choose whether to do gravitropic (curvature) analysis in addition to straight for" 
				L"ward root growth");
			this->cb_gravitropic->UseVisualStyleBackColor = true;
			this->cb_gravitropic->CheckedChanged += gcnew System::EventHandler(this, &Form1::cb_gravitropic_CheckedChanged);
			// 
			// tabooPointChkBox
			// 
			this->tabooPointChkBox->AutoSize = true;
			this->tabooPointChkBox->Location = System::Drawing::Point(103, 204);
			this->tabooPointChkBox->Name = L"tabooPointChkBox";
			this->tabooPointChkBox->Size = System::Drawing::Size(88, 17);
			this->tabooPointChkBox->TabIndex = 34;
			this->tabooPointChkBox->Text = L"Taboo points";
			this->tabooPointChkBox->UseVisualStyleBackColor = true;
			this->tabooPointChkBox->Visible = false;
			this->tabooPointChkBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::tabooPointChkBox_CheckedChanged);
			// 
			// groupBox8
			// 
			this->groupBox8->Controls->Add(this->b_setOutputFolder);
			this->groupBox8->Controls->Add(this->tb_outputFolder);
			this->groupBox8->Location = System::Drawing::Point(390, 263);
			this->groupBox8->Name = L"groupBox8";
			this->groupBox8->Size = System::Drawing::Size(135, 75);
			this->groupBox8->TabIndex = 33;
			this->groupBox8->TabStop = false;
			this->groupBox8->Text = L"Output folder";
			// 
			// b_setOutputFolder
			// 
			this->b_setOutputFolder->Enabled = false;
			this->b_setOutputFolder->Location = System::Drawing::Point(9, 18);
			this->b_setOutputFolder->Name = L"b_setOutputFolder";
			this->b_setOutputFolder->Size = System::Drawing::Size(42, 23);
			this->b_setOutputFolder->TabIndex = 32;
			this->b_setOutputFolder->Text = L"Set";
			this->toolTip1->SetToolTip(this->b_setOutputFolder, L"Set the folder to store output files in. Default: image folder, or output subfold" 
				L"er if one exists");
			this->b_setOutputFolder->UseVisualStyleBackColor = true;
			this->b_setOutputFolder->Click += gcnew System::EventHandler(this, &Form1::b_setOutputFolder_Click);
			// 
			// tb_outputFolder
			// 
			this->tb_outputFolder->Location = System::Drawing::Point(9, 44);
			this->tb_outputFolder->Name = L"tb_outputFolder";
			this->tb_outputFolder->ReadOnly = true;
			this->tb_outputFolder->Size = System::Drawing::Size(121, 20);
			this->tb_outputFolder->TabIndex = 30;
			// 
			// b_quickLoad
			// 
			this->b_quickLoad->Enabled = false;
			this->b_quickLoad->Location = System::Drawing::Point(399, 181);
			this->b_quickLoad->Name = L"b_quickLoad";
			this->b_quickLoad->Size = System::Drawing::Size(91, 34);
			this->b_quickLoad->TabIndex = 29;
			this->b_quickLoad->Text = L"QuickLoad";
			this->toolTip1->SetToolTip(this->b_quickLoad, L"Quickload background regions and start points");
			this->b_quickLoad->UseVisualStyleBackColor = true;
			this->b_quickLoad->Click += gcnew System::EventHandler(this, &Form1::b_quickLoad_Click);
			// 
			// b_quickSave
			// 
			this->b_quickSave->Enabled = false;
			this->b_quickSave->Location = System::Drawing::Point(399, 223);
			this->b_quickSave->Name = L"b_quickSave";
			this->b_quickSave->Size = System::Drawing::Size(91, 34);
			this->b_quickSave->TabIndex = 28;
			this->b_quickSave->Text = L"QuickSave";
			this->toolTip1->SetToolTip(this->b_quickSave, L"Quicksave background regions and start points (can have one of these for each inp" 
				L"ut image folder)");
			this->b_quickSave->UseVisualStyleBackColor = true;
			this->b_quickSave->Click += gcnew System::EventHandler(this, &Form1::Q_Click);
			// 
			// Flow
			// 
			this->Flow->Controls->Add(this->pictureBox7);
			this->Flow->Controls->Add(this->label11);
			this->Flow->Controls->Add(this->pictureBox6);
			this->Flow->Controls->Add(this->pictureBox5);
			this->Flow->Controls->Add(this->pictureBox3);
			this->Flow->Controls->Add(this->b_go);
			this->Flow->Controls->Add(this->pictureBox2);
			this->Flow->Controls->Add(this->label8);
			this->Flow->Controls->Add(this->label7);
			this->Flow->Controls->Add(this->pictureBox1);
			this->Flow->Controls->Add(this->b_crop);
			this->Flow->Controls->Add(this->pictureBox4);
			this->Flow->Controls->Add(this->button1);
			this->Flow->Location = System::Drawing::Point(219, 13);
			this->Flow->Name = L"Flow";
			this->Flow->Size = System::Drawing::Size(174, 325);
			this->Flow->TabIndex = 27;
			this->Flow->TabStop = false;
			this->Flow->Text = L"Flow";
			// 
			// pictureBox7
			// 
			this->pictureBox7->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox7.Image")));
			this->pictureBox7->Location = System::Drawing::Point(58, 121);
			this->pictureBox7->Name = L"pictureBox7";
			this->pictureBox7->Size = System::Drawing::Size(34, 32);
			this->pictureBox7->TabIndex = 33;
			this->pictureBox7->TabStop = false;
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(37, 105);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(80, 13);
			this->label11->TabIndex = 32;
			this->label11->Text = L"Set calibration\?";
			// 
			// pictureBox6
			// 
			this->pictureBox6->BackColor = System::Drawing::Color::Transparent;
			this->pictureBox6->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox6.Image")));
			this->pictureBox6->Location = System::Drawing::Point(58, 71);
			this->pictureBox6->Name = L"pictureBox6";
			this->pictureBox6->Size = System::Drawing::Size(34, 32);
			this->pictureBox6->TabIndex = 31;
			this->pictureBox6->TabStop = false;
			// 
			// pictureBox5
			// 
			this->pictureBox5->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox5.Image")));
			this->pictureBox5->Location = System::Drawing::Point(131, 153);
			this->pictureBox5->Name = L"pictureBox5";
			this->pictureBox5->Size = System::Drawing::Size(19, 41);
			this->pictureBox5->TabIndex = 30;
			this->pictureBox5->TabStop = false;
			// 
			// pictureBox3
			// 
			this->pictureBox3->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox3.Image")));
			this->pictureBox3->Location = System::Drawing::Point(131, 223);
			this->pictureBox3->Name = L"pictureBox3";
			this->pictureBox3->Size = System::Drawing::Size(19, 41);
			this->pictureBox3->TabIndex = 28;
			this->pictureBox3->TabStop = false;
			// 
			// b_go
			// 
			this->b_go->Location = System::Drawing::Point(30, 300);
			this->b_go->Name = L"b_go";
			this->b_go->Size = System::Drawing::Size(97, 22);
			this->b_go->TabIndex = 26;
			this->b_go->Text = L"Go";
			this->toolTip1->SetToolTip(this->b_go, L"Start processing");
			this->b_go->UseVisualStyleBackColor = true;
			this->b_go->Click += gcnew System::EventHandler(this, &Form1::b_go_Click);
			// 
			// pictureBox2
			// 
			this->pictureBox2->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox2.Image")));
			this->pictureBox2->Location = System::Drawing::Point(58, 245);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(34, 32);
			this->pictureBox2->TabIndex = 25;
			this->pictureBox2->TabStop = false;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(6, 228);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(129, 13);
			this->label8->TabIndex = 24;
			this->label8->Text = L"and background patches.";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(6, 215);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(91, 13);
			this->label7->TabIndex = 23;
			this->label7->Text = L"Select start points";
			this->label7->Click += gcnew System::EventHandler(this, &Form1::label7_Click);
			// 
			// pictureBox1
			// 
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->Location = System::Drawing::Point(58, 178);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(34, 32);
			this->pictureBox1->TabIndex = 22;
			this->pictureBox1->TabStop = false;
			// 
			// b_crop
			// 
			this->b_crop->Enabled = false;
			this->b_crop->Location = System::Drawing::Point(26, 153);
			this->b_crop->Name = L"b_crop";
			this->b_crop->Size = System::Drawing::Size(102, 23);
			this->b_crop->TabIndex = 19;
			this->b_crop->Text = L"Crop";
			this->b_crop->UseVisualStyleBackColor = true;
			this->b_crop->Click += gcnew System::EventHandler(this, &Form1::b_crop_Click);
			// 
			// pictureBox4
			// 
			this->pictureBox4->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox4.Image")));
			this->pictureBox4->Location = System::Drawing::Point(99, 203);
			this->pictureBox4->Name = L"pictureBox4";
			this->pictureBox4->Size = System::Drawing::Size(19, 44);
			this->pictureBox4->TabIndex = 29;
			this->pictureBox4->TabStop = false;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(32, 19);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(91, 51);
			this->button1->TabIndex = 9;
			this->button1->Text = L"&Import images";
			this->toolTip1->SetToolTip(this->button1, L"Select which images to process");
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click_1);
			// 
			// m_message
			// 
			this->m_message->AutoSize = true;
			this->m_message->Location = System::Drawing::Point(6, 165);
			this->m_message->Name = L"m_message";
			this->m_message->Size = System::Drawing::Size(62, 13);
			this->m_message->TabIndex = 21;
			this->m_message->Text = L"Not running";
			// 
			// m_progress
			// 
			this->m_progress->Location = System::Drawing::Point(6, 148);
			this->m_progress->Name = L"m_progress";
			this->m_progress->Size = System::Drawing::Size(197, 10);
			this->m_progress->Step = 5;
			this->m_progress->TabIndex = 20;
			// 
			// groupBox5
			// 
			this->groupBox5->Controls->Add(this->label10);
			this->groupBox5->Controls->Add(this->label9);
			this->groupBox5->Controls->Add(this->tb_minsPerFile);
			this->groupBox5->Controls->Add(this->label6);
			this->groupBox5->Controls->Add(this->b_SetCalib);
			this->groupBox5->Controls->Add(this->pixLabel);
			this->groupBox5->Controls->Add(this->label5);
			this->groupBox5->Controls->Add(this->textBox2);
			this->groupBox5->Location = System::Drawing::Point(0, 211);
			this->groupBox5->Name = L"groupBox5";
			this->groupBox5->Size = System::Drawing::Size(164, 127);
			this->groupBox5->TabIndex = 18;
			this->groupBox5->TabStop = false;
			this->groupBox5->Text = L"Calibration";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(3, 104);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(108, 13);
			this->label10->TabIndex = 24;
			this->label10->Text = L"Default (5 min/image)";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(65, 76);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(56, 13);
			this->label9->TabIndex = 23;
			this->label9->Text = L"min/image";
			// 
			// tb_minsPerFile
			// 
			this->tb_minsPerFile->Enabled = false;
			this->tb_minsPerFile->Location = System::Drawing::Point(11, 74);
			this->tb_minsPerFile->Name = L"tb_minsPerFile";
			this->tb_minsPerFile->Size = System::Drawing::Size(48, 20);
			this->tb_minsPerFile->TabIndex = 22;
			this->tb_minsPerFile->Text = L"5";
			this->toolTip1->SetToolTip(this->tb_minsPerFile, L"Enter the number of minutes between images");
			this->tb_minsPerFile->TextChanged += gcnew System::EventHandler(this, &Form1::tb_minsPerFile_TextChanged);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(3, 48);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(99, 13);
			this->label6->TabIndex = 21;
			this->label6->Text = L"Default (1.0mm/pix)";
			// 
			// b_SetCalib
			// 
			this->b_SetCalib->Enabled = false;
			this->b_SetCalib->Location = System::Drawing::Point(112, 101);
			this->b_SetCalib->Name = L"b_SetCalib";
			this->b_SetCalib->Size = System::Drawing::Size(50, 20);
			this->b_SetCalib->TabIndex = 20;
			this->b_SetCalib->Text = L"Set";
			this->b_SetCalib->UseVisualStyleBackColor = true;
			this->b_SetCalib->Click += gcnew System::EventHandler(this, &Form1::b_SetCalib_Click);
			// 
			// pixLabel
			// 
			this->pixLabel->AutoSize = true;
			this->pixLabel->Location = System::Drawing::Point(101, 22);
			this->pixLabel->Name = L"pixLabel";
			this->pixLabel->Size = System::Drawing::Size(0, 13);
			this->pixLabel->TabIndex = 19;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(65, 22);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(41, 13);
			this->label5->TabIndex = 18;
			this->label5->Text = L"mm/pix";
			// 
			// textBox2
			// 
			this->textBox2->Enabled = false;
			this->textBox2->Location = System::Drawing::Point(11, 18);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(48, 20);
			this->textBox2->TabIndex = 17;
			this->textBox2->Text = L"1";
			this->toolTip1->SetToolTip(this->textBox2, L"Enter the number of mm per pixel");
			this->textBox2->TextChanged += gcnew System::EventHandler(this, &Form1::textBox2_TextChanged);
			// 
			// b_loadPrevious
			// 
			this->b_loadPrevious->Location = System::Drawing::Point(399, 43);
			this->b_loadPrevious->Name = L"b_loadPrevious";
			this->b_loadPrevious->Size = System::Drawing::Size(91, 28);
			this->b_loadPrevious->TabIndex = 15;
			this->b_loadPrevious->Text = L"[Load &previous]";
			this->toolTip1->SetToolTip(this->b_loadPrevious, L"Load the previous image set");
			this->b_loadPrevious->UseVisualStyleBackColor = true;
			this->b_loadPrevious->Click += gcnew System::EventHandler(this, &Form1::b_loadPrevious_Click);
			// 
			// autoResizeCheck
			// 
			this->autoResizeCheck->AutoSize = true;
			this->autoResizeCheck->Location = System::Drawing::Point(103, 170);
			this->autoResizeCheck->Name = L"autoResizeCheck";
			this->autoResizeCheck->Size = System::Drawing::Size(87, 17);
			this->autoResizeCheck->TabIndex = 11;
			this->autoResizeCheck->Text = L"Resize 50%\?";
			this->toolTip1->SetToolTip(this->autoResizeCheck, L"Resize very large images by a fixed amount");
			this->autoResizeCheck->UseVisualStyleBackColor = true;
			this->autoResizeCheck->Visible = false;
			this->autoResizeCheck->CheckedChanged += gcnew System::EventHandler(this, &Form1::autoResizeCheck_CheckedChanged);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->button2);
			this->groupBox1->Controls->Add(this->textBox1);
			this->groupBox1->Location = System::Drawing::Point(4, 13);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(201, 120);
			this->groupBox1->TabIndex = 10;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Console";
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(8, 81);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(87, 22);
			this->button2->TabIndex = 4;
			this->button2->Text = L"Clear";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click_1);
			// 
			// textBox1
			// 
			this->textBox1->BackColor = System::Drawing::Color::Black;
			this->textBox1->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->textBox1->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(192)), 
				static_cast<System::Int32>(static_cast<System::Byte>(0)));
			this->textBox1->Location = System::Drawing::Point(4, 14);
			this->textBox1->Multiline = true;
			this->textBox1->Name = L"textBox1";
			this->textBox1->ReadOnly = true;
			this->textBox1->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->textBox1->Size = System::Drawing::Size(182, 56);
			this->textBox1->TabIndex = 1;
			this->textBox1->Text = L"Welcome to RootTrace 0.9.6";
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->groupBox2);
			this->tabPage2->Controls->Add(this->cb_EnableEnhancing);
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(533, 345);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"Enhance Image";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->cb_bg_removal);
			this->groupBox2->Controls->Add(this->cb_bilat_filter);
			this->groupBox2->Controls->Add(this->cb_med_filter);
			this->groupBox2->Controls->Add(this->cb_gaus_filter);
			this->groupBox2->Controls->Add(this->cb_suppressHor);
			this->groupBox2->Controls->Add(this->cb_localmaxfilter);
			this->groupBox2->Controls->Add(this->cb_blowHighlights);
			this->groupBox2->Controls->Add(this->label1);
			this->groupBox2->Controls->Add(this->tb_blowHighlights);
			this->groupBox2->Location = System::Drawing::Point(13, 42);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(514, 302);
			this->groupBox2->TabIndex = 1;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Enhance";
			// 
			// cb_bg_removal
			// 
			this->cb_bg_removal->AutoSize = true;
			this->cb_bg_removal->Location = System::Drawing::Point(286, 23);
			this->cb_bg_removal->Name = L"cb_bg_removal";
			this->cb_bg_removal->Size = System::Drawing::Size(220, 17);
			this->cb_bg_removal->TabIndex = 8;
			this->cb_bg_removal->Text = L"Remove background (expects light roots)";
			this->toolTip1->SetToolTip(this->cb_bg_removal, L"Applies a large blur to reveal low-frequency background features, and then \r\nperf" 
				L"orms a subtraction. Can help with uneven illumination.");
			this->cb_bg_removal->UseVisualStyleBackColor = true;
			this->cb_bg_removal->CheckedChanged += gcnew System::EventHandler(this, &Form1::interactiveEnhance);
			// 
			// cb_bilat_filter
			// 
			this->cb_bilat_filter->AutoSize = true;
			this->cb_bilat_filter->Location = System::Drawing::Point(10, 63);
			this->cb_bilat_filter->Name = L"cb_bilat_filter";
			this->cb_bilat_filter->Size = System::Drawing::Size(85, 17);
			this->cb_bilat_filter->TabIndex = 7;
			this->cb_bilat_filter->Text = L"Bilateral filter";
			this->cb_bilat_filter->UseVisualStyleBackColor = true;
			this->cb_bilat_filter->CheckedChanged += gcnew System::EventHandler(this, &Form1::interactiveEnhance);
			// 
			// cb_med_filter
			// 
			this->cb_med_filter->AutoSize = true;
			this->cb_med_filter->Location = System::Drawing::Point(10, 43);
			this->cb_med_filter->Name = L"cb_med_filter";
			this->cb_med_filter->Size = System::Drawing::Size(83, 17);
			this->cb_med_filter->TabIndex = 6;
			this->cb_med_filter->Text = L"Median filter";
			this->cb_med_filter->UseVisualStyleBackColor = true;
			this->cb_med_filter->CheckedChanged += gcnew System::EventHandler(this, &Form1::interactiveEnhance);
			// 
			// cb_gaus_filter
			// 
			this->cb_gaus_filter->AutoSize = true;
			this->cb_gaus_filter->Location = System::Drawing::Point(10, 23);
			this->cb_gaus_filter->Name = L"cb_gaus_filter";
			this->cb_gaus_filter->Size = System::Drawing::Size(92, 17);
			this->cb_gaus_filter->TabIndex = 5;
			this->cb_gaus_filter->Text = L"Gaussian filter";
			this->cb_gaus_filter->UseVisualStyleBackColor = true;
			this->cb_gaus_filter->CheckedChanged += gcnew System::EventHandler(this, &Form1::interactiveEnhance);
			// 
			// cb_suppressHor
			// 
			this->cb_suppressHor->AutoSize = true;
			this->cb_suppressHor->Location = System::Drawing::Point(10, 219);
			this->cb_suppressHor->Name = L"cb_suppressHor";
			this->cb_suppressHor->Size = System::Drawing::Size(142, 17);
			this->cb_suppressHor->TabIndex = 4;
			this->cb_suppressHor->Text = L"Suppress horizontal lines";
			this->cb_suppressHor->UseVisualStyleBackColor = true;
			this->cb_suppressHor->CheckedChanged += gcnew System::EventHandler(this, &Form1::interactiveEnhance);
			// 
			// cb_localmaxfilter
			// 
			this->cb_localmaxfilter->AutoSize = true;
			this->cb_localmaxfilter->Enabled = false;
			this->cb_localmaxfilter->Location = System::Drawing::Point(10, 184);
			this->cb_localmaxfilter->Name = L"cb_localmaxfilter";
			this->cb_localmaxfilter->Size = System::Drawing::Size(100, 17);
			this->cb_localmaxfilter->TabIndex = 3;
			this->cb_localmaxfilter->Text = L"Local Max Filter";
			this->cb_localmaxfilter->UseVisualStyleBackColor = true;
			this->cb_localmaxfilter->CheckedChanged += gcnew System::EventHandler(this, &Form1::cb_EvolveTipPoints_CheckedChanged);
			// 
			// cb_blowHighlights
			// 
			this->cb_blowHighlights->AutoSize = true;
			this->cb_blowHighlights->Location = System::Drawing::Point(10, 120);
			this->cb_blowHighlights->Name = L"cb_blowHighlights";
			this->cb_blowHighlights->Size = System::Drawing::Size(15, 14);
			this->cb_blowHighlights->TabIndex = 2;
			this->cb_blowHighlights->UseVisualStyleBackColor = true;
			this->cb_blowHighlights->CheckedChanged += gcnew System::EventHandler(this, &Form1::interactiveEnhance);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(26, 120);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(77, 13);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Blow highlights";
			this->label1->Click += gcnew System::EventHandler(this, &Form1::label1_Click_1);
			// 
			// tb_blowHighlights
			// 
			this->tb_blowHighlights->BackColor = System::Drawing::SystemColors::ControlLightLight;
			this->tb_blowHighlights->LargeChange = 10;
			this->tb_blowHighlights->Location = System::Drawing::Point(2, 140);
			this->tb_blowHighlights->Maximum = 255;
			this->tb_blowHighlights->Name = L"tb_blowHighlights";
			this->tb_blowHighlights->Size = System::Drawing::Size(193, 45);
			this->tb_blowHighlights->TabIndex = 0;
			this->tb_blowHighlights->TickFrequency = 20;
			this->tb_blowHighlights->Value = 255;
			this->tb_blowHighlights->Scroll += gcnew System::EventHandler(this, &Form1::interactiveEnhance);
			// 
			// cb_EnableEnhancing
			// 
			this->cb_EnableEnhancing->AutoSize = true;
			this->cb_EnableEnhancing->Location = System::Drawing::Point(12, 19);
			this->cb_EnableEnhancing->Name = L"cb_EnableEnhancing";
			this->cb_EnableEnhancing->Size = System::Drawing::Size(112, 17);
			this->cb_EnableEnhancing->TabIndex = 0;
			this->cb_EnableEnhancing->Text = L"Enable enhancing";
			this->cb_EnableEnhancing->UseVisualStyleBackColor = true;
			this->cb_EnableEnhancing->CheckedChanged += gcnew System::EventHandler(this, &Form1::cb_EnableEnhancing_CheckedChanged);
			// 
			// tabPage3
			// 
			this->tabPage3->Controls->Add(this->brightness_text);
			this->tabPage3->Controls->Add(this->label16);
			this->tabPage3->Controls->Add(this->tb_brightness);
			this->tabPage3->Controls->Add(this->groupBox10);
			this->tabPage3->Location = System::Drawing::Point(4, 22);
			this->tabPage3->Name = L"tabPage3";
			this->tabPage3->Padding = System::Windows::Forms::Padding(3);
			this->tabPage3->Size = System::Drawing::Size(533, 345);
			this->tabPage3->TabIndex = 2;
			this->tabPage3->Text = L"Misc";
			this->tabPage3->UseVisualStyleBackColor = true;
			// 
			// brightness_text
			// 
			this->brightness_text->AutoSize = true;
			this->brightness_text->Location = System::Drawing::Point(154, 147);
			this->brightness_text->Name = L"brightness_text";
			this->brightness_text->Size = System::Drawing::Size(25, 13);
			this->brightness_text->TabIndex = 6;
			this->brightness_text->Text = L"255";
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Location = System::Drawing::Point(16, 148);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(102, 13);
			this->label16->TabIndex = 5;
			this->label16->Text = L"Ideal root brightness";
			// 
			// tb_brightness
			// 
			this->tb_brightness->BackColor = System::Drawing::SystemColors::ControlLightLight;
			this->tb_brightness->LargeChange = 50;
			this->tb_brightness->Location = System::Drawing::Point(11, 173);
			this->tb_brightness->Maximum = 255;
			this->tb_brightness->Minimum = 1;
			this->tb_brightness->Name = L"tb_brightness";
			this->tb_brightness->Size = System::Drawing::Size(206, 45);
			this->tb_brightness->TabIndex = 4;
			this->tb_brightness->TickFrequency = 50;
			this->tb_brightness->Value = 1;
			this->tb_brightness->Scroll += gcnew System::EventHandler(this, &Form1::brightnessScroll);
			// 
			// groupBox10
			// 
			this->groupBox10->Controls->Add(this->cb_EvolveTipPoints);
			this->groupBox10->Controls->Add(this->label15);
			this->groupBox10->Controls->Add(this->numeric_tipPointsNum);
			this->groupBox10->Location = System::Drawing::Point(11, 15);
			this->groupBox10->Name = L"groupBox10";
			this->groupBox10->Size = System::Drawing::Size(207, 97);
			this->groupBox10->TabIndex = 3;
			this->groupBox10->TabStop = false;
			this->groupBox10->Text = L"Tip angle fitting";
			// 
			// cb_EvolveTipPoints
			// 
			this->cb_EvolveTipPoints->AutoSize = true;
			this->cb_EvolveTipPoints->Location = System::Drawing::Point(70, 46);
			this->cb_EvolveTipPoints->Name = L"cb_EvolveTipPoints";
			this->cb_EvolveTipPoints->Size = System::Drawing::Size(104, 17);
			this->cb_EvolveTipPoints->TabIndex = 2;
			this->cb_EvolveTipPoints->Text = L"Evolve tip points";
			this->cb_EvolveTipPoints->UseVisualStyleBackColor = true;
			this->cb_EvolveTipPoints->CheckedChanged += gcnew System::EventHandler(this, &Form1::cb_EvolveTipPoints_CheckedChanged);
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(5, 21);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(112, 13);
			this->label15->TabIndex = 1;
			this->label15->Text = L"Tip points to fit (2 - 30)";
			// 
			// numeric_tipPointsNum
			// 
			this->numeric_tipPointsNum->Location = System::Drawing::Point(120, 19);
			this->numeric_tipPointsNum->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {30, 0, 0, 0});
			this->numeric_tipPointsNum->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {2, 0, 0, 0});
			this->numeric_tipPointsNum->Name = L"numeric_tipPointsNum";
			this->numeric_tipPointsNum->Size = System::Drawing::Size(54, 20);
			this->numeric_tipPointsNum->TabIndex = 0;
			this->toolTip1->SetToolTip(this->numeric_tipPointsNum, L"Enter the number of points back from the tip to fit through to get tip angle");
			this->numeric_tipPointsNum->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {2, 0, 0, 0});
			this->numeric_tipPointsNum->ValueChanged += gcnew System::EventHandler(this, &Form1::numeric_tipPointsNum_ValueChanged);
			// 
			// Comments
			// 
			this->Comments->Controls->Add(this->b_saveComments);
			this->Comments->Controls->Add(this->rtext_comments);
			this->Comments->Location = System::Drawing::Point(4, 22);
			this->Comments->Name = L"Comments";
			this->Comments->Padding = System::Windows::Forms::Padding(3);
			this->Comments->Size = System::Drawing::Size(533, 345);
			this->Comments->TabIndex = 3;
			this->Comments->Text = L"Comments";
			this->Comments->UseVisualStyleBackColor = true;
			// 
			// b_saveComments
			// 
			this->b_saveComments->Enabled = false;
			this->b_saveComments->Location = System::Drawing::Point(390, 194);
			this->b_saveComments->Name = L"b_saveComments";
			this->b_saveComments->Size = System::Drawing::Size(75, 23);
			this->b_saveComments->TabIndex = 1;
			this->b_saveComments->Text = L"Save";
			this->b_saveComments->UseVisualStyleBackColor = true;
			this->b_saveComments->Click += gcnew System::EventHandler(this, &Form1::b_saveComments_Click);
			// 
			// rtext_comments
			// 
			this->rtext_comments->Enabled = false;
			this->rtext_comments->Location = System::Drawing::Point(39, 24);
			this->rtext_comments->Name = L"rtext_comments";
			this->rtext_comments->Size = System::Drawing::Size(442, 152);
			this->rtext_comments->TabIndex = 0;
			this->rtext_comments->Text = L"";
			// 
			// groupBox6
			// 
			this->groupBox6->Controls->Add(this->savePoints);
			this->groupBox6->Controls->Add(this->loadPoints);
			this->groupBox6->Location = System::Drawing::Point(14, 25);
			this->groupBox6->Name = L"groupBox6";
			this->groupBox6->Size = System::Drawing::Size(118, 88);
			this->groupBox6->TabIndex = 17;
			this->groupBox6->TabStop = false;
			this->groupBox6->Text = L"Start points";
			// 
			// savePoints
			// 
			this->savePoints->Enabled = false;
			this->savePoints->Location = System::Drawing::Point(17, 52);
			this->savePoints->Name = L"savePoints";
			this->savePoints->Size = System::Drawing::Size(77, 28);
			this->savePoints->TabIndex = 8;
			this->savePoints->Text = L"Save";
			this->toolTip1->SetToolTip(this->savePoints, L"Save your set of start points");
			this->savePoints->UseVisualStyleBackColor = true;
			this->savePoints->Click += gcnew System::EventHandler(this, &Form1::savePoints_Click_1);
			// 
			// loadPoints
			// 
			this->loadPoints->Enabled = false;
			this->loadPoints->Location = System::Drawing::Point(17, 17);
			this->loadPoints->Name = L"loadPoints";
			this->loadPoints->Size = System::Drawing::Size(77, 29);
			this->loadPoints->TabIndex = 7;
			this->loadPoints->Text = L"Load";
			this->toolTip1->SetToolTip(this->loadPoints, L"Load a previous set of start points");
			this->loadPoints->UseVisualStyleBackColor = true;
			this->loadPoints->Click += gcnew System::EventHandler(this, &Form1::loadPoints_Click);
			// 
			// b_saveBG
			// 
			this->b_saveBG->Location = System::Drawing::Point(15, 184);
			this->b_saveBG->Name = L"b_saveBG";
			this->b_saveBG->Size = System::Drawing::Size(107, 23);
			this->b_saveBG->TabIndex = 16;
			this->b_saveBG->Text = L"Save background";
			this->b_saveBG->UseVisualStyleBackColor = true;
			this->b_saveBG->Click += gcnew System::EventHandler(this, &Form1::b_saveBG_Click);
			// 
			// cb_selectBG
			// 
			this->cb_selectBG->Appearance = System::Windows::Forms::Appearance::Button;
			this->cb_selectBG->AutoSize = true;
			this->cb_selectBG->Checked = true;
			this->cb_selectBG->CheckState = System::Windows::Forms::CheckState::Checked;
			this->cb_selectBG->Location = System::Drawing::Point(15, 126);
			this->cb_selectBG->Name = L"cb_selectBG";
			this->cb_selectBG->Size = System::Drawing::Size(107, 23);
			this->cb_selectBG->TabIndex = 13;
			this->cb_selectBG->Text = L"Select background";
			this->cb_selectBG->UseVisualStyleBackColor = true;
			this->cb_selectBG->CheckedChanged += gcnew System::EventHandler(this, &Form1::cb_selectBG_CheckedChanged);
			// 
			// b_loadBG
			// 
			this->b_loadBG->Location = System::Drawing::Point(15, 155);
			this->b_loadBG->Name = L"b_loadBG";
			this->b_loadBG->Size = System::Drawing::Size(107, 23);
			this->b_loadBG->TabIndex = 14;
			this->b_loadBG->Text = L"Load background file";
			this->b_loadBG->UseVisualStyleBackColor = true;
			this->b_loadBG->Click += gcnew System::EventHandler(this, &Form1::b_loadBG_Click);
			// 
			// advancedOptions
			// 
			this->advancedOptions->Appearance = System::Windows::Forms::Appearance::Button;
			this->advancedOptions->AutoSize = true;
			this->advancedOptions->Location = System::Drawing::Point(456, 429);
			this->advancedOptions->Name = L"advancedOptions";
			this->advancedOptions->Size = System::Drawing::Size(81, 23);
			this->advancedOptions->TabIndex = 10;
			this->advancedOptions->Text = L"Adv&anced >>";
			this->toolTip1->SetToolTip(this->advancedOptions, L"Access some advanced settings");
			this->advancedOptions->UseVisualStyleBackColor = true;
			this->advancedOptions->CheckedChanged += gcnew System::EventHandler(this, &Form1::advancedOptions_CheckedChanged);
			// 
			// slider_BGprior
			// 
			this->slider_BGprior->LargeChange = 10;
			this->slider_BGprior->Location = System::Drawing::Point(9, 74);
			this->slider_BGprior->Maximum = 99;
			this->slider_BGprior->Minimum = 30;
			this->slider_BGprior->Name = L"slider_BGprior";
			this->slider_BGprior->Size = System::Drawing::Size(121, 45);
			this->slider_BGprior->TabIndex = 11;
			this->slider_BGprior->TickFrequency = 10;
			this->toolTip1->SetToolTip(this->slider_BGprior, L"Prior probability of background model");
			this->slider_BGprior->Value = 99;
			this->slider_BGprior->Scroll += gcnew System::EventHandler(this, &Form1::appModelStDev_Scroll);
			this->slider_BGprior->ValueChanged += gcnew System::EventHandler(this, &Form1::appModelStDev_ValueChanged);
			// 
			// stDevDisplay
			// 
			this->stDevDisplay->AutoSize = true;
			this->stDevDisplay->Location = System::Drawing::Point(132, 68);
			this->stDevDisplay->Name = L"stDevDisplay";
			this->stDevDisplay->Size = System::Drawing::Size(13, 13);
			this->stDevDisplay->TabIndex = 12;
			this->stDevDisplay->Text = L"0";
			this->stDevDisplay->Click += gcnew System::EventHandler(this, &Form1::stDevDisplay_Click);
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->label14);
			this->groupBox3->Controls->Add(this->label13);
			this->groupBox3->Controls->Add(this->cb_defaultToRoot);
			this->groupBox3->Controls->Add(this->lowerHystLabel);
			this->groupBox3->Controls->Add(this->upperHystLabel);
			this->groupBox3->Controls->Add(this->label4);
			this->groupBox3->Controls->Add(this->label3);
			this->groupBox3->Controls->Add(this->tb_lowerHyst);
			this->groupBox3->Controls->Add(this->tb_upperHyst);
			this->groupBox3->Controls->Add(this->label2);
			this->groupBox3->Controls->Add(this->stDevDisplay);
			this->groupBox3->Controls->Add(this->slider_BGprior);
			this->groupBox3->Controls->Add(this->motionModelTrackerBar);
			this->groupBox3->Location = System::Drawing::Point(12, 464);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(194, 234);
			this->groupBox3->TabIndex = 13;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"Tracking parameters";
			this->groupBox3->Enter += gcnew System::EventHandler(this, &Form1::groupBox3_Enter);
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(131, 26);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(0, 13);
			this->label14->TabIndex = 23;
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(16, 18);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(61, 13);
			this->label13->TabIndex = 22;
			this->label13->Text = L"Blender bar";
			this->label13->Click += gcnew System::EventHandler(this, &Form1::label13_Click);
			// 
			// cb_defaultToRoot
			// 
			this->cb_defaultToRoot->AutoSize = true;
			this->cb_defaultToRoot->Location = System::Drawing::Point(16, 211);
			this->cb_defaultToRoot->Name = L"cb_defaultToRoot";
			this->cb_defaultToRoot->Size = System::Drawing::Size(145, 17);
			this->cb_defaultToRoot->TabIndex = 20;
			this->cb_defaultToRoot->Text = L"End detection version #2";
			this->cb_defaultToRoot->UseVisualStyleBackColor = true;
			this->cb_defaultToRoot->CheckedChanged += gcnew System::EventHandler(this, &Form1::cb_defaultToRoot_CheckedChanged);
			// 
			// lowerHystLabel
			// 
			this->lowerHystLabel->AutoSize = true;
			this->lowerHystLabel->Location = System::Drawing::Point(131, 181);
			this->lowerHystLabel->Name = L"lowerHystLabel";
			this->lowerHystLabel->Size = System::Drawing::Size(35, 13);
			this->lowerHystLabel->TabIndex = 19;
			this->lowerHystLabel->Text = L"label6";
			// 
			// upperHystLabel
			// 
			this->upperHystLabel->AutoSize = true;
			this->upperHystLabel->Location = System::Drawing::Point(131, 127);
			this->upperHystLabel->Name = L"upperHystLabel";
			this->upperHystLabel->Size = System::Drawing::Size(35, 13);
			this->upperHystLabel->TabIndex = 18;
			this->upperHystLabel->Text = L"label5";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(16, 166);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(85, 13);
			this->label4->TabIndex = 17;
			this->label4->Text = L"Lower hysteresis";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(13, 112);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(85, 13);
			this->label3->TabIndex = 16;
			this->label3->Text = L"Upper hysteresis";
			// 
			// tb_lowerHyst
			// 
			this->tb_lowerHyst->LargeChange = 10;
			this->tb_lowerHyst->Location = System::Drawing::Point(8, 178);
			this->tb_lowerHyst->Maximum = 99;
			this->tb_lowerHyst->Name = L"tb_lowerHyst";
			this->tb_lowerHyst->Size = System::Drawing::Size(117, 45);
			this->tb_lowerHyst->TabIndex = 15;
			this->tb_lowerHyst->TickFrequency = 10;
			this->tb_lowerHyst->Value = 1;
			this->tb_lowerHyst->Scroll += gcnew System::EventHandler(this, &Form1::tb_lowerHyst_Scroll);
			// 
			// tb_upperHyst
			// 
			this->tb_upperHyst->LargeChange = 10;
			this->tb_upperHyst->Location = System::Drawing::Point(8, 124);
			this->tb_upperHyst->Maximum = 99;
			this->tb_upperHyst->Minimum = 1;
			this->tb_upperHyst->Name = L"tb_upperHyst";
			this->tb_upperHyst->Size = System::Drawing::Size(117, 45);
			this->tb_upperHyst->TabIndex = 14;
			this->tb_upperHyst->TickFrequency = 10;
			this->tb_upperHyst->Value = 97;
			this->tb_upperHyst->Scroll += gcnew System::EventHandler(this, &Form1::tb_upperHyst_Scroll);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(14, 63);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(88, 13);
			this->label2->TabIndex = 13;
			this->label2->Text = L"Background prior";
			this->label2->Click += gcnew System::EventHandler(this, &Form1::label2_Click);
			// 
			// motionModelTrackerBar
			// 
			this->motionModelTrackerBar->LargeChange = 1;
			this->motionModelTrackerBar->Location = System::Drawing::Point(10, 32);
			this->motionModelTrackerBar->Name = L"motionModelTrackerBar";
			this->motionModelTrackerBar->Size = System::Drawing::Size(116, 45);
			this->motionModelTrackerBar->TabIndex = 21;
			this->motionModelTrackerBar->Scroll += gcnew System::EventHandler(this, &Form1::motionModelTrackerBar_Scroll);
			// 
			// autoSmoothCheck
			// 
			this->autoSmoothCheck->AutoSize = true;
			this->autoSmoothCheck->Location = System::Drawing::Point(377, 657);
			this->autoSmoothCheck->Name = L"autoSmoothCheck";
			this->autoSmoothCheck->Size = System::Drawing::Size(167, 17);
			this->autoSmoothCheck->TabIndex = 14;
			this->autoSmoothCheck->Text = L"Automatically &smooth images\?";
			this->autoSmoothCheck->UseVisualStyleBackColor = true;
			this->autoSmoothCheck->CheckedChanged += gcnew System::EventHandler(this, &Form1::autoSmoothCheck_CheckedChanged);
			// 
			// autoEnhanceCheck
			// 
			this->autoEnhanceCheck->AutoSize = true;
			this->autoEnhanceCheck->Location = System::Drawing::Point(377, 680);
			this->autoEnhanceCheck->Name = L"autoEnhanceCheck";
			this->autoEnhanceCheck->Size = System::Drawing::Size(176, 17);
			this->autoEnhanceCheck->TabIndex = 15;
			this->autoEnhanceCheck->Text = L"Automatically &enhance Images\?";
			this->autoEnhanceCheck->UseVisualStyleBackColor = true;
			this->autoEnhanceCheck->CheckedChanged += gcnew System::EventHandler(this, &Form1::autoEnhanceCheck_CheckedChanged);
			// 
			// toolTip1
			// 
			this->toolTip1->Popup += gcnew System::Windows::Forms::PopupEventHandler(this, &Form1::toolTip1_Popup);
			// 
			// groupBox4
			// 
			this->groupBox4->Controls->Add(this->cb_invert);
			this->groupBox4->Controls->Add(this->radioButton2);
			this->groupBox4->Controls->Add(this->radioButton1);
			this->groupBox4->Location = System::Drawing::Point(382, 464);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Size = System::Drawing::Size(171, 82);
			this->groupBox4->TabIndex = 18;
			this->groupBox4->TabStop = false;
			this->groupBox4->Text = L"Process image on load";
			this->toolTip1->SetToolTip(this->groupBox4, L"Select whether the software will begin tracing left to right, or top to bottom, r" 
				L"elative to the image");
			// 
			// cb_invert
			// 
			this->cb_invert->AutoSize = true;
			this->cb_invert->Location = System::Drawing::Point(6, 22);
			this->cb_invert->Name = L"cb_invert";
			this->cb_invert->Size = System::Drawing::Size(53, 17);
			this->cb_invert->TabIndex = 18;
			this->cb_invert->Text = L"Invert";
			this->cb_invert->UseVisualStyleBackColor = true;
			this->cb_invert->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox1_CheckedChanged_1);
			// 
			// radioButton2
			// 
			this->radioButton2->AutoSize = true;
			this->radioButton2->Enabled = false;
			this->radioButton2->Location = System::Drawing::Point(32, 49);
			this->radioButton2->Name = L"radioButton2";
			this->radioButton2->Size = System::Drawing::Size(78, 17);
			this->radioButton2->TabIndex = 17;
			this->radioButton2->Text = L"Left to right";
			this->radioButton2->UseVisualStyleBackColor = true;
			this->radioButton2->Visible = false;
			// 
			// radioButton1
			// 
			this->radioButton1->AutoSize = true;
			this->radioButton1->Checked = true;
			this->radioButton1->Enabled = false;
			this->radioButton1->Location = System::Drawing::Point(32, 26);
			this->radioButton1->Name = L"radioButton1";
			this->radioButton1->Size = System::Drawing::Size(91, 17);
			this->radioButton1->TabIndex = 16;
			this->radioButton1->TabStop = true;
			this->radioButton1->Text = L"Top to bottom";
			this->radioButton1->UseVisualStyleBackColor = true;
			this->radioButton1->Visible = false;
			// 
			// openFileDialogBMP
			// 
			this->openFileDialogBMP->FileName = L"openFileDialogBMP";
			this->openFileDialogBMP->Filter = L"Windows Bitmap files|*.bmp|All file types|*.*";
			this->openFileDialogBMP->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &Form1::openFileDialog2_FileOk);
			// 
			// groupBox7
			// 
			this->groupBox7->Controls->Add(this->groupBox6);
			this->groupBox7->Controls->Add(this->b_saveBG);
			this->groupBox7->Controls->Add(this->cb_selectBG);
			this->groupBox7->Controls->Add(this->b_loadBG);
			this->groupBox7->Location = System::Drawing::Point(220, 464);
			this->groupBox7->Name = L"groupBox7";
			this->groupBox7->Size = System::Drawing::Size(147, 233);
			this->groupBox7->TabIndex = 19;
			this->groupBox7->TabStop = false;
			this->groupBox7->Text = L"Saving/Loading";
			// 
			// outputFolderBrowser
			// 
			this->outputFolderBrowser->Description = L"Please select a folder in which to place all output files";
			this->outputFolderBrowser->SelectedPath = L"C:\\";
			// 
			// tb_radius
			// 
			this->tb_radius->Location = System::Drawing::Point(512, 628);
			this->tb_radius->Name = L"tb_radius";
			this->tb_radius->Size = System::Drawing::Size(28, 20);
			this->tb_radius->TabIndex = 20;
			this->tb_radius->Text = L"3";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(379, 631);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(127, 13);
			this->label12->TabIndex = 21;
			this->label12->Text = L"Radius for tracking model";
			// 
			// groupBox9
			// 
			this->groupBox9->Controls->Add(this->cb_rot_CCW);
			this->groupBox9->Controls->Add(this->cb_rot_CW);
			this->groupBox9->Controls->Add(this->cb_flip_hor);
			this->groupBox9->Location = System::Drawing::Point(381, 559);
			this->groupBox9->Name = L"groupBox9";
			this->groupBox9->Size = System::Drawing::Size(172, 63);
			this->groupBox9->TabIndex = 22;
			this->groupBox9->TabStop = false;
			this->groupBox9->Text = L"Transform image on load";
			// 
			// cb_rot_CCW
			// 
			this->cb_rot_CCW->AutoSize = true;
			this->cb_rot_CCW->Location = System::Drawing::Point(82, 42);
			this->cb_rot_CCW->Name = L"cb_rot_CCW";
			this->cb_rot_CCW->Size = System::Drawing::Size(86, 17);
			this->cb_rot_CCW->TabIndex = 2;
			this->cb_rot_CCW->Text = L"Rotate CCW";
			this->cb_rot_CCW->UseVisualStyleBackColor = true;
			this->cb_rot_CCW->CheckedChanged += gcnew System::EventHandler(this, &Form1::cb_rot_CCW_CheckedChanged);
			// 
			// cb_rot_CW
			// 
			this->cb_rot_CW->AutoSize = true;
			this->cb_rot_CW->Location = System::Drawing::Point(6, 42);
			this->cb_rot_CW->Name = L"cb_rot_CW";
			this->cb_rot_CW->Size = System::Drawing::Size(79, 17);
			this->cb_rot_CW->TabIndex = 1;
			this->cb_rot_CW->Text = L"Rotate CW";
			this->cb_rot_CW->UseVisualStyleBackColor = true;
			this->cb_rot_CW->CheckedChanged += gcnew System::EventHandler(this, &Form1::cb_rot_CW_CheckedChanged);
			// 
			// cb_flip_hor
			// 
			this->cb_flip_hor->AutoSize = true;
			this->cb_flip_hor->Location = System::Drawing::Point(54, 19);
			this->cb_flip_hor->Name = L"cb_flip_hor";
			this->cb_flip_hor->Size = System::Drawing::Size(90, 17);
			this->cb_flip_hor->TabIndex = 0;
			this->cb_flip_hor->Text = L"Flip horizontal";
			this->cb_flip_hor->UseVisualStyleBackColor = true;
			this->cb_flip_hor->CheckedChanged += gcnew System::EventHandler(this, &Form1::cb_flip_hor_CheckedChanged);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(565, 708);
			this->Controls->Add(this->groupBox9);
			this->Controls->Add(this->label12);
			this->Controls->Add(this->tb_radius);
			this->Controls->Add(this->groupBox7);
			this->Controls->Add(this->groupBox4);
			this->Controls->Add(this->autoEnhanceCheck);
			this->Controls->Add(this->autoSmoothCheck);
			this->Controls->Add(this->groupBox3);
			this->Controls->Add(this->advancedOptions);
			this->Controls->Add(this->tabControl);
			this->Controls->Add(this->toolStrip1);
			this->Controls->Add(this->Mainwindowmenus);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->MainMenuStrip = this->Mainwindowmenus;
			this->Name = L"Form1";
			this->Text = L"RootTrace";
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &Form1::Form1_FormClosed);
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->Mainwindowmenus->ResumeLayout(false);
			this->Mainwindowmenus->PerformLayout();
			this->toolStrip1->ResumeLayout(false);
			this->toolStrip1->PerformLayout();
			this->tabControl->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->tabPage1->PerformLayout();
			this->groupBox8->ResumeLayout(false);
			this->groupBox8->PerformLayout();
			this->Flow->ResumeLayout(false);
			this->Flow->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox7))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox6))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox5))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox4))->EndInit();
			this->groupBox5->ResumeLayout(false);
			this->groupBox5->PerformLayout();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->tabPage2->ResumeLayout(false);
			this->tabPage2->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->tb_blowHighlights))->EndInit();
			this->tabPage3->ResumeLayout(false);
			this->tabPage3->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->tb_brightness))->EndInit();
			this->groupBox10->ResumeLayout(false);
			this->groupBox10->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numeric_tipPointsNum))->EndInit();
			this->Comments->ResumeLayout(false);
			this->groupBox6->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->slider_BGprior))->EndInit();
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->tb_lowerHyst))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->tb_upperHyst))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->motionModelTrackerBar))->EndInit();
			this->groupBox4->ResumeLayout(false);
			this->groupBox4->PerformLayout();
			this->groupBox7->ResumeLayout(false);
			this->groupBox7->PerformLayout();
			this->groupBox9->ResumeLayout(false);
			this->groupBox9->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	static CMainCode* mainprog = new CMainCode;
/*
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				 System::Windows::Forms::DialogResult result = this->openFileDialog1->ShowDialog();
				 if (result==System::Windows::Forms::DialogResult::OK) {
				 
				 //Load images button
				 
				 array<String^>^  s = this->openFileDialog1->FileNames;
				 textBox1->Text::set("Loading...");
				 toolStripButton1->Enabled::set(true);
				 toolStripButton2->Enabled::set(true);
				  b_setOutputFolder->Enabled::set(true);
				 loadPoints->Enabled::set(true);
				 savePoints->Enabled::set(true);
				 b_quickLoad->Enabled::set(true);
				 b_quickSave->Enabled::set(true);
				 b_crop->Enabled::set(true);
				 mainprog->setDoGravitropic(cb_gravitropic->Checked);
				 mainprog->setTabooPointsEnabled(tabooPointChkBox->Checked);

				

				 //try to set to output subfolder?
				 bool outputDirectoryExists = System::IO::Directory::Exists((s[0]->Substring(0, s[0]->LastIndexOf("\\")))+"\\output\\");
				 if (outputDirectoryExists) outputFolderBrowser->SelectedPath::set( (s[0]->Substring(0, s[0]->LastIndexOf("\\")))+"\\output\\");
				 else outputFolderBrowser->SelectedPath::set(s[0]->Substring(0, s[0]->LastIndexOf("\\")));

				 //outputFolderBrowser->SelectedPath::set(s[0]->Substring(0, s[0]->LastIndexOf("\\")));
				 tb_outputFolder->Text::set(outputFolderBrowser->SelectedPath::get());
				 outputFolderString = this->outputFolderBrowser->SelectedPath;
				 mainprog->setOutputFolderString(outputFolderString);

				 workingFolder = s[0]->Substring(0, s[0]->LastIndexOf("\\"));
				 this->Text = this->Text::get()+" - "+workingFolder;
				 mainprog->setWorkingFolder(workingFolder);

				 //help
				 help = gcnew System::Windows::Forms::HelpProvider;
				 help->HelpNamespace = "rthelp.chm";

				 help->SetShowHelp  ( b_quickSave, true );
				 help->SetHelpString( b_quickSave, "Save the background regions and start points " );

				 
				 int tb_lr;
				 if (radioButton1->Checked::get())
					 tb_lr = 0;
				 else
					 tb_lr = 1;

				 //disable stuff
				 radioButton1->Enabled::set(false);
				 radioButton2->Enabled::set(false);
				 //slider_BGprior->Enabled::set(false);
				 
				 //save file list, not sorted at this point (sorted in mainprog->init)
				 mainprog->saveFileList(s);

				 //Enable calib
				 textBox2->Enabled::set(true);
				 tb_minsPerFile->Enabled::set(true);
				 b_SetCalib->Enabled::set(true);

				 
				 this->motionMixture = (double)this->motionModelTrackerBar->Value::get();
				 mainprog->motionMixture = this->motionMixture;
				 this->label14->Text::set(motionMixture + " ");
				 if (motionMixture==0) this->label14->Text::set(motionMixture + " (Grav)");

				 b_saveComments->Enabled = true;
				 rtext_comments->Enabled = true;

				 //start main program
				 mainprog->init(s, idealr, idealg, idealb, tb_lr, processX, processY, this->motionMixture, this);
			 }
			 
			 }
*/ //K_I

	private: System::Void openFileDialog1_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) {
			 }
	private: System::Void aboutToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 About about;
			 about.ShowDialog();
			 }
private: System::Void openImageToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		 //same as button press
			 button1_Click(sender, e);
		 }
private: System::Void toolStripButton1_Click(System::Object^  sender, System::EventArgs^  e) {
			 //mainprog->enhance(this);
		 }
private: System::Void toolStripButton2_Click(System::Object^  sender, System::EventArgs^  e) {
			 //mainprog->smooth(this);
		 }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->textBox1->Clear();
		 }
private: System::Void checkBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		 }

public: bool autoResizeLargeImages() {
			if (this->autoResizeCheck->CheckState::get()== System::Windows::Forms::CheckState::Checked)
			{return true;}
			else
			{return false;}
			
		}

public: bool autoSmoothImages() {
			if (this->autoSmoothCheck->CheckState::get()== System::Windows::Forms::CheckState::Checked)
			{return true;}
			else
			{return false;}
			
		}

public: bool autoEnhanceImages() {
			if (this->autoEnhanceCheck->CheckState::get()== System::Windows::Forms::CheckState::Checked)
			{return true;}
			else
			{return false;}
			
		}
private: System::Void R_Click(System::Object^  sender, System::EventArgs^  e) {
			 // load points button

			     System::Windows::Forms::DialogResult result = this->openFileDialogStartPoints->ShowDialog();
				 if (result==System::Windows::Forms::DialogResult::OK) {
				 
				 //char* filename = (char*)(void*)Marshal::StringToHGlobalAnsi(this->saveFileDialogStartPoints->FileName);
				 
				 array<String^>^  s = this->openFileDialogStartPoints->FileNames;


				 mainprog->loadStartPoints(s[0]);
			}


		 }

			
		 
private: System::Void savePoints_Click(System::Object^  sender, System::EventArgs^  e) {
			  // save points button

			 System::Windows::Forms::DialogResult result = this->saveFileDialogStartPoints->ShowDialog();
				 if (result==System::Windows::Forms::DialogResult::OK) {
				 
				 //char* filename = (char*)(void*)Marshal::StringToHGlobalAnsi(this->saveFileDialogStartPoints->FileName);
				 
				 array<String^>^  s = this->saveFileDialogStartPoints->FileNames;


				 mainprog->saveStartPoints(s[0]);
			}


		 }
private: System::Void startPoints_Enter(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
			 //init
			 
			 this->Height -= 250;
			 this->stDevDisplay->Text::set(""+slider_BGprior_value);
			 this->slider_BGprior->Value::set(int(slider_BGprior_value*100));

			 this->upperHystLabel->Text::set(""+upperHyst);
			 this->tb_upperHyst->Value::set(int(upperHyst*100));

			 this->lowerHystLabel->Text::set(""+lowerHyst);
			 this->tb_lowerHyst->Value::set(int(lowerHyst*10000));

		 }
private: System::Void button1_Click_1(System::Object^  sender, System::EventArgs^  e) {
			 button1_Click(sender, e);		// redirected from conversion to tabbed pane
		 }
private: System::Void loadPoints_Click(System::Object^  sender, System::EventArgs^  e) {
			 R_Click(sender, e);			// redirected from conversion to tabbed pane
		 }
private: System::Void savePoints_Click_1(System::Object^  sender, System::EventArgs^  e) {
			 savePoints_Click(sender, e);	// redirected from conversion to tabbed pane
		 }
private: System::Void button2_Click_1(System::Object^  sender, System::EventArgs^  e) {
			 button2_Click(sender, e);		// redirected from conversion to tabbed pane
		 }
private: System::Void autoResizeCheck_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void advancedOptions_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (!advancedOptionsVisible) {
				 //show
				this->Height += 250;
				advancedOptionsVisible = true;
				this->advancedOptions->Text::set("Advanced <<");

			 } else{
				this->Height -= 250;
				advancedOptionsVisible = false;
				this->advancedOptions->Text::set("Advanced >>");

			 }
		 }
private: System::Void appModelStDev_Scroll(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void appModelStDev_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 slider_BGprior_value = (double)this->slider_BGprior->Value::get()/100;
			 this->stDevDisplay->Text::set(""+slider_BGprior_value);

		 }
private: System::Void stDevDisplay_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void pictureBox1_Click(System::Object^  sender, System::EventArgs^  e) {
		 }

public: void updateMeasureWindow(IplImage* measurei) {
			
			// This is BAD.  I should convert the IplImage to an Image^ rather than loading,
			// but I'm a bad person

			// This cod eis for MFC?? Whats the .NET equivalent
			/*HBITMAP hBitmap=CreateBitmap(measuresi->width,measuresi->height,
                          measuresi->nChannels*8,1, NULL);
			SetBitmapBits(hBitmap,measuresi->imageSize,measuresi->imageData);*/

			//Breaks and is terrible anyway, so just save.
			//previewMeasureImage->Image::set(Image::FromFile("c:\\measuresi.jpg"));
			//previewMeasureImage->Show();

			


			/*System::Drawing::Bitmap ^ bmp = gcnew System::Drawing::Bitmap(;
           
            // Lock bits and obtain a pointer to the data
            System::Drawing::Imaging::BitmapData ^ bt = bmp->LockBits(System::Drawing::Rectangle(0, 0, width, height),
                System::Drawing::Imaging::ImageLockMode::ReadOnly, System::Drawing::Imaging::PixelFormat::Format24bppRgb);

            // Copy data from bt to image_data
           
            unsigned char * imgPtr;
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    imgPtr = (unsigned char*)bt->Scan0.ToPointer() + y * bt->Stride + x * sizeof(unsigned char) * 3;
                    image_data[(y * width * 3) + (x * 3)] = *imgPtr;
                    image_data[(y * width * 3) + (x * 3) + 1] = *(imgPtr + 1);
                    image_data[(y * width * 3) + (x * 3) + 2] = *(imgPtr + 2);
                }
            }
            bmp->UnlockBits(bt); */


			if (measurei!=NULL) {
				cvNamedWindow("Measurement image", 2);
				cvShowImage("Measurement image", measurei);
				//cvWaitKey(0);
				//cvReleaseImage(&measurei);
			}
			
			
		 }

public: double getSlider_BGprior_value() {
		return slider_BGprior_value;
		}

public: double getSlider_upperHyst() {
			return upperHyst;
		}

public: double getSlider_lowerHyst() {
			return lowerHyst;
		}

public: bool getSelectBG() {
			return cb_selectBG->Checked::get();
		}

private: System::Void autoEnhanceCheck_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void b_updateMeasurei_Click(System::Object^  sender, System::EventArgs^  e) {
			
			 updateMeasureWindow(mainprog->getMeasuresi());
		 }
private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void toolTip1_Popup(System::Object^  sender, System::Windows::Forms::PopupEventArgs^  e) {
		 }
private: System::Void b_loadBG_Click(System::Object^  sender, System::EventArgs^  e) {

			  System::Windows::Forms::DialogResult result = this->openFileDialogBMP->ShowDialog();
				 if (result==System::Windows::Forms::DialogResult::OK) {
				 
				 //char* filename = (char*)(void*)Marshal::StringToHGlobalAnsi(this->saveFileDialogStartPoints->FileName);
				 
				 array<String^>^  s = this->openFileDialogBMP->FileNames;
				 //MessageBox::Show(s[0]);


				 mainprog->setBGMask(s[0]);
				 }

		 }
private: System::Void b_loadPrevious_Click(System::Object^  sender, System::EventArgs^  e) {
				
			 toolStripButton1->Enabled = true;
			 toolStripButton2->Enabled = true;
			 b_setOutputFolder->Enabled = true;
			 loadPoints->Enabled::set(true);
			 savePoints->Enabled::set(true);
			 b_crop->Enabled::set(true);
			 
			 //Enable Calib
			 b_SetCalib->Enabled = true;
			 textBox2->Enabled = true;
			 tb_minsPerFile->Enabled = true;

			 // Enable quicksaves 
			 b_quickLoad->Enabled::set(true);
			 b_quickSave->Enabled::set(true);

			 int tb_lr;
			 if (radioButton1->Checked::get())
				 tb_lr = 0;
			 else
				 tb_lr = 1;

			 //disable stuff
			 radioButton1->Enabled::set(false);
			 radioButton2->Enabled::set(false);
			 //slider_BGprior->Enabled::set(false);

			 array<System::String^>^ s = mainprog->loadFileLists();

			 //try to set to output subfolder?
			 bool outputDirectoryExists = System::IO::Directory::Exists((s[0]->Substring(0, s[0]->LastIndexOf("\\")))+"\\output\\");
			 if (outputDirectoryExists) outputFolderBrowser->SelectedPath::set( (s[0]->Substring(0, s[0]->LastIndexOf("\\")))+"\\output\\");
			 else outputFolderBrowser->SelectedPath::set(s[0]->Substring(0, s[0]->LastIndexOf("\\")));

			 //outputFolderBrowser->SelectedPath::set(s[0]->Substring(0, s[0]->LastIndexOf("\\")));
			 tb_outputFolder->Text::set(outputFolderBrowser->SelectedPath::get());
			 outputFolderString = this->outputFolderBrowser->SelectedPath;
			 mainprog->setOutputFolderString(outputFolderString);

			 workingFolder = s[0]->Substring(0, s[0]->LastIndexOf("\\"));
			 this->Text = this->Text::get()+" - "+workingFolder;
			 mainprog->setWorkingFolder(workingFolder);
			 
			 
			 this->motionMixture = (double)this->motionModelTrackerBar->Value::get();
			 mainprog->motionMixture = this->motionMixture;
			 this->label14->Text::set(motionMixture + " ");
			 if (motionMixture==0) this->label14->Text::set(motionMixture + " (Grav)");
			 
			 b_saveComments->Enabled = true;
			 rtext_comments->Enabled = true;

			 mainprog->init(s, idealr, idealg, idealb, tb_lr, processX, processY/*, this->motionMixture*/, this);
			 
			
			

		 }
private: System::Void cb_selectBG_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void b_saveBG_Click(System::Object^  sender, System::EventArgs^  e) {
			 System::Windows::Forms::DialogResult result = this->saveFileDialog_bgmask->ShowDialog();
				 if (result==System::Windows::Forms::DialogResult::OK) {
				 
				 //char* filename = (char*)(void*)Marshal::StringToHGlobalAnsi(this->saveFileDialogStartPoints->FileName);
				 
					 array<String^>^  s = this->saveFileDialog_bgmask->FileNames;


				 mainprog->saveBGMask(s[0]);
			}
		 }
private: System::Void openFileDialog2_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) {
		 }
private: System::Void groupBox3_Enter(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void label2_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void tb_upperHyst_Scroll(System::Object^  sender, System::EventArgs^  e) {
			 upperHyst = (double)this->tb_upperHyst->Value::get()/100;
			 this->upperHystLabel->Text::set(""+upperHyst);
		 }
private: System::Void tb_lowerHyst_Scroll(System::Object^  sender, System::EventArgs^  e) {
			 lowerHyst = (double)this->tb_lowerHyst->Value::get()/10000;
			 this->lowerHystLabel->Text::set(""+lowerHyst);
		 }
/*public: void enableCalib() {
			b_SetCalib->Enabled = true;
			textBox2->Enabled = true;
		private: System::Void scanForBarcodeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 }*/
private: System::Void motionModelTrackerBar_Scroll(System::Object^  sender, System::EventArgs^  e) {
			 this->motionMixture = (double)this->motionModelTrackerBar->Value::get();
			 this->label14->Text::set(motionMixture + " ");
			 if (motionMixture==0) this->label14->Text::set(motionMixture + " (Grav)");
			 mainprog->motionMixture = this->motionMixture;
		 
}

public: void setCalibLabel(System::String^ s) {
			label6->Text = s;
		}

public: void setCalibLabel2(System::String^ s) {
			label10->Text = s;
		}

private: System::Void b_SetCalib_Click(System::Object^  sender, System::EventArgs^  e) {
			 mainprog->setupCalib(textBox2->Text, tb_minsPerFile->Text, this);
			 b_SetCalib->BackColor::set(System::Drawing::Color::Transparent);
		 }
private: System::Void b_crop_Click(System::Object^  sender, System::EventArgs^  e) {
			 mainprog->prepareCrop();

		 }

public: System::Void setProgressBar(int value) {
			m_progress->Value = value;
		}

public: System::Void setMessage(System::String ^message) {
			m_message->Text = message;
		}

public: bool getDefaultToRootState() {
			//if true, then when we have no data for bayes rule (ie fg = bg = 0) then assume we are above high thresh rather than in middle thresh
			return cb_defaultToRoot->Checked;
}

public: System::Void grey() {
			//prevent re-running
			button1->Enabled = false;
			b_loadPrevious->Enabled = false;
			b_go->Enabled = false;
		}
private: System::Void pictureBox1_Click_1(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void label7_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void b_go_Click(System::Object^  sender, System::EventArgs^  e) {
			 
			 this->motionMixture = (double)this->motionModelTrackerBar->Value::get();
			 this->label14->Text::set(motionMixture + " ");
			 if (motionMixture==0) this->label14->Text::set(motionMixture + " (Grav)");
			 mainprog->motionMixture = this->motionMixture;
			 mainprog->setTabooPointsEnabled(tabooPointChkBox->Checked);
			 mainprog->setDoGravitropic(cb_gravitropic->Checked);

			 mainprog->toggleGoButtonPressed();
		 }
private: System::Void Q_Click(System::Object^  sender, System::EventArgs^  e) {
			 mainprog->saveStartPoints(workingFolder+"\\quicksave_starts.xml");
			 mainprog->saveBGMask(workingFolder+"\\quicksave_background.bmp");
		 }
private: System::Void b_quickLoad_Click(System::Object^  sender, System::EventArgs^  e) {
			 mainprog->loadStartPoints(workingFolder+"\\quicksave_starts.xml");
			 mainprog->setBGMask(workingFolder+"\\quicksave_background.bmp");
			
		 }
private: System::Void b_setOutputFolder_Click(System::Object^  sender, System::EventArgs^  e) {

			 System::Windows::Forms::DialogResult result = this->outputFolderBrowser->ShowDialog();
			 if (result==System::Windows::Forms::DialogResult::OK) {
			 
			 outputFolderString = this->outputFolderBrowser->SelectedPath;

			 tb_outputFolder->Text::set(outputFolderString);
			 mainprog->setOutputFolderString(outputFolderString);
			 }


		 }
private: System::Void textBox2_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 b_SetCalib->BackColor::set(System::Drawing::Color::Red);
		 }
private: System::Void tb_minsPerFile_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			  b_SetCalib->BackColor::set(System::Drawing::Color::Red);
		 }
private: System::Void autoSmoothCheck_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void cb_gravitropic_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 mainprog->setDoGravitropic(cb_gravitropic->Checked);
		 }
private: System::Void helpToolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e) {
			 Help::ShowHelp(this, "rthelp.chm");

		 }
private: System::Void Form1_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
			 cvDestroyAllWindows();
			 mainprog->onClose();
		 }
public: int getRadius() {
			return System::Convert::ToInt32(tb_radius->Text);
		 }
private: System::Void cb_tipAngles_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 mainprog->setDoTipAngles(cb_tipAngles->Checked);
			 //if (cb_tipAngles->Checked) {
				// //Add disclaimer about angle range  - should be ok now, July 2010
				// this->textBox1->Text = L"NOTE tip angle measures only correct for range 90 to 180� between tip and main root.";

			 //}
		 }
private: System::Void cb_flip_hor_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 mainprog->setImageTranform(cb_flip_hor->Checked, cb_rot_CW->Checked, cb_rot_CCW->Checked);
		 }
private: System::Void cb_rot_CW_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			  mainprog->setImageTranform(cb_flip_hor->Checked, cb_rot_CW->Checked, cb_rot_CCW->Checked);
		 }
private: System::Void cb_rot_CCW_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			  mainprog->setImageTranform(cb_flip_hor->Checked, cb_rot_CW->Checked, cb_rot_CCW->Checked);
		 }
private: System::Void cb_defaultToRoot_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void label13_Click(System::Object^  sender, System::EventArgs^  e) {
		 }

private: System::Void scanForBarcodeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 mainprog->findBarcodeInImage();
		 }
private: System::Void tabooPointChkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			  mainprog->setTabooPointsEnabled(tabooPointChkBox->Checked);
		 }
private: System::Void b_fitImage_Click(System::Object^  sender, System::EventArgs^  e) {
			 mainprog->fitWindowToImage();
		 }
private: System::Void numeric_tipPointsNum_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 mainprog->setTipPointsNumToFit(Decimal::ToInt32(numeric_tipPointsNum->Value));
			 //MessageBox::Show("Tip points set to "+Decimal::ToInt32(numeric_tipPointsNum->Value));

		 }
private: System::Void cb_EvolveTipPoints_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (cb_EvolveTipPoints->Checked) numeric_tipPointsNum->Enabled = false;
			 else numeric_tipPointsNum->Enabled = true;
			 mainprog->setEvolveTipPoints(cb_EvolveTipPoints->Checked);
			 
		 }


private: System::Void interactiveEnhance(System::Object^  sender, System::EventArgs^  e) {
			 // called on scroll
			 if (cb_EnableEnhancing->Checked) {  //if we're doing enhancing

				mainprog->interactiveEnhance(cb_bilat_filter->Checked, cb_med_filter->Checked, cb_gaus_filter->Checked, cb_suppressHor->Checked, cb_localmaxfilter->Checked, cb_blowHighlights->Checked, Decimal::ToInt32(tb_blowHighlights->Value), cb_bg_removal->Checked);

			 }

		 }
private: System::Void cb_EnableEnhancing_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 mainprog->setEnableEnhancing(cb_EnableEnhancing->Checked);
			 mainprog->interactiveEnhance(cb_bilat_filter->Checked, cb_med_filter->Checked, cb_gaus_filter->Checked, cb_suppressHor->Checked, cb_localmaxfilter->Checked, cb_blowHighlights->Checked, Decimal::ToInt32(tb_blowHighlights->Value), cb_bg_removal->Checked); //update image

		 }
private: System::Void label1_Click_1(System::Object^  sender, System::EventArgs^  e) {
		 }

private: System::Void brightnessScroll(System::Object^  sender, System::EventArgs^  e) {
			 this->brightness_text->Text = String::Concat( "", this->tb_brightness->Value);
			 mainprog->setIdealRGBinCond(this->tb_brightness->Value);

		 }
private: System::Void cb_countLaterals_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		 mainprog->setCountLaterals(cb_countLaterals->Checked);
		 }
private: System::Void b_saveComments_Click(System::Object^  sender, System::EventArgs^  e) {
			 mainprog->saveComments();

		 }
public: System::String^ getCommentString() { return rtext_comments->Text->ToString();}

public: System::Void setCalibBoxText(double a, double b) {
			//MessageBox::Show("calib = "+a+", "+b);
			this->textBox2->Text = ""+a;
			this->tb_minsPerFile->Text = ""+b; 

		};

private: System::Void checkBox1_CheckedChanged_1(System::Object^  sender, System::EventArgs^  e) {
			 //Invert
			 mainprog->setInvert(this->cb_invert->Checked);
		 }
};








}

