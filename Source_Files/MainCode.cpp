
/* LICENCE
Copyright (c) 2009, University of Nottingham
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. 
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer 
in the documentation and/or other materials provided with the distribution. 
Neither the name of the University of Nottingham nor the names of its contributors may be used to endorse or promote products 
derived from this software without specific prior written permission. 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
BUT NOT LIMITED TO, TH E IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO 
EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/




#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1

#include "../Header_Files/stdafx.h"
#include "cv.h"
#include "highgui.h"
#include "../Header_Files/MainCode.h"
#include <iostream>
#include "../Header_Files/Condensation.h"
#include "../Header_Files/StatsWriter.h"
//#include "3dgraph.h"
#include "../Header_Files/Form1.h"
#include "time.h"
//#include "nr.h"
//#include "nagmk21.hxx"
#include "../Header_Files/ZTable.h"
#include <tchar.h>
//#include <Windows.h>
//#include "barcode.h" //need to add back in barcode.h and barcode.cpp to project

#include "../Header_Files/ccnode.h"
#include "../Header_Files/queue.h"
#include "../Header_Files/stack.h"



 
using namespace std;
//using namespace System::Windows::Forms; //K_I
//using namespace System::Xml; //K_I
//using namespace System::IO; //K_I

//extern "C" __declspec(dllimport)HMENU GetSystemMenu(int hwnd, int revert); 


//using namespace RootTrace2;

#define PI 3.14159265

const int max_files = 300;// was 750
const int max_length = 1000;
const int max_numroots = 40;

double bg_mean[255][3];
double bg_sd[255][3];




const char* outputExtension = "bmp";//type of output image saved. bmp better quality, jpg smaller

bool annotateImage = false;  //mark on image the various conditions for the probability model end finding.

const bool do_bend_detection = true;
const double bend_angle_thresh = 180-30; //degrees 135 = 180-45
const int grav_response_threshold = 45; //used for response histogram 

//const bool recalcBGStats = true; // now in init


      bool snapToRoot = true;
const bool drawBendInfo = false;// *
const bool drawBendPoint= false;
const bool writeLengthOnImage = false;
const bool writeAngleOnRoot = true;
const int d = 15;		// 35, distance over which to measure curvature, was 10. 5 ok. So is 15. So is 25. 30.

const bool save_image = true;

const bool drawsamples = false;
const bool showPrunedSamples = false;
const bool draw_root_trace = true;

const int max_number_steps_to_trace = max_length; //effectively the largest measurable root length approx


const bool animate_trace = false; // false doesn't update crop window but is much faster

const int trace_pause_orig = 100; //was 5. Pause while drawing the samples online

const int num_paths_to_test = 1; //2000 good. Use 1 for Dijkstra

const double infinity = DBL_MAX; //infinity approx. 

const float curvature_empty_flag = -500;

//const double threshold_st_dev_trace = 5.5;// 3 ok. Bigger = more leniant
const double num_st_devs = 1; // 1, the number of prob st devs outside of which vertices get pruned, was 0.5
							  // 0.5 sometimes removed all end nodes, as sd was so small.....? 25/10/07

const bool do_line_end_finding = false;

const bool debug = false;

const bool showLiveZoom = true;

double gravOnsetLength[max_numroots];
double gravOnsetAngle[max_numroots];

const int num_cols_in_map = 7;
CvScalar color_map[num_cols_in_map];
float color_bins[num_cols_in_map]; // curvature 


// ASAD
double angle1, angle2;
double angle1Array[10000];
double angle2Array[10000];
int stepCounter=0;
CvPoint subRootsLeft[500], subRootsRight[500];
int subRootCountLeft, subRootCountRight;
ofstream subRootTxt;
int currentLeftSub, currentRightSub; // for counting current left and right sub roots of a root
// -ASAD

bool doingManualMeasures = false;
double manualAngles[255];



//const double dij_prob_threshold = 1E-400; //breaks if too high, pointless if too low....

//const float fontsize = 0.4f;
//const int fontwidth = 1;



void on_mouse_wrapper( int event, int x, int y, int flags, void* param );




void on_mouse_wrapper( int event, int x, int y, int flags, void* param )
{
	//cout<<"on mouse wrapper"<<endl;
	CMainCode * c =(CMainCode*)param;
    c->on_mouse(event, x, y, flags);	

}

void CMainCode::saveStartPoints(String^ filename) {
	   savedStartPoints=true;   
	   XmlWriterSettings ^ settings = gcnew XmlWriterSettings();
	   settings->Indent = true;
	   settings->IndentChars = ("    ");

	   XmlWriter^ writer = XmlWriter::Create(filename, settings);

	   
	   
	   writer->WriteStartElement("Data");
	   writer->WriteStartElement("RootStartPoints");
	   writer->WriteComment("Stores the users start points for the root traces");
	   MessageBox::Show(filename+" writing");
	   for (int n=0; n<num_roots; n++) {
		   writer->WriteStartElement("root"+ n);
		   writer->WriteElementString("startx", ""+start_ps[n].x);
		   writer->WriteElementString("starty", ""+start_ps[n].y);
		   writer->WriteEndElement();
	   }
	   writer->WriteEndElement();


	   if (croprect.width!=-1) {
		   writer->WriteStartElement("Crop");
		   writer->WriteComment("Stores the crop coords");
		   writer->WriteElementString("cropx", ""+croprect.x);
		   writer->WriteElementString("cropy", ""+croprect.y);
		   writer->WriteElementString("croph", ""+croprect.height);
		   writer->WriteElementString("cropw", ""+croprect.width);
		   writer->WriteEndElement();
	   }

	   //calibration
	   writer->WriteStartElement("Calibration");
	   writer->WriteElementString("mm_per_pix", ""+(int)(calib_mmPerPix*100000));//save as int, then convert back to double later
	   writer->WriteElementString("min_per_image", ""+(int)(calib_minPerImage*100000));
	   writer->WriteEndElement();

	  
	   writer->WriteEndElement(); //Data

	   writer->Flush();
}

void CMainCode::saveFileList(array<System::String^>^ filenames) {
	   XmlWriterSettings ^ settings = gcnew XmlWriterSettings();
	   settings->Indent = true;
	   settings->IndentChars = ("    ");

	   

	   XmlWriter^ writer = XmlWriter::Create((System::IO::FileInfo(Application::ExecutablePath).DirectoryName)+"\\RootTraceLastFiles.xml", settings);
	   
	   writer->WriteStartElement("LastFiles");
	   writer->WriteComment("Stores the last image file(s) opened by the user");
	   //MessageBox::Show(((System::IO::FileInfo(Application::ExecutablePath).DirectoryName)+"\\RootTraceLastFiles.xml"));
	   
	   writer->WriteStartElement("ArrayInfo");
	   writer->WriteElementString("NumberOfFiles",""+filenames->Length);
	   writer->WriteEndElement();
	   
	   for (int n=0; n<filenames->Length::get(); n++) {
		   writer->WriteStartElement("File"+ n);
		   writer->WriteElementString("Name",filenames[n]);
		   writer->WriteEndElement();
	   }

	   if (invert)  {writer->WriteStartElement("Invert"); writer->WriteEndElement();}
	   if (rotate_cw)  {writer->WriteStartElement("RotateCW"); writer->WriteEndElement();}
	   if (rotate_ccw)  {writer->WriteStartElement("RotateCCW"); writer->WriteEndElement();}
	   if (flip_hor)  {writer->WriteStartElement("FlipHor"); writer->WriteEndElement();}

	   writer->WriteEndElement();

	   writer->Flush();
}


array<System::String^>^ CMainCode::loadFileLists() {

	array<System::String^>^ filenames;

	XmlReader^ reader = XmlReader::Create((System::IO::FileInfo(Application::ExecutablePath).DirectoryName)+"\\RootTraceLastFiles.xml");
	   
	 reader->ReadStartElement("LastFiles");

	 int n=0;
	 int size=0;

	 
	 bool numberOfFiles = false;
	 bool file = false;

	  while (reader->Read()) {
		   //reader->ReadStartElement("root"+ num_roots);
		   //MessageBox::Show(reader->Value);
		   /*reader->Read();
		   MessageBox::Show(reader->Value);
		   start_ps[num_roots].x = Convert::ToInt32(reader->Value);//Convert::ToInt32(reader->ReadElementString("startx"));
		   
		   
		   reader->Read();
		   start_ps[num_roots].y = Convert::ToInt32(reader->Value);//Convert::ToInt32(reader->ReadElementString("starty"));
		   reader->ReadEndElement();
		   num_roots++;*/
		 
		   switch (reader->NodeType) {
			  case XmlNodeType::Element:
				  //Console.Write("<{0}>", reader.Name);
				  //MessageBox::Show("Name: "+reader->Name);
				 // MessageBox::Show("Value: "+ reader->Name);
				  if (reader->Name->Equals("NumberOfFiles")) numberOfFiles = true;
				  if (reader->Name->Equals("Name")) file = true;
				  if (reader->Name->Equals("Invert")) invert=true;
				  if (reader->Name->Equals("RotateCW")) rotate_cw = true;
				  if (reader->Name->Equals("RotateCCW")) rotate_ccw = true;
				  if (reader->Name->Equals("FlipHor")) flip_hor = true;
				  break;
			  case XmlNodeType::Text:{
				  
				  
					  if (size==0&&numberOfFiles) {
						  size = Convert::ToInt32(reader->Value);
						  filenames = gcnew array<System::String^>(size);
						  numberOfFiles=false;
					  }else if (file){

						  filenames[n] = reader->Value;
						  n++;
						  file=false;
					  }

				  //Console.Write(reader.Value);
				  break;
				}
		   }
	  }

	  return filenames;



}

System::Windows::Forms::Form^ CMainCode::getMainWindow() {


 return mainWindowGlobal;
}

void CMainCode::loadStartPoints(String^ filename) {
	   

	  // System::String^ folder = %System::String(workingFolder);	
	   //MessageBox::Show(""+folder + "\\" +filename + " reading...");
	   XmlReader^ reader = XmlReader::Create(/*folder + "\\" +*/filename);
	   reader->ReadStartElement("Data");
	   //reader->ReadStartElement("RootStartPoints");
	   
	   
	   num_roots=0;

	   bool gotX=false;
	   bool getCropData=false;
	   bool RootStartPoints=false;
	   bool calibration=false;
	   bool gotDistCalib = false;
	   bool gotTimecalib = false;
	   int cropDataItemsRead;
	   
	   while (reader->Read()) {
		   //reader->ReadStartElement("root"+ num_roots);
		   //MessageBox::Show(reader->Value);
		   /*reader->Read();
		   MessageBox::Show(reader->Value);
		   start_ps[num_roots].x = Convert::ToInt32(reader->Value);//Convert::ToInt32(reader->ReadElementString("startx"));
		   
		   
		   reader->Read();
		   start_ps[num_roots].y = Convert::ToInt32(reader->Value);//Convert::ToInt32(reader->ReadElementString("starty"));
		   reader->ReadEndElement();
		   num_roots++;*/
		   switch (reader->NodeType) {
			  case XmlNodeType::Element:
				  //Console.Write("<{0}>", reader.Name);
				  //MessageBox::Show("Name: "+reader->Name);
				  if (reader->Name->Equals("RootStartPoints")) {
					  RootStartPoints = true;
				  } else
				  if (reader->Name->Equals("Calibration")) {
					  calibration = true;
					  RootStartPoints = false;
				  } else
				  if (reader->Name->Equals("Crop")) {
					  RootStartPoints = false;
					 //Console.Write("CROP found");
					 getCropData = true;
					 cropDataItemsRead = 0;

				  }
				  break;
			  case XmlNodeType::Text:
				  //MessageBox::Show("Value: "+ reader->Value);

				  // this could be neater! But no time to do it now...
				  if (getCropData) {
					  
					  int data = Convert::ToInt32(reader->Value);
					  cropDataItemsRead++;
					  /*  writer->WriteElementString("cropx", ""+croprect.x);
						  writer->WriteElementString("cropy", ""+croprect.y);
						  writer->WriteElementString("croph", ""+croprect.height);
						  writer->WriteElementString("cropw", ""+croprect.width);*/
					  if (cropDataItemsRead==1) croprect.x = data;
					  else if (cropDataItemsRead==2) croprect.y = data;
					  else if (cropDataItemsRead==3) croprect.height = data;
					  else if (cropDataItemsRead==4) {
						  croprect.width = data;
						  //MessageBox::Show("Data read. Do Crop.");
						  cropImages(croprect);
						  getCropData=false;
					  }

				  } else if (RootStartPoints) {	
					  //get coords
					  if (!gotX) {
						start_ps[num_roots].x = Convert::ToInt32(reader->Value);
						//MessageBox::Show("x: "+ start_ps[num_roots].x);
						gotX=true;
					  }
					  else
					  {
						start_ps[num_roots].y = Convert::ToInt32(reader->Value);
						//MessageBox::Show("y: "+ start_ps[num_roots].y);
						gotX=false;
						num_roots++;
					  }
					  
				  }
				  else if (calibration) {	
					  
					  if (!gotDistCalib) {
						  calib_mmPerPix    = ((double)(Convert::ToInt32(reader->Value)))/100000; 
						  gotDistCalib = true;
					  } else 
					  if (!gotTimecalib) {
						  calib_minPerImage = ((double)(Convert::ToInt32(reader->Value)))/100000; 
						  gotTimecalib = true;
						  RootTrace2::Form1^ mainWindow_RT = ((RootTrace2::Form1^)getMainWindow());
					      mainWindow_RT->setCalibBoxText(calib_mmPerPix, calib_minPerImage);
					  }
				  }
				  //Console.Write(reader.Value);
				  break;
			  case XmlNodeType::CDATA:
				   //Console.Write("<![CDATA[{0}]]>", reader.Value);
				   break;
			  case XmlNodeType::ProcessingInstruction:
				   //Console.Write("<?{0} {1}?>", reader.Name, reader.Value);
				   break;
			  case XmlNodeType::Comment:
				   //Console.Write("<!--{0}-->", reader.Value);
				   //MessageBox::Show("Comment: "+reader->Value);
				   break;
			  case XmlNodeType::XmlDeclaration:
				   //Console.Write("<?xml version='1.0'?>");
				   break;
			  case XmlNodeType::Document:
				   break;
			   case XmlNodeType::DocumentType:
				   //Console.Write("<!DOCTYPE {0} [{1}]", reader.Name, reader.Value);
				   break;
			   case XmlNodeType::EntityReference:
				   //Console.Write(reader.Name);
				   break;
			   case XmlNodeType::EndElement:
				   //Console.Write("</{0}>", reader.Name);
				   //MessageBox::Show("End Element: "+reader->Name);
				   break;
			

		} 

	   }

	    IplImage* image_copy = cvCloneImage(image);
		for (int loop=0; loop<num_roots; loop++) {
			CvPoint p = cvPoint(start_ps[loop].x, start_ps[loop].y);
			cvCircle(image_copy, p, 3, cvScalar(0,0, 255));
		}
		cvShowImage("Image", image_copy);
		//cvWaitKey();
		cvReleaseImage(&image_copy);
	}


bool CMainCode::getBGStats(IplImage* bg_mask, IplImage* image) {

					UCHAR r;
					UCHAR g;
					UCHAR b;

					bool colors[256];
					for (int i=0; i<256; i++) {
						colors[i]=false;

					}

					//scan through image to see how many intensity levels/bg regions there are
					for (int j=0; j<bg_mask->width; j++) 
						for (int k=1; k<bg_mask->height; k++) {
							MyGetPixel(bg_mask, j, k, &r, &g, &b);
							int index = (int)r;
							if (r>0) {
								colors[index] = true; // don't count black
								//MessageBox::Show("Found col "+index);
							}

						}

					// now count how many discrete colours there are
					int num_colors = 0;
					for (int i=0; i<256; i++) {
						if (colors[i]==true) num_colors++;
					}

					//MessageBox::Show("Num colors in BG img = " + num_colors);


					//now calc stats for each region
					for (int level=0; level<num_colors; level++) {

						int stat_count=0;
						double st_dev_col[3];
						double running_total_col[3];
						double mean_col[3];
						double cum_total_sq[3];	
						double cum_total_x_sq[3];	

						mean_col[0] = 0;
						mean_col[1] = 0;
						mean_col[2] = 0;

						cum_total_sq[0]=0;
						cum_total_sq[1]=0;
						cum_total_sq[2]=0;
						cum_total_x_sq[0] = 0;
						cum_total_x_sq[1] = 0;
						cum_total_x_sq[2] = 0;

						st_dev_col[0]=0;
						st_dev_col[1]=0;
						st_dev_col[2]=0;
						running_total_col[0] = 0;
						running_total_col[1] = 0;
						running_total_col[2] = 0;


						for (int xx=1; xx<image->width; xx++) {
							for (int yy=1; yy<image->height; yy++) {
								
								MyGetPixel(bg_mask, xx, yy, &r, &g, &b);
								if (r==level+1) {  //if the pixel intensity relates to the current level (excluding intensity 0)
									// in selected region
									MyGetPixel(image, xx, yy,  &r, &g, &b);

									dataR[stat_count] = r;
									dataG[stat_count] = g;
									dataB[stat_count] = b;

									running_total_col[0] += r; 
									running_total_col[1] += g; 
									running_total_col[2] += b; 
									stat_count++;
									// mean col is stable across whole image...
									mean_col[0] = running_total_col[0]/stat_count;
									mean_col[1] = running_total_col[1]/stat_count;
									mean_col[2] = running_total_col[2]/stat_count;
								    
									cum_total_sq[0] = running_total_col[0]*running_total_col[0];
									cum_total_sq[1] = running_total_col[1]*running_total_col[1];
									cum_total_sq[2] = running_total_col[2]*running_total_col[2];

									cum_total_x_sq[0] += (r*r);
									cum_total_x_sq[1] += (g*g);
									cum_total_x_sq[2] += (b*b);
				
									// FIXED problem, by casting ints to doubles below (I suspect it was denominator maxing out...)
									st_dev_col[0] = sqrt( (stat_count * cum_total_x_sq[0]-cum_total_sq[0])/ (double)((double)stat_count*((double)stat_count-1)) ) ;
									st_dev_col[1] = sqrt( (stat_count * cum_total_x_sq[1]-cum_total_sq[1])/ (double)((double)stat_count*((double)stat_count-1)) ) ;
									st_dev_col[2] = sqrt( (stat_count * cum_total_x_sq[2]-cum_total_sq[2])/ (double)((double)stat_count*((double)stat_count-1)) ) ;

									//mainWindowText(mainWindow_RT, "cum_tot_x_sq : "+cum_total_x_sq[0]);
									
								}
							}
						}
						
						/*
						// test calc again by calcutating long way
						double rmean, gmean, bmean;
						double rtotal=0, gtotal=0, btotal=0;
						for (int i=0; i<stat_count; i++) {
							rtotal+=dataR[i];
							gtotal+=dataG[i];
							btotal+=dataB[i];
						}

						rmean = rtotal/stat_count;
						gmean = gtotal/stat_count;
						bmean = btotal/stat_count;

						double totalVarianceSqR=0, totalVarianceSqG=0, totalVarianceSqB=0;
						for (int i=0; i<stat_count; i++) {
							totalVarianceSqR+=(dataR[i]-rmean)*(dataR[i]-rmean);
							totalVarianceSqG+=(dataG[i]-gmean)*(dataG[i]-gmean);
							totalVarianceSqB+=(dataB[i]-bmean)*(dataB[i]-bmean);
						}

						double sdr = sqrt(totalVarianceSqR/stat_count);
						double sdg = sqrt(totalVarianceSqG/stat_count);
						double sdb = sqrt(totalVarianceSqB/stat_count);


						mainWindowText(mainWindow_RT, "sdr  : "+sdr);
						*/

						//MessageBox::Show("Mean BG: "+mean_col[0]+"\nrmean"+rmean+"\nSD:"+st_dev_col[0]+"\nsdr: "+sdr+"\n n = "+stat_count);
						/*mainWindowText(mainWindow_RT, " ");
						mainWindowText(mainWindow_RT, "stat_count : "+stat_count);
						mainWindowText(mainWindow_RT, "cum_tot_sq : "+cum_total_sq[0]);
						mainWindowText(mainWindow_RT, "cum_tot_x_sq : "+cum_total_x_sq[0]);
						mainWindowText(mainWindow_RT, "running_total_col[0] : "+running_total_col[0]);
						mainWindowText(mainWindow_RT, "st_dev_col[0] : "+st_dev_col[0]);*/

						

						//mainWindowText(getMainWindow(), "Background (R,G,B)");
						//mainWindowText(getMainWindow(), "Means:"+mean_col[0]+", "+mean_col[1]+", "+mean_col[2]);
						//mainWindowText(getMainWindow(), "StDevs:"+st_dev_col[0]+", "+st_dev_col[1]+", "+st_dev_col[2]);


						bg_mean[level][0] = mean_col[0];
						bg_mean[level][1] = mean_col[1];
						bg_mean[level][2] = mean_col[2];
						bg_sd[level][0] = st_dev_col[0];
						bg_sd[level][1] = st_dev_col[1];
						bg_sd[level][2] = st_dev_col[2];
					} // end level loop

					bool multi_colours;
					if (num_colors>1) multi_colours = true; 
					else multi_colours = false;

					return multi_colours;


}

void CMainCode::on_mouse( int event, int x, int y, int flags)
{

	

	//is it on valid part of image?
	if (image!=NULL && x>1 && y>1 && x<image->width-1 && y<image->height-1)  {
		
		// use last_image if selected. 
		// Need to check this doesn't affect anything in a bad way...
		if (showLastImageToggle) base_image = last_image/*cvCopyImage(last_image, base_image)*/;
		else base_image = measuresi; /*cvCopyImage(originali, base_image);*/ //, was originali, changed to measuresi so enhancing visible...

	switch( event )
    {
	case CV_EVENT_MBUTTONUP:
		{
			if(tabooPointsEnabled == true)
			{
				drawSubrootsToggle = !drawSubrootsToggle;

				//drawSubrootsToggle = !drawSubrootsToggle;
				if( drawSubrootsToggle== false)
				{   
					//cvCircle(image,cvPoint(x,y),8,CV_RGB(0,0,0),-1);
					cvCircle(measuresiTaboo,cvPoint(x,y),8,CV_RGB(0,0,0),-1);
					this->tabooPoints[this->tabooPointCounter].x = x;
					this->tabooPoints[this->tabooPointCounter].y = y;
					this->tabooPointCounter++;
				}

				// cvShowImage("Image",image);
				cvShowImage("Image", measuresiTaboo);
			}
			
			break;

		}

    case CV_EVENT_LBUTTONDOWN:
        {
			RootTrace2::Form1^ mainWindow_RT = ((RootTrace2::Form1^)getMainWindow());
			
			
			if (doingManualMeasures) {
				doManualAngles(x, y);
			} else
			if (doCrop) {
				if (crop1.x==-1) {
					crop1 = cvPoint(x,y);
					if (i_copy==NULL) {
						//if (r_copy!=NULL) i_copy = cvCloneImage(r_copy);
						/*else*/ i_copy = cvCloneImage(base_image);
					}
					crop_temp = cvPoint(x, y);
				}
				//	else {
				//	crop2  = cvPoint(x,y);
				//	croprect = cvRect(crop1.x, crop1.y, crop2.x-crop1.x, crop2.y - crop1.y);
				//	cropImages(croprect);
				//	crop1.x=-1;
				//	cvShowImage("Image", image);
				//	doCrop=false;
				//	if (mainWindow_RT->autoEnhanceImages()) enhance(mainWindow_RT); // redo enhance over cropped section
				//}
			}
			/*else if (doFixedDistMeasure) {
				if (fixedPoint1.x==-1) fixedPoint1 = cvPoint(x,y);
				else 
				{
					fixedPoint2 = cvPoint(x,y);
					cvLine(image, fixedPoint1, fixedPoint2, cvScalar(0, 128, 255), 2);
					
					fixedDistance = sqrt( (double)((fixedPoint1.x-fixedPoint2.x)*(fixedPoint1.x-fixedPoint2.x) + (fixedPoint1.y-fixedPoint2.y)*(fixedPoint1.y-fixedPoint2.y) ));
					char s[50];
					sprintf(s, "%.1f pix", fixedDistance);

					imgTxt(image, s, fixedPoint2, cvScalar(0, 128, 255));
					cvShowImage("Image", image);
					fixedPoint1.x=-1;
					mainWindow_RT->enableCalib();
					doFixedDistMeasure=false;

				}

			}*/
			
			else{//background

			if (mainWindow_RT->getSelectBG()) {
				  
				
				// select a region for background stats
				if (bg_rect_click.x==-1) {
					if (i_copy==NULL) {
						//if (r_copy!=NULL) i_copy = cvCloneImage(r_copy);
						/*else*/ i_copy = cvCloneImage(base_image);
					}
					i_copy2 = cvCloneImage(i_copy);
					bg_rect_click.x = x;
					bg_rect_click.y = y;
					cvCircle(i_copy, cvPoint(x,y), 2, cvScalar(255), -1);
					bg_temp = cvPoint(x, y);
				} 
			
				
			}

			//calibration
			/*else if (calibration_mode&&calib_line_points<2) {  // define the line to draw
				if (calib_line_points==0) {
					calib_line1 = cvPoint(x, y);
					calib_line_points++;
					cvCopy(image, tempimage);
					imgTxt(tempimage, myspf("Now select the line end point..."), cvPoint(5, 40), CV_RGB(0,255,0) );
					cvShowImage("Image", tempimage);
				}
				else
				if (calib_line_points==1) {
					calib_line2 = cvPoint(x, y);
					calib_line_points++;
					cvLine(base_image, calib_line1, calib_line2, cvScalar(idealR, idealG, idealB), 1);
					cvCopy(base_image, image);
					cvCopy(image, tempimage);
					imgTxt(tempimage, myspf("Now trace the line 3 times..."), cvPoint(5, 40), CV_RGB(0,255,0) );
					cvShowImage("Image", tempimage);
				}
					
			}*/

			else { //NORMAL
			//handle mouse clicks
			
			cvShowImage("Image", image);

			clickx=x;
			clicky=y;
			crop_orig_x= min(max(x-(cropx/2), 0), image->width-cropx);
			crop_orig_y= min(max(y-(cropy/2), 0), image->height-cropy);


			//cvCopy(image, tempimage);
			cvSetImageROI(image, cvRect( crop_orig_x, crop_orig_y, cropx, cropy) );
			
			//cvDrawRect(tempimage,cvPoint(crop_orig_x, crop_orig_y), cvPoint(crop_orig_x+cropx, crop_orig_y+cropy), cvScalar(0, 255, 0)); 
			//cvShowImage("Image", tempimage);

			
			
			
			cvCopy(image, crop);
			
			//draw cross
			MyPutPixel(crop, clickx - crop_orig_x, cropy-(clicky - crop_orig_y), 255, 0, 0); //centre
			//1 step
			MyPutPixel(crop, clickx - crop_orig_x-1, cropy-(clicky - crop_orig_y), 255, 0, 0);//left
			MyPutPixel(crop, clickx - crop_orig_x+1, cropy-(clicky - crop_orig_y), 255, 0, 0);//right
			MyPutPixel(crop, clickx - crop_orig_x, cropy-(clicky - crop_orig_y)-1, 255, 0, 0);
			MyPutPixel(crop, clickx - crop_orig_x, cropy-(clicky - crop_orig_y)+1, 255, 0, 0);
			//2 step
			MyPutPixel(crop, clickx - crop_orig_x-2, cropy-(clicky - crop_orig_y), 255, 0, 0);//left
			MyPutPixel(crop, clickx - crop_orig_x+2, cropy-(clicky - crop_orig_y), 255, 0, 0);//right
			MyPutPixel(crop, clickx - crop_orig_x, cropy-(clicky - crop_orig_y)-2, 255, 0, 0);
			MyPutPixel(crop, clickx - crop_orig_x, cropy-(clicky - crop_orig_y)+2, 255, 0, 0);


			cvNamedWindow("Zoom", 2);
			cvShowImage("Zoom", crop);
			
			cvResetImageROI(image);
			}
			
			}//else
			
        }
        break;

		case CV_EVENT_LBUTTONUP:
		{
			RootTrace2::Form1^ mainWindow_RT = ((RootTrace2::Form1^)getMainWindow());
			
			if (!doingManualMeasures)
			if (doCrop&&crop1.x!=-1) {
					// finish cropping
					crop2  = cvPoint(x,y);
					croprect = cvRect(crop1.x, crop1.y, crop2.x-crop1.x, crop2.y - crop1.y);
					cropImages(croprect);
					crop1.x=-1;
					cvShowImage("Image", image);
					doCrop=false;
					//if (mainWindow_RT->autoEnhanceImages()) enhance(mainWindow_RT); // redo enhance over cropped section
					cvReleaseImage(&i_copy);
				}
			else
				// finish setting BG region
				if (bg_rect_click.x!=-1) {
					 
					// got rectangle
				
					if (bg_mask==NULL) {
						bg_mask = cvCreateImage(cvGetSize(i_copy), 8, 3);
						cvZero(bg_mask);//on zero once - additive masking
					}

					//cvDrawRect(i_copy, cvPoint(x, y), cvPoint(bg_rect_click.x, bg_rect_click.y), cvScalar(255, 0,0), -1);
					
					// create mask image here for bg calc
					if (removeBGflag==true) {
						//remove
						cvDrawRect(bg_mask, cvPoint(x, y), cvPoint(bg_rect_click.x, bg_rect_click.y), cvScalar(0, 0, 0), -1); //draw black
						int startx = min(bg_rect_click.x, bg_temp.x)-2;
						int starty = min(bg_rect_click.y, bg_temp.y)-2;
						int widthx = abs(bg_rect_click.x - bg_temp.x)+4;
						int widthy = abs(bg_rect_click.y - bg_temp.y)+4;
					
						cvSetImageROI(base_image, cvRect(startx, starty, widthx, widthy));
						cvSetImageROI(i_copy, cvRect(startx, starty, widthx, widthy));
						cvCopy(base_image, i_copy);
						cvResetImageROI(base_image);
						cvResetImageROI(i_copy);
					    removeBGflag=false;//return to off state
					}else
					{ //normal
						//first draw in background mask image
						cvDrawRect(bg_mask, cvPoint(x, y), cvPoint(bg_rect_click.x, bg_rect_click.y), cvScalar(BG_region_counter+1, BG_region_counter+1, BG_region_counter+1), -1); //not 0
						//now display on image
						CvScalar unique_colour = cvScalar(((BG_region_counter+1)*21)%255, ((BG_region_counter+1)*71)%255, ((BG_region_counter+1)*171)%255);
						cvDrawRect(i_copy, cvPoint(x, y), cvPoint(bg_rect_click.x, bg_rect_click.y), unique_colour, -1);
						imgTxt(i_copy, myspf("%i", BG_region_counter), cvPoint(x, y), cvScalar(0, 255, 50));
						BG_region_counter++;
					}

		
					bg_rect_click.x = -1;
					bg_rect_click.y = -1;

					//cvReleaseImage(&bg_mask);
					cvShowImage("Image", i_copy);
					cvReleaseImage(&i_copy2);

				}
				

		}
		break;
		case CV_EVENT_RBUTTONDOWN: 
		{

			if (num_roots<max_numroots) {
				int click_x = x;
				CvPoint lockedp;
				if (snapToRoot) {
					lockedp = lockToRoot(click_x, y);
					
				} else {
					lockedp.x = click_x;
					lockedp.y = y;
				}
				
				/*IplImage* */ 
				//if (r_copy==NULL) {
				if (i_copy==NULL) {
						i_copy = cvCloneImage(base_image);
				}
					//else r_copy = cvCloneImage(i_copy);
				//}

				start_ps[num_roots] = lockedp;
				
 

				cvCircle(i_copy, start_ps[num_roots], 4, cvScalar(0,0, 255), -1);
				//new line to draw start number, like with background. Draw on output too?
				imgTxt(i_copy, myspf("%i", num_roots), cvPoint(x, y), cvScalar(0, 255, 50));

				num_roots++;
				if (num_roots>=max_numroots) {
					MessageBox::Show( "Sorry, limited to "+max_numroots+" roots per plate at the moment. No more please","No more roots");
				}
				cvShowImage("Image", i_copy);
			}
			}

			break;
		case CV_EVENT_MOUSEMOVE: 
		{
			
			
			if (bg_rect_click.x!=-1) {
				
				if (bg_rect_click.x!=bg_temp.x && bg_rect_click.y!=bg_temp.y) {
					int startx = min(bg_rect_click.x, bg_temp.x)-1;
					int starty = min(bg_rect_click.y, bg_temp.y)-1;
					int widthx = abs(bg_rect_click.x - bg_temp.x)+3;
					int widthy = abs(bg_rect_click.y - bg_temp.y)+3;
					//if (r_copy==NULL){
						cvSetImageROI(i_copy2, cvRect(startx, starty, widthx, widthy));
						cvSetImageROI(i_copy, cvRect(startx, starty, widthx, widthy));
						cvCopy(i_copy2, i_copy);
						cvResetImageROI(i_copy2);
						cvResetImageROI(i_copy);
					/*} else {
						cvSetImageROI(r_copy, cvRect(startx, starty, widthx, widthy));
						cvSetImageROI(i_copy, cvRect(startx, starty, widthx, widthy));
						cvCopy(r_copy, i_copy);
						//cvZero(i_copy);
						cvResetImageROI(r_copy);
						cvResetImageROI(i_copy);
					} 
*/
				}
				CvScalar colour;
				if (removeBGflag) 
					colour = cvScalar(0, 0, 255);
				else 
					colour = cvScalar(((BG_region_counter+1)*21)%255, ((BG_region_counter+1)*71)%255, ((BG_region_counter+1)*171)%255);
				
				cvDrawRect(i_copy, cvPoint(x, y), cvPoint(bg_rect_click.x, bg_rect_click.y), colour, 2);
				cvShowImage("Image", i_copy);
				bg_temp.x=x;
				bg_temp.y=y;
				
			} else 
			if (doCrop&&crop1.x!=-1) {
				if (crop1.x!=crop_temp.x && crop1.y!=crop_temp.y) {
					int startx = min(crop1.x, crop_temp.x)-4;
					int starty = min(crop1.y, crop_temp.y)-4;
					int widthx = abs(crop1.x - crop_temp.x)+8;
					int widthy = abs(crop1.y - crop_temp.y)+8;
					cvSetImageROI(base_image, cvRect(startx, starty, widthx, widthy));
					cvSetImageROI(i_copy, cvRect(startx, starty, widthx, widthy));
					cvCopy(base_image, i_copy);
					//cvZero(i_copy);
					cvResetImageROI(base_image);
					cvResetImageROI(i_copy);
				}
				cvDrawRect(i_copy, cvPoint(x, y), cvPoint(crop1.x, crop1.y), cvScalar(0, 0, 0), 4);
				cvShowImage("Image", i_copy);

				crop_temp.x = x;
				crop_temp.y = y;

			}

			
			if (showLiveZoom&&base_image!=NULL ) {
				int rect_x = min(	max(x-(liveZoomImg_width/2), 0) ,
						base_image->width-liveZoomImg_width);
				int rect_y = 	min(	max(y-(liveZoomImg_height/2), 0), 
						base_image->height-liveZoomImg_height);

				int diff_x = rect_x-(x-(liveZoomImg_width/2));
				int diff_y = rect_y-(y-(liveZoomImg_height/2));
				
				
				if (liveZoomImg->width > base_image->width || liveZoomImg->height > base_image->height) {
					MessageBox::Show("Basic image is smaller than the zoom window display due to over cropping.\n Cannot create zoom window image. ", "Error");
					exit(-1);
				}
				
				cvSetImageROI(base_image, cvRect(rect_x, rect_y, liveZoomImg_width, liveZoomImg_height ));
				
				//MessageBox::Show("Mouse: "+liveZoomImg_width);
				

				cvCopy(base_image, liveZoomImg);
				cvResetImageROI(base_image);
				//cvNamedWindow("Test", 2);
				//cvShowImage("Test", base_image);
				//cvSetImageROI(base_image, cvRect(0,0,50, 50) );

			
				// ASad
				// this part only draws the cross hair or the red circle in the zoom window
				// depending on mid button clicks
				if(drawSubrootsToggle== false)
				{
					MyPutPixel(liveZoomImg, liveZoomImg_width/2 -diff_x, liveZoomImg_height/2 +diff_y, 255, 0, 0);
					MyPutPixel(liveZoomImg, liveZoomImg_width/2 -diff_x -1, liveZoomImg_height/2 +diff_y, 255, 0, 0);
					MyPutPixel(liveZoomImg, liveZoomImg_width/2 -diff_x -2, liveZoomImg_height/2 +diff_y, 255, 0, 0);
					MyPutPixel(liveZoomImg, liveZoomImg_width/2 -diff_x +1, liveZoomImg_height/2 +diff_y, 255, 0, 0);
					MyPutPixel(liveZoomImg, liveZoomImg_width/2 -diff_x +2, liveZoomImg_height/2 +diff_y, 255, 0, 0);
					MyPutPixel(liveZoomImg, liveZoomImg_width/2 -diff_x, liveZoomImg_height/2 +diff_y -1, 255, 0, 0);
					MyPutPixel(liveZoomImg, liveZoomImg_width/2 -diff_x, liveZoomImg_height/2 +diff_y -2, 255, 0, 0);
					MyPutPixel(liveZoomImg, liveZoomImg_width/2 -diff_x, liveZoomImg_height/2 +diff_y +1, 255, 0, 0);
					MyPutPixel(liveZoomImg, liveZoomImg_width/2 -diff_x, liveZoomImg_height/2 +diff_y +2, 255, 0, 0);
				}
				else
				{
					cvCircle(liveZoomImg,cvPoint(liveZoomImg_width/2 -diff_x, liveZoomImg_height/2 +diff_y),8,CV_RGB(255,0,0),14);

				}
				// -Asad 

				
				cvShowImage("LiveZoom", liveZoomImg);
				//cvResetImageROI(base_image);

			}

		}
		
    }
 
  } //if (image!=NULL && x>1 && y>1 && x<image->width-1 && y<image->height-1)

 }//on_mouse





 char* CMainCode::myspf(char* strToFormat, ...)				// my version of sprintf =)
{
	va_list varArgLst;
	char* resStr = new char[255];

    va_start(varArgLst, strToFormat);
	int len = _vscprintf( strToFormat, varArgLst ) // _vscprintf doesn't count
                               + 1; // terminating '\0'

	vsprintf_s(resStr, len, strToFormat, varArgLst ); 

	return resStr;
}
void CMainCode::imgTxt(IplImage *img, char *message, CvPoint pos, CvScalar colour)
{
	// Outputs some text to an image in one line of code - ahhhhh how nice!

	CvFont font;
	float fontsize = max(img->height/1000, 0.6);
	int fontwidth = max(ceil(fontsize*2),1);
	cvInitFont(&font, CV_FONT_VECTOR0, fontsize,fontsize,0,fontwidth);
	cvPutText(img, message, pos, &font, colour);	
	delete message;
}

