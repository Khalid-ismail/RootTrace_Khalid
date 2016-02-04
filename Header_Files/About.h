//#pragma once

/*using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;*/


/*namespace RootTrace2 {

	/// <summary>
	/// Summary for About
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class About : public System::Windows::Forms::Form
	{
	public:
		About(void)
		{ 
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			licences_textbox->Text = ("Version "+(System::Reflection::Assembly::GetExecutingAssembly())->GetName()->Version);
			licences_textbox->AppendText("\r\n Third party licences:");
			licences_textbox->AppendText("\r\nMath.NET Iridium, part of the Math.NET Project http://mathnet.opensourcedotnet.info");
			licences_textbox->AppendText("\r\nCopyright (c) 2002-2008, Christoph R�egg,  http://christoph.ruegg.name");
			licences_textbox->AppendText( "\r\nJoannes Vermorel, http://www.vermorel.com");


			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~About()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;

	private: System::Windows::Forms::LinkLabel^  linkLabel1;
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::TextBox^  licences_textbox;


	protected: 

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(About::typeid));
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->linkLabel1 = (gcnew System::Windows::Forms::LinkLabel());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->licences_textbox = (gcnew System::Windows::Forms::TextBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(98, 106);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(198, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"(C) 2007, CPIB, University of Nottingham";
			// 
			// linkLabel1
			// 
			this->linkLabel1->AutoSize = true;
			this->linkLabel1->Location = System::Drawing::Point(113, 130);
			this->linkLabel1->Name = L"linkLabel1";
			this->linkLabel1->Size = System::Drawing::Size(170, 13);
			this->linkLabel1->TabIndex = 2;
			this->linkLabel1->TabStop = true;
			this->linkLabel1->Text = L"Centre for Plant Integrative Biology";
			this->linkLabel1->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &About::linkLabel1_LinkClicked);
			// 
			// pictureBox1
			// 
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->Location = System::Drawing::Point(127, 7);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(141, 69);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox1->TabIndex = 1;
			this->pictureBox1->TabStop = false;
			// 
			// licences_textbox
			// 
			this->licences_textbox->Location = System::Drawing::Point(25, 171);
			this->licences_textbox->Multiline = true;
			this->licences_textbox->Name = L"licences_textbox";
			this->licences_textbox->Size = System::Drawing::Size(368, 70);
			this->licences_textbox->TabIndex = 3;
			// 
			// About
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(405, 262);
			this->Controls->Add(this->licences_textbox);
			this->Controls->Add(this->linkLabel1);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->label1);
			this->Name = L"About";
			this->Text = L"About";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void linkLabel1_LinkClicked(System::Object^  sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^  e) {
			 try
				  {
					 VisitLink();
				  }
				  catch (Exception ^ ex)
				  {
					 MessageBox::Show("Unable to open link that was clicked.");
				  }
			 
			 }

	private: void VisitLink()
		   {
			  // Change the color of the link text by setting LinkVisited 
			  // to true.
			  linkLabel1->LinkVisited = true;
			  // Call the Process.Start method to open the default browser 
			  // with a URL:
			  System::Diagnostics::Process::Start("http://www.cpib.ac.uk");
		   }

};
}*/
