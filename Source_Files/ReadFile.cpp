/*
 * ReadFile.cpp
 *
 *  Created on: 3 Feb 2016
 *      Author: Khalid Ismail
 */

#include <stdio.h>


void read_file(void) {
	FILE *inImage = fopen("filename","r");

	//If file doesn't exist then abort
	if(inImage == NULL){
		printf("\n couldn't find image file - Abort \n");
	}

				 //System::Windows::Forms::DialogResult result = this->openFileDialog1->ShowDialog();
				 if (result==System::Windows::Forms::DialogResult::OK) {

				 //Load images button

				 /*array<String^>^  s = this->openFileDialog1->FileNames;
				 textBox1->Text::set("Loading...");
				 toolStripButton1->Enabled::set(true);
				 toolStripButton2->Enabled::set(true);
				  b_setOutputFolder->Enabled::set(true);
				 loadPoints->Enabled::set(true);
				 savePoints->Enabled::set(true);
				 b_quickLoad->Enabled::set(true);
				 b_quickSave->Enabled::set(true);
				 b_crop->Enabled::set(true);*/ //K_I
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

				 /*//help
				 help = gcnew System::Windows::Forms::HelpProvider;
				 help->HelpNamespace = "rthelp.chm";

				 help->SetShowHelp  ( b_quickSave, true );
				 help->SetHelpString( b_quickSave, "Save the background regions and start points " );*/


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
				 mainprog->init(s, idealr, idealg, idealb, tb_lr, processX, processY/*, this->motionMixture*/, this);
			 }

			 }