void CMainCode::imgTxt(IplImage *img, char *message, CvPoint pos, CvScalar colour, float fontsize)
{
	// Outputs some text to an image in one line of code - ahhhhh how nice!

	CvFont font;
	int fontwidth = max(ceil(fontsize*2),1);
	cvInitFont(&font, CV_FONT_VECTOR0, fontsize,fontsize,0,fontwidth);
	cvPutText(img, message, pos, &font, colour);	
	delete message;
}

void CMainCode::enhance(Form^ mainWindow, IplImage* i){
			IplImage* grey = cvCreateImage(cvGetSize(i), 8, 1);
			cvCvtColor(i, grey, CV_RGB2GRAY);

			cvEqualizeHist(grey, grey);
			cvCvtColor(grey, i, CV_GRAY2RGB);
			//cvCopy(measuresi, image);
			//cvCopy(originali, output);
			cvShowImage("Image", i);
			//cvCopy(originali, previousi); //cannot undo so make this clear

			cvReleaseImage(&grey);
			mainWindowText(mainWindow, "Enhanced.");

}

void CMainCode::smooth(Form^ mainWindow, IplImage* i){
			cvSmooth(i, i, CV_MEDIAN, 3, 0, 0); //was 5
			cvSmooth(i, i, CV_GAUSSIAN, 7, 7, 0); //was 5
			//cvSmooth(measuresi, measuresi, CV_GAUSSIAN, 7, 7, 0); //was 5
			//cvCopy(originali, image);
			//cvCopy(originali, output);
			cvShowImage("Image", i);
			mainWindowText(mainWindow, "Filtered / smoothed.");
}

void CMainCode::mainWindowText(Form^ mainWindow, String^ string) {
	((RootTrace2::Form1^)mainWindow)->textBox1->Text::set(((RootTrace2::Form1^)mainWindow)->textBox1->Text::get()+"\r\n"+string);


}

IplImage* CMainCode::loadImage(const char* loadpath, System::Windows::Forms::Form^ mainWindow) {

	RootTrace2::Form1^ mainWindow_RT = ((RootTrace2::Form1^)mainWindow);
	IplImage* limage;
	IplImage* loadedi = cvLoadImage(loadpath);
	if (loadedi==NULL) {
		MessageBox::Show("Sorry, cannot load that file.", "Bye", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
		exit(-10);
	}

	if (/*(loadedi->width>2000 || loadedi ->height>2000) && */mainWindow_RT->autoResizeLargeImages()) {
		
		//DialogResult result = MessageBox::Show("The image is very large.  Do you want to downsize? (recommend Yes)", "Image Size",MessageBoxButtons::YesNo, MessageBoxIcon::Information);
		//if (result == DialogResult::Yes) {
			limage = cvCreateImage(cvSize((int)(loadedi->width*0.5), (int)(loadedi->height*0.5)), 8, 3);
			cvResize(loadedi, limage);
			mainWindowText(mainWindow, "Resized to 50%");
			//limage = cvCloneImage(image);

		} else 
			limage = cvCloneImage(loadedi);
		

	//}  
	/*else
	if ((loadedi->width>2000 || loadedi ->height>2000) && mainWindow_RT->autoResizeLargeImages()) {
		
		
			limage = cvCreateImage(cvSize(loadedi->width*0.6, loadedi->height*0.6), 8, 3);
			cvResize(loadedi, limage);
			mainWindowText(mainWindow, "Resized to 60%");

	}*/
	//else
		//limage = cvCloneImage(loadedi);

	 
	 cvReleaseImage(&loadedi);

	 if (invert) {
		 // invert the image
		 UCHAR r, g, b;
		 for (int i=0;i<limage->width;i++)
			 for (int j=0; j<limage->height; j++){
				 MyGetPixel(limage, i, j, &r, &g, &b);
				 MyPutPixel(limage, i, j, 255-r, 255-g, 255-b);
	    }
	 }

	 //mainWindowText(mainWindow, "test");
	 if (flip_hor) {
		 cvFlip(limage, limage, 1);
		 mainWindowText(mainWindow, "Flipping.");
	 }
	 if (rotate_ccw) {
			IplImage* rotated = cvCreateImage(cvSize(limage->height, limage->width), 8, 3); //size rotated 90 degrees
			UCHAR r, g, b;
			for (int x=0; x<limage->width; x++)
				for (int y=0; y<limage->height; y++) {
					MyGetPixel(limage, x, y, &r, &g, &b);
					MyPutPixel(rotated, rotated->width-y, x, r, g, b);
					
				}
			cvReleaseImage(&limage);
			limage = cvCloneImage(rotated);
			cvReleaseImage(&rotated);

	 }
	  if (rotate_cw) {
			IplImage* rotated = cvCreateImage(cvSize(limage->height, limage->width), 8, 3); //size rotated 90 degrees
			UCHAR r, g, b;
			for (int x=0; x<limage->width; x++)
				for (int y=0; y<limage->height; y++) {
					MyGetPixel(limage, x, y, &r, &g, &b);
					MyPutPixel(rotated, y, rotated->height-x, r, g, b);
					
				}
			cvReleaseImage(&limage);
			limage = cvCloneImage(rotated);
			cvReleaseImage(&rotated);

	 }


	// AUTO SMOOTH AND ENHANCE!!!  now done with enhance tab
	//if (mainWindow_RT->autoSmoothImages()) smooth(mainWindow, limage);
	//if (mainWindow_RT->autoEnhanceImages()) enhance(mainWindow, limage);
	


	


	 return limage;
}


void CMainCode::setBGMask(System::String^ filename) {

	char loadBG[255];
	char* st = (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(filename);
	
	sprintf(loadBG, "%s", st); //Used to add working folder here, not any more
	bg_mask = cvLoadImage(loadBG);

	multiple_BG_stats = getBGStats(bg_mask, measuresi);

	IplImage* i_copy = cvCloneImage(originali);
	cvOr(bg_mask, i_copy, i_copy);//for display
	cvShowImage("Image", i_copy);
	cvWaitKey(1);
	cvReleaseImage(&i_copy);

}


void CMainCode::saveBGMask(System::String^ filename) {

	char* filenameCH = (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(filename);
	cvSaveImage(filenameCH, bg_mask);

}

void CMainCode::init(array<System::String^>^ filenames, UCHAR idealr, UCHAR idealg, UCHAR idealb, UCHAR tb_lr, double proSigmaX, double proSigmaY/*, double motionMixture*/, System::Windows::Forms::Form^ mainWindow) {
	
	
	

	//NULL all image pointers
	image = originali = previousi = crop = tempimage = measuresi = measuresiTaboo = smoothedi = output = debuggraph = preview_lengthsi = i_copy = i_copy2 = last_image = base_image = NULL;


	//doGravitropic=false;
	//doTipAngles=false;
	//rotate_cw=false;
	//rotate_ccw=false;
	//flip_hor=false;

	numberOfManualAngleMeasures=0;
	manualAngleCount=0;

	// remap input parameters
	idealR = idealr;
	idealG = idealg;
	idealB = idealb;

	setupParameters();

	

	//Create zoomed window
	cvNamedWindow("LiveZoom", CV_WINDOW_AUTOSIZE );
	liveZoomImg = cvCreateImage(cvSize(liveZoomImg_width, liveZoomImg_height), 8, 3);
	cvZero(liveZoomImg);
	cvShowImage("LiveZoom", liveZoomImg);
	cvWaitKey(1);

	//Create tip trace image
	IplImage* tipimage = cvCreateImage(cvSize(200, 200), 8, 3);
	cvZero(tipimage);

	for (int i=0; i<max_numroots; i++) {
		gravOnsetLength[i] = -1;
		gravOnsetAngle[i] = -1;
	}



	double lastLengths[max_roots];
	for (int i=0; i<max_roots; i++)
		lastLengths[i] = -1;

	CvPoint initialTipLocations[max_numroots];
	int initialTipPathLocation[max_numroots];
	for (int j=0; j<max_numroots; j++) {
		initialTipLocations[j] = cvPoint(0,0);
		initialTipPathLocation[j] = 0;

	}


	if (filenames->Length::get()>max_files) {
		MessageBox::Show("WARNING: maximum number of files ("+max_files+") exceeded. Exiting...");
		exit(-10);
	}


	mainWindowGlobal = mainWindow;




	array<float,3>^ curvatures = gcnew array<float,3>(max_files, max_length, max_roots);
	//array<float,3>^ curvatures2 = gcnew array<float,3>(max_files, max_length, max_roots); //store using new method
	array<float,3>^ lengthForCurvature = gcnew array<float,3>(max_files, max_length, max_roots);
	for (int temp1=0; temp1<max_files; temp1++) 
		for (int temp2=0; temp2<max_length; temp2++)
			for (int temp3=0; temp3<max_roots; temp3++){
			curvatures[temp1, temp2, temp3] = curvature_empty_flag;
			//curvatures2[temp1, temp2, temp3] = curvature_empty_flag;
			}

	array<float,2>^ finalLengths = gcnew array<float,2>(max_files, max_numroots);

	RootTrace2::Form1^ mainWindow_RT = ((RootTrace2::Form1^)mainWindow);

	double calib_measures[3];

	int filenumber = 0;

	num_roots=0;

	char* argv[1];
	argv[0] = "test";



	const char* folder   = "test";
	
	
	char loadpath[255];
	char savepath[255];
	
	
	System::Array::Sort(filenames); // SORTING!!

	m_filenames = gcnew array<String^>(filenames->Length);
	filenames->CopyTo(m_filenames, 0);
	

	CStatsWriter stats, debugstats, tip_stats, tip_angles, tip_angles_vert, trace_points;

	debugstats.ChangeFile("debug.csv");

	//grav stats
	CStatsWriter gravstats = CStatsWriter();
	char gravsavepath[512];
	CStatsWriter gravdiststats = CStatsWriter();

	
	char gravdiststring[255];
	char gravstring[255];



	//load last image, for info
	char* last_filename = (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(filenames[filenames->Length-1]);
	last_image = cvLoadImage(last_filename);
			// transform last image. Must be done or can break when cropping
			if (flip_hor) {
				 cvFlip(last_image, last_image, 1);
			 }
			 if (rotate_ccw) {
					IplImage* rotated = cvCreateImage(cvSize(last_image->height, last_image->width), 8, 3); //size rotated 90 degrees
					UCHAR r, g, b;
					for (int x=0; x<last_image->width; x++)
						for (int y=0; y<last_image->height; y++) {
							MyGetPixel(last_image, x, y, &r, &g, &b);
							MyPutPixel(rotated, rotated->width-y, x, r, g, b);
							
						}
					cvReleaseImage(&last_image);
					last_image = cvCloneImage(rotated);
					cvReleaseImage(&rotated);

			 }
			  if (rotate_cw) {
					IplImage* rotated = cvCreateImage(cvSize(last_image->height, last_image->width), 8, 3); //size rotated 90 degrees
					UCHAR r, g, b;
					for (int x=0; x<last_image->width; x++)
						for (int y=0; y<last_image->height; y++) {
							MyGetPixel(last_image, x, y, &r, &g, &b);
							MyPutPixel(rotated, y, rotated->height-x, r, g, b);
							
						}
					cvReleaseImage(&last_image);
					last_image = cvCloneImage(rotated);
					cvReleaseImage(&rotated);

			 }
	int lastTipFileSave = 0; //marks the last number of minutes the file was saved at



	// LOOP THROUGH FILES  vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	while (filenumber<filenames->Length) {
		
	//mainWindowText(mainWindow_RT, "File number "+filenumber);
	
	char* filename = (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(filenames[filenumber]);
	
	sprintf(loadpath,    "%s", filename);

	
	
	String^ s = filenames[filenumber]->Insert(filenames[filenumber]->LastIndexOf("\\")+1, "_");
	

	String^ justFilename = filenames[filenumber]->Substring(filenames[filenumber]->LastIndexOf("\\")+1);
	String^ justSaveFilename = s->Substring(filenames[filenumber]->LastIndexOf("\\")+1);

	
	image = loadImage(loadpath, mainWindow);

	mainWindow_RT->setProgressBar(( ((double)filenumber) / (filenames->Length)) *100);
	mainWindow_RT->setMessage(justFilename);

	

	


	originali = cvCloneImage(image);

	
	previousi = cvCloneImage(image);
	output = cvCloneImage(image);
	tempimage = cvCloneImage(image);
	measuresi = cvCloneImage(image);
	measuresiTaboo = cvCloneImage(image); //now updated after start, in case of enhancing.
	//base_image = cvCloneImage(originali);

	
	//MessageBox::Show("enableEnhancing = "+enableEnhancing);
	if (enableEnhancing) interactiveEnhance(m_bilat_filter, m_med_filter, m_gaus_filter, m_suppressHor, m_bLocaLMaxFilter, m_bblowHighlights, m_blowHighlights, m_bgRemoval);

	


	if (croprect.width!=-1) cropImages(croprect);

	
	
	crop = cvCreateImage(cvSize(cropx, cropy), 8, 3);

	// AUTO SMOOTH AND ENHANCE!!! //moved to loadImage
	//if (mainWindow_RT->autoSmoothImages()) smooth(mainWindow);
	//if (mainWindow_RT->autoEnhanceImages()) enhance(mainWindow);
	
	if (filenumber==0) //first time only 
	{
		cvSaveImage("measuresi.jpg", measuresi); 
		preview_lengthsi = cvCreateImage(cvSize(filenames->Length+2, 800), 8, 1);
		cvZero(preview_lengthsi);
		//mainWindow_RT->updateMeasureWindow(measuresi);
	}


	//graph init
	CvMemStorage *g_storage = cvCreateMemStorage();
	CvGraph *g = cvCreateGraph(CV_SEQ_KIND_GRAPH | CV_GRAPH_FLAG_ORIENTED,sizeof(CvGraph),sizeof(MyVertex), sizeof(MyEdge), g_storage);

	// show the image in a window
	cvNamedWindow("Image", 2);
	if (filenumber==0) cvResizeWindow("Image", image->width/2, image->height/2);//resize window first time through, as image is likely to be large
	//fitWindowToImage();
	cvShowImage("Image", image);

	// get mouseclicks from the image
	cvSetMouseCallback( "Image", on_mouse_wrapper, this );

	quitnow = false;
	int rootnum = 0;

	
	//if (filenumber==0){
	//	char statspath[255];
	//	sprintf(statspath,    "%s\\%s_data.csv", outputFolderString, justFilename);
	//	stats.ChangeFile(statspath);
	//	stats.WriteTitles("File, Roots...");
	//}

	int new_state = tb_lr;

	//while(rootnum<num_roots){  

	
	int code;
	if (filenumber==0) code = cvWaitKey(10);
	else code = 32;

	while ( (goButtonPressed==false&&(/*code!=32 && */(rootnum==0 || rootnum==num_roots))) || bg_mask==NULL) {  //wait for space first time round UPDATE now space doesn't work - all from go button
		
		checkKeyPresses(code);
		if (quitnow) {
			mainWindowText(mainWindow, "Finished");	
			break;
		}

		code = cvWaitKey(10);//was 0 
		

	}
	if (quitnow) break;

	//cvCopyImage(measuresi, measuresiTaboo); // now done to copy potentially enhanced measures image



	sprintf(savepath,    "%s\\%s.%s", outputFolderString, justSaveFilename, outputExtension);//was jpg
	saveComments();//make sure at least the filenames are saved here
	//backup quicksave files
	if (savedStartPoints) {
		char quicksavep[256];
		char quicksavepout[256];
		sprintf(quicksavepout,"%s\\quicksave_starts.xml.bak", outputFolderString );
		String^ quicksavepoutss = gcnew String(quicksavepout);
		sprintf(quicksavep,"%s\\quicksave_starts.xml", workingFolder );
		String^ quicksavepss = gcnew String(quicksavep);
		File::Copy(quicksavepss, quicksavepoutss, true);

		char quicksavep2[256];
		char quicksavepout2[256];
		sprintf(quicksavepout2,"%s\\quicksave_background.bmp.bak", outputFolderString );
		String^ quicksavepoutss2 = gcnew String(quicksavepout2);
		sprintf(quicksavep2,"%s\\quicksave_background.bmp", workingFolder );
		String^ quicksavepss2 = gcnew String(quicksavep2);
		File::Copy(quicksavepss2, quicksavepoutss2, true);

		//mainprog->saveStartPoints(workingFolder+"\\quicksave_starts.xml");
		//mainprog->saveBGMask(workingFolder+"\\quicksave_background.bmp");

	}


	
	
	//cout<<"Loading "<<loadpath<<"..."<<endl;
	//mainWindowText(mainWindow_RT, "Save file: "+justSaveFilename);
	

	// re calc bg stats each image?
	
	if (recalcBGStats) multiple_BG_stats = getBGStats(bg_mask, measuresi);

    // ***********SSSSSSSSSSSS************ ROOT LOOP *************SSSSSSSSSSSSS*************

 	int num_grav_roots = 0;

	if (filenumber==0) { // First time only please

		// Asad  , for noting the sub roots found in each frame,
		//ofstream subRootTxt;
		subRootTxt.open(myspf("%s\\subroottxt.txt", outputFolderString));
		   
		subRootTxt<<"Total Subroots found per frame in each root\n---------------------------------------------------\n\n";

		for(int i=0; i<num_roots; i++)
			subRootTxt<<"\troot"<<i;
		subRootTxt<<"\n----------------------------------------------------------------------------"<<endl; 


		// -Asad

		// Moved so that output folder should now be set before this happens
		char statspath[255];
		sprintf(statspath,    "%s\\%s_data.csv", outputFolderString, justFilename);
		stats.ChangeFile(statspath);
		stats.WriteTitles("File, Roots...");
		
		char statspath2[255];
		sprintf(statspath2,    "%s\\tip_data.csv", outputFolderString);
		tip_stats.ChangeFile(statspath2);

		char statspath3[255];
		sprintf(statspath3,    "%s\\tip_angles.csv", outputFolderString);
		tip_angles.ChangeFile(statspath3);
		tip_angles.AppendDataRow("Time, Root Tip Angles");

		char statspath4[255];
		sprintf(statspath4,    "%s\\tip_angles_to_vertical.csv", outputFolderString);
		tip_angles_vert.ChangeFile(statspath4);
		tip_angles_vert.AppendDataRow("Time, Root Tip Angles To Vertical");

		char statspath5[255];
		sprintf(statspath5,    "%s\\trace_points.csv", outputFolderString);
		trace_points.ChangeFile(statspath5);


		sprintf(gravsavepath,    "%s\\%s", outputFolderString, "grav.csv");
		//String^ test = gcnew String(gravsavepath);
		//MessageBox::Show(test);
		gravstats.ChangeFile(gravsavepath);
		gravstats.WriteTitles("File, Num. grav roots");

		//grav distances.  Stores for each frame and root the estimated dij distance of grav point, and size and confidence
		char gravdistsavepath[512];
		sprintf(gravdistsavepath,    "%s\\%s", outputFolderString, "grav_dist.csv");	
		gravdiststats.ChangeFile(gravdistsavepath);
		gravdiststats.AppendDataRow_NoReturn("File, Root, Grav dist., Grav. onset dist, Angle, Confidence, ");
		for (int j=0; j<num_roots; j++) 
			gravdiststats.AppendDataRow_NoReturn("Root, Grav dist., Grav. onset dist, Angle, Confidence, ");
		gravdiststats.CarriageReturn();
	}





	while(rootnum<num_roots){ 

	doRootTracking( idealR, idealG, idealB, proSigmaX, proSigmaY, new_state, g, rootnum, filenumber, mainWindow_RT->getSlider_BGprior_value(), mainWindow_RT->getSlider_upperHyst(), mainWindow_RT->getSlider_lowerHyst(), mainWindow);


	doGraphTraversal(g, mainWindow);
		


	int best_indx=0;// Dijkstra only gives us one path
	int new_path_size = paths[best_indx].path.size();
	//int new_path_size2 = paths[best_indx].dist;
	CvPoint maxAreaPoint = cvPoint(0,0);
	CvPoint rootStart = cvPoint(0,0);
	CvPoint rootEnd = cvPoint(0,0);
	CvPoint furthestFromLine = cvPoint(0,0);
	double length_to_maxAreaPoint = -1;
	if (g==NULL) MessageBox::Show("g==null");
	//paths[0].path[0];//ok
	//((MyVertex*) cvGetGraphVtx(g, paths[0].path[0])); //breaks
	rootStart.x = ((MyVertex*) cvGetGraphVtx(g, paths[0].path[0]))->x;   /// ERROR HERE!!!
	rootStart.y = ((MyVertex*) cvGetGraphVtx(g, paths[0].path[0]))->y;
	rootEnd.x   = ((MyVertex*) cvGetGraphVtx(g, paths[0].path[new_path_size-1]))->x;
	rootEnd.y   = ((MyVertex*) cvGetGraphVtx(g, paths[0].path[new_path_size-1]))->y;
	
	if (filenumber==0) initialTipPathLocation[rootnum] = new_path_size-1;
	
	// TODO trace through the graph, but bail out when probability falls?
	double prob_running_total = 0;
	double prob_mean = 0;
	int prob_stat_count = 0;
	double cum_sq_devs=0;
	//st_dev = 0;
	
	
	
	for (int vtxn = 0; vtxn<paths[best_indx].path.size()-1;vtxn++) {
		MyVertex * myv1 = (MyVertex*) cvGetGraphVtx(g, paths[best_indx].path[vtxn]);
		//MyVertex * myv2 = (MyVertex*) cvGetGraphVtx(g, paths[best_index].path[vtxn+1]);

		if (debug)cout << "Node log10 prob: "<< log10(min(myv1->prob_non_normalized,1))<<endl;

		if (vtxn>15) {
			// start building model of probs
			prob_running_total += log10(min(myv1->prob_non_normalized, 1)); // 1 is max prob
			prob_stat_count++;
			prob_mean = prob_running_total/prob_stat_count;
			if (debug) cout << "  mean prob: "<< prob_mean<<endl;

			 //CHECK WHICH PATH THIS WORKS ON! POSSIBLE BUG? Spotted when moved graph traversal to a separate method
			/*if (do_line_end_finding&&vtxn>25 && log10(min(myv1->prob_non_normalized, 1)) < prob_mean - (threshold_st_dev_trace*st_dev)) 
			{
				cout <<" BREAK! End of root found"<<endl;
				//path.dist = myv1->dij_dist;  //distance to this node
				//cout <<" * dist = " <<path.dist<<endl;
				paths[best_indx].dist = myv1->dij_dist;
				//paths.clear();
				//paths.push_back(path);  //update with distance.  With dij we only have one path anyway though, so don't really
										// need the vector
				new_path_size= vtxn;  //shorten number of nodes in path
				break; 

			}*/




			// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			// !!!!! THIS WAY OF CALCULATING S.D cumulatively is WRONG!!!!
			// !!!!! SEE http://www.westgard.com/lesson34.htm         !!!!
			// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			// (because mean is changing all the time)

			//cum_sq_devs+= (prob_mean - log10(min(myv1->prob_non_normalized, 1)))*(prob_mean - log10(min(myv1->prob_non_normalized, 1)));
			//cout <<"  cum sqs "<<cum_sq_devs<< endl;
			
			//st_dev = sqrt(cum_sq_devs / (prob_stat_count-1));

			//cout << "  st dev prob: "<< st_dev<<endl<<endl;
		}


	} //end vtxn loop




	// get max and min probs found
	double max_prob = 0;
	double min_prob = 2;
	for (int q=0; q<num_paths_to_test; q++) {
		if (paths[q].prob>max_prob) 
			max_prob = paths[q].prob;
		if (paths[q].prob<min_prob) 
			min_prob = paths[q].prob;
	}

	double min_dist = 9999999999999999999;
	int best_index = -1;
	for (int q=0; q<num_paths_to_test; q++) {
	//cout<<"xxxxxxxxxxx"<<endl;
	//cout<<"Path "<<q<<endl;
	//cout<<" Prob "<<paths[q].prob<<", dist "<<paths[q].dist<<endl;
	
	// only accept if in the upper range of probs??  COMMENTED OUT
	if (min_dist>paths[q].dist/* && paths[q].prob>(min_prob+(0.75*(double)(max_prob-min_prob)))*/) {
		min_dist = paths[q].dist;
		best_index=q;
	}

	}
	
	cout<<endl<<"Chosen path "<<best_index<<endl;
	cout<<"Prob = "<<paths[best_index].prob<<", dist = "<< paths[best_index].dist<<endl;
	cout<<"Path length = "<<paths[best_index].path.size()<<endl;

	
	
	cvCopy(originali, image);

	int old_px=-1;
	int tot_x_backtracks=0;
	int old_py=-1;
	int tot_y_backtracks=0;
	CvPoint p1, p2;

	double lineSumSqs[max_number_steps_to_trace]; 
	double bend_angles[max_number_steps_to_trace];
	double a1_store[max_number_steps_to_trace];
	double b1_store[max_number_steps_to_trace];
	double a2_store[max_number_steps_to_trace];
	double b2_store[max_number_steps_to_trace];

	for (int i=0; i<max_number_steps_to_trace; i++) {
		lineSumSqs[i]=0;
		bend_angles[i]=0;
		a1_store[i]=0;
		a2_store[i]=0;
		b1_store[i]=0;
		b2_store[i]=0;
	}
	
	// ****************************** DRAWING ******************************************

	const int border = d+1;	// border between which we try and find bend point, ie exclude ends. was 20
	double max_area = 0; 
	double max_distance = 0; 
	for (int vtxn = 0; vtxn<new_path_size;vtxn++) { /// >>>>>>OOOOOOOOoooooooOOOOOOOO>>>>>>>>>> TO DO - TAKE OUT -1 so that curvature saved to last vertex??
		//get nodes for drawing line
		MyVertex * myv1 = (MyVertex*) cvGetGraphVtx(g, paths[best_index].path[vtxn]);
		MyVertex * myv2;
		if (vtxn<new_path_size-1) myv2 = (MyVertex*) cvGetGraphVtx(g, paths[best_index].path[vtxn+1]); //only look ahead if not at least segment

		MyVertex * prev_step_v = NULL;
		MyVertex * next_step_v = NULL;

		p1.x = myv1->x;
		p1.y = myv1->y;
		// Choose next point to draw segment to, unless we are the last segment in which case it it won't be drawn anyway
		if ( vtxn<new_path_size-1){
			p2.x = myv2->x;
			p2.y = myv2->y;
		} else {
			p2.x = 0;
			p2.y = 0;
		}


		bool insideBorder = true;


		// CURVATURE ESTIMATION
		//---------------------

		// <<<< LINES >>>>
		bool do_line_fitting = true;
		
		if (do_line_fitting) {
			

			//double prev_x[d+1];//was d+1
			//double prev_y[d+1];//was d+1
			//double next_x[d+1];//was d+1
			//double next_y[d+1];//was d+1


			//float  sig[d];

			//for (int init=0; init<d; init++) 
			//	sig[init] = 0;

			

			// 11111111111111111111111111111111
			// Curvature and gravitropic point

			if (do_bend_detection) {
				CvPoint start;
				CvPoint end;
				CvPoint current;
				double length_to_here = -1;
				current.x = ((MyVertex*) cvGetGraphVtx(g, paths[best_index].path[vtxn]))->x;
				current.y = ((MyVertex*) cvGetGraphVtx(g, paths[best_index].path[vtxn]))->y;
				length_to_here = ((MyVertex*) cvGetGraphVtx(g, paths[best_index].path[vtxn]))->dij_dist;
				double length_to_end = ((MyVertex*) cvGetGraphVtx(g, paths[best_index].path[new_path_size-1]))->dij_dist;//new path size???
				
				if (length_to_here>border && length_to_here < length_to_end-border){
					// no point measuring if too near the ends??
					
					//// we need point arrays 0 to vtxn, and vtxn to end.
					//double line1x[max_number_steps_to_trace];
					//double line1y[max_number_steps_to_trace];
					//double line2x[max_number_steps_to_trace];
					//double line2y[max_number_steps_to_trace];
					//
					//int line1_count=0;//0
					//int line2_count=0;

					//for (int i=0; i<max_number_steps_to_trace; i++) {
					//	line1x[i] = -1;
					//	line1y[i] = -1;
					//	line2x[i] = -1;
					//	line2y[i] = -1;
					//}

										
					// line points, 0 to vtxn
					/*for (int i=0; i<=vtxn; i++){
						line1x[i] = ((MyVertex*) cvGetGraphVtx(g, paths[best_index].path[i]))->x;
						line1y[i] = ((MyVertex*) cvGetGraphVtx(g, paths[best_index].path[i]))->y;
						line1_count++;
						//MyPutPixel(output, line1x[i], output->height-line1y[i], 255, 128, 255);
						//cvCircle(output,  cvPoint(line1x[i], line1y[i]), 2, cvScalar(255, 128, 255), -1); 
					}

					//line points, vtxn to end
					for (int i=vtxn; i<new_path_size-1; i++) {
						line2x[i-vtxn] = ((MyVertex*) cvGetGraphVtx(g, paths[best_index].path[i]))->x;
						line2y[i-vtxn] = ((MyVertex*) cvGetGraphVtx(g, paths[best_index].path[i]))->y;
						line2_count++;
						//MyPutPixel(output, line2x[i-vtxn], output->height-line2y[i-vtxn], 128, 255, 128);
						//cvCircle(output,  cvPoint(line2x[i], line2y[i]), 2, cvScalar(128, 255, 128), -1);
					}*/

					// NEW way.  work out angle between current vtxn and points (border-1) away?
					
					double diff_length = 0;
					int dv = 1;
					while(diff_length<border && vtxn-dv>=0) {
						double current_dij_dist = ((MyVertex*) cvGetGraphVtx(g, paths[best_index].path[vtxn-dv]))->dij_dist;
						diff_length = length_to_here-current_dij_dist;
						start.x = ((MyVertex*) cvGetGraphVtx(g, paths[best_index].path[vtxn-dv]))->x;
						start.y = ((MyVertex*) cvGetGraphVtx(g, paths[best_index].path[vtxn-dv]))->y;
						dv++;
					}

					if (!(vtxn-dv>=0)) insideBorder  = false;
					
					diff_length = 0; 
					dv = 1;
					while(diff_length<border && vtxn+dv<=new_path_size) {
						double current_dij_dist = ((MyVertex*) cvGetGraphVtx(g, paths[best_index].path[vtxn+dv]))->dij_dist;
						diff_length = current_dij_dist-length_to_here;
						//MessageBox::Show("diff_length = "+diff_length);
						end.x = ((MyVertex*) cvGetGraphVtx(g, paths[best_index].path[vtxn+dv]))->x;
						end.y = ((MyVertex*) cvGetGraphVtx(g, paths[best_index].path[vtxn+dv]))->y;
						dv++;
					}

					if (!(vtxn+dv<=new_path_size)) insideBorder  = false;

					CvPoint vectorStoC;
					vectorStoC.x = current.x-start.x;
					vectorStoC.y = current.y-start.y;
					CvPoint vectorCtoE;
					vectorCtoE.x = end.x-current.x;
					vectorCtoE.y = end.y-current.y;
					double gradient1 = (double)vectorStoC.y/vectorStoC.x;
					double gradient2 = (double)vectorCtoE.y/vectorCtoE.x;
					double overallAngle;
					if ( (gradient2<0 && gradient1<0) || (gradient2>0 && gradient1>0) ) {
						// both same sign
						overallAngle = abs(atan(abs(gradient2))-atan(abs(gradient1)));
					}else {
						overallAngle = abs(atan(abs(gradient2))+atan(abs(gradient1)));
					}

					overallAngle = overallAngle*57.2957795; 
					
					if (gradient2*gradient1>0) overallAngle = 180-overallAngle;  // if both same sign...
					if (overallAngle<90) overallAngle = 180-overallAngle;
					
					//cvLine(output, start, current, cvScalar(120, 120, 120), 1);
					//cvLine(output, current, end, cvScalar(120, 120, 120), 1);
					//cvCircle(output,  start, 5, cvScalar(128, 255, 128), -1);
					//cvCircle(output,  end, 5, cvScalar(128, 255, 128), -1);
					
					
					// end new angle calc


					
					// old angle calc
					/*double total_sumSq=0;

					double results[21];
					int fail;
					double a1, a2, b1, b2;
					 //commented out to try to find bug
					// NAG Fortran library - calc fits for large bend detection
					G02CAF(line1_count, line1x, line1y, results, fail);
					total_sumSq+= results[15];//was 15
					b1 = results[5];
					a1 = results[6];
					
					G02CAF(line2_count, line2x, line2y, results, fail);
					total_sumSq+= results[15];//was 15
					b2 = results[5];
					a2 = results[6];

					//Now we have total error for both fitted lines
					lineSumSqs[vtxn] = total_sumSq;*/

					//cvShowImage("Image", output);
					//cvWaitKey(1);
					//MessageBox::Show("SS = "+total_sumSq);

					
			//// debug graph
			//if (debuggraph==NULL) {
			//	debuggraph = cvCreateImage(cvSize(700, 300), 8, 1);
			//	cvZero(debuggraph);
			//	oldpoint.x=-1;
			//	
			//}
			//
			//// marker lines
			////int yoffset=0;
			////cvLine(debuggraph, cvPoint(0, yoffset), cvPoint(debuggraph->width-1, yoffset), cvScalar(128), 1);//0 line
			////cvLine(debuggraph, cvPoint(0, 0-90+yoffset), cvPoint(debuggraph->width-1, 0-90+yoffset), cvScalar(64), 1);//-pi/2 line
			////cvLine(debuggraph, cvPoint(0, 90+yoffset), cvPoint(debuggraph->width-1, 90+yoffset), cvScalar(64), 1);//pi/2 line

			//double scale = 0.8;
			//if (rootnum==0) {
			//	if (oldpoint.x==-1) {
			//		oldpoint.x = myv1->dij_dist*2;
			//		oldpoint.y = total_sumSq; 					
			//	}
			//	cvLine(debuggraph, oldpoint, cvPoint(myv1->dij_dist*2, (double)total_sumSq/500000*500.0), cvScalar(255), 2);
			//	oldpoint.x = myv1->dij_dist*2;
			//	oldpoint.y = (double)total_sumSq/500000*500.0; 
			//	cvNamedWindow("Graph", 2);
			//	cvShowImage("Graph", debuggraph);
			//}
			//
			// end debug graph

					//angle between two lines 
					// tan theta = (m2-m1) / (1+m1m2)
					
					
					// OLD WAY
					//bend_angles[vtxn] = ( atan( (b1-b2) / (1+b1*b2) )*57.2957795); // and convert to degrees. Adding abs keeps angle 0-180
					
					/*// NEW!!
					double angle;
					if ( (b2<0 && b1<0) || (b2>0 && b1>0) ) {
						// both same sign
						angle = abs(atan(abs(b2))-atan(abs(b1)));
					}else {
						angle = abs(atan(abs(b2))+atan(abs(b1)));
					}

					angle = angle*57.2957795; 
					
					if (b2*b1>0) angle = 180-angle;  // if both same sign...
					if (angle<90) angle = 180-angle;*/

					// WARNING these assume that no angle is <90� (i.e. all obtuse, is this sensible for grav response?...) 



					// *********** ACTUALLY USE NEW ANGLE **************
					double angle = overallAngle;
					a1_store[vtxn] = -1.0*(gradient1*current.x - current.y);
					a2_store[vtxn] = -1.0*(gradient2*current.x - current.y);
					b1_store[vtxn] = gradient1;
					b2_store[vtxn] = gradient2; 

					// new curvature calc, based on curvature equation HK2003
					double dist_s = sqrt((double)vectorStoC.x*vectorStoC.x + vectorStoC.y*vectorStoC.y);
					double dist_e = sqrt((double)vectorCtoE.x*vectorCtoE.x + vectorCtoE.y*vectorCtoE.y);
					//to mm?
					dist_s = dist_s*calib_mmPerPix;// should I do this?, perhaps not...
					dist_e = dist_e*calib_mmPerPix;
					double angle_s = atan( (double)abs(start.x-current.x)/abs(start.y-current.y) ); 
					double angle_e = atan( (double)abs(end.x-current.x)/abs(end.y-current.y) ); 
					double theta = 0.5*angle_s + 0.5*angle_e;
					double d_e = abs(angle_e-theta);
					double d_s = abs(angle_s-theta);
					double curvature = 0.5*(d_s/dist_s) + 0.5*(d_e/dist_e);

					//curvature = curvature * 57.2957795; // WHY DO I DO THIS? DOESN'T MAKE SENSE

					//MessageBox::Show("Angle = "+overallAngle+", curv = "+curvature);




					//store best fit grav angle at this point....
					bend_angles[vtxn] = angle;

					myv1->angle = 180-angle; // ********** THIS WAS STORED BELOW, IN LOCAL CURV CALC
					
					myv1->curvature = curvature; 
					
			} else insideBorder  = false;
			// AREA OF TRIANGLE FROM POINTS? Three points are start, end, current?
			double area = 0.5 * abs( rootStart.x*(rootEnd.y-current.y) + current.x*(rootStart.y-rootEnd.y) + rootEnd.x*(current.y-rootStart.y) );
			if (area>max_area) { 
				maxAreaPoint.x = current.x;
				maxAreaPoint.y = current.y;
				max_area = area;
				length_to_maxAreaPoint = length_to_here;
			}

			//distance to line between start and end
			CvPoint vectorStoE;

			double distance = abs((rootStart.y-rootEnd.y)*current.x + (rootEnd.x-rootStart.x)*current.y + (rootStart.x*rootEnd.y - rootEnd.x*rootStart.y))
								/ sqrt( pow((double)(rootEnd.x-rootStart.x),2)+ pow((double)(rootEnd.y - rootStart.y),2)) ;
			if (distance > max_distance) {
				max_distance = distance;
				furthestFromLine.x = current.x;
				furthestFromLine.y = current.y;
			}


			}


			// 2222222222222222222222222222222222222222222222222222222222
			// we also want to measure curvature at each point along root
			// This is done by fitting small lines either side of
			// point of interest.
			//if (vtxn>d && vtxn<new_path_size-d-1) {
			//	//IplImage* im = cvCloneImage(image);

			//	CvPoint oldp;
			//	int counter = 0; //seems this does need to be 0 and not 1 for Fortran... hmm
			//	bool xchanged=false, ychanged=false;
			//	for (int count = vtxn-d; count<=vtxn; count++) {
			//			CvPoint2D32f p;
			//			p.x = ((MyVertex*) cvGetGraphVtx(g, paths[best_index].path[count]))->x;
			//			p.y = ((MyVertex*) cvGetGraphVtx(g, paths[best_index].path[count]))->y;
			//			//cvSeqPush(prev_points,&cvPoint2D32f(p.x, image->height-p.y));
			//			//MyPutPixel(im, p.x, im->height-p.y, 255, 0 ,255);//highlight point on image
			//			prev_x[counter] = (double)p.x;
			//			prev_y[counter] = (double)p.y;
			//			if (counter>0){
			//				if (oldp.x!=p.x) xchanged = true;
			//				if (oldp.y!=p.y) ychanged = true;

			//			}
			//			oldp.x = p.x;
			//			oldp.y = p.y;
			//			counter++;

			//	}
			//	if (!xchanged || !ychanged) MessageBox::Show("WARNING x or y line set has identical numbers");

			//	counter = 0;
			//	xchanged=false;
			//	ychanged=false;
			//	for (int count = vtxn; count<=vtxn+d; count++) {
			//			CvPoint2D32f p;
			//			p.x =  ((MyVertex*) cvGetGraphVtx(g, paths[best_index].path[count]))->x;
			//			p.y =  ((MyVertex*) cvGetGraphVtx(g, paths[best_index].path[count]))->y;
			//			//cvSeqPush(next_points,&cvPoint2D32f(p.x, image->height-p.y));
			//			//MyPutPixel(im, p.x, im->height-p.y, 255, 255 ,0);//highlight point on image
			//			next_x[counter] = p.x;
			//			next_y[counter] = p.y;
			//			if (counter>0){
			//				if (oldp.x!=p.x) xchanged = true;
			//				if (oldp.y!=p.y) ychanged = true;

			//			}
			//			oldp.x = p.x;
			//			oldp.y = p.y;
			//			counter++;
			//			
			//	}

			//	if (!xchanged || !ychanged) MessageBox::Show("WARNING x or y line set has identical numbers");


			//

			//// calculate fits for local curvature?

			////double *resultp = new double[21];
			//double resultp[21];
			//int fail;

			//CvPoint p;
			//p.x =  myv1->x;
			//p.y =  myv1->y;

			//cvCircle(output, p, 3, cvScalar(255, 0 , 50), -1);
			//cvShowImage("Image", output);
			//cvWaitKey(1);


			//
			//// NAG Fortran library
			//G02CAF(d-1, prev_x, prev_y, resultp, fail);//d-1 so we know we aren't reading into unset values


			////y = a + bx;
			//double a_prev = resultp[6]; 
			//double b_prev = resultp[5];

			////delete [] resultp; //this causes corrupted memory exception in debug

			////double *resultn = new double[21];
			//double resultn[21];

			//// NAG Fortran library
			//G02CAF(d-1, next_x, next_y, resultn, fail);


			////y = a + bx;
			//double a_next=  resultn[6]; 
			//double b_next = resultn[5];

			////delete [] resultn; //this casues corrupted memory exception in debug

			////angle between two lines 
			//// tan theta = (m2-m1) / (1+m1m2)

			//double angle = 0-1;
			////if (b_next>b_prev) angle = atan((b_next-b_prev)/ (1.0+ b_next * b_prev));  OLD WAY
			////else               angle = atan((b_prev-b_next)/ (1.0+ b_prev * b_next));

			//// NEW way - see p 83 lab book onwards
			//if ( (b_next<0 && b_prev<0) || (b_next>0 && b_prev>0) ) {
			//	// both same sign
			//	angle = abs(atan(abs(b_next))-atan(abs(b_prev)));
			//}else {
			//	angle = abs(atan(abs(b_next))+atan(abs(b_prev)));
			//}

			//angle = angle*57.2957795; // radians to degrees

			//if (b_next*b_prev>0) angle = 180-angle;  // if both same sign...
			//if (angle<90) angle = 180-angle;

			//// WARNING these assume that no angle is <90� (i.e. all obtuse, which is sensible...) 


			////work out sign of angle! - see page 88 lab book
			//// only need to do negative ones
			//if (  b_prev<0 && b_next>0) angle *= -1.0;

			//if ( (b_prev>0 && b_next>0) && (abs(b_prev)>abs(b_next))) angle *= -1.0;

			//if ( (b_prev<0 && b_next<0) && (abs(b_next)>abs(b_prev))) angle *= -1.0;

			//// + and - seems other way around to what I'ds expect...

			//


			//// END new way


			//char debugstring[255];
			//sprintf(debugstring, "%.1f, %.1f", b_prev, b_next);
			//debugstats.AppendDataRow(debugstring);

			//		

			//int line_length_mult = 30;
			//CvPoint prev_secondLinePoint;			
			//prev_secondLinePoint.y = myv1->y - 20;//myv1->x +(line_length_mult*line_prev[0]);
			//prev_secondLinePoint.x = ((double)prev_secondLinePoint.y)/b_prev - (a_prev/b_prev);//myv1->y +(line_length_mult*line_prev[1]);

			//CvPoint next_secondLinePoint;
			//next_secondLinePoint.y = myv1->y + 20;//myv1->x +(line_length_mult*line_prev[0]);
			//next_secondLinePoint.x = ((double)next_secondLinePoint.y)/b_next - (a_next/b_next);//myv1->y +(line_length_mult*line_prev[1]);


			//// Show the lines either side of current point
			///*
			//IplImage* im = cvCloneImage(image);//need to declare this earlier if you want to draw points, above
			//
			//if (debuggraph==NULL) {
			//	debuggraph = cvCreateImage(cvSize(500, 300), 8, 1);
			//	cvZero(debuggraph);
			//}
			//cvLine(im, cvPoint(myv1->x, myv1->y), prev_secondLinePoint, cvScalar(255, 0, 0), 2);
			//cvLine(im, cvPoint(myv1->x, myv1->y), next_secondLinePoint, cvScalar(0, 0, 255), 2);
			//double newangle = angle;// its the same thing these days...
			//imgTxt(im, myspf("Angle %.2f, m1=%.1f, m2=%.2f",      newangle, b_prev, b_next   ), cvPoint(30, 30), CV_RGB(255,0,0) );
			//int yoffset = 150;//added to angle so we can display negative values
			//
			//// marker lines
			//cvLine(debuggraph, cvPoint(0, yoffset), cvPoint(debuggraph->width-1, yoffset), cvScalar(128), 1);//0 line
			//cvLine(debuggraph, cvPoint(0, 0-90+yoffset), cvPoint(debuggraph->width-1, 0-90+yoffset), cvScalar(64), 1);//-pi/2 line
			//cvLine(debuggraph, cvPoint(0, 90+yoffset), cvPoint(debuggraph->width-1, 90+yoffset), cvScalar(64), 1);//pi/2 line

			//double scale = 0.8;
			//if (rootnum==0) {
			//	if (oldpoint.x==-1) {
			//		oldpoint.x = myv1->dij_dist*2;
			//		// oldpoint.y = newangle*scale+yoffset; old
			//		if (angle>=90) 
			//			oldpoint.y = yoffset+(180-newangle)*scale;
			//		else //-ve
			//			oldpoint.y = yoffset+(-180-newangle)*scale;
			//		
			//	}
			//	//MyPutPixel(debuggraph, myv1->dij_dist, newangle, 255, 255, 255);
			//	double thetay;
			//	if (angle>=90) 
			//			thetay = yoffset+(180-newangle)*scale;
			//		else //-ve
			//			thetay = yoffset+(-180-newangle)*scale;
			//	cvLine(debuggraph, oldpoint, cvPoint(myv1->dij_dist*2, thetay), cvScalar(255), 2);
			//	oldpoint.x = myv1->dij_dist*2;
			//	//oldpoint.y = newangle*scale+yoffset;
			//	if (angle>=90) 
			//			oldpoint.y = yoffset+(180-newangle)*scale;
			//		else //-ve
			//			oldpoint.y = yoffset+(-180-newangle)*scale;
			//	cvNamedWindow("Graph", 2);
			//	cvShowImage("Graph", debuggraph);
			//}


			////display animated angle lines on image
			//cvNamedWindow("Output", 2);			
			//cvShowImage("Output", im);
			//cvWaitKey(0);
			//cvReleaseImage(&im);
			//*/



			//// **** STORE ANGLE AT THIS NODE

			//myv1->angle = angle;
			//
			//
			////cvReleaseMemStorage(&store1);
			////cvReleaseMemStorage(&store2);
			//
			//}  // if we can look at points either side of current point ie. not near ends 
			
		} //end vtxn loop, 
		
			
		bool draw_x_backtracks = false;
		bool draw_y_backtracks = false;


		//double angle = atan((double)(abs(old_px-p1.x)/abs(old_py-p1.y)))* 180 / PI;
		//cout << "angle = "<< angle<<endl;
		CvScalar col;
		if (myv1->state==0/*&&myv2->state==0*/) {
			//col = cvScalar(255,0 , 255);
			col = cvScalar(255,0 , 255);
			if (draw_x_backtracks && old_px>p1.x) {
				cout <<"WARNING: line backtracking in x axis..."<<endl;
				tot_x_backtracks++;
				col = cvScalar(0);
			}
		} else
		if (myv1->state==1/*&&myv2->state==1*/) {
			//col = cvScalar(0, 255, 255);
			col = cvScalar(0, 255, 255);
			if (draw_y_backtracks && old_py>=p1.y) {
				cout <<"WARNING: line backtracking in y axis..."<<endl;
				tot_y_backtracks++;
				col = cvScalar(0);
			}
		} /*else
		if ((myv1->state==0&&myv2->state==1)  || (myv1->state==1&&myv2->state==0)) {
			col = cvScalar(255, 255, 0);
		}*/

	
		// actually, change the colour depending on curvature
		/*if (abs(myv1->angle)>170) //was 5
			col = cvScalar(150, 0, 50);
		else if (abs(myv1->angle)>160)
			col = cvScalar(130, 0, 70);
		else if (abs(myv1->angle)>150)
			col = cvScalar(110, 0, 90);
		else if (abs(myv1->angle)>140)
			col = cvScalar(50, 50, 180);
		else if (abs(myv1->angle)>130)
			col = cvScalar(0, 200, 255);
		else if (abs(myv1->angle)>120)
			col = cvScalar(0, 255, 255);
		else if (abs(myv1->angle)>110)
			col = cvScalar(100, 255, 255);
		else 
		    col = cvScalar(255, 210, 210);*/ //OLD COLOURS

			 
		if (abs(myv1->angle)>color_bins[0]) 
			col = color_map[0];
		else if (abs(myv1->angle)>color_bins[1])
			col = color_map[1];
		else if (abs(myv1->angle)>color_bins[2])
			col = color_map[2];
		else if (abs(myv1->angle)>color_bins[3])
			col = color_map[3];
		else if (abs(myv1->angle)>color_bins[4])
			col = color_map[4];
		else if (abs(myv1->angle)>color_bins[5])
			col = color_map[5];
		else if (abs(myv1->angle)>=color_bins[6])
			col = color_map[6];




		if (vtxn==0) {
			//write out root number
			imgTxt(output, myspf("%i",rootnum), start_ps[rootnum], cvScalar(0, 0, 255));

		}
		
		if (insideBorder) {
			curvatures[filenumber,vtxn, rootnum] = myv1->angle;
			//curvatures2[filenumber,vtxn, rootnum] = myv1->curvature;
			lengthForCurvature[filenumber, vtxn, rootnum] = myv1->dij_dist;
		}
		else 
		{
			// no curvature measurment possible
			curvatures[filenumber,vtxn, rootnum] = 999;
			//curvatures2[filenumber,vtxn, rootnum] = 999;
			lengthForCurvature[filenumber, vtxn, rootnum] = myv1->dij_dist;
			col = cvScalar(0,0,0); // no value so paint black

		}
		


		old_px = p1.x;
		old_py = p1.y;

		if (draw_root_trace && vtxn<new_path_size-1) cvLine(output, p1, p2, col, 2);  //drawing the actual root trace, unless we're at final segment (in which case there is no point to draw to)
		//if (gravOnsetLength[rootnum]>0 && myv1->dij_dist>gravOnsetLength[rootnum]) cvCircle(output, p1, 3, cvScalar(0, 255, 255), 1);
		
		if (filenumber==filenames->Length-1) {//last image, so plot final traces
			char tracestring[255];
			sprintf(tracestring, "%i, %.1f, %.1f", rootnum, myv1->x, myv1->y);
			trace_points.AppendDataRow(tracestring);
		}
		
	}// for all vertices in path

	

	cout<<"Total y backtracks = "<<tot_y_backtracks<<endl;
	cout<<"Total x backtracks = "<<tot_x_backtracks<<endl;


	//find grav point
	double thresh_angle = bend_angle_thresh;
	double minSS = 99999999;
	double ultimateMinSS = 99999999;
	double bendx=-1, bendy=-1, benda1=-1, bendb1=-1, benda2=-1, bendb2=-1, bendAngle=999;
	double confidence = 0;
	

	int minPointsForLine = 0;
	int totalCount = 0;
	for (int i=border+1+minPointsForLine; i<new_path_size-1-border-minPointsForLine; i++) {
		if (lineSumSqs[i]<ultimateMinSS)
			ultimateMinSS = lineSumSqs[i];
		totalCount++;
	}
	
	
	CvPoint start, end;
	int bend_angle_index;
	double dij_bend_dist = 0, dij_bend_onset_dist=0;//distance to bend point.  can be used to work out onset?  Store all and median?
	for (int i=border+1+minPointsForLine; i<new_path_size-1-border-minPointsForLine; i++) {
		
		if (bend_angles[i]<bendAngle && bend_angles[i]<bend_angle_thresh/*lineSumSqs[i]<=minSS &&  bend_angles[i]<thresh_angle*//*lineSumSqs[i]<ultimateMinSS*1.1 && bend_angles[i]<bendAngle*/ ) {
			bendx = ((MyVertex*) cvGetGraphVtx(g, paths[best_index].path[i]))->x;
			bendy = ((MyVertex*) cvGetGraphVtx(g, paths[best_index].path[i]))->y;
			minSS = lineSumSqs[i];
			benda1 = a1_store[i];
			bendb1 = b1_store[i];
			benda2 = a2_store[i];
			bendb2 = b2_store[i];
			bendAngle = bend_angles[i];
			bend_angle_index = i;
			start.x = ((MyVertex*) cvGetGraphVtx(g, paths[best_index].path[i-border+1]))->x;
			start.y = ((MyVertex*) cvGetGraphVtx(g, paths[best_index].path[i-border+1]))->y;
			end.x = ((MyVertex*) cvGetGraphVtx(g, paths[best_index].path[i+border-1]))->x;
			end.y = ((MyVertex*) cvGetGraphVtx(g, paths[best_index].path[i+border-1]))->y;
			dij_bend_dist = ((MyVertex*) cvGetGraphVtx(g, paths[best_index].path[i]))->dij_dist;
			// seems to work nicely, currently uses same distance as for overall curvature measure.
			// is that ok? Can I simplify?

			int lowerCount = 0;
			for (int j=border+1+minPointsForLine; j<new_path_size-1-border-minPointsForLine; j++) {
				if (bend_angles[j]<bendAngle *1.2)
					lowerCount++;
			}
			confidence = 100.0-((double)lowerCount/totalCount)*100.0;
			// low confidence indicates other lines fitted better but might be ABOVE angle thresh?
			
		}
	}

	int bend_onset_x=0, bend_onset_y=0;	
	if (bendx!=-1) {
		if (drawBendPoint) {
			if (confidence > 30) {
				cvCircle(output, cvPoint(bendx, bendy), 5, cvScalar(0, 0, 255), -1);		
				// plot line to debug
				int draw_point_1, draw_point_2;
				if (bendb1>0) draw_point_1 = -10; else draw_point_1 = 10;
				if (bendb2>0) draw_point_2 = 10;  else draw_point_2 = -10;
				int p1y = (double)(bendx+(double)draw_point_1)*bendb1+benda1;
				int p2y = (double)(bendx+(double)draw_point_2)*bendb2+benda2;
				//int p1y = (double)(bendx+(double)draw_point_1)*bendb1+benda1;
				//int p2y = (double)(bendx+(double)draw_point_2)*bendb2+benda2;
				if (drawBendInfo) cvLine(output, cvPoint(bendx, bendy), cvPoint(bendx+draw_point_1, p1y), cvScalar(0, 255, 0), 2);
				if (drawBendInfo) cvLine(output, cvPoint(bendx, bendy), cvPoint(bendx+draw_point_2, p2y), cvScalar(0, 255, 0), 2);
				if (drawBendInfo) cvLine(output, cvPoint(bendx, bendy), end, cvScalar(255, 255, 0), 1);
				if (drawBendInfo) cvLine(output, cvPoint(bendx, bendy), start, cvScalar(255, 255, 0), 1);
				if (drawBendInfo) imgTxt(output, myspf("%0.1f (%0.1f), %0.1f",bendAngle, confidence, dij_bend_dist), cvPoint(bendx+10, bendy), CV_RGB(255,0,0) );
				if (drawBendInfo) cvCircle(output,  start, 5, cvScalar(128, 255, 128), -1); //yucky green colour, start point
				if (drawBendInfo) cvCircle(output,  end, 5, cvScalar(128, 255, 128), -1);   //yucky green colour, end point
				num_grav_roots++;


				// Find ONSET point, by looking back through the curvature values from highest curve point
				int temp_i=bend_angle_index;
				while (bend_angles[temp_i]<bendAngle*1.1 && temp_i>border) {  // ie find point 10% wider angle
					bend_onset_x = ((MyVertex*) cvGetGraphVtx(g, paths[best_index].path[temp_i]))->x;
					bend_onset_y = ((MyVertex*) cvGetGraphVtx(g, paths[best_index].path[temp_i]))->y;
					dij_bend_onset_dist = ((MyVertex*) cvGetGraphVtx(g, paths[best_index].path[temp_i]))->dij_dist;
					temp_i--;
				}
				cvCircle(output, cvPoint(bend_onset_x, bend_onset_y), 4, cvScalar(255, 0, 0), -1);

				
				////write out grav_dist stats
				////File, Root, Grav dist., Angle, Confidence (for each root)
				//if (rootnum==0) sprintf(gravdiststring, "%i, %i, %.1f, %.1f, %.1f,", filenumber, rootnum, dij_bend_dist, bendAngle, confidence);
				//else sprintf(gravdiststring, "%i, %.1f, %.1f, %.1f,", rootnum, dij_bend_dist, confidence);// no file num
				//gravdiststats.AppendDataRow_NoReturn(gravdiststring);

			}//confident
			else cvCircle(output, cvPoint(bendx, bendy), 5, cvScalar(0, 0, 0), -1); //black, low confidence

		} // if drawbendpoint

	} //if (bendx!=-1)

	//draw bend point based on area of triangle
	if (drawBendInfo) {
		cvCircle(output, maxAreaPoint, 5, cvScalar(255, 255, 255), 1);
		cvCircle(output, furthestFromLine, 4, cvScalar(255, 0, 255), 1);
		cvLine(output, rootStart, maxAreaPoint, cvScalar(255, 255, 255), 1);  // largest angle line
		cvLine(output, rootEnd, maxAreaPoint, cvScalar(255, 255, 255), 1);    // largest angle line
	}

	if (doGravitropic) {
		// ANGLE OF GRAV TROP POINT - NEW version March 09
		CvPoint vectorStoC;
		vectorStoC.x = maxAreaPoint.x-rootStart.x;
		vectorStoC.y = maxAreaPoint.y-rootStart.y;
		CvPoint vectorCtoE;
		vectorCtoE.x = rootEnd.x-maxAreaPoint.x;
		vectorCtoE.y = rootEnd.y-maxAreaPoint.y;
		double gradient1 = (double)vectorStoC.y/vectorStoC.x;
		double gradient2 = (double)vectorCtoE.y/vectorCtoE.x;
		double largeScaleAngle;
		if ( (gradient2<0 && gradient1<0) || (gradient2>0 && gradient1>0) ) {
			// both same sign
			largeScaleAngle = abs(atan(abs(gradient2))-atan(abs(gradient1)));
		}else {
			largeScaleAngle = abs(atan(abs(gradient2))+atan(abs(gradient1)));
		}

		largeScaleAngle = largeScaleAngle*57.2957795; 

		if (gradient2*gradient1>0) largeScaleAngle = 180-largeScaleAngle;  // if both same sign...
		if (largeScaleAngle<90) largeScaleAngle = 180-largeScaleAngle;

		largeScaleAngle = 180 - largeScaleAngle;

		//if (writeAngleOnRoot) imgTxt(output, myspf("A=%.1f", largeScaleAngle), cvPoint(maxAreaPoint.x+10, maxAreaPoint.y), cvScalar(0));// Moved to below...

		// end new version of grav angle

		//write out grav_dist stats
		//File, Root, Grav dist., Angle, Confidence (for each root)
		//Old way
		/*if (rootnum==0) sprintf(gravdiststring, "%i, %i, %.1f, %.1f, %.1f, %.1f,", filenumber, rootnum, dij_bend_dist, dij_bend_onset_dist,  bendAngle, confidence);
		else sprintf(gravdiststring, "%i, %.1f, %.1f, %.1f, %.1f,", rootnum, dij_bend_dist, dij_bend_onset_dist, bendAngle,confidence);// no file num
		gravdiststats.AppendDataRow_NoReturn(gravdiststring);*/
		if (rootnum==0) sprintf(gravdiststring, "%i, %.1f, %.1f,", filenumber, largeScaleAngle, length_to_maxAreaPoint);
		else sprintf(gravdiststring, "%.1f, %.1f,", largeScaleAngle, length_to_maxAreaPoint);// no file num
		gravdiststats.AppendDataRow_NoReturn(gravdiststring);

		if (largeScaleAngle>grav_response_threshold && gravOnsetLength[rootnum]<0) {
			gravOnsetLength[rootnum] = length_to_maxAreaPoint;
			gravOnsetAngle[rootnum] = largeScaleAngle;
		}  

	}

	

		

	//repaint crop window
	cvSetImageROI(output, cvRect( crop_orig_x, crop_orig_y, cropx, cropy) );		
	cvCopy(output, crop);
	cvNamedWindow("Zoom", 2);
	cvShowImage("Zoom", crop);
	cvResetImageROI(output);

	// write out stats
	
	//calibrate to mm
	//paths[best_index].dist = paths[best_index].dist * calib_mmPerPix;  // calib is 1.0 by default

	char  st[255];
	if (!calibration_mode) 
		if (calib_dist!=-1) {
			sprintf(st,"%i, %.1f, %.2f, %.1f", rootnum, paths[best_index].dist, calib_dist, paths[best_index].dist/(calib_dist/50));
			
		} else
		{ //"normal", no calib
			if (rootnum==0) sprintf(st,"%s,  %.3f", filename, paths[best_index].dist * calib_mmPerPix);
			else sprintf(st,  "%.3f", paths[best_index].dist * calib_mmPerPix);
			
		}
	else  //in calibration mode
		sprintf(st,"%i, %.1f, , , Calib length %i", rootnum,paths[best_index].dist, num_calib_traces);

	stats.AppendDataRow_NoReturn(st);
	if (rootnum==num_roots-1) {
		stats.CarriageReturn();
	}
	
	//stats.CarriageReturn();

	//output root tip stats
	char  tip_st[255];
	if (rootnum==0) sprintf(tip_st,"%s,  %.3f, %i, %i", filename, paths[best_index].dist * calib_mmPerPix, rootEnd.x, rootEnd.y);
	else sprintf(tip_st,  "%.3f, %i, %i", paths[best_index].dist * calib_mmPerPix, rootEnd.x, rootEnd.y);
	tip_stats.AppendDataRow_NoReturn(tip_st);
	if (rootnum==num_roots-1) tip_stats.CarriageReturn();
	// now draw tip on tipimage
	int saveEveryXMinutes = 30;
	if (filenumber==0) {
		initialTipLocations[rootnum] = cvPoint(rootEnd.x, rootEnd.y);
		//initialTipPathLocation set where rootEnd set
	}else //don't do below at filenumber==0
	if (filenumber*calib_minPerImage>lastTipFileSave) {
		
		//cvSaveImage(myspf("%s\\rootTips%i.bmp",outputFolderString,(int)((filenumber-1)*calib_minPerImage)), tipimage); //saved BEFORE any roots at this timepoint added, hence the -1
		lastTipFileSave+=saveEveryXMinutes;
	}

	//draw length of root at frame 0 on image
	cvLine(output, cvPoint(initialTipLocations[rootnum].x-3, initialTipLocations[rootnum].y), cvPoint(initialTipLocations[rootnum].x+3, initialTipLocations[rootnum].y), CV_RGB(0, 0, 0), 1);
	
	
	if (evolveTipPoints) tipPointsNumToFit = max(new_path_size - initialTipPathLocation[rootnum], 3); //fit through newly grown points, if this checked

	if (/*initialTipPathLocation[rootnum]*/ new_path_size>tipPointsNumToFit+1  && new_path_size>=initialTipPathLocation[rootnum] && doTipAngles) {//we have some points.. changed to compare numToFit to CURRENT path length, and test to make sure root is longer now than it was to start!, 15/6/2010 

	//Fit line to last few points to get tangent at tip
	// e.g. ((MyVertex*) cvGetGraphVtx(g, paths[0].path[new_path_size-1]))->x;
	CvMemStorage* pointsToFitStorage = cvCreateMemStorage(0);
	CvSeq* pointsToFit = cvCreateSeq( CV_32FC2, sizeof(CvSeq), sizeof(CvPoint2D32f), pointsToFitStorage );
	
	for (int pointi=1; pointi<=tipPointsNumToFit; pointi++){
		if (new_path_size<tipPointsNumToFit+1) MessageBox::Show("rootnum: "+rootnum+"\n"+"nps: "+new_path_size+"\n"+"pointi: "+pointi);
		cvSeqPush(pointsToFit, &cvPoint2D32f(((MyVertex*) cvGetGraphVtx(g, paths[0].path[new_path_size-pointi]))->x, 
			((MyVertex*) cvGetGraphVtx(g, paths[0].path[new_path_size-pointi]))->y));
		MyPutPixel(output, ((MyVertex*) cvGetGraphVtx(g, paths[0].path[new_path_size-pointi]))->x, output->height-((MyVertex*) cvGetGraphVtx(g, paths[0].path[new_path_size-pointi]))->y, 255, 0, 0);//mark red points used to fit line
		if (pointi==1) MyPutPixel(output, ((MyVertex*) cvGetGraphVtx(g, paths[0].path[new_path_size-pointi]))->x, output->height-((MyVertex*) cvGetGraphVtx(g, paths[0].path[new_path_size-pointi]))->y, 255, 0, 255);//mark pink point at end
	}
	
	float line_params[4];//normalized vector x, y, some point on the line x, y
	cvFitLine(pointsToFit, CV_DIST_L2, 0, 0.01, 0.01, line_params);
	double slope = line_params[1]/line_params[0];
	//intercept calc, y=mx+c; c = y-mx??
	double intercept = line_params[3]-(slope*line_params[2]);
	//now draw line across image
	CvPoint2D32f lineStartf, lineEndf;
	CvPoint lineStart, lineEnd;

	int temp = 30;//abs(initialTipLocations[rootnum].x-rootEnd.x);
	float diff=20.0;
	
	lineStartf.x = (float)rootEnd.x-diff;
	lineStartf.y = lineStartf.x*slope+intercept+30;//offset below root!
	lineEndf.x = (float)rootEnd.x+diff;
	lineEndf.y = lineEndf.x*slope+intercept+30;    //offset below root!
	lineStart=cvPoint((int)lineStartf.x, (int)lineStartf.y);
	lineEnd=cvPoint((int)lineEndf.x, (int)lineEndf.y);
	while (abs(lineEndf.y-lineStartf.y)>temp && diff>0.01){  //keep decreasing x distance used, so that the line is not really large in y-axis
		diff/=2.0;
		lineStartf.x = (float)rootEnd.x-diff;
		lineStartf.y = lineStartf.x*slope+(float)intercept+30.0;//offset below root!
		lineEndf.x = (float)rootEnd.x+diff;
		lineEndf.y = lineEndf.x*slope+(float)intercept+30.0;    //offset below root!
		lineStart=cvPoint((int)lineStartf.x, (int)lineStartf.y);
		lineEnd=cvPoint((int)lineEndf.x, (int)lineEndf.y);
		//cvLine(output, lineStart, lineEnd, CV_RGB(255, 0, 0), 1);
	}
	//special vertical case
	if (lineStart.x<lineEnd.x-1 || lineStart.x>lineEnd.x+1) cvLine(output, lineStart, lineEnd, CV_RGB(250, 168, 25), 1); //not 'vertical'
	else  cvLine(output, cvPoint(rootEnd.x, rootEnd.y+30+10),  cvPoint(rootEnd.x, rootEnd.y+30-10), CV_RGB(250, 168, 25), 1) ;//'vertical', so just draw a vertical line

	//Calculate tip angle to vertical
	double angleToVertical = 90.0- (57.2957795 *atan(abs(slope)));//90 - horizontal
	/*if ( ((MyVertex*) cvGetGraphVtx(g, paths[0].path[new_path_size-1]))->y < ((MyVertex*) cvGetGraphVtx(g, paths[0].path[new_path_size-tipPointsNumToFit]))->y) {
		//Tip bent upwards
		angleToVertical = 57.2957795 *atan(abs(slope));	//angle to horizontal
		angleToVertical+=90;			//plus 90
	}*/   //replaced by new slope-based code below
		
	if ( ((MyVertex*) cvGetGraphVtx(g, paths[0].path[new_path_size-1]))->x < ((MyVertex*) cvGetGraphVtx(g, paths[0].path[new_path_size-tipPointsNumToFit]))->x) {//if end x is further left 
			//bending to the left, make negative
		  // cvCircle(output, cvPoint( ((MyVertex*) cvGetGraphVtx(g, paths[0].path[new_path_size-1]))->x,  ((MyVertex*) cvGetGraphVtx(g, paths[0].path[new_path_size-1]))->y+5),  3, CV_RGB(255, 255, 0),  -1);//mark as left
			angleToVertical = 0.0-angleToVertical;
			if (slope>0) { //bending to left and > 90 deg
				angleToVertical = -90-(90+angleToVertical);
			}
	} else {
		//bending to right
		if (slope<0) { //bending to right and > 90 deg
			angleToVertical = 90+(90-angleToVertical);
		}

	}
	

	
	cvClearSeq(pointsToFit);
	cvReleaseMemStorage(&pointsToFitStorage);
	

	// now do the same but for points BEFORE we started
	pointsToFitStorage = cvCreateMemStorage(0);
	pointsToFit = cvCreateSeq( CV_32FC2, sizeof(CvSeq), sizeof(CvPoint2D32f), pointsToFitStorage );
	//if (rootnum==8) MessageBox::Show("initialTipPathLocation[rootnum]: "+initialTipPathLocation[rootnum]+ "\n"+"nps: "+new_path_size);
	for (int pointi=0; pointi<initialTipPathLocation[rootnum]-2; pointi+=2){//subsample by 2
		cvSeqPush(pointsToFit, &cvPoint2D32f(((MyVertex*) cvGetGraphVtx(g, paths[0].path[pointi]))->x, 
			((MyVertex*) cvGetGraphVtx(g, paths[0].path[pointi]))->y));
		    MyPutPixel(output, ((MyVertex*) cvGetGraphVtx(g, paths[0].path[pointi]))->x, output->height-((MyVertex*) cvGetGraphVtx(g, paths[0].path[pointi]))->y, 0, 0, 0);//mark black points used to fit line
	}
	//if (rootnum==8) MessageBox::Show("C");
	line_params[4];//normalized vector x, y, some point on the line x, y
	cvFitLine(pointsToFit, CV_DIST_L2, 0, 0.01, 0.01, line_params);
	double slope2 = line_params[1]/line_params[0];
	//intercept calc, y=mx+c; c = y-mx??
	intercept = line_params[3]-(slope2*line_params[2]);
	
	//now draw line down root, from top y to bottom. (y=mx+c therefore, x = y/m -c/m...
	//CvPoint lineStart, lineEnd;
	lineStart.y = rootStart.y;
	lineStart.x = (double)lineStart.y/slope2 - intercept/slope2;
	lineEnd.y = initialTipLocations[rootnum].y;
	lineEnd.x = (double)lineEnd.y/slope2 - intercept/slope2;
	cvLine(output, lineStart, lineEnd, CV_RGB(250, 168, 25), 1);

	cvClearSeq(pointsToFit);
	cvReleaseMemStorage(&pointsToFitStorage);


	//angle
	double angle = 999;
	if ( (slope2<0 && slope<0) || (slope2>0 && slope>0) ) {
			// both same sign
			angle = abs(atan(abs(slope2))-atan(abs(slope)));
		}else {
			angle = abs(atan(abs(slope2))+atan(abs(slope)));
		}

		angle = angle*57.2957795; 

		if (slope2*slope>0) angle = 180-angle;  // if both same sign...
		if (angle<90) angle = 180-angle;

		//angle = 180 - angle;
		//imgTxt(output, myspf("%i, %i", (int)angle, (int)angleToVertical), cvPoint(initialTipLocations[rootnum].x+10, initialTipLocations[rootnum].y+30), CV_RGB(255, 0, 0));
		

		//save tip angles
		char  tip_st[255];
		if (rootnum==0) sprintf(tip_st,"%i,  %i", (int)((double)filenumber*calib_minPerImage), (int)angle);
		else sprintf(tip_st,  "%i",  (int)angle);
		tip_angles.AppendDataRow_NoReturn(tip_st);
		if (rootnum==num_roots-1) tip_angles.CarriageReturn();

		//save tip angle to vertical
		char  tip_st2[255];
		if (rootnum==0) sprintf(tip_st2,"%i,  %i", (int)((double)filenumber*calib_minPerImage), (int)angleToVertical);
		else sprintf(tip_st2,  "%i",  (int)angleToVertical);
		tip_angles_vert.AppendDataRow_NoReturn(tip_st2);
		if (rootnum==num_roots-1) tip_angles_vert.CarriageReturn();


	}//if we have some points
	else {
		//can't do tip angles
		//write 999s to angles file
		char  tip_st[255];
		if (rootnum==0) sprintf(tip_st,"%i,  %i", (int)((double)filenumber*calib_minPerImage), 999);
		else sprintf(tip_st,  "%i",  (int)999);
		tip_angles.AppendDataRow_NoReturn(tip_st);
		if (rootnum==num_roots-1) tip_angles.CarriageReturn();

		//save tip angle to vertical - can't do this! so save 999's
		char  tip_st2[255];
		if (rootnum==0) sprintf(tip_st2,"%i,  %i", (int)((double)filenumber*calib_minPerImage), (int)999);
		else sprintf(tip_st2,  "%i",  (int)999);
		tip_angles_vert.AppendDataRow_NoReturn(tip_st2);
		if (rootnum==num_roots-1) tip_angles_vert.CarriageReturn();
	}


	

	int tip_scale = 2;
	int tipx = (rootEnd.x - initialTipLocations[rootnum].x)*tip_scale;
	int tipy = (rootEnd.y - initialTipLocations[rootnum].y)*tip_scale;

	if (tipx>0 && tipy>0 && tipx<200 && tipy<200) {
		UCHAR R, G, B;
		MyGetPixel(tipimage, tipx, 200-tipy, &R, &G, &B);
		if (R==0) {R=40; G=40; B=150;}
		else {R = min(255, R+20);G = min(255, G+40);B = min(255, B+5);}
		MyPutPixel(tipimage, tipx, 200-tipy, R, G, B);
	}	


	// write length on image
	if (writeLengthOnImage) imgTxt(output, myspf("D= %.1f",paths[best_index].dist ), cvPoint(p2.x+5, p2.y), CV_RGB(255,0,0) );
	finalLengths[filenumber, rootnum] = paths[best_index].dist;// save lengths for future use

	// Quick preview graph
	MyPutPixel(preview_lengthsi,filenumber+1, paths[best_index].dist, 255, 255, 255);
	cvNamedWindow("Preview Lengths", 0);//was 2
	cvShowImage("Preview Lengths", preview_lengthsi);

	//draw point of max curve
	if (doGravitropic)
		for (int vtxn = 0; vtxn<new_path_size-1;vtxn++) {
			//get nodes for drawing line
			MyVertex * myv1 = (MyVertex*) cvGetGraphVtx(g, paths[best_index].path[vtxn]);
			if (gravOnsetLength[rootnum]>0 && myv1->dij_dist>gravOnsetLength[rootnum]) {
				cvCircle(output, cvPoint(myv1->x, myv1->y), 3, cvScalar(0, 255, 255), 1);
				if (writeAngleOnRoot) imgTxt(output, myspf("A=%.1f", gravOnsetAngle[rootnum]), cvPoint(myv1->x+10, myv1->y), cvScalar(0));
				break;
			}
		}


	//velocity/growth rate display - too noisy
	/*if (lastLengths[rootnum]!=-1) {
		double velocity= paths[best_index].dist-lastLengths[rootnum];
		if (writeLengthOnImage) imgTxt(output, myspf("G= %.1f",velocity ), cvPoint(p2.x+5, p2.y+18), CV_RGB(0,0,0) );
	}
	lastLengths[rootnum] = paths[best_index].dist;*/
	
	cvCopyImage(output, image);
	cvNamedWindow("Image", 2);
	cvShowImage("Image", image);
	cvWaitKey(1);

	if (num_calib_traces<=2&&calibration_mode) {
			calib_measures[num_calib_traces] = paths[best_index].dist;

		}
	if (num_calib_traces==2&&calibration_mode) {
		cvCopy(image, tempimage);
		calib_dist = (calib_measures[0] + calib_measures[1] + calib_measures[2])/3;
		imgTxt(tempimage, myspf("Calibration done. 50mm = %.2f", calib_dist ), cvPoint(5, 20), CV_RGB(0,255,0) );
		cvShowImage("Image", tempimage);
		calibration_mode = false;
	}  
		

    //code = cvWaitKey();
	
	cvClearGraph(g);
	paths.clear();

	
	rootnum++;
    } // end while rootnum<num_roots

	subRootTxt<<endl;

	if (doGravitropic){
		gravdiststats.CarriageReturn();
		sprintf(gravstring, "%i, %i", filenumber, num_grav_roots);
		gravstats.AppendDataRow(gravstring);  // should store confidence as well?
	}

	if (save_image) cvSaveImage(savepath, image);

	cvReleaseMemStorage(&g_storage);
	//cvClearGraph(g);
	//delete g;
	
	filenumber++;

	if ((image!=NULL)&&(filenumber<filenames->Length-1)) cvReleaseImage(&image);//release every time except last time through (released at very end)
	if (originali!=NULL) cvReleaseImage(&originali);
	if ((output!=NULL)&&(filenumber<filenames->Length-1)) cvReleaseImage(&output); //release every time except last time through (released at very end)
	if (smoothedi!=NULL)	cvReleaseImage(&smoothedi);
	if (crop!=NULL)			cvReleaseImage(&crop);
	if (previousi!=NULL)	cvReleaseImage(&previousi);
	if (tempimage!=NULL)	cvReleaseImage(&tempimage);
	if (measuresi!=NULL)	cvReleaseImage(&measuresi);
	if (measuresiTaboo!=NULL) cvReleaseImage(&measuresiTaboo);
	//cvReleaseImage(&r_copy);
	

	}// end filenumber while

	
	// Gone through all files now and done all measuring.
	
	int fileNumberOfGravOnset[max_numroots];//file number for each root when bending distance reach

	if (doGravitropic) {
		// For each root, lets see what file number bend onset happened on
		// We do this by looking to see which file num the recorded length of the root reaches the grav length.
		// can convert this to time by multiplying file number by capture period

		CStatsWriter grav_response_histogram = CStatsWriter();
		grav_response_histogram.ChangeFile(myspf("%s\\grav_response_histogram.csv", outputFolderString));
		char responsestring[255];
		grav_response_histogram.WriteTitles(myspf("Time, No. roots responding >%i",grav_response_threshold) );

		

		//median filter final lengths to try and remove erroneous blips
		array<float,2>^ filteredFinalLengths = gcnew array<float,2>(max_files, max_numroots);
		const int filterSize = 5;//ODD VALUES ONLY
			assert(filterSize%2!=0);

		for (int r=0; r<num_roots; r++) {
			for (int f=0; f<filenames->Length; f++)
				if (f>filterSize/2 && f<filenames->Length-filterSize/2) {
					//within bounds, so can do filtering
					array<float,1>^ values = gcnew array<float,1>(filterSize);
					for (int j=0; j<filterSize; j++) {
						values[j] = finalLengths[f+(((filterSize-1)/2)-j), r];
					}
					System::Array::Sort(values);
					//for (int j=0; j<filterSize; j++) mainWindowText(mainWindow, "Values["+j+"] = " + values[j]);
					filteredFinalLengths[f, r] = values[(filterSize-1)/2];

				}
				else filteredFinalLengths[f, r] = finalLengths[f, r];// if at ends of values so can't do filetering, use unfiltered value
		}
		
		// see when grav onset length exceeded
		// changed to filteredFinalLengths, 20/4/09
		int num_gravitropic_roots = 0;
		for (int r=0; r<num_roots; r++) {
			fileNumberOfGravOnset[r]=-1;//init
			for (int f=0; f<filenames->Length; f++)
				if (filteredFinalLengths[f, r]>gravOnsetLength[r] && gravOnsetLength[r]>0 ) {
					//mainWindowText(mainWindow, "Final Lengths "+filteredFinalLengths[f, r]+" file "+f+", root "+r+" exceeded gravOnset "+gravOnsetLength[r]);
					fileNumberOfGravOnset[r] = f;
					num_gravitropic_roots++;
					break;//don't check through any further files 
				}
		}

		//now for each root we have fileNumberOfGravOnset[]= filenumber or -1 if none
		int histogramBins[1000];

		int binWidth = 30; //minutes, was 30

		int numBins = (filenames->Length * calib_minPerImage) / binWidth;

		//count num roots responding in each bin
		int binNum=0;
		int cumulative = 0;
		for (int b=0; b<numBins*binWidth; b+=binWidth) {
			histogramBins[binNum]=cumulative;
			int binLower = b;
			int binUpper = b+binWidth;
			for (int r=0; r<num_roots; r++) {
				if ( (fileNumberOfGravOnset[r]*calib_minPerImage)>=binLower && (fileNumberOfGravOnset[r]*calib_minPerImage)<binUpper) {
					histogramBins[binNum]++;
				}
			}
			
			sprintf(responsestring, "%i, %i", b, histogramBins[binNum]);
			grav_response_histogram.AppendDataRow(responsestring);
			cumulative = histogramBins[binNum];
			if (cumulative==num_gravitropic_roots) {
				//we are at the bin which contains all roots, so no further plotting necesary
				numBins = binNum+1;
				break;
			}
			binNum++;
		}

		/*for (int i=0; i<numBins; i++) {
			mainWindowText(mainWindow, "Bin["+i+"] = "+histogramBins[i]);
		}*/

		int border = 30;

		double binScale = 50;
		double yScale = 50;
		IplImage* gravHist = cvCreateImage(cvSize(binScale*numBins+border, yScale*num_roots+2*border), 8, 1);
		cvRectangle(gravHist,cvPoint(0,0), cvPoint(gravHist->width-1, gravHist->height-1), cvScalar(255), -1);
		for (int i=0; i<numBins; i++){ 
			cvRectangle(gravHist, cvPoint(i*binScale+border, gravHist->height-(0+border)), cvPoint(i*binScale+binScale+border ,gravHist->height-(histogramBins[i]*yScale+border)), cvScalar(0), -1);
			cvLine(gravHist, cvPoint(i*binScale+border, gravHist->height-(0+border)), cvPoint(i*binScale+border, 0), cvScalar(255), 3);
			imgTxt(gravHist, myspf("%i", i*binWidth), cvPoint((i*binScale)+border, gravHist->height-10), cvScalar(0), 0.3);
		}

		//y axis labels, number of roots
		for (int i=0; i<gravHist->height; i++) {
			imgTxt(gravHist, myspf("%i", i), cvPoint(2, gravHist->height-(i*yScale+border)), cvScalar(0), 0.3);

		}
		cvNamedWindow("Grav response histogram", 2);
		cvShowImage("Grav response histogram", gravHist);
		char gravhistfilestring[512];
		sprintf(gravhistfilestring, "%s\\%s", outputFolderString, "gravHistogram.bmp");
		cvSaveImage(gravhistfilestring, gravHist);
		//cvWaitKey(0);

	} //if (doGravitropic)
	


	if (filenumber>1){//Do not do if only one image
	// show curvature test
	const double yscale = 2;  //curvature chart y scale
	const int xscale = 4;
	const bool xinterpolation = false;  // NOT WORKING YET

	for (int rootnum=0; rootnum<num_roots; rootnum++){
	
	IplImage* curvature = cvCreateImage(cvSize(filenumber*xscale, max_length*yscale), 8, 3);
	cvZero(curvature);
	
	CStatsWriter curvstats = CStatsWriter();
	curvstats.ChangeFile(myspf("%s\\curvature_root%i.csv", outputFolderString, rootnum));
	char curvstring[255];
	curvstats.WriteTitles("File, Dij dist, Curvature");

	
	if (filenumber>1) {//don't do for just one file
	for (int numfiles=0; numfiles<filenumber/*-1*/;numfiles++) {
		int segment =0;
		double oldy=0;
		

		while (curvatures[numfiles, segment, rootnum]!=curvature_empty_flag) {

			CvScalar col;
			CvScalar col2;
			bool col2set = false;
			int col_interp[3];
			

			//Using angles
			if (abs(curvatures[numfiles, segment, rootnum])==999)//no value
				col = cvScalar(80, 80, 80);
			else if (abs(curvatures[numfiles, segment, rootnum])>=color_bins[0]) 
				col = color_map[0];
			else if (abs(curvatures[numfiles, segment, rootnum])>color_bins[1])
				col = color_map[1];
			else if (abs(curvatures[numfiles, segment, rootnum])>color_bins[2])
				col = color_map[2];
			else if (abs(curvatures[numfiles, segment, rootnum])>color_bins[3])
				col = color_map[3];
			else if (abs(curvatures[numfiles, segment, rootnum])>color_bins[4])
				col = color_map[4];
			else if (abs(curvatures[numfiles, segment, rootnum])>color_bins[5])
				col = color_map[5];
			else if (abs(curvatures[numfiles, segment, rootnum])>=color_bins[6])
				col = color_map[6];

			if (xinterpolation && curvatures[numfiles+1, segment, rootnum]!=curvature_empty_flag) {
				//get next colour for interpolation
				//Using angles
			if (abs(curvatures[numfiles+1, segment, rootnum])==999)//no value
				col2 = cvScalar(80, 80, 80);
			else if (abs(curvatures[numfiles+1, segment, rootnum])>=color_bins[0]) 
				col2 = color_map[0];
			else if (abs(curvatures[numfiles+1, segment, rootnum])>color_bins[1])
				col2 = color_map[1];
			else if (abs(curvatures[numfiles+1, segment, rootnum])>color_bins[2])
				col2 = color_map[2];
			else if (abs(curvatures[numfiles+1, segment, rootnum])>color_bins[3])
				col2 = color_map[3];
			else if (abs(curvatures[numfiles+1, segment, rootnum])>color_bins[4])
				col2 = color_map[4];
			else if (abs(curvatures[numfiles+1, segment, rootnum])>color_bins[5])
				col2 = color_map[5];
			else if (abs(curvatures[numfiles+1, segment, rootnum])>=color_bins[6])
				col2 = color_map[6];
			col2set = true;
			col_interp[0] = abs(col2.val[0]-col.val[0])/xscale;
			col_interp[1] = abs(col2.val[1]-col.val[1])/xscale;
			col_interp[2] = abs(col2.val[2]-col.val[2])/xscale;
			}

			
			//Draw lines - one for each upscaling
			for (int upscale=0; upscale <xscale; upscale++){ // repeat line next to other one for upscaling
				if (xinterpolation&&col2set)cvLine(curvature, cvPoint(numfiles*xscale+upscale,oldy), cvPoint(numfiles*xscale+upscale, lengthForCurvature[numfiles, segment, rootnum]*yscale), cvScalar(col.val[0]+col_interp[0],col.val[1]+col_interp[1],col.val[2]+col_interp[2]));
				else cvLine(curvature, cvPoint(numfiles*xscale+upscale,oldy), cvPoint(numfiles*xscale+upscale, lengthForCurvature[numfiles, segment, rootnum]*yscale), col);
			}
			oldy = lengthForCurvature[numfiles, segment, rootnum]*yscale;

			

			sprintf(curvstring, "%i, %.1f, %.1f", numfiles, lengthForCurvature[numfiles, segment, rootnum], curvatures[numfiles, segment, rootnum]/*, curvatures2[numfiles, segment, rootnum]*/);
			curvstats.AppendDataRow(curvstring);
			
			
			segment++;

		}


	} //gone through each files length for a root

	
	//write time scale
	for (int f=0; f<filenumber; f++)
		if ((int)(calib_minPerImage*f)%120==0 && (int)(calib_minPerImage*f)!=0) {
					int time = (int)(calib_minPerImage*f)/60;//hours
					imgTxt(curvature, myspf("%ih", time), cvPoint(f*xscale, curvature->height-30), cvScalar(255, 255, 255), 0.4);
				}

	//write length scale
	int last_done = -1;// prevent drawing blurred numbers over small increases in y_
	int tick_mm = 10;
	for (int y_=0; y_<curvature->height; y_++)
		if ((int)(((double)y_/yscale)*calib_mmPerPix)%tick_mm==0 && 
			(int)(((double)y_/yscale)*calib_mmPerPix)!=last_done    ) {
					int length = (int)((y_/yscale)*calib_mmPerPix);
					imgTxt(curvature, myspf("%imm", length), cvPoint(5, y_), cvScalar(255, 255, 255), 0.4);
					last_done = (int)(((double)y_/yscale)*calib_mmPerPix);
				}

	//write colour key on chart
	int box_dim = 15;
	int xpos = 40;
	int ypos = curvature->height-150;

	for (int c=0; c<num_cols_in_map; c++) {
		cvRectangle(curvature, cvPoint(xpos, ypos), cvPoint(xpos+box_dim, ypos+box_dim), color_map[c], -1);
		if (c==0) imgTxt(curvature, myspf("%.3f+", color_bins[0]), cvPoint(xpos+box_dim+1, ypos+box_dim-1), cvScalar(255, 255, 255), 0.4);
		if (c==num_cols_in_map-1) imgTxt(curvature, myspf("%.3f", color_bins[num_cols_in_map-1]), cvPoint(xpos+box_dim+1, ypos+box_dim-1), cvScalar(255, 255, 255), 0.4);
		ypos+=box_dim;
	}

	// draw on determined lenght/time of gravitropic point
	if (doGravitropic && fileNumberOfGravOnset[rootnum]!=-1) {
		cvLine(curvature, cvPoint(fileNumberOfGravOnset[rootnum]*xscale, 0), cvPoint(fileNumberOfGravOnset[rootnum]*xscale, curvature->height), cvScalar(128, 128, 128), 1);
		cvLine(curvature, cvPoint(0, gravOnsetLength[rootnum]*yscale), cvPoint(curvature->width, gravOnsetLength[rootnum]*yscale), cvScalar(128, 128, 128), 1);
	}


	

	//cvNamedWindow("Curvature", 2);
	//cvShowImage("Curvature", curvature);
	//
	
	cvSaveImage(myspf("%s\\curvature_root%i.bmp",outputFolderString, rootnum), curvature);
	cvSaveImage(myspf("%s\\rootTipsFinal.bmp",outputFolderString), tipimage);

	


	//cvSaveImage("BGmask.bmp", bg_mask);


	if (curvature!=NULL) cvReleaseImage(&curvature);
	
	

	}//end if filenumber>1



	}// root num


	}// loop through all roots in image

	//doManualAngles();

	doingManualMeasures = true;

	
	// EXIT
	
	mainWindow_RT->grey();
	if (bg_mask!=NULL) cvReleaseImage(&bg_mask);
	if (preview_lengthsi!=NULL) cvReleaseImage(&preview_lengthsi);
	
	MessageBox::Show("Finished.", "RootTrace",MessageBoxButtons::OK,MessageBoxIcon::Information);

	cvWaitKey(0);
	if (numberOfManualAngleMeasures>0){
		cvSaveImage(myspf("%s\\manualAngles.bmp",outputFolderString), image);
		CStatsWriter manualAngleStats = CStatsWriter();
		manualAngleStats.ChangeFile(myspf("%s\\manualAngles.csv",outputFolderString));
		
		for(int a=0; a<numberOfManualAngleMeasures; a++)
			manualAngleStats.AppendDataRow(myspf("%.1f", manualAngles[a]));
	}

	if (output!=NULL) cvReleaseImage(&output); 
	if (image!=NULL) cvReleaseImage(&image); 


	
	cvDestroyAllWindows();

	

	
	

}


int CMainCode::doManualAngles(int x, int y)
{
	
	if (manualAngleCount==0) {
		manualAnglePoint1.x = x;
		manualAnglePoint1.y = y;
		cvCircle(image, manualAnglePoint1, 2, CV_RGB(0, 0, 0),-1); 
		manualAngleCount++;
		cvShowImage("Image", image);
		cvWaitKey(1);
	
	} else
	if (manualAngleCount==1) {
		manualAnglePoint2.x = x;
		manualAnglePoint2.y = y;
		cvLine(image, manualAnglePoint1, manualAnglePoint2, CV_RGB(128, 255, 255), 1);
		cvCircle(image, manualAnglePoint2, 2, CV_RGB(0, 0, 0),-1); 
		manualAngleCount++;
		cvShowImage("Image", image);
		cvWaitKey(1);
	
	}else
	if (manualAngleCount==2) {
		manualAnglePoint3.x = x;
		manualAnglePoint3.y = y;
		cvLine(image, manualAnglePoint2, manualAnglePoint3, CV_RGB(128, 255, 255), 1);
		cvCircle(image, manualAnglePoint3, 2, CV_RGB(0, 0, 0),-1); 
		
		
		manualAngleCount = 0;

		//angle
		/*double overallAngle;
		CvPoint vectorStoC;
		vectorStoC.x = manualAnglePoint2.x-manualAnglePoint1.x;
		vectorStoC.y = manualAnglePoint2.y-manualAnglePoint1.y;
		CvPoint vectorCtoE;
		vectorCtoE.x =  manualAnglePoint3.x-manualAnglePoint2.x;
		vectorCtoE.y =  manualAnglePoint3.y-manualAnglePoint2.y;
		double gradient1 = (double)vectorStoC.y/vectorStoC.x;
		double gradient2 = (double)vectorCtoE.y/vectorCtoE.x;
		overallAngle;
		if ( (gradient2<0 && gradient1<0) || (gradient2>0 && gradient1>0) ) {
			// both same sign
			overallAngle = abs(atan(abs(gradient2))-atan(abs(gradient1)));
		}else {
			overallAngle = abs(atan(abs(gradient2))+atan(abs(gradient1)));
		}

		overallAngle = overallAngle*57.2957795; 
		
		if (gradient2*gradient1>0) overallAngle = 180-overallAngle;  // if both same sign...
		if (overallAngle<90) overallAngle = 180-overallAngle;
		*/

		// ANGLE OF GRAV TROP POINT - NEW version March 09
		double overallAngle;
		CvPoint vectorStoC;
		vectorStoC.x = manualAnglePoint2.x-manualAnglePoint1.x;
		vectorStoC.y = manualAnglePoint2.y-manualAnglePoint1.y;
		CvPoint vectorCtoE;
		vectorCtoE.x = manualAnglePoint3.x-manualAnglePoint2.x;
		vectorCtoE.y = manualAnglePoint3.y-manualAnglePoint2.y;
		double gradient1 = (double)vectorStoC.y/vectorStoC.x;
		double gradient2 = (double)vectorCtoE.y/vectorCtoE.x;
		//double largeScaleAngle;
		if ( (gradient2<0 && gradient1<0) || (gradient2>0 && gradient1>0) ) {
			// both same sign
			overallAngle = abs(atan(abs(gradient2))-atan(abs(gradient1)));
		}else {
			overallAngle = abs(atan(abs(gradient2))+atan(abs(gradient1)));
		}

		overallAngle = overallAngle*57.2957795; 

		//old way
		if (gradient2*gradient1>0) overallAngle = 180-overallAngle;   // if both same sign...  MAKES 90>180
		if (overallAngle<90) overallAngle = 180-overallAngle;			// MAKES DIFF SIGN CASE 90>180 IF ANGLE<90    (SAME SIGN 90>180 ANYWAY FROM LINE ABOVE)	

		//new 
		//if (overallAngle>90) overallAngle = 180 - overallAngle;

		//26/4/2010  Allow for obtuse deviation. Relies on prior knowledge of how plant should look (eg grav bend to right), and how points are clicked. See p161 lab book
		if (gradient2>0) overallAngle = 180 - overallAngle;

		
		//180 - for deviation...
		overallAngle = 180 - overallAngle; //removed for testing, 14/1/10
		

		imgTxt(image, myspf("%.1f", overallAngle), cvPoint(manualAnglePoint2.x+10, manualAnglePoint2.y), CV_RGB(255, 255, 255));

		cvShowImage("Image", image);
		cvWaitKey(1);
		manualAngles[numberOfManualAngleMeasures] = overallAngle;
		numberOfManualAngleMeasures++;

		
	}

	


					


	return 0;
}





CvPoint CMainCode::drawSample(IplImage* copy, int t, UCHAR r, UCHAR g, UCHAR b, bool drawsamples, int iters, int x_offset, int y_offset) {
	//returns 'best estimate' of target position
	CvPoint p;

	//IplImage* tempi = copy;// use whole image not zoom
	//copy = image;
	
	

	if (drawsamples){
	//Draw samples: only every stepTH...
	double hiPi = cond.getHighestPi();
	int step = 1;
	for (int n=0; n<NSAMPLES-step; n+=step) {  
		p.x = (int)cond.s_new[n].x[t];
		p.y = (int)cond.s_new[n].y[t];
		//cvCircle(copy, p, 2, CV_RGB(200, 0, 50), 1);
		double colour_mutliplier;
		colour_mutliplier = cond.s_new[n].pi/hiPi;
		
		
		if (cond.s_new[n].state[t]==0) {
		r=255; g=0; b=0;

		}
		else if (cond.s_new[n].state[t]==1){
		r=0; g=0; b=255;

		}

		colour_mutliplier = 1; // const intensity
		
		/*
		// old version which uses copy image
		if (p.x+x_offset>1&&p.x+x_offset<copy->width&& copy->height - (p.y+y_offset)>1&&copy->height - (p.y+y_offset)<copy->height){ 
			MyPutPixel(copy, p.x+x_offset, copy->height - (p.y+y_offset), colour_mutliplier*r, colour_mutliplier*g, colour_mutliplier*b); //dot
			//cout <<" Drawing:"<<p.x+x_offset<<" , "<<copy->height - (p.y+y_offset)<<endl;
		}  else {
			//cout <<" FAILED:"<<p.x+x_offset<<" , "<<copy->height - (p.y+y_offset)<<endl;
		}
		*/


		// new version which uses output image
		if (p.x>1&&p.x<output->width&& output->height - p.y>1&&output->height - p.y<output->height){ 
			UCHAR red, green , blue;
			MyGetPixel(output, p.x, output->height - p.y, &red, &green, &blue);
			MyPutPixel(output, p.x, output->height - p.y, colour_mutliplier*r, green, colour_mutliplier*b); //dot
			//cout <<" Drawing:"<<p.x+x_offset<<" , "<<copy->height - (p.y+y_offset)<<endl;
		}  else {
			//cout <<" FAILED:"<<p.x+x_offset<<" , "<<copy->height - (p.y+y_offset)<<endl;
		}


	}
	}
	
	//copy = tempi; //back again

	//Sample info = cond.getAverageSample();
	//for (t=0; t<numTs; t++){

		//draw averages / best
		
		int thickness = 1;
	
		//int bestIndex = cond.getBestIndex();
		//p.x =  info.x[t];
		//p.y =  info.y[t];
		p.x = cond.getBestX(0);
		p.y = cond.getBestY(0);
		int state = cond.getMajorityState(0);

		// get colour under best sample to detect whether bg or not
		UCHAR re, gr, bl;
		MyGetPixel(originali, p.x, originali->height-p.y, &re, &gr, &bl);

		
		//draw BEST circle
		//if (p.x>1&&p.x<copy->width-1&&p.y>1&&p.y<copy->height-1) cvCircle(copy, p, 1, CV_RGB(r, g, b), thickness);
		double mult = 3;
		if (  (re> BG_mean_r-BG_st_dev_r*mult && re< BG_mean_r+BG_st_dev_r*mult) &&
			  (gr> BG_mean_g-BG_st_dev_g*mult && gr< BG_mean_g+BG_st_dev_g*mult) &&
			  (bl> BG_mean_b-BG_st_dev_b*mult && bl< BG_mean_b+BG_st_dev_b*mult) ) {

				r=0; g=0; b=0; //black indicates over BG
		} 
		else {
		if (state == 0) {r=255; g=0; b=0;}
		if (state == 1) {r=0; g=0; b=255;}
		if (state == 2) {r=255; g=0; b=255;}
		}

		bool drawzoomwindow = true;
		if (drawzoomwindow) {
		// RECENTRE CROP WINDOW
			crop_orig_x= min(max(p.x-(cropx/2), 0), image->width-cropx);
			crop_orig_y= min(max(p.y-(cropy/2), 0), image->height-cropy);


			//cvCopy(image, tempimage);
			cvSetImageROI(image, cvRect( crop_orig_x, crop_orig_y, cropx, cropy) );
			
			cvDrawRect(tempimage,cvPoint(crop_orig_x, crop_orig_y), cvPoint(crop_orig_x+cropx, crop_orig_y+cropy), cvScalar(0, 255, 0)); 
			
			
			
			cvCopy(image, crop);
			cvNamedWindow("Zoom", 2);
			cvShowImage("Zoom", crop);
			cvResetImageROI(image);
		
			// Need to recalc offsets as window moved.
			x_offset = 0-crop_orig_x;
			y_offset = 0-crop_orig_y;
			if (p.x+x_offset>1&&p.x+x_offset<copy->width&& copy->height - (p.y+y_offset)>1&&copy->height - (p.y+y_offset)<copy->height) {
			
			// draw cross
			MyPutPixel(copy, p.x+x_offset, copy-> height - (p.y+y_offset), r, g, b); //centre 
			// 1 step
			MyPutPixel(copy, p.x+x_offset+1, copy->height - (p.y+y_offset), r, g, b); //left 
			MyPutPixel(copy, p.x+x_offset-1, copy->height - (p.y+y_offset), r, g, b); //right 
			MyPutPixel(copy, p.x+x_offset, copy->height - (p.y+y_offset)+1, r, g, b); //
			MyPutPixel(copy, p.x+x_offset, copy->height - (p.y+y_offset)-1, r, g, b); //
			//2 step
			MyPutPixel(copy, p.x+x_offset+2, copy->height - (p.y+y_offset), r, g, b); //left
			MyPutPixel(copy, p.x+x_offset-2, copy->height - (p.y+y_offset), r, g, b); //right 
			MyPutPixel(copy, p.x+x_offset, copy->height - (p.y+y_offset)+2, r, g, b); //
			MyPutPixel(copy, p.x+x_offset, copy->height - (p.y+y_offset)-2, r, g, b); //


			}
			}
		

		


	/*	
	//write stats to file
	if (writeStats) {
		char  st[70];
		sprintf(st, "%i, %i, %f, %f", p.x, copy->height - p.y, info.u[t], info.v[t]);
		//cout << st<<endl;
		m_stats->AppendDataRow_NoReturn(st);
		}
	*/
	
		return p;

  }

void CMainCode::MyPutPixel(IplImage *m_image, int x, int y, UCHAR b, UCHAR g, UCHAR r)
{
       CvPoint point;
       point.x = x;
       point.y = y;

       if ((x<0||x>m_image->width)||(y<1||y>m_image->height)) {
           //cout <<" ERROR in MyPutPixel. x = " <<x <<", y = "<<y<<endl;
           //MessageBox::Show("WARNING trying to write value off image");
		   //exit(1);
	   } else{

       CvPixelPosition8u drawPos;

       CV_INIT_PIXEL_POS(drawPos, (uchar*)m_image->imageData, m_image->widthStep,
                       cvSize(m_image->width, m_image->height), 0, 0, 0);

       uchar* ptrPixel = CV_MOVE_TO(drawPos, point.x, m_image->height - point.y, m_image->nChannels);
              if (m_image->nChannels==3){
           ptrPixel[0] = r;
           ptrPixel[1] = g;
           ptrPixel[2] = b;
       } else
       if (m_image->nChannels==1){
           ptrPixel[0] = r;
       }
	   }

       //do I need to delete pointer
}

void CMainCode::MyGetPixel(IplImage *m_image, int x, int y, UCHAR *b, UCHAR *g, UCHAR *r)
{
       CvPoint point;
       point.x = x;
       point.y = y;

       if ((x<0||x>m_image->width)||(y<1||y>m_image->height)) {
           //cout <<" ERROR in MyGetPixel. x = " <<x <<", y = "<<y<<endl;
           //MessageBox::Show("WARNING trying to read value off image");
		   //exit(1);
		   r=0;
		   g=0;
		   b=0;

	   }else{

       CvPixelPosition8u drawPos;

       CV_INIT_PIXEL_POS(drawPos, (uchar*)m_image->imageData, m_image->widthStep,
                       cvSize(m_image->width, m_image->height), 0, 0, 0);

       uchar* ptrPixel = CV_MOVE_TO(drawPos, point.x, m_image->height - point.y, m_image->nChannels);
              if (m_image->nChannels==3){
           *r= ptrPixel[0];
           *g= ptrPixel[1];
           *b= ptrPixel[2];
       } else
       if (m_image->nChannels==1){
           *r = ptrPixel[0];
       }
	   }
}

void CMainCode::doRootTracking( UCHAR idealr, UCHAR idealg, UCHAR idealb, double proSigmaX, double proSigmaY, UCHAR new_state, CvGraph* g, int rootnum, int filenum, double prior_bg, double upperHyst, double lowerHyst, System::Windows::Forms::Form^ mainWindow) {
	

	//MessageBox::Show("upperHyst = "+upperHyst);
	// backwards compatible
	int clickx = start_ps[rootnum].x;
	int clicky = start_ps[rootnum].y;

	RootTrace2::Form1^ mainWindow_RT = ((RootTrace2::Form1^)mainWindow);

	//MessageBox::Show("upperHyst = "+upperHyst); // this DOES change during running of the program

	if (debuggraph) cvZero(debuggraph);

		// Need to set an initial start point as first step (well, step -1).  This happen prior to 
	// first cycle of Condensation 12/8/08
	
	CvPoint best_lock = lockToRoot(clickx, clicky);
	
	
	cout<<"Starting tracking at "<<best_lock.x<<", "<<start_ps[rootnum].y<<endl;
	//CCondensation cond; // NEW - why does this break? Becuase its used elsewhere.  Init'ing should reset it anyway
	//cond.init(0, clickx, clicky, 0, 0,idealr, idealg, idealb, new_state, proSigmaX, proSigmaY);
	cond.init(0, best_lock.x, best_lock.y, 0, 0,idealr, idealg, idealb, new_state, proSigmaX, proSigmaY, mainWindow_RT->getRadius(), motionMixture);
	
	CvPoint bestGuess_t_1;
	bestGuess_t_1.x = clickx;
	bestGuess_t_1.y = clicky;

	static CStatsWriter prob_stats;
	if (filenum==0&&rootnum==0) {
		prob_stats.ChangeFile(myspf("%s\\prob_stats.csv", outputFolderString));
		prob_stats.WriteTitles("Root, R, G, B, mean R, mean G, mean B, sd R, sd G, sd B, Mult, NOTES: Records all roots in first image only.");
	}


	
    int code=0;
	int i =0;
	bool aboveHighThresh = false;
	const int hiCounterStart = 100;//20, max length during medium hysteresis




	
	MyVertex root;
	root.x = /*clickx*/best_lock.x;
	root.y = clicky;
	root.prob = 1;
	root.end = false;
	root.step=-1; //only one root, set at level -1 so first full set of nodes is 0
	rootIndex = cvGraphAddVtx(g, &root);
	

	vector<int> vec, lastvec, lastNodesAboveHi;

	
	int trace_pause = trace_pause_orig;

	cvCopy(image, previousi);// save for undo

		// stats for on the fly root end detection using cumulative stats
		int stat_count=0;
		int pre_window=2;//was 5 //number of steps before stat model begins.  Aims to eliminate clutter at start
		double st_dev_col[3];
		double running_total_col[3];
		double mean_col[3];
		double cum_total_sq[3];	
		double cum_total_x_sq[3];	

		mean_col[0] = 0;
		mean_col[1] = 0;
		mean_col[2] = 0;

		cum_total_sq[0]=0;
		cum_total_sq[1]=0;
		cum_total_sq[2]=0;
		cum_total_x_sq[0] = 0;
		cum_total_x_sq[1] = 0;
		cum_total_x_sq[2] = 0;

		st_dev_col[0]=0;
		st_dev_col[1]=0;
		st_dev_col[2]=0;
		running_total_col[0] = 0;
		running_total_col[1] = 0;
		running_total_col[2] = 0;



	//for (int i=0; i<200; i++) {
	if (calibration_mode) num_calib_traces++;

	// just to keep track of distance along /this/ root
	//double last_euc_step_size = 0;
	//double last_euc_distance = 0;
	//double cum_dist_prob = 0;
	
	
	//rulematchedcounter=2; //higher number, more matches to find end? should deduc tthis from lenght?!

	const int range = 10;
	double dataset1r[range];// not used any more
	double dataset1g[range];
	double dataset1b[range];
	double dataset2r[range*2];
	double dataset2g[range*2];
	double dataset2b[range*2];
	double mean_col1[3] = {0,0,0};
	double mean_col2[3] = {0,0,0};
	double st_dev1[3] = {0,0,0};
	double st_dev2[3] = {0,0,0};
	
	for (int rangecount=0; rangecount<range; rangecount++) {
		dataset1r[rangecount] = 0;
		dataset1g[rangecount] = 0;
		dataset1b[rangecount] = 0;

		dataset2r[rangecount] = 0;
		dataset2g[rangecount] = 0;
		dataset2b[rangecount] = 0;
		dataset2r[rangecount+range] = 0;
		dataset2g[rangecount+range] = 0;
		dataset2b[rangecount+range] = 0;
	}
	
	int hiCounter = hiCounterStart; // max number steps to stay within mid hysteresis

	//asad
	CvPoint pt;
	const int maskSize = 20;
	const int maxSteps = 1000;
	double sRsArray[maxSteps][41];
	double arrayroot[maxSteps][41];
	double maxRed;

	for(int r=0;r<maxSteps;r++)
		for(int c=0;c<=40;c++)
			sRsArray[r][c] = 0;

	int stepCounter=0,ctr;
	double col;

	// PARAMETERS FOR CHOOSING FOREGROUND PIXELS AND CONNECTED COMPOENTS SIZES. 
	//THIS IS THE CONNECTED COMPONENT SIZE THAT DETERMINES IF IT IS A SUBROOT OR NOT.
	int min_root_size = 15; 
	double thresh=10;


	int xmin,xmax;
	UCHAR r1,g1,b1;

	CvPoint backBone[1000];

	// -asad

	// asad

	//CONNECTED COMPONENT CODE.( CCC )
	//These two arrays contain the FG pixels in the 
	//Left and right side of the root.
	CvPoint left_Sub_Root[10000];    
	CvPoint right_Sub_Root[10000];
	int leftRootCount=0;
	int rightRootCount=0;
	drawTabooPoints();

	double backBoneAngle = 0;
	IplImage* ironImg= cvCreateImage(cvSize(40,1000),8,3);


	// - asad

	CvPoint oldp;
	oldp.x = -1;
	oldp.y = -1;
	//dragon=false;
	//if (filenum==17&&rootnum==1) {
	//	MessageBox::Show("Dragon!");
	//	dragon=true;
	//}

	while (i<max_number_steps_to_trace && code!=32) {
		if (i==max_number_steps_to_trace-1) MessageBox::Show( "Tracing size has exceeded maximum trace size.\nTerminating Trace for this root. ", "Warning", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		/*
		if (i>20) {
			//TEST
			MyVertex *m = (MyVertex*)cvGetGraphVtx(g, 5);
			cout<<"TEST prob = "<<m->prob<<", x = "<<m->x<<endl;

		}*/

		/*if (code==91) trace_pause = trace_pause + 20;
		if (code==93 && trace_pause > 31) trace_pause = trace_pause - 30;
		if (code==93 && trace_pause <= 30) trace_pause = 1;*/
		
		//cout<<"Iter "<<i<<endl;
		//cout<<"Picking samples..."<<endl;
		cond.incrementStepCount();

		cond.pick_samples();
		//cout<<"Processing samples..."<<endl;
		
		//cvCopyImage(originali, image); //just clear things up...
		cond.process_samples(bestGuess_t_1, i, image); ///DONT USE originali
		
		//cout<<"Measurement phase..."<<endl;
		cond.measurementPhase(measuresiTaboo);//WAS measuresi ; changed so we can modify it to prevent backtracking....

		cond.updateBestPos();

		//cvCopy(originali, image); // only show latest point
		
		
		if (pre_window>0) pre_window--; // Don't begin forming stat model immediately
		

		if (numTs>=1 && pre_window<=0)  {

			CvPoint p;// = drawSample(crop, 0,  255, 255, 255, drawsamples, i, 0-crop_orig_x, 0-crop_orig_y);
			drawSample(crop, 0,  255, 255, 255, drawsamples, i, 0-crop_orig_x, 0-crop_orig_y);//still update zoom window
			
			p.x = cond.getBestX(0);
			p.y = cond.getBestY(0);

			
			
			
			
			if (motionMixture>0/*&&!useGravProcessModel */&& i>=max(15, cond.getNumPointsToFit()+1)) { // but doesn't this paint over the measurement region in section below...?!?!?!?!?!?? Nope, that uses measurei
						// ALSO make sure 15 is the right number. Don't want this sticking at the top...

				//cond.paintParticleSet(measuresiTaboo,image,0,0,0);//OLD WAY. Was painting on 'image', changed by apf
				

				//new way - paint a circle at best position
				//now do on measuresi as well as image...?
				/*if (oldp.x!=-1) cvLine( image, oldp, p, CV_RGB(0,0,0), 3);
				cvCircle(image, p, 3, CV_RGB(0, 0, 0), -1);
				if (oldp.x!=-1) cvLine( measuresiTaboo, oldp, p, CV_RGB(0,0,0), 3);
				cvCircle(measuresiTaboo, p, 3, CV_RGB(0, 0, 0), -1);*/

				//Try convex hull
				CvMemStorage* storage = cvCreateMemStorage();
				CvSeq* ptseq = cvCreateSeq( CV_SEQ_KIND_GENERIC|CV_32SC2, sizeof(CvContour),
					sizeof(CvPoint), storage );
				CvSeq* hull;
				CvPoint pt0;
				for( int i = 0; i < Nsamples; i++ )
				{
					pt0.x = cond.s_new[i].x[0];
					pt0.y = cond.s_new[i].y[0];
					cvSeqPush( ptseq, &pt0 );
				}
				hull = cvConvexHull2( ptseq, 0, CV_CLOCKWISE, 0 );
				int hullcount = hull->total;

				CvPoint* pts = new CvPoint[Nsamples];//at most NSamples
				pt0 = **CV_GET_SEQ_ELEM( CvPoint*, hull, hullcount - 1 );
				for( int i = 0; i < hullcount; i++ )
				{
					CvPoint pt = **CV_GET_SEQ_ELEM( CvPoint*, hull, i );
					pts[i] = cvPoint(pt.x, pt.y);
					//cvLine( image, pt0, pt, CV_RGB( 0, 0, 0 ));
					//cvLine( measuresiTaboo, pt0, pt, CV_RGB( 0, 0, 0 ));
					pt0 = pt;
				}
				//cvFillConvexPoly(image, pts, hullcount, CV_RGB(0,0,0), 8  );
				cvFillConvexPoly(measuresiTaboo, pts, hullcount, CV_RGB(0,0,0), 8  );
				cvReleaseMemStorage(& storage );
				cvReleaseMemStorage(& hull->storage );
				delete [] pts;

				
				
				double uu, vv;
				int j = 3;
				uu = p.x-oldp.x;
				vv = p.y-oldp.y;
				CvPoint line1, line2;

				/*
				if (uu==0)					{ line1.x = -j; line1.y =   0; line1.x = j; line2.y =  0; }
				else if (uu==1&&vv==1)		{ line1.x = -j; line1.y =  -j; line2.x = j; line2.y = j; }
				else if (uu==1&&vv==-1)		{ line1.x = -j; line1.y =   j; line2.x = j; line2.y = -j; }
				else if (uu==-1&&vv==1)		{ line1.x = -j; line1.y =  -j; line2.x = j; line2.y = j; }
				else if (uu==-1&&vv==-1)	{ line1.x = -j; line1.y =   j; line2.x = j; line2.y = -j; }
				else if (uu==-1)			{ line1.x = 0;  line1.y =  -j; line1.x = 0; line2.y =  j; }
				else MessageBox::Show("Eep");

				

				line1.x = p.x+line1.x;
				line1.y = p.y+line1.y;
				line2.x = p.x+line2.x;
				line2.y = p.y+line2.y;*/
				
				//cvCopyImage(originali, image); //just clear things up...

				//debug
				//cond.showSamples(image);//these get blanked out above with black (paintParticleSet), and written over here in yellow
				
				/*
				double rad = max((sqrt((uu * uu) + (vv * vv))-1.0), 1);
				if (oldp.x!=-1) cvCircle(measuresiTaboo, cvPoint((double)oldp.x, (double)oldp.y),rad, CV_RGB(0,0,0), -1) ;
				if (oldp.x!=-1) cvCircle(image,          cvPoint((double)oldp.x, (double)oldp.y), rad, CV_RGB(0,0,0), -1) ;
				MyPutPixel(image, oldp.x, image->height-oldp.y, 128, 0, 128);
				MyPutPixel(image, p.x, image->height-p.y, 255, 0, 255);*/

				/*cvLine(image, line1, line2, CV_RGB(0,0,0), 1);
				cvLine(measuresiTaboo, line1, line2, CV_RGB(0,0,0), 1);*/

				MyPutPixel(image, p.x, image->height-p.y, 255, 0, 255);  //draw dots down root during tracking

				//RE WEIGHT TEST
				//cond.measurementPhase(measuresiTaboo);
				
				
				cvShowImage("Image", image);
				// Refresh zoom window from image ??

				
				cvSetImageROI(image, cvRect( crop_orig_x, crop_orig_y, cropx, cropy) );		
				cvCopy(image, crop);
				cvShowImage("Zoom", crop);
				cvResetImageROI(image);
				//cvWaitKey(1);
				
				
				
			}
			else if (motionMixture>0 /*&&!useGravProcessModel*/ && i<15){
				//if we're <15, just draw a black line across image at this y. Can do this because we know we're moving down 1 pixel each time
				
				//cvLine(image, cvPoint(p.x-10, p.y), cvPoint(p.x+10, p.y), CV_RGB(0,0,0), 1);
				cvLine(measuresiTaboo, cvPoint(p.x-10, p.y), cvPoint(p.x+10, p.y), CV_RGB(0,0,0), 1);


			}
			//Sample best = cond.getAverageSample();
			//MessageBox::Show("au, av: "+cond.getBestAU(0) + ", "+  cond.getBestAV(0)  );

			oldp.x = p.x;
			oldp.y = p.y;
			
			
			UCHAR re, gr, bl;
			
			//MyGetPixel(measuresi, p.x, measuresi->height-p.y, &re, &gr, &bl); // GET FROM MEASUREMENT IMAGE
			re = cond.GetColourMeasureAtPos( p.x, measuresi->height-p.y, 2, measuresi,  0);
			gr = cond.GetColourMeasureAtPos( p.x, measuresi->height-p.y, 2, measuresi,  1);
			bl = cond.GetColourMeasureAtPos( p.x, measuresi->height-p.y, 2, measuresi,  2);

			




			running_total_col[0] += re; 
			running_total_col[1] += gr; 
			running_total_col[2] += bl; 
		    stat_count++;
		    mean_col[0] = running_total_col[0]/stat_count;
			mean_col[1] = running_total_col[1]/stat_count;
			mean_col[2] = running_total_col[2]/stat_count;
		    
			cum_total_sq[0] = running_total_col[0]*running_total_col[0];
			cum_total_sq[1] = running_total_col[1]*running_total_col[1];
			cum_total_sq[2] = running_total_col[2]*running_total_col[2];

			cum_total_x_sq[0] += (re*re);
			cum_total_x_sq[1] += (gr*gr);
			cum_total_x_sq[2] += (bl*bl);

			
		    st_dev_col[0] = sqrt( (stat_count * cum_total_x_sq[0]-cum_total_sq[0])/ (stat_count*(stat_count-1)) ) ;
			st_dev_col[1] = sqrt( (stat_count * cum_total_x_sq[1]-cum_total_sq[1])/ (stat_count*(stat_count-1)) ) ;
			st_dev_col[2] = sqrt( (stat_count * cum_total_x_sq[2]-cum_total_sq[2])/ (stat_count*(stat_count-1)) ) ;
			
			
			//stat_count++;  
			dataset1r[stat_count%range] = re;
			dataset1g[stat_count%range] = gr;
			dataset1b[stat_count%range] = bl;

			dataset2r[stat_count%(range*2)] = re;
			dataset2g[stat_count%(range*2)] = gr;
			dataset2b[stat_count%(range*2)] = bl;

			/*
			// count stats for last 10 steps - this is stored in dataset1, mean_col1 and st_dev1
			if (stat_count>range*2) {
				// MEAN
				double mean=0;
				//red
				for (int m=0; m<range; m++)
					mean+=dataset1r[m];
				mean_col1[0] = mean/range;
				
				//green
				mean=0;
				for (int m=0; m<range; m++)
					mean+=dataset1g[m];
				mean_col1[1] = mean/range;
				
				//blue
				mean=0;
				for (int m=0; m<range; m++)
					mean+=dataset1b[m];
				mean_col1[2] = mean/range;


				// SD
				// red
				double sum=0;
				for (int m=0; m<range; m++)
					sum+=dataset1r[m]*dataset1r[m] - mean_col1[0]*mean_col1[0];
				sum /=range;
				st_dev1[0] = sqrt(sum);
				
				//green
				sum=0;
				for (int m=0; m<range; m++)
					sum+=dataset1g[m]*dataset1g[m] - mean_col1[1]*mean_col1[1];
				sum /=range;
				st_dev1[1] = sqrt(sum);
				
				//blue
				sum=0;
				for (int m=0; m<range; m++)
					sum+=dataset1b[m]*dataset1b[m] - mean_col1[2]*mean_col1[2];
				sum /=range;
				st_dev1[2] = sqrt(sum);

			}




			// count stats for between 20 and 10 steps - this is stored in dataset1, mean_col1 and st_dev1
			int start = stat_count+1;
			if (stat_count>range*2) {
				// MEAN - only look at oldest 'range' in data set of 'range'*2
				double mean=0;
				//red
				for (int m=start; m<start+range; m++)
					mean+=dataset2r[m%(range*2)];
				mean_col2[0] = mean/range;
				
				//green
				mean=0;
				for (int m=start; m<start+range; m++)
					mean+=dataset2g[m%(range*2)];
				mean_col2[1] = mean/range;
				
				//blue
				mean=0;
				for (int m=start; m<start+range; m++)
					mean+=dataset2b[m%(range*2)];
				mean_col2[2] = mean/range;


				// SD
				// red
				double sum=0;
				for (int m=start; m<start+range; m++)
					sum+=dataset2r[m%(range*2)]*dataset2r[m%(range*2)] - mean_col2[0]*mean_col2[0];
				sum /=range;
				st_dev2[0] = sqrt(sum);
				
				//green
				sum=0;
				for (int m=start; m<start+range; m++)
					sum+=dataset2g[m%(range*2)]*dataset2g[m%(range*2)] - mean_col2[1]*mean_col2[1];
				sum /=range;
				st_dev2[1] = sqrt(sum);
				
				//blue
				sum=0;
				for (int m=start; m<start+range; m++)
					sum+=dataset2b[m%(range*2)]*dataset2b[m%(range*2)] - mean_col2[2]*mean_col2[2];
				sum /=range;
				st_dev2[2] = sqrt(sum);

			}

			MessageBox::Show("Mean new " +  mean_col1[0] +", mean old "+mean_col2[0]+"\n\nSt New "+st_dev1[0]+", stdev old "+st_dev2[0]);
			*/


			int window = 5;//was 50. Actually can't measure root smaller than pre_window.  This is just 
						   // the number below which estimate stats are used

			// HACK FIX use const stats for short roots

			if (stat_count<=window) {
				st_dev_col[0] = 50;//2.5
				st_dev_col[1] = 50;
				st_dev_col[2] = 50;
				mean_col[0] = 255;
				mean_col[1] = 255;
				mean_col[2] = 255;
			}

			// UPDATE COND APPEARANCE MODEL STATS
			//cond.setObsModelParams(mean_col[0], mean_col[1], mean_col[2],st_dev_col[0], st_dev_col[1], st_dev_col[2]);



			if (filenum==0) {
				char st[255];
				sprintf(st,"%i, %i, %i, %i, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f", rootnum, re, gr, bl, mean_col[0], mean_col[1], mean_col[2], st_dev_col[0], st_dev_col[1], st_dev_col[2]); 
				
				prob_stats.AppendDataRow(st);

			}

		
			//
			//// Western electric rules
			
			bool rulematched = false;

			int stats_index;
			if (multiple_BG_stats) stats_index = rootnum; //we have multiple BG models
			else stats_index = 0; //we only have one background model
			

			double colrange = 0.5;// this is grey level accuracy, hence range
			double sd_expander = 0; //increased sd due to expected base level of variation even if not measured. Bit of a hack
			double z_fg_r1 = ((double)re -  mean_col[0]-colrange)/ (st_dev_col[0]+sd_expander);
			double z_fg_g1 = ((double)gr -  mean_col[1]-colrange)/ (st_dev_col[1]+sd_expander);
			double z_fg_b1 = ((double)bl -  mean_col[2]-colrange)/ (st_dev_col[2]+sd_expander);

			double z_fg_r2 = ((double)re -  mean_col[0]+colrange)/ (st_dev_col[0]+sd_expander);//10
			double z_fg_g2 = ((double)gr -  mean_col[1]+colrange)/ (st_dev_col[1]+sd_expander);
			double z_fg_b2 = ((double)bl -  mean_col[2]+colrange)/ (st_dev_col[2]+sd_expander);
			
		
			double z_bg_r1 = ((double)re -  bg_mean[stats_index][0]-colrange)/bg_sd[stats_index][0];
			double z_bg_g1 = ((double)gr -  bg_mean[stats_index][1]-colrange)/bg_sd[stats_index][1];
			double z_bg_b1 = ((double)bl -  bg_mean[stats_index][2]-colrange)/bg_sd[stats_index][2];

			double z_bg_r2 = ((double)re -  bg_mean[stats_index][0]+colrange)/bg_sd[stats_index][0];
			double z_bg_g2 = ((double)gr -  bg_mean[stats_index][1]+colrange)/bg_sd[stats_index][1];
			double z_bg_b2 = ((double)bl -  bg_mean[stats_index][2]+colrange)/bg_sd[stats_index][2];
			

			//now calc value of normal curve for these z scores:
			//double fg_prob = evaluate_gaussian(z_fg_r, 1) * evaluate_gaussian(z_fg_g, 1) * evaluate_gaussian(z_fg_b, 1);
			//double bg_prob = evaluate_gaussian(z_bg_r, 1) * evaluate_gaussian(z_bg_g, 1) * evaluate_gaussian(z_bg_b, 1);
			

			// JUST RED FOR NOW?
			double bg_prob = CZTable::probBetweenScores(z_bg_r1, z_bg_r2);
			double fg_prob = CZTable::probBetweenScores(z_fg_r1, z_fg_r2);


			//test to see if table prob measure is same as reading off gaus curve
			// yes, at least where range = 1 (which is the case here)
			/* static const double sqrt2xConPi = 2.50662827463100050241;
			 double sigma = st_dev_col[0];
			 double val = mean_col[0] - re;
			 double evalg = 1.0/(sqrt2xConPi * sigma) *
				 exp(-0.5 * (val*val / (sigma*sigma)));
			MessageBox::Show("fg_prob =    " + fg_prob +"\neval_gaus = "+evalg);*/



			/*
			// DEBUG show seg image
			UCHAR rr, gg, bb;
			for (int xx=0; xx<originali->width; xx++) {
				for (int yy=0; yy<originali->height; yy++) {
					MyGetPixel(originali, xx, yy, &rr, &gg, &bb);
					double fg_prob = evaluate_gaussian(rr-mean_col[0], st_dev_col[0]) * evaluate_gaussian(gg-mean_col[1], st_dev_col[1]) * evaluate_gaussian(bb-mean_col[2], st_dev_col[2]);
					double bg_prob = evaluate_gaussian(rr-bg_mean[0], bg_sd[0]) * evaluate_gaussian(gg-bg_mean[1], bg_sd[1]) * evaluate_gaussian(bb-bg_mean[2], bg_sd[2]);
					if (bg_prob / fg_prob > 1) MyPutPixel(originali, xx, yy, 0, gg, 0);
				}
			}
			cvNamedWindow("TEST", 2);
			cvShowImage("TEST", originali);
			cvSaveImage("seg.bmp", originali);
			cvWaitKey(0);
			exit(9982);*/
			
			

			/*
			// calculate length from start
			double x_length = clickx-p.x;
			double y_length = clicky-p.y;

			double euc_dist = sqrt((x_length*x_length) + (y_length*y_length));
			
			last_euc_step_size = abs(euc_dist-last_euc_distance);

			double st_dist = 2;//pixels, euc?
			double range   = 0.5*last_euc_step_size; // 1 pix
			double z_dist1 = (last_frame_euc_dist[rootnum] -  euc_dist-range)/st_dist;
			double z_dist2 = (last_frame_euc_dist[rootnum] -  euc_dist+range)/st_dist;

			// try and calc prob of distance from z table - look up small range either side
			
			double dist_prob = CZTable::probBetweenScores(z_dist1, z_dist2);
			cum_dist_prob+= dist_prob;
			*/

			
     		//double prior_bg = 0.5;//0.999;
			double prior_fg = 1-prior_bg;//0.1;

			//double threshold=1.1;
			
			//if (bg_prob / fg_prob > 1) rulematched = true;
			
			/*if (  (bg_prob*prior_bg*cum_dist_prob) /
				  (fg_prob*prior_fg*(1-cum_dist_prob))
				  > threshold) 
					rulematched = true;*/

			// BAYES prob(BGModel| data)
			double posterior_bg_model; //= (bg_prob*prior_bg )/ ( (bg_prob*prior_bg) + (fg_prob*prior_fg));
			double posterior_fg_model; //= 1-posterior_bg_model;

			double hiHystThreshold = upperHyst;//0.97;//0.65 for dataset A, 0.8, 0.9
			double loHystThreshold = lowerHyst;//0.4; // 0.1 for dataset A, 0.05, 0.4

			

			if (bg_prob<0.0000000001 && fg_prob<0.0000000001) {
				// handle divide by zero, shouldn't really occur but does at the moment...
				// low prob of both FG and BG so set FG prob to be between hyst levels
				
				
				if (!mainWindow_RT->getDefaultToRootState()) {
					// if no data, assume we are between thresholds
					posterior_fg_model = (hiHystThreshold + loHystThreshold)/2;
					posterior_bg_model = 1- posterior_fg_model;
					//MessageBox::Show("Zero special case: step "+i);
				}
				else
				{
					//if getDefaultToRootState true, then when we have no data for bayes rule (ie fg = bg = 0) then assume we are above high thresh rather than middle thresh
					posterior_fg_model = 1;
					posterior_bg_model = 0;
				}

				
				

				if (annotateImage) cvLine(output, cvPoint(p.x-15, p.y), cvPoint(p.x-5, p.y), cvScalar(0, 255, 255), 1);//mark zeros

			}
			else
			{
				//normal
				// NOTE should this really all revolve around the BG calc?
				// do posterior_bg_model + posterior_fg_model really = 1?
				// UPDATE yes this seems to work ok
				 posterior_bg_model = (bg_prob*prior_bg )/ ( (bg_prob*prior_bg) + (fg_prob*prior_fg));
				 posterior_fg_model = (fg_prob*prior_fg )/ ( (bg_prob*prior_bg) + (fg_prob*prior_fg));
				 //posterior_fg_model = 1-posterior_bg_model;
				 //MessageBox::Show("Step "+i+": fg "+fg_prob+ ", bg "+bg_prob +", post_fg "+posterior_fg_model);
			}
			
			

			if (posterior_fg_model>hiHystThreshold) {
				hiCounter=hiCounterStart;     // Hi Hysteresis thresh //rulematched = true;// rulematchedcounter--;
				if (annotateImage) cvLine(output, cvPoint(p.x+14, p.y), cvPoint(p.x+20, p.y), cvScalar(0, 255, 0), 1);//high thresh

			}
			else if (posterior_fg_model>loHystThreshold) {
				hiCounter--; // Middle threshold
				//MessageBox::Show("FGProb = "+fg_prob+", re="+re+", mean="+mean_col[0]+", sd = "+st_dev_col[0]+"\nz1 = "+z_fg_r1+", z2 = "+z_fg_r2  +"\n\nBGProb = "+bg_prob+", re="+re+", mean="+bg_mean[0]+", sd = "+bg_sd[0]+"\nz1 = "+z_bg_r1+", z2 = "+z_bg_r2+"\n\nFG posterior prob = "+posterior_fg_model);
				if (annotateImage) cvLine(output, cvPoint(p.x+10, p.y), cvPoint(p.x+15, p.y), cvScalar(255, 0, 255), 1);//mid thresh
			
			}
			else { //below low threshold
				
				//cvCircle(output, p, 3, cvScalar(255, 0, 255),-1);

				if (annotateImage) cvLine(output, cvPoint(p.x-10, p.y), cvPoint(p.x+10, p.y), cvScalar(255, 255, 255), 1);//mark end at low threshold

				//imgTxt(output, myspf("R = I(%i), M(%i)", (int)re, (int)mean_col[0]), cvPoint(10, 20), cvScalar(255, 80, 20));
				//MessageBox::Show("hiCounter==0\nposterior_fg_model = "+posterior_fg_model+"\nR = I("+re+"), M("+(int)mean_col[0]+")");
				//MessageBox::Show("Step "+i+"\nhihystthreshold = "+hiHystThreshold+", low = "+loHystThreshold+"\np_fg = "+posterior_fg_model+"\n fg "+fg_prob+", bg "+bg_prob );

				hiCounter=0;							  // Low threshold option
			}
			// else rulematchedcounter=2;
			//MessageBox::Show("FG with prob "+posterior_fg_model+"\nBG with "+posterior_bg_model);
			
			
			bool doHystDebuggraph = false;
			// DEBUG GRAPH
			if (doHystDebuggraph){
				int dgscale=200;
				if (debuggraph==NULL) 
					debuggraph = cvCreateImage(cvSize(700, dgscale), 8, 3);
				if (i<2){
					cvZero(debuggraph);
					cvLine(debuggraph, cvPoint(0,(double)dgscale-hiHystThreshold*((double)dgscale-1.0)),cvPoint(700,(double)dgscale-hiHystThreshold*((double)dgscale-1.0)), cvScalar(0,200,0),1);
					cvLine(debuggraph, cvPoint(0,(double)dgscale-loHystThreshold*((double)dgscale-1.0)),cvPoint(700,(double)dgscale-loHystThreshold*((double)dgscale-1.0)), cvScalar(0,0,200),1);

				}
			    
				//if (rootnum==0) {
					if (oldpoint.x==-1) {
						oldpoint.x = 0;
						oldpoint.y = (double)dgscale-posterior_fg_model*((double)dgscale-1.0);
					}
					
					cvLine(debuggraph, oldpoint, cvPoint(i, (double)dgscale-posterior_fg_model*((double)dgscale-1.0)), cvScalar(255, 255, 255), 1);
					oldpoint.x = i;
					oldpoint.y = (double)dgscale-posterior_fg_model*((double)dgscale-1.0);
					cvNamedWindow("Graph", 2);
					cvShowImage("Graph", debuggraph);
				//}

				cvWaitKey(30);
			}
			// END DEBUG GRAPH
			

			//double posterior_fg_model = (fg_prob*prior_fg )/ ( (fg_prob*prior_fg) + (bg_prob*prior_bg));




			//mainWindowText(mainWindow, ""+bg_prob+", "+fg_prob+";  "+ posterior_bg_model);

			


			//// BG rule
			//double ruleBG_sd = 5;
			//if (  /*stat_count>window &&*/
			//	  ( (re< bg_mean[0]+bg_sd[0]*ruleBG_sd  && re> bg_mean[0]-bg_sd[0]*ruleBG_sd)   &&
			//	    (gr< bg_mean[1]+bg_sd[1]*ruleBG_sd  && gr> bg_mean[1]-bg_sd[1]*ruleBG_sd)   &&
			//	    (bl< bg_mean[2]+bg_sd[2]*ruleBG_sd  && bl> bg_mean[2]-bg_sd[2]*ruleBG_sd) 
			//		)) {
			//			rulematched = true;
			//			MessageBox::Show("Rule BG matched");
			//}
			


			////test rule 1 : 1>3sd
			//double rule1_sd = 3.5;
			//if (  /*stat_count>window &&*/
			//	  ( (re> mean_col[0]+st_dev_col[0]*rule1_sd  || re< mean_col[0]-st_dev_col[0]*rule1_sd)   ||
			//	    (gr> mean_col[1]+st_dev_col[1]*rule1_sd  || gr< mean_col[1]-st_dev_col[1]*rule1_sd)   ||
			//	    (bl> mean_col[2]+st_dev_col[2]*rule1_sd  || bl< mean_col[2]-st_dev_col[2]*rule1_sd) 
			//		)) {
			//			rulematched = true;
			//			MessageBox::Show("Rule 1 matched");
			//}

			////test rule 2 : 2 out of 3 consecutive > 2 sd, on same side of mean
			//double rule2_sd = 2.5;
			//if (  /*stat_count>window &&*/
			//	  ( (re> mean_col[0]+st_dev_col[0]*rule2_sd  || re< mean_col[0]-st_dev_col[0]*rule2_sd)   ||
			//	    (gr> mean_col[1]+st_dev_col[1]*rule2_sd  || gr< mean_col[1]-st_dev_col[1]*rule2_sd)   ||
			//	    (bl> mean_col[2]+st_dev_col[2]*rule2_sd  || bl< mean_col[2]-st_dev_col[2]*rule2_sd) 
			//		))
			//	{

			//		
			//		if (rule2_count>=0) {
			//			if ( (re-mean_col[0]>0 && rule_2_polarity) ||(re-mean_col[0]<=0 && !rule_2_polarity) )
			//			{
			//			rulematched = true;
			//			MessageBox::Show("Rule 2 matched");
			//			}
			//		}

			//		rule2_count=2;
			//		if (re-mean_col[0]>0) rule_2_polarity=true; else rule_2_polarity = false; //just check red channel
			//		
			//}

				


			/*if (  hiCounter==0) {

					// We aren't over a root any more, break out of trace before we add to graph
				   
					  
					  break;	


				   //last_frame_euc_dist[rootnum] = euc_dist;
			} */

		
			//last_euc_distance = euc_dist; //lsat step, not last frame
		}

		cvShowImage("Zoom", crop);

		//double cum_prob_test = 0;
		

		if (hiCounter==hiCounterStart) lastNodesAboveHi.clear();
		else if (hiCounter==0 ) {
				// we are over background.
				// Set last stored Hi nodes to end
				//MessageBox::Show("lastNodesAboveHi.size() = "+lastNodesAboveHi.size());
				for (unsigned int vn=0; vn<lastNodesAboveHi.size();vn++){
					//if (dragon ) MessageBox::Show("end=true set.");
					int old_index = lastNodesAboveHi.at(vn);
					MyVertex * endvtx = (MyVertex*) cvGetGraphVtx( g, old_index );
					endvtx->end = true;
					if (annotateImage) cvLine(output, cvPoint(endvtx->x+8, endvtx->y), cvPoint(endvtx->x+12, endvtx->y), cvScalar(0, 0, 0), 1);//break point
					
				}
				//MessageBox::Show("Breaking");
				oldpoint.x=0;
				break;  //moved from If above, as it would cause this code to be bypassed
		}

		// add each sample to the graph this timestep
		// and connect to all added at the last timestep
		for (int n=0; n<NSAMPLES; n++) {  
			double x = cond.s_new[n].x[0];
			double y = cond.s_new[n].y[0];
			
			/*
			// Write out stats NB before pruning! and not nromalized!
			char  st[70];
			sprintf(st,"%i, %f, %f, %f", i, cond.s_new[n].x[0], cond.s_new[n].y[0], cond.s_new[n].pi);

			stats.AppendDataRow(st);
			*/


			MyVertex myv;
			CvGraphVtx **inserted_myv=NULL;
			myv.x = x;
			myv.y = y;
			myv.prob = cond.s_new[n].pi/cond.largest_cumulative_prob;  //divide by cum prob normalizes probs to 0..1?!
			myv.prob_non_normalized = cond.s_new[n].pi;
			//cout <<" PROB TEST: cond: "<<cond.s_new[n].pi<<", myv: "<<myv.prob<<endl;
			//cum_prob_test+= myv.prob;
			myv.end = true;
			myv.state = cond.getMajorityState(0); //store the best state at this time
			myv.step = i; // iteration, or graph level - root is -1, set above;
			int index = cvGraphAddVtx(g, &myv, inserted_myv);
			// cout <<" Vertex added at index "<<index<<endl;

			// add new vertices to a set so we can find them next iteration
			vec.push_back(index);

			// If at this stage we are still above high threshold, set this so we can back track here
			// if hysterysis drops below low thresh later
			if (hiCounter==hiCounterStart) {
				lastNodesAboveHi.push_back(index);
				//mainWindowText(mainWindow, "Push back hi index");
				//MessageBox::Show("push_back "+index);
			}
			


			// connect each vertex created at t-1 to this one
			if (!lastvec.empty()){
				for (unsigned int vn=0; vn<lastvec.size();vn++){
					//char * ptrToElem = cvGetSeqElem(lastVertices, vn);
					int old_index = lastvec.at(vn);
					
					// we now link from the old vtx
					// so it is no longer an 'end' point
					MyVertex * oldvtx = (MyVertex*) cvGetGraphVtx( g, old_index );
					oldvtx->end = false;
				

					int result = cvGraphAddEdge(g, old_index, index);
					
					if (result==0) cout << "WARNING Edge already exists."<<endl;
							
				}
					} else {
							//connect this vertex to the root vertex
							cvGraphAddEdge(g, rootIndex, index);
							
				}


			
		}// loop through each sample at time t
		//cout <<" cum prob TEST = "<<cum_prob_test<<endl; //should = 1 - and does
		
		//if (!lastvec.empty()) {
				lastvec.clear();
				lastvec.swap(vec);
				//cout<<" *************** vec size = "<<vec.size()<<endl;
		//}



		cond.getStateProbability(0);
		/*
		char  st[70];
		sprintf(st,"%i, %i, %i, %.3f", (int)cond.getBestX(0), (int)cond.getBestY(0), cond.getMajorityState(0), cond.getStateProbability(0));

		stats.AppendDataRow(st);*/

		if (findLaterals){
		// Asad
		//********************************************************
		//this code will calculate a rectangular area -20 +20 pixels
		//with the tracked root in the center

		subRootCountLeft = 0; //apf
		subRootCountRight= 0; //apf
		
        pt.x = cond.getBestX(0);
        pt.y = cond.getBestY(0);
        
        //this backbone is the centerline of the main root
        backBone[stepCounter].x = pt.x;
        backBone[stepCounter].y = pt.y;

		cond.setBack(stepCounter, 0,  pt.x);
		cond.setBack(stepCounter, 1,  pt.y);

		meanColour[stepCounter][0] = mean_col[0];
		meanColour[stepCounter][1] = mean_col[1];
		meanColour[stepCounter][2] = mean_col[2];
        
        //lets work out the backbone angle
        if(stepCounter >5 )
        {
            int xaway, yaway;
            int oldX = backBone[stepCounter-3].x;
            int oldY = backBone[stepCounter-3].y;
            int curX = backBone[stepCounter].x;
            int curY = backBone[stepCounter].y;
           
            int theta1,theta2 ;
  //          backBoneAngle = asin(  (curY-oldY) / (sqrt( 1.0*(curX-oldX)*(curX-oldX) + 1.0*(curY-oldY)*(curY-oldY)  )  ) );
            backBoneAngle = atan( 1.0*((curY - oldY)  ) / (curX - oldX ) )*180/PI ;
            
            int quardrant = getAngleQuardrant(oldX,oldY,curX,curY);
            if( quardrant ==1)
            {
                theta1 = ((int)abs(backBoneAngle)+90)%360;
                theta2 = (theta1+180)%360;
            }
            if(quardrant == 2)
            {
                theta1 = (180-(int)abs(backBoneAngle)+90)%360;
                theta2 = (theta1+180)%360;
            }
            if(quardrant == 3)
            {
                theta1 = ((int)abs(backBoneAngle) + 180+90)%360;
                theta2 = (theta1+180)%360;
                
            }
            if(quardrant == 4)
            {
                theta1 = (360-(int)abs(backBoneAngle)+90)%360;
                theta2 = (theta1+180)%360;
            
            
            }
            
            angle1 = PI*theta1/180;
            angle2 = PI*theta2/180;
            
            angle1Array[stepCounter] = angle1;
            angle2Array[stepCounter] = angle2;
            
            xaway = curX + 20*cos(angle1);
            yaway = curY + 20*sin(angle1);                
            
           // cvLine(image,cvPoint(backBone[stepCounter].x+100,backBone[stepCounter].y),cvPoint(xaway+100,yaway),CV_RGB(0,250,0),2,1);
            
            xaway = curX + 20*cos(angle2);
            yaway = curY + 20*sin(angle2);                
            
           // cvLine(image,cvPoint(backBone[stepCounter].x+100,backBone[stepCounter].y),cvPoint(xaway+100,yaway),CV_RGB(0,0,250),2,1);
            
           // cvLine(image,cvPoint(20,20),cvPoint(20+10*cos(theta),20+10*sin(theta)),CV_RGB(255,0,0),2);
           
        
        }
        
        
        MyGetPixel(measuresi, pt.x, measuresi->height-pt.y, &b1, &g1, &r1);
        maxRed = (double)r1;
        
        //pt is the best guess now.           
        
        //lets populate the srsarray with the linear mask values.
        ctr=0;
        xmin = pt.x - maskSize ;
        xmax = pt.x + maskSize ;
        CvPoint mask[40];
        
        for(int ii=20; ii>=0; ii--)
        {
            mask[20-ii].x = pt.x + ii*cos(angle1);
            mask[20-ii].y = pt.y + ii*sin(angle1);   
        }
        for(int ii=0; ii<20; ii++)
        {
            mask[20+ii].x = pt.x + ii*cos(angle2);
            mask[20+ii].y = pt.y + ii*sin(angle2);
        }
        
       // cvCircle(image,cvPoint(pt.x+50,pt.y),2,CV_RGB(200,35,62),2);
        
        
       
        
        
        for(int p=0;p<=40;p++)
        {
        
           // MyGetPixel(measuresi, p, image->height-pt.y, &b1, &g1, &r1);
            MyGetPixel(/*originali*/measuresi,  mask[p].x, image->height-mask[p].y,  &b1, &g1, &r1); // 3/11/2010 apf, was measuresi
             
           //GO TO THE OTHER PLACE  
           // sRsArray[stepCounter][ctr] =  (int)((((double)r1) + ((double)g1) + ((double)b1))/3); 
            
            MyPutPixel(ironImg,p,1000-stepCounter,b1,g1,r1);
            
            
            ctr++;  
                                       
        } 
        
    
		}//if findLaterals
		// -Asad
		
		if (animate_trace) {
			code=cvWaitKey(trace_pause);//pause, was 5
			cvShowImage("Image", image);
		}

		cond.swap_arrays();
		i++;
		stepCounter++;
		

		

	}// i loop

	/*
	delete [] st_dev_col;
	delete [] running_total_col;
	delete [] mean_col;
	delete [] cum_total_sq;	
	delete [] cum_total_x_sq;	*/

	 //writing the ironed image
    
   
    if (findLaterals){


	IplImage* writeIronImage = cvCreateImage(cvSize(40,stepCounter),8,3);
    
    //cvCopy(ironImg,writeIronImage);
    
    
    for(int i=0; i<40; i++)
    {
        for(int j=0; j<stepCounter; j++)
        {
            MyGetPixel(ironImg , i ,ironImg->height-j , &b1 , &g1 , &r1);
            MyPutPixel(writeIronImage,i,writeIronImage->height-j,b1,g1,r1);
        
        }
    
    }
     
    cvSaveImage("ironedImage.bmp", writeIronImage);
    cvWaitKey(1);
   
   
    //now we have the ironed image
    //lets convert it to the binary image and then set the SrS array.
   //****************************************************************
   
    
    if(true)
    {
        int count,tmp;
        IplImage *tempiron = cvCreateImage(cvSize(writeIronImage->width,writeIronImage->height), IPL_DEPTH_8U, 1);
        cvCvtColor(writeIronImage, tempiron, CV_BGR2GRAY);
        cvDilate( tempiron, tempiron, NULL, 2 );
	    cvErode( tempiron, tempiron, NULL, 1 );
	    
	    int sum=0,cnt=0, avg;
	    
        for(int y =0; y<tempiron->height; y++)
        {
           for(int x=0; x<tempiron->width/2-15; x++)
           {
              sum  = sum + ((uchar*)(tempiron->imageData + tempiron->widthStep*y))[x];  
              cnt++;         		 
           }
        }
        
        for(int y =0; y<tempiron->height; y++)
        {
           for(int x=tempiron->width/2+5; x<tempiron->width; x++)
           {
              sum  = sum + ((uchar*)(tempiron->imageData + tempiron->widthStep*y))[x];  
              cnt++;         		 
           }
        }
       
        //=============================


         avg = sum/cnt;
         avg = avg*1.2;
	
	    for(int y =0; y<tempiron->height; y++)
        {
           for(int x=0; x<tempiron->width; x++)
           {
              tmp = ((uchar*)(tempiron->imageData + tempiron->widthStep*y))[x];
    		
    		 		   
              if(tmp<avg)
			  //if(tmp<(meanColour[y][2]))
              {
                    sRsArray[y][x] =  0;
	              //((uchar*)(temp->imageData + temp->widthStep*y))[x] = 255;
              }
              else
                    sRsArray[y][x] =  8;
	              //((uchar*)(temp->imageData + temp->widthStep*y))[x] = 0;

           }
        }
        //cvErode( tempiron, tempiron, NULL, 1 );
        
        ofstream oft("sRsArray.txt");
        for(int r=0; r<stepCounter; r++)
        {    
            for(int c=0; c<40; c++)
                oft<<sRsArray[r][c]<<", " ;
            oft<<endl;
        }
        oft.close();
       
	
	}
	
   
   
    //filling the left subroots
    ofstream outlfile;
    outlfile.open("outLfile.txt");
                      	
	for(int row=0; row<stepCounter; row++)
	{
	    for(int col =0; col < 10; col++)
	    {
	        outlfile<< sRsArray[row][col]<<",";
	        if(sRsArray[row][col]>0)
	        { 
	            left_Sub_Root[leftRootCount].x = col;//backBone[row].x  + (20-i)*cos(angle1Array[row]);
	            left_Sub_Root[leftRootCount].y = row;//backBone[row].y  + (20-i)*sin(angle1Array[row]);
	            leftRootCount++;
	        }    	    
	    }
	    outlfile<<endl;
	}
	outlfile.close();
         
    ofstream outrfile;
    outrfile.open("outRfile.txt");
    
	//filling the right subroots
	for(int row=0; row<stepCounter; row++)
	{
	    for(int col =30; col < 40; col++)
	    {
	        outrfile<< sRsArray[row][col]<<",";
	        if(sRsArray[row][col]>0)
	        {    
	             right_Sub_Root[rightRootCount].x = col;//backBone[row].x +  i*cos(angle2Array[row]);
	             right_Sub_Root[rightRootCount].y = row;//backBone[row].y +  i*sin(angle2Array[row]); 
	             rightRootCount++;    	             
	        }    	    
	    }
	    outrfile<<endl;
	}
	outrfile.close();
	//MessageBox::Show("The Left Subroot FG = " + leftRootCount +"\nThe Rite Subroot FG = " + rightRootCount);
	//NOW Roots left and right portions are filled. 
	//we can do connected components on the two arrays.
	
	//CvPoint subRootsLeft[500], subRootsRight[500];
	//int subRootCountLeft=0, subRootCountRight=0;
	
	//***************************************
	// PROCESSING LEFT ROOT
	   currentLeftSub = 0; 
	   
	   spstack s1,s2;
	   spque q1;
	   q1.emptyQue();
	   s1.emptyspstack();
	   s2.emptyspstack();
	   
	   int px,py,tx,ty;
	   
	   ccnode* tmpptr, *backptr;
	   
	   int flag = 1;

	  
	   for(int i=0; i<leftRootCount; i++)
		   q1.enque(left_Sub_Root[i]);
	   
	   while(!q1.isEmpty())
	   {
		
		   s1.push(q1.deque());	
		   while(2)
			{				
			  
			   flag = 0;
			   px = s1.MasterPtr->pt.x;
			   py = s1.MasterPtr->pt.y;

			   tmpptr = q1.head;
			   while(tmpptr != 0)
			   {
				   tx = tmpptr->pt.x;
				   ty = tmpptr->pt.y;

				   if(abs(px-tx)<=1 && abs(py-ty)<= 1)
				   {
					   backptr = tmpptr;
					   tmpptr = tmpptr->next;
					   s1.push(q1.dequemid(backptr));
					   flag = 1;
				   }
				   else
					   tmpptr = tmpptr->next;
				   if(tmpptr == NULL) 
					   break;

			   }
			    
			   if(flag == 0)
			   {
				   if(s1.isEmpty())
					   break;
				   else
				   {
				       s2.push(s1.pop());				       
				   }
				   
				   if(s1.isEmpty())
					   break;
				   
			   }
		    } //inner while loop

		   int numberofpts = s2.numberOfccnodes;

            
           
           
		   if(numberofpts  > min_root_size)
		   {
               //MessageBox::Show(numberofpts+" ");
               //cvCircle( image, cvPoint(s2.MasterPtr->pt.x,s2.MasterPtr->pt.y), 10, CV_RGB(255,0,0),7);               
               // test cvCircle( image, cvPoint(200,100*subRootCount), 10, CV_RGB(255,0,0),7);
			   currentLeftSub++;
			   
			   int x=0;
			   int y=99999, yl=0;
			   while(!s2.isEmpty())
			   {
			       if(x<s2.MasterPtr->pt.x)
			            x = s2.MasterPtr->pt.x;
			       if(y>s2.MasterPtr->pt.y)
			            y = s2.MasterPtr->pt.y; 
			       if(yl<s2.MasterPtr->pt.y)
			            yl = s2.MasterPtr->pt.y;    
			            
				 //  x = x + s2.MasterPtr->pt.x;
				 //  y = y + s2.MasterPtr->pt.y;
				   s2.pop();
			   }
			   y = (y+yl)/2;
			  // x = x/numberofpts+2;
			  // y = y/numberofpts+2;
			   subRootsLeft[subRootCountLeft].x =  backBone[y].x;
			   subRootsLeft[subRootCountLeft].y =  backBone[y].y;
			   subRootCountLeft++;
			   
			   //cvCircle( image, backBone[y], 10, CV_RGB(255,0,0),7);
			   
			   
		   }
		   else
			   s2.emptyspstack();
	    

	   }//LEFT SUBROOT while loop
	   
	    
	       
	  
	   	//***************************************
	// PROCESSING right ROOT
	
	   currentRightSub=0;
	   
	   q1.emptyQue();
	   s1.emptyspstack();
	   s2.emptyspstack();
	  
	   flag = 1;

	  
	   for(int i=0; i<rightRootCount; i++)
		   q1.enque(right_Sub_Root[i]);
	   
	   while(!q1.isEmpty())
	   {
		
		   s1.push(q1.deque());	
		   while(2)
			{				
			  
			   flag = 0;
			   px = s1.MasterPtr->pt.x;
			   py = s1.MasterPtr->pt.y;

			   tmpptr = q1.head;
			   while(tmpptr != 0)
			   {
				   tx = tmpptr->pt.x;
				   ty = tmpptr->pt.y;

				   if(abs(px-tx)<=1 && abs(py-ty)<= 1)
				   {
					   backptr = tmpptr;
					   tmpptr = tmpptr->next;
					   s1.push(q1.dequemid(backptr));
					   flag = 1;
				   }
				   else
					   tmpptr = tmpptr->next;
				   if(tmpptr == NULL) 
					   break;

			   }
			    
			   if(flag == 0)
			   {
				   if(s1.isEmpty())
					   break;
				   else
					   s2.push(s1.pop());
				   if(s1.isEmpty())
					   break;
				   
			   }
			} //inner while loop

		   int numberofpts = s2.numberOfccnodes;

            

		   if(numberofpts  > min_root_size)
		   {
		       currentRightSub++;
               //MessageBox::Show(numberofpts+" , right");
               //cvCircle( image, cvPoint(s2.MasterPtr->pt.x,s2.MasterPtr->pt.y), 10, CV_RGB(0,0,255),7);
			   int x=9999;
			   int y=0, yl=9999;
			   while(!s2.isEmpty())
			   {
				  if(x>s2.MasterPtr->pt.x)
			            x = s2.MasterPtr->pt.x;
			       if(y<s2.MasterPtr->pt.y)
			            y = s2.MasterPtr->pt.y; 
			       if(yl>s2.MasterPtr->pt.y)
			            yl = s2.MasterPtr->pt.y;
			            
				   s2.pop();
			   }
			   y = (y+yl)/2;
			   subRootsRight[subRootCountRight].x =  backBone[y].x;
			   subRootsRight[subRootCountRight].y =  backBone[y].y;
			   subRootCountRight++;
		   }
		   else
			   s2.emptyspstack();
	    

	   }//right SUBROOT while loop
	  
	 
	        
	   //cvShowImage("Image",image);
		//	cvWaitKey(0); 
	   
	   
	
	    cvCopy(measuresi,image);
	     
	    //cvShowImage("Image", image);
	   // cvWaitKey(1);
	    char subrootsSavePath[512];
		sprintf(subrootsSavePath, "%s\\%s", outputFolderString,"\\subroots.jpg" );
	    cvSaveImage(subrootsSavePath,image);
		
	
	   // MessageBox::Show("Left SUbroots = " + subRootCountLeft + "       Right Subroots = " + subRootCountRight);
	
	
	      
	
	
	    //filling the file
	    ofstream outfile;
		char subrootsSavePath2[512];
		sprintf(subrootsSavePath2, "%s\\rootMask.txt", outputFolderString );
	    outfile.open(subrootsSavePath2);
		

	    for(int row=0; row<stepCounter; row++)
	    {
	        for(int col=0; col<=40; col++)
	        {
	            outfile<<sRsArray[row][col]<<", ";
	        }
	        outfile<<endl;
    	
	    }
	    outfile.close();
    	
    	
	
	
	//DRAGON
   for(int i=0; i<subRootCountLeft; i++)
        cvLine( output, subRootsLeft[i], cvPoint( subRootsLeft[i].x-10,  subRootsLeft[i].y),  CV_RGB(255,0,0),2);
    
   for(int i=0; i<subRootCountRight; i++)
        cvLine( output, subRootsRight[i], cvPoint( subRootsRight[i].x+10,  subRootsRight[i].y),  CV_RGB(0,0,255),2);
        
	cvShowImage("Image", output);
	

    subRootTxt<<"\t"<<currentLeftSub<<","<<currentRightSub;	
	

	}// if findLaterals

	//imgTxt(image, myspf("Press Esc when ready..."), cvPoint(5, 20), CV_RGB(255,0,0) );
	//cvShowImage("Image", image);
	/*
	cvWaitKey();
	while (code!=32) {  //wait for space
	
		
	code = cvWaitKey();
	}*/

	//imgTxt(image, myspf("Please wait..."), cvPoint(5, 40), CV_RGB(255,0,0) );
	//cvShowImage("Image", image);
	cvWaitKey(1);
}


void CMainCode::doGraphTraversal(CvGraph* g, System::Windows::Forms::Form^ mainWindow) {
	// <<<<<<<<<<<<<<<<<<<<<<<<<<< DIJKSTRA GRAPH TRAVERSAL (shortest distance) >>>>>>>>>>>>>>>>>>>>>>>>>>>>
	// ... after pruning low prob nodes from graph

	//RootTrace2::Form1^ mainWindow_RT = ((RootTrace2::Form1^)mainWindow);

	
	// set all vertices to have shortest dist = inifinity...
	int vertex_count = cvGraphGetVtxCount(g);
	if (vertex_count==0) MessageBox::Show( "cvGraphGetVtxCount(g) == 0 ", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
	vector<int> S, Q;		// S = all vertices (indexes) for which a min dist has been found
							// Q = all vertices (indexes) not in S

	MyVertex* end = NULL;

	System::GC::Collect();//force collection of existing arrays? Trying to find cause of memory errors...
	array<double>^ cum_prob = gcnew array< double >(max_number_steps_to_trace); // to store stats at each graph 'level'
	array<double>^ mean_prob= gcnew array< double >(max_number_steps_to_trace); 
	array<double>^ st_dev   = gcnew array< double >(max_number_steps_to_trace); 
	array<double>^ cum_sq_devs = gcnew array< double >(max_number_steps_to_trace); 
	
	
	//initialisation
	// We calc stats for each level  of the graph/step along the root
	// the 'root' point is level -1 and so not counted here
	// first NSAMPLES (first step) are level 0
	for (int loop=0; loop<max_number_steps_to_trace; loop++) {
		cum_prob[loop]=0.0;
		mean_prob[loop]=0.0;
		st_dev[loop]=0.0;
		cum_sq_devs[loop]=0.0;

	}

	int num_levels=0;
	for (int vv = 0; vv<vertex_count; vv++) {
		MyVertex* v = (MyVertex*)cvGetGraphVtx(g, vv);
		if (v==NULL) MessageBox::Show("ERROR v==NULL");
		v->dij_dist = infinity;
		v->dij_dist_metric = infinity;
		v->dij_prob = -1;
		v->dij_prev = -1; //undefined
		if (v->step>=0) {// ie not root node
			cum_prob[v->step]+=v->prob;
			if (v->step+1>num_levels) num_levels = v->step+1;
		}

	}

	//MyVertex* v = (MyVertex*)cvGetGraphVtx(g, 2);
	//MessageBox::Show(""+v->dij_dist);  // does == 99999999...

	for (int loop=0; loop<num_levels; loop++) {
	  mean_prob[loop] = cum_prob[loop]/NSAMPLES;//per level
	}

	//calc st dev of probs
	//cum_sq_devs = 0;
	for (int vv = 0; vv<vertex_count; vv++) {
		MyVertex* v = (MyVertex*)cvGetGraphVtx(g, vv);
		if (v->step>=0) //ie not root node
			cum_sq_devs[v->step]+= (mean_prob[v->step] - v->prob)*(mean_prob[v->step] - v->prob);
	}

	for (int loop=0; loop<num_levels; loop++) {
	  st_dev[loop] = sqrt(cum_sq_devs[loop] / (NSAMPLES-1));
	  //mainWindowText(mainWindow, " Level "+loop+" mean " +mean_prob[loop]+", sd "+st_dev[loop]);
	}

	
	

	// TO DO remove vertices with probs lower than x sd?????  Be careful to remove also from any lists already created...
	// tried, can remove all nodes at time t and hence breaks...
	// OR only update min distances if prob threshold criteria is also met?? - tried, unpredictable behaviour
	
	
	int num_end_nodes_removed=0;
	int found_end_test=0;
	int end_level;
	vector<int> remove_list;
	//double prob_cutoff = 0;//mean_prob - (num_st_devs*st_dev);  // MAKE = 0 to keep all vertices
	for (int vv = 0; vv<vertex_count; vv++) {
		MyVertex* v = (MyVertex*)cvGetGraphVtx(g, vv);
		if (v->end) {
			found_end_test ++;
			//mainWindowText(mainWindow, "End node encountered at Level "+ v->step);
			end_level = v->step;
		}

		if (vv!=rootIndex && v->prob < /*prob_cutoff*/ mean_prob[v->step]-(num_st_devs*st_dev[v->step]) ) {
			// remove from graph, using local level stats. Should only move a few each level
			// not all! Thats the theory anyway.
			
			remove_list.push_back(vv);

			// show pruned samples
			if (drawsamples && showPrunedSamples) {
				UCHAR red, green , blue;
				MyGetPixel(output, v->x, output->height - v->y, &red, &green, &blue);
				MyPutPixel(output, v->x, output->height - v->y, 0, 110, blue); //Draw pruned pixel on graph
				
			}
			
			//mainWindowText(mainWindow, "Removing node from level "+v->step);
			//if (v->end) mainWindowText(mainWindow, "Pruning an end node! At Level "+ v->step);
			if (v->end) num_end_nodes_removed++;
			
		}
	}

	

	if (num_end_nodes_removed==NSAMPLES) MessageBox::Show("WARNING num_end_nodes_removed==NSAMPLES \n Mean = "+mean_prob[end_level]+", sd "+st_dev[end_level]);


	// find out how many end nodes we have
	if (found_end_test==0) 
		MessageBox::Show("WARNING No vertices with End flag!!");
	//else
		//mainWindowText(mainWindow, found_end_test+" end vertices found");

	//actually remove
	for (int r=remove_list.size()-1; r>=0; r--) //remove highest numbers first, in case of shuffling effects
		cvGraphRemoveVtx(g, remove_list[r]);

	// DONT UPDATE VERTEX COUNT - need to still cover the higher vertex numbers!!!!!!!!!!!!!!!!!!!
	//mainWindowText(mainWindow, "Vertex count before pruning "+vertex_count);
	//vertex_count = cvGraphGetVtxCount(g); //**** NEW 24/10/07 count vertices again now that we've removed some
	//mainWindowText(mainWindow, "Vertex count after pruning "+vertex_count);

	// now build set Q after low prob vertices have been removed
	// vertex_count = cvGraphGetVtxCount(g);
	for (int vv = 0; vv<vertex_count; vv++) {
		MyVertex* v = (MyVertex*)cvGetGraphVtx(g, vv);
		
		if (v!=NULL) { // if NULL, it has been removed in the above step.
			Q.push_back(cvGraphVtxIdx( g,v));
			//cout<<"Adding vertex "<<vv<<endl;
			//if (v->end) MessageBox::Show("End node pushed back into Q!");
			if (v->end==true && end==NULL) { end = (MyVertex*)cvGetGraphVtx(g, vv); // does this do anything now?
											 //MessageBox::Show("Found end. x, y = "+end->x+", "+end->y);
											}
		}
	}

	//'end' here is corrupt, hence crash? Exists, but impossible x,y. vertex_count = 25001. 

	//root vertex has shortest distance 0
	MyVertex* rootv = (MyVertex*)cvGetGraphVtx( g, rootIndex);
	rootv->dij_dist = 0;
	rootv->dij_prob = 1;
	rootv->dij_dist_metric=0;
    //cout <<" Index vertex id = " << rootIndex << endl;

	
	//startTimer();
	// THE ACTUAL ALGORITHM
	bool endfound = false;
	bool nullexit = false;
	int dcount=0;
	while (!Q.empty() && !endfound)
	{
		//cout <<"Q size = "<<Q.size()<<endl;
		dcount++;
		
		MyVertex* u;
		//Get vertex with min dist in Q, u.
		double min_dist = infinity;			//using distance alone
		double min_dist_metric = infinity;	//using dij_distance_metric
		int best_idx = -1;
		int total_inf = 0;
		for(int vv=0; vv<Q.size(); vv++) {
			MyVertex* vv_vtx = (MyVertex*)cvGetGraphVtx(g, Q[vv]);
			if (vv_vtx->dij_dist==infinity)total_inf++;  //dragon
			if ( vv_vtx->dij_dist_metric <= min_dist_metric) {  // 24/10/07 changed this from < to <=, so if only dis is huge, still pick it
				//if(dragon) MessageBox::Show("vtx "+vv+" dist = infinity");
				min_dist = vv_vtx->dij_dist;  // Root first time round is only one ==0. 
				min_dist_metric = vv_vtx->dij_dist_metric;//NEW 16 Feb 09. Uses dist metric NOT spatial distance
				best_idx = Q[vv];		
			}
		}
		//if (dragon) MessageBox::Show("total_inf="+total_inf);

		if (best_idx==-1) { 
			if (Q.size()==1) {
				MyVertex* vv_vtx = (MyVertex*)cvGetGraphVtx(g, Q[0]);//must be Q[0] as only 1
				mainWindowText(mainWindow, 	"Q[0]->dij_dist = "+vv_vtx->dij_dist);
			}
			MessageBox::Show("WARNING best_idx==-1. Q.size() = "+Q.size());

		}

		//cout << "Index "<<best_idx<<" selected as 'u'"<<endl;
		u = (MyVertex*)cvGetGraphVtx( g, best_idx);
		//int u_idx = cvGraphVtxIdx( g, best_idx);
		
		//MyPutPixel(output, u->x, output->height-u->y, 255, 255, 0);  //debug
		//cvShowImage("Image", output);
		
		// edge set to be first edge leaving vertex u
		if (u==NULL) {
			//cout <<"WARNING u = NULL - going to break!!!"<<endl;
			MessageBox::Show("WARNING u==NULL. Exiting...");
			// THIS MUST MEAN WE'RE AT THE LAST NODE< SO COULD BREAK HERE? endfound=true;?
			//exit(-10);
			endfound=true;
			nullexit=true;
		} else
		if (u->end==true) {
			//dcount=best_idx;
			//MessageBox::Show("End node found - might break here");
			//found an end so stop after this one?
			//if (cvGraphVtxIdx( g, (CvGraphVtx*)(end)) == cvGraphVtxIdx( g, (CvGraphVtx*)(u) )) //removed 24/10/07
			//if (dragon)MessageBox::Show("ENDFOUND!!");
			endfound=true;
			end=u; //new 25/10/07

		} else {
		
		CvGraphEdge * edge;
		//if (u->first==NULL) cout <<"ERROR null edge"<<endl;  // breaks before this prints :-S
		//else
		
		edge = u->first;


		// check dist to each vertex on an exiting edge from u
		CvGraphEdge* tempedge = edge;
		
		//vector<double> probs;
		//if (dragon) MessageBox::Show("stop!!");
		while (tempedge->next[0]!=NULL) {
			
			int v_first_index = cvGraphVtxIdx( g, (CvGraphVtx*)( tempedge->vtx[0]));
			
			if (v_first_index==best_idx) {//make sure we only look at edges LEAVING v_first_index
				MyVertex * v = (MyVertex *)tempedge->vtx[1];
				
				MyEdge* e = (MyEdge*) tempedge;
				double edge_dist =-1;

				double v1x = ((MyVertex*)(tempedge->vtx[0]))->x;
				double v1y = ((MyVertex*)(tempedge->vtx[0]))->y;
				double v2x = ((MyVertex*)(tempedge->vtx[1]))->x;
				double v2y = ((MyVertex*)(tempedge->vtx[1]))->y;

				edge_dist =  sqrt((v1x-v2x)*(v1x-v2x) + (v1y-v2y)*(v1y-v2y)) ;  //Euclidean
				//if (dragon) MessageBox::Show("dcount="+dcount+", edge_dist = "+edge_dist);
				/*
				//rounding, as in ImageJ?? Except this rounds EVERY step, ImageJ rounds at end only
				if (edge_dist-floor(edge_dist) <0.5 )
					edge_dist = floor(edge_dist); //round down
				else
					edge_dist = ceil(edge_dist);//round up
				*/
				
				double cum_prob_dij = u->dij_prob * v->prob;
				double new_dist_to_v_metric = min_dist_metric /*dist to u*/ + edge_dist/(u->prob*v->prob); // NEW added division by prob, 16 feb 09. p129 lab book
				double new_dist_to_v = min_dist /*dist to u*/ + edge_dist; 
				
				if (new_dist_to_v_metric < v->dij_dist_metric/* && cum_prob_dij>dij_prob_threshold*/) {  // UPDATE CRITERIA - prob threshold new
					v->dij_dist = new_dist_to_v;
					//if (dragon) MessageBox::Show("Setting dij_dist");//NEVER CALLED!!****************** FIXED? Increased size of inifity approx.
					v->dij_dist_metric = new_dist_to_v_metric;//so that dij_distance represents true spatial distance still?
					v->dij_prob = cum_prob_dij;
					v->dij_prev = best_idx; //idx to u
					//cout <<"new_dist_to_v, "<<cvGraphVtxIdx(g, edge->vtx[0])<<"->"<<cvGraphVtxIdx(g, edge->vtx[1])<< "= "<<new_dist_to_v<<endl;
				}
				//if (v->end==true&&dragon) MessageBox::Show("Found an end vertex via edges.");  // Normally finds this, but not on error root!!!

			}
			tempedge = tempedge->next[0];
		}// while there is another adjoining edge
		
		} //else not end vertex
		
		for (int e=0; e<Q.size(); e++) {
			if (Q[e]==best_idx){
				//cout <<"Removing vtx "<<Q[e]<<endl;
				Q.erase(Q.begin()+e);
				break;
			}
		}
		
		S.push_back(best_idx);


	}  //while Q not empty
	//stopTimer();



	if (nullexit) MessageBox::Show("WARNING Exited loop with u==NULL");

	//cout <<"Exited Q loop"<<endl;

	MyPath path;
	if (end==NULL) MessageBox::Show("WARNING no end node pointer set.");
	MyVertex * vtx = end;
	path.path.clear();
	int vcount=0;
	while (vtx->dij_prev!=-1) {
		vcount++;
		int idx = cvGraphVtxIdx(g, vtx);
		path.path.insert(path.path.begin(), idx);
		vtx = (MyVertex*)cvGetGraphVtx(g, vtx->dij_prev);
	}

	path.prob = end->dij_prob;
	path.dist = end->dij_dist;

	//if (path.dist==0) MessageBox::Show( "path.dist==0", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
	//if (path.path.size()==0) { 
		//MyPutPixel(output, end->x, output->height-end->y, 255, 0, 255);
		//cvShowImage("Image", output);
		//MessageBox::Show( "path.path.size()==0. end->dij_dist: "+end->dij_dist+"\ndcount = "+dcount+"\nQ.size = "+Q.size()+"\nvcount = "+vcount, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);

	//}

	//cvWaitKey(0);
	
	paths.push_back(path); //just one for now


	// <<<<<<<<<<<<<<<<<<<<<<<<<<< END DIJKSTRA >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


	// OLD
	

	/*
	// <<<<<<<<<<<<<<<<<<<<<<<<<<< DO PROBABILISTIC GRAPH TRACING >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	double prob_thresh = 0;//if 0 this is effectively ignored. Sometimes hangs if too high...
	// UPDATE now altered automagically? no

	int num_paths=0;
	




	while (num_paths<num_paths_to_test) {
	// TRAVERSE GRAPH from 'root' to an 'end point'
	
	vector<int> banned_vtx, path;

	
	bool get_next_vertex = true;

	
	double cum_prob = 1;
	double distance = 0;

	
	CvGraphEdge * edge;
	
	// edge set to be first edge leaving root vertex
	edge = (cvGetGraphVtx( g, rootIndex))->first;
	int v_index = rootIndex;

	int attempts_at_edge=0;





	while (get_next_vertex) { //this loop is a little messy...or at least the name is wrong...
		//cout <<" LOOPING get_next_vertex loop"<<endl;
		attempts_at_edge++;

		bool got_edge = false;
		
		CvGraphEdge* orig_edge = edge;
		int v_origin_index = cvGraphVtxIdx( g, (CvGraphVtx*)( orig_edge->vtx[0]));
		

		//calculate the number of edges leaving the vertex
			int num_edges = 0;
			CvGraphEdge* tempedge = edge;
			vector<double> probs;
			double cumul_total=0;
			while (tempedge->next[0]!=NULL) {
				int v_first_index = cvGraphVtxIdx( g, (CvGraphVtx*)( tempedge->vtx[0]));
				
				if (v_first_index==v_origin_index) {//make sure we only look at edges LEAVING v_first_index
					MyVertex * end_v = (MyVertex *)tempedge->vtx[1];
					probs.push_back(cumul_total); // first one should be 0;
					cumul_total += end_v->prob;
					num_edges++;

				}
				tempedge = tempedge->next[0];
			}



			while (!got_edge){
			//cout <<" LOOPING got_edge loop"<<endl;
	
			//int edge_num = cond.uniform_random()*num_edges;

			//select an edge based on probabilities, using standard cum. prob technique
			double c = cond.uniform_random()*cumul_total;
			int low = 0;
			int high = num_edges;
			int middle;
			while (high>(low+1)) {
				middle = (high+low)/2;
				if (c > probs[middle])
				low = middle;
			else high = middle;
			}

			int edge_num = low; // this is our probabilistically randomly chosen edge

			//cout <<"Picked edge "<<edge_num<<endl;

			
			// next[0] should only refer to exiting edges??
			//for (int e=0; e<edge_num; e++) 
				//edge = edge->next[0];

			// New way to explicitly check for edges leaving vertex, similar to edge counting count above.
			int sel_edge=0;
			
			while (sel_edge<edge_num) {
				int v_first_index = cvGraphVtxIdx( g, (CvGraphVtx*)( edge->vtx[0]));
				
				if (v_first_index==v_origin_index) {//make sure we only count edges LEAVING v_first_index
					sel_edge++;
				}
				edge = edge->next[0];
			}

				 v_index = cvGraphVtxIdx( g, (CvGraphVtx*)( edge->vtx[1]));
				 int v_first_index = cvGraphVtxIdx( g, (CvGraphVtx*)( edge->vtx[0]));
				 
				 //cout << e<<"/"<<edge_num<<": Testing edge that connects " <<v_first_index<<" to "<< v_index<<" (Original edge vert[0] = "<<v_origin_index<<")."<<endl;
				 
				 if (banned_vtx.empty()) 
					 got_edge=true;
				 else
				 for (int v_test = 0; v_test<banned_vtx.size(); v_test++) {
					if (v_index==banned_vtx[v_test]) //vertex is on banned list or n
					{	
						//cout << "Vertex "<<v_index << " is on a banned list!"<<endl;
						got_edge=false; 
						edge = orig_edge;
					}
					else got_edge=true;
				 }
			
		}//while not got edge



		// **** 'edge' now holds the edge we're interested in - now need to test it heuristically? ****
		
		MyVertex* myv = (MyVertex*)(edge->vtx[1]); //vertex the edge points to?
		v_index = cvGraphVtxIdx( g, (CvGraphVtx*)( edge->vtx[1]));

		double new_cum_prob = cum_prob*myv->prob;

		cout <<num_paths<<" Prob = " <<new_cum_prob<<", thresh = "<<prob_thresh<<endl;
		if (new_cum_prob<prob_thresh) {
			//reject this route!
			cout <<"New cum prob < thresh."<<endl;
			banned_vtx.push_back(v_index);
			edge = orig_edge; //get ready to try another edge
			//cout<<"in 'if (new_cum_prob<prob_thresh)': Banned vertices: "<<banned_vtx.size()<<", num edges = "<<num_edges<<endl;
			
			if (banned_vtx.size()==num_edges) {
				cout<<"All edges exhausted at vertex "<<endl;
				banned_vtx.clear();
				cout <<"TEST b v size = "<<banned_vtx.size()<<endl;
				
				//cout << " ERROR Implausible path reached  - set prob thresh higher."<<endl;
				//exit(-10);

				prob_thresh = prob_thresh/2;//drop thresh again as exhausted paths from last node at that thresh
				
				//break;  //path is rubbish so do not add, just try again
						//we could make prob thresh more lenient here?
			}
			
		} 
		else{
			//got edge and passed heuristics?
			attempts_at_edge = 0;
			cum_prob *= myv->prob;
			//if (cum_prob>1) cum_prob=1; //maximum prob = 1?
			//get_next_vertex = false; //loop again, but next vertex this time
			banned_vtx.clear();
			//cout<<"in 'got edge and passed heuristics?': Banned vertices: "<<banned_vtx.size()<<" (should be 0), num edges = "<<num_edges<<endl;

			//cout <<"Adding vertex " << v_index << " to path..."<<endl;
			path.push_back(v_index);
			

			double edge_dist =-1;

			double v1x = ((MyVertex*)(edge->vtx[0]))->x;
			double v1y = ((MyVertex*)(edge->vtx[0]))->y;
			double v2x = ((MyVertex*)(edge->vtx[1]))->x;
			double v2y = ((MyVertex*)(edge->vtx[1]))->y;

			edge_dist =  sqrt((v1x-v2x)*(v1x-v2x) + (v1y-v2y)*(v1y-v2y)) ;  //Euclidean

			//if (abs(v1x-v2x)==1 && abs(v1y-v2y)==1) edge_dist=1;  //HACKY

			

			distance += edge_dist;

			if (myv->end==true) {
				cout <<"Graph "<<num_paths<<" end found. Prob : "<<cum_prob<<endl;
				
				// ********** remove this line to remove updating threshold!
				//prob_thresh = cum_prob;// make sure the next graph found is of greater prob?
				
				
				num_paths++;
				//cout << "num_paths increased to "<<num_paths<<endl;
				MyPath myp;
				myp.prob = cum_prob;
				myp.dist = distance;
				myp.path.swap(path);
				path.clear();

				paths.push_back(myp);
				break; // break out of get_next_vertex loop, get another path
			}

			edge = myv->first;
			if (edge->vtx[0]!=myv) {
				cout <<"ERROR !!!"<<endl; //check edge is outgoing..
				exit(-5);
			}
			//else cout<<"Addresses: "<<edge->vtx[0]<<", "<<myv<<endl;
				
		}


	} //while get_next_vertex


	

	}// end paths cycle loop

	// <<<<<<<<<<<<<<<<<<<<<<<<< END GRAPH TRAVERSAL >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	*/

}





IplImage* CMainCode::getMeasuresi(void)
{

	return measuresi;
}

void CMainCode::setupCalib(System::String^ s, System::String^ s2, System::Windows::Forms::Form^ mainWindow) {
	//there are 2 calibration methods... this is the second one
	RootTrace2::Form1^ mainWindow_RT = ((RootTrace2::Form1^)mainWindow);
	//double mm = System::Convert::ToDouble(s);
	calib_mmPerPix = System::Convert::ToDouble(s);//mm /fixedDistance;
	calib_minPerImage =  System::Convert::ToDouble(s2);
	
	
	char c[50];
	sprintf(c, "(%.3f mm/pix)", calib_mmPerPix);
	System::String^ label = %System::String(c);
	mainWindow_RT->setCalibLabel(label);

	char c2[50];
	sprintf(c2, "(%.3f min/image)", calib_minPerImage);
	label = %System::String(c2);
	mainWindow_RT->setCalibLabel2(label);

}

void CMainCode::cropImages(CvRect rect) { 
	/*originali = cvCloneImage(image);
	previousi = cvCloneImage(image);
	output = cvCloneImage(image);
	tempimage = cvCloneImage(image);
	measuresi = cvCloneImage(originali);*/
	

	IplImage* image2     = cvCreateImage(cvSize(rect.width, rect.height), 8, 3);
	IplImage* originali2 = cvCreateImage(cvSize(rect.width, rect.height), 8, 3);
	IplImage* previousi2 = cvCreateImage(cvSize(rect.width, rect.height), 8, 3);
	IplImage* output2    = cvCreateImage(cvSize(rect.width, rect.height), 8, 3);
	IplImage* tempimage2 = cvCreateImage(cvSize(rect.width, rect.height), 8, 3);
	IplImage* measuresi2 = cvCreateImage(cvSize(rect.width, rect.height), 8, 3);
	IplImage* measuresiTaboo2 = cvCreateImage(cvSize(rect.width, rect.height), 8, 3);
	
	
	cvSetImageROI(image, rect);
	cvSetImageROI(originali, rect);
	cvSetImageROI(previousi, rect);
	cvSetImageROI(output, rect);
	cvSetImageROI(tempimage, rect);
	cvSetImageROI(measuresi, rect);
	cvSetImageROI(measuresiTaboo, rect);


	cvCopy(image, image2);
	cvCopy(originali, originali2);
	cvCopy(previousi, previousi2);
	cvCopy(output, output2);
	cvCopy(tempimage, tempimage2);
	cvCopy(measuresi, measuresi2);
	cvCopy(measuresiTaboo, measuresiTaboo2);
	
	
	cvReleaseImage(&image);
	cvReleaseImage(&originali);
	cvReleaseImage(&previousi);
	cvReleaseImage(&output);
	cvReleaseImage(&tempimage);
	cvReleaseImage(&measuresi);
	cvReleaseImage(&measuresiTaboo);
	

	image = image2;
	originali = originali2;
	previousi = previousi2;
	output    = output2;
	tempimage = tempimage2;
	measuresi = measuresi2;
	measuresiTaboo = measuresiTaboo2;

	if (last_image->width!=rect.width && last_image->height!=rect.height) { //haven't cropped already so do now (only needs doing once)
		IplImage* last_image2 = cvCreateImage(cvSize(rect.width, rect.height), 8, 3);
		cvSetImageROI(last_image, rect);
		cvCopy(last_image, last_image2);
		cvReleaseImage(&last_image);
		last_image = last_image2;
	}
	
	fitWindowToImage();

}

double CMainCode::evaluate_gaussian(double val, double sigma)
{
  /* This private definition is used for portability */
  //static const double Condense_PI = 3.14159265358979323846;
  static const double sqrt2xConPi = 2.50662827463100050241;

  /* OLD
  return 1.0/(sqrt2xConPi * sigma) *
    exp(-0.5 * (val*val / (sigma*sigma)));
 */
  double output = 1.0/(sqrt2xConPi * sigma) *
    exp(-0.5 * (val*val / (sigma*sigma)));
  
  // NEW to make max prob = 1.  Putting in causes more hypotheses to be explored longer
  //if (output>1) output=1;

  return output;
}
CvPoint CMainCode::lockToRoot(int clickx, int clicky)
{
		// starting at (clickx,clicky) search x raster within range to look for most root like structure. Used to work on originali, now measuresi
	int xrange = 30;
	UCHAR r1, g1, b1, r2, g2, b2, r3, g3, b3, r4, g4, b4, r5, g5, b5;
	int max = 0;
	int bestx = 0;
	int measure = -1;
	for (int searchx=max(clickx-xrange, 1); searchx<min(clickx+xrange, measuresi->width-1); searchx++) {
		MyGetPixel(measuresi, searchx-1, measuresi->height-clicky, &r1, &g1, &b1);//simple filter
		MyGetPixel(measuresi, searchx,   measuresi->height-clicky, &r2, &g2, &b2);
		MyGetPixel(measuresi, searchx+1, measuresi->height-clicky, &r3, &g3, &b3);
		//MyGetPixel(originali, searchx-5, originali->height-clicky, &r4, &g4, &b4);
		//MyGetPixel(originali, searchx+5, originali->height-clicky, &r5, &g5, &b5);
		measure = r1*g1*b1 + r2*g2*b2 + r3*g3*b3;
		if (r1*g1*b1 + r2*g2*b2 + r3*g3*b3/*r1+r2*2+r3-r4-r5*/ > max) {
			max = r1*g1*b1 + r2*g2*b2 + r3*g3*b3 /*r1+r2*2+r3-r4-r5*/;
			bestx = searchx;  
		}
		
		//show
		/*MyPutPixel(image, searchx,   originali->height-clicky, (double)measure/(255*255*255+255*255*255+255*255*255)*255, (double)measure/(255*255*255+255*255*255+255*255*255)*255, (double)measure/(255*255*255+255*255*255+255*255*255)*255);
		cvNamedWindow("test", 2);
		cvShowImage("test", image);
		cvWaitKey(0);*/
	}
	
	CvPoint return_point = cvPoint(bestx, clicky);

	return return_point;
}

void CMainCode::prepareCrop(void)
{
	cvCopy(image, tempimage);
	imgTxt(tempimage, myspf("CROP"), cvPoint(5, 45), CV_RGB(0,255,0) );
	cvShowImage("Image", tempimage);
	doCrop = true;
}

void CMainCode::toggleGoButtonPressed(void)
{
	goButtonPressed = !goButtonPressed;
}

void CMainCode::setOutputFolderString(String^ folder)
{
	sprintf(outputFolderString, "%s", (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(folder));
}

void CMainCode::setWorkingFolder(String^ folder)
{
	sprintf(workingFolder, "%s", (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(folder));
}

/*void CMainCode::startTimer(void)
{
	timer_val = timeGetTime();
}*/ //K_I


/*void CMainCode::stopTimer(void)
{

	DWORD new_timer_val;
	new_timer_val = timeGetTime();

	int time_msecs = abs((int)(new_timer_val-timer_val));
	MessageBox::Show(""+time_msecs+" ms.");
}*/ //K_I

// //set up values for some parameters
void CMainCode::setupParameters(void)
{
	
	showLastImageToggle = false;

	removeBGflag = false;
	
	goButtonPressed = false;
	
	cropx = 50;//200
	cropy = 50;//200
	doFixedDistMeasure = false;
	fixedPoint1.x = -1;
	fixedPoint2.x = -1;
	fixedDistance = -1;
	calib_mmPerPix   =1.0;//default
	calib_minPerImage=5.0;//default

	crop1.x = -1;
	crop2.x = -1;
	doCrop = false;
	croprect.width=-1;
	
	multiple_BG_stats = false;
	BG_region_counter = 0;

	bg_mask=NULL;
	originali=NULL;
	recalcBGStats = true;

	liveZoomImg_width = 200;
	liveZoomImg_height= 200;

	// Colour maps for angles
	color_map[0] = cvScalar(0, 0, 255);		// high curvature
	color_map[1] = cvScalar(30, 70, 240);
	color_map[2] = cvScalar(20, 180, 250);
	color_map[3] = cvScalar(65, 180, 132);
	color_map[4] = cvScalar(169, 170, 0);
	color_map[5] = cvScalar(170, 100, 0);
	color_map[6] = cvScalar(130, 50, 40);	// low curvature

	color_bins[0] = 60;
	color_bins[1] = 50;
	color_bins[2] = 40;
	color_bins[3] = 30;
	color_bins[4] = 20;
	color_bins[5] = 10;
	color_bins[6] = 0;

	bg_rect_click.x=-1;
	bg_rect_click.y=-1;

	for (int j=0; j<256; j++) {
		bg_mean[j][0] = 0.0;
		bg_mean[j][1] = 0.0;
		bg_mean[j][2] = 0.0;
		bg_sd[j][0] = 1.0;
		bg_sd[j][1] = 1.0;
		bg_sd[j][2] = 1.0;
	}

	calib_dist = -1;

	num_calib_traces=-1;

	calibration_mode = false;
	calib_line_points=0;

	lastVertices = NULL;

	oldpoint.x = -1;

	rule2_count = 0;
	rule_2_polarity = true;


}

int CMainCode::checkKeyPresses(int code)
{
	if (code==27) {
			quitnow = true;
		} //ESC exits	
		else
		/*if (code==117) {//u = undo. Go back to previous image
			cvCopy(previousi, image);
			cvShowImage("Image", image);
			cvCopy(image, output);
			rootnum--;

			//repaint crop window
			cvSetImageROI(image, cvRect( crop_orig_x, crop_orig_y, cropx, cropy) );		
			cvCopy(image, crop);
			cvNamedWindow("Zoom", 2);
			cvShowImage("Zoom", crop);
			cvResetImageROI(image);
		} 
		else*/
		/*if (code==92) {// '\', switch top to bottom or left to right.
			new_state = cond.toggle_state();
			
			cvCopy(image, tempimage);

			if (new_state==0) {
				cout <<"Top to bottom tracing selected."<<endl;
				imgTxt(tempimage, myspf("Top to bottom tracing selected."), cvPoint(5, 40), CV_RGB(255,0,0) );
			}
			if (new_state==1) {
				cout <<"Left to right tracing selected."<<endl;
				imgTxt(tempimage, myspf("Left to right tracing selected."), cvPoint(5, 40), CV_RGB(255,0,0) );
			}
			cvShowImage("Image", tempimage);
			

		} 
		else*/
		if (code=='r') {
			if (i_copy!=NULL && BG_region_counter>=1) {
				BG_region_counter--;
				cvCopy(i_copy, tempimage);
				imgTxt(tempimage, myspf("Continue BG region %i", BG_region_counter), cvPoint(5, 40), CV_RGB(0,255,0) );
				cvShowImage("Image", tempimage);
				
			}

		}else
		if (code=='u') {
			cvCopy(i_copy, tempimage);
			imgTxt(tempimage, myspf("REMOVE BG region", BG_region_counter), cvPoint(5, 40), CV_RGB(255,0,0) );
			cvShowImage("Image", tempimage);
			removeBGflag = true;

		}else
		if (code=='l') {  //last image
			showLastImageToggle = !showLastImageToggle;
			if (showLastImageToggle)
				cvShowImage("Image", last_image);
			else
				cvShowImage("Image", image);

		}else
		if (code==113) { //q = calibrate
			cvCopy(image, tempimage);
			imgTxt(tempimage, myspf("CALIBRATION MODE. Left click first line point..."), cvPoint(5, 40), CV_RGB(0,255,0) );
			cvShowImage("Image", tempimage);
			calibration_mode = true;
		} 
		else
		/*if (code==101) { //e = enhance contrast

			enhance(mainWindow);


		}
		else*/
		if (code==99) { //c = calibrate to fixed distance;
			cvCopy(image, tempimage);
			imgTxt(tempimage, myspf("CALIBRATE FROM RULER"), cvPoint(5, 45), CV_RGB(0,255,0) );
			cvShowImage("Image", tempimage);
			doFixedDistMeasure = true;


		}
		else
		/*if (code==102) { //f = gaussian Filter

			smooth(mainWindow);


		}
		else*/
		if (code==109) { //m = show Measures image
			

			cvShowImage("Image", measuresi);


		}

		else
		if (code==111) { //o = show Original image
			

			cvShowImage("Image", originali);


		}

		/*else
			if (code == 98 ) { // b = calc bg stats from selected areas
			multiple_BG_stats = getBGStats(bg_mask, measuresi);
		}*/
		else
			if (code == 120 ) { // x = crop
				/*cvCopy(image, tempimage);
				imgTxt(tempimage, myspf("CROP"), cvPoint(5, 45), CV_RGB(0,255,0) );
				cvShowImage("Image", tempimage);
				doCrop = true;*/
				prepareCrop();
		}
			
		else
			if (code == 's' ) { // s = snap to root on right click
			snapToRoot = !snapToRoot;
		}
			else
			if (code == 'b' ) { // b = barcode
			findBarcodeInImage();
		}
		else 
			if (code== 8) { //backspace, undo last start point
				if (num_roots>0) {
					num_roots--;
					cvCircle(i_copy, start_ps[num_roots], 4, cvScalar(0,0, 0), -1);
					cvShowImage("Image", i_copy);
				}
			
			}
			





	
	return 0;
}

void CMainCode::setDoGravitropic(bool in)
{
	doGravitropic = in;
}

void CMainCode::setDoTipAngles(bool in)
{
	doTipAngles = in;
}

void CMainCode::setImageTranform(bool flipin, bool rotCWin, bool rotCCWin)
{
	flip_hor = flipin;
	rotate_cw = rotCWin;
	rotate_ccw = rotCCWin;

}

CMainCode::CMainCode() 
{
	//MessageBox::Show("Constructed");
	//This is Coinstructed right at the start, so can init here
	doGravitropic=false;
	doTipAngles=false;
	rotate_cw=false;
	rotate_ccw=false;
	flip_hor=false;
	tipPointsNumToFit = 2;
	evolveTipPoints = false;
	enableEnhancing = false;
	findLaterals = false;
	invert = false;

	m_bilat_filter = false;
	m_med_filter = false;
	m_gaus_filter = false;
	m_suppressHor = false;
	m_bLocaLMaxFilter = false;
	m_bblowHighlights = false;
	m_blowHighlights = false;
	m_bgRemoval = false;

	savedStartPoints = false;


}

/*__int64*/ int CMainCode::findBarcodeInImage(void) //K_I
{
	MessageBox::Show("Find barcode disabled for now.");
	
	//NEED TO UNCOMMENT barcode.h
	//__int64 barcode = CBarcode::findBarcodeInImage(image);
	//MessageBox::Show("INT64 BARCODE: "+barcode+"\n(NOTE looses leading 0's when converted to int)");
	//NOTE looses leading 0's when converted to int

	return /*barcode*/-1;
}

int CMainCode::getAngleQuardrant(int x1, int y1, int x2, int y2)
{
	if(x2>=x1 && y2>=y1)
		return 1; //first quardrant
	else if(x2 < x1 && y2 >=y1)
		return 2; //second quardrant
	else if(x2<x1 && y2<y1)
		return 3; //third quardrant
	else if(x2>=x1 && y2<y1)
		return 4; //forth quardrant

}


void CMainCode::drawTabooPoints()
{
	for(int i=0; i<tabooPointCounter; i++)
		cvCircle(image,cvPoint(tabooPoints[i].x,tabooPoints[i].y),8,CV_RGB(100,100,100),14);    

}

void CMainCode::setTabooPointsEnabled(bool val)
{
	tabooPointsEnabled = val;
}

void CMainCode::fitWindowToImage(void)
{
	cvResizeWindow( "Image", image->width, image->height); 
	cvShowImage("Image", image);
}


void CMainCode::setTipPointsNumToFit(int in)
{
	tipPointsNumToFit = in;
}


void CMainCode::setEvolveTipPoints(bool in)
{
	evolveTipPoints = in;
}


void CMainCode::interactiveEnhance(bool bilat_filter, bool med_filter, bool gaus_filter, bool suppressHor, bool bLocaLMaxFilter, bool bblowHighlights, int blowHighlights, bool bgRemoval)
{
	UCHAR r, g, b, r2, g2, b2, r3, g3, b3;

	m_bilat_filter = bilat_filter;
	m_med_filter =  med_filter;
	m_gaus_filter = gaus_filter;
	m_suppressHor =  suppressHor;
	m_bLocaLMaxFilter = bLocaLMaxFilter;
	m_bblowHighlights = bblowHighlights;
	m_blowHighlights = blowHighlights;
	m_bgRemoval = bgRemoval;

	IplImage* suppressi;
	/*if (suppressHor) {
		suppressi= cvCloneImage(originali);
		cvZero(suppressi);
	}*/

	if (enableEnhancing) {

		
		cvCopyImage(originali, measuresi);

		//BG removal;
		if (bgRemoval){
			//MessageBox::Show("bgRemoval flag detected.");
			IplImage* tempout = cvCloneImage(measuresi); //out of memory here
			IplImage* out = cvCloneImage(measuresi);
			cvSmooth(measuresi, tempout, CV_GAUSSIAN, 0, 0, 20);
			cvSub(measuresi, tempout, out);
			cvCopyImage(out, measuresi);
			// invert the image
			/*UCHAR r, g, b;
			for (int i=0;i<measuresi->width;i++)
				for (int j=0; j<measuresi->height; j++){
					MyGetPixel(measuresi, i, j, &r, &g, &b);
					MyPutPixel(measuresi, i, j, 255-r, 255-g, 255-b);
				}*/
			cvReleaseImage(&tempout);
			cvReleaseImage(&out);
		}
		
		//Filtering
		
		if (gaus_filter){
			IplImage* tempout = cvCloneImage(measuresi);
			cvSmooth(measuresi, tempout, CV_GAUSSIAN, 3, 3);
			cvCopyImage(tempout, measuresi);
			cvReleaseImage(&tempout);
		}
		if (med_filter)	{
			IplImage* tempout = cvCloneImage(measuresi);
			cvSmooth(measuresi, tempout, CV_MEDIAN, 5, 5);
			cvCopyImage(tempout, measuresi);
			cvReleaseImage(&tempout);
		}
		if (bilat_filter)	{
			IplImage* tempout = cvCloneImage(measuresi);
			cvSmooth(measuresi, tempout, CV_BILATERAL, 50, 3);
			cvCopyImage(tempout, measuresi);
			cvReleaseImage(&tempout);
		}
		
			
		if (bblowHighlights||suppressHor)
		for (int x=0; x<originali->width; x++)
			for (int y=0; y<originali->height; y++) {

				MyGetPixel(measuresi, x, y, &r, &g, &b);


				if (bblowHighlights) {
					if (r>blowHighlights) r2=min((int)((double)r*3),255); else r2=r;
					MyPutPixel(measuresi, x, y, r2, r2, r2);
					}


				/*if (bLocaLMaxFilter){
				for (int i=x-2; i<x+2; i++)
					for (int j=y-2; j<y+2; j++) {
						r3=0;
						if (i>2 && i<measuresi->width-2 && j>2 && j<measuresi->height-2){
							MyGetPixel(measuresi, i, j, &r2, &g2, &b2);
							if (r2>= r) r3=r2; 
						}
						MyPutPixel(measuresi, x, y, r3, r3, r3);
						
					}
			       }*/

				if (suppressHor) {
					
						MyGetPixel(measuresi, x, y-3, &r2, &g2, &b2);
						MyGetPixel(measuresi, x, y+3, &r3, &g3, &b3);
						if (r3<(double)r*0.6 && r2 <(double)r*0.6) {
							MyPutPixel(measuresi, x, y, 0, 0, 0);

						}

					
				}

				//todo
				/*if (suppressi) {//TODO add connected components and ANDing code?

				MyPutPixel(measuresi, x, y, r, r, r);
				cvNamedWindow("suppressi", 2);
				cvShowImage("suppressi", suppressi);
				cvWaitKey(0);

				cvReleaseImage(&suppressi);
				}*/


		}
			

	} else cvCopyImage(originali, measuresi);

	cvCopyImage(measuresi, image);
	cvCopyImage(measuresi, measuresiTaboo);
	

	cvShowImage("Image", measuresi);
	//cvWaitKey(2000);
}


void CMainCode::setEnableEnhancing(bool in)
{
	enableEnhancing = in;
}


void CMainCode::setIdealRGBinCond(int brightness)
{
	idealR=brightness;
	idealG=brightness;
	idealB=brightness;
	cond.setIdealRGB(brightness, brightness, brightness);
}


void CMainCode::setCountLaterals(bool m_countLaterals)
{
	findLaterals = m_countLaterals;
}


void CMainCode::onClose(void)
{
	cvDestroyAllWindows();
	
	if (output!=NULL) cvReleaseImage(&output); 
	if (image!=NULL) cvReleaseImage(&image); 
	exit(0);

	
}


void CMainCode::saveComments()
{
	char commentPath[512];
	sprintf(commentPath, "%s\\metadata.txt", outputFolderString);
	ofstream output(commentPath);

	
		output<<"RootTrace Meta Data "<<endl<<endl;
		output<<"# Comments"<<endl;
		String^ comments = ((RootTrace2::Form1^)getMainWindow())->getCommentString();
		if (comments!=nullptr) {
			char* st = (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(comments);
			output<<st;
			output<<endl;
			output<<"# End of comments"<<endl<<endl;

			output<<"# Input files"<<endl;
			for (int i=0; i<m_filenames->Length;i++) {
				char* st2 = (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(m_filenames[i]);
				output<<st2<<endl;
			}
			output<<"# End of input files"<<endl;
			
		}
		output.close();
		
	
}


void CMainCode::setInvert(bool flag)
{
	invert = flag;
}
