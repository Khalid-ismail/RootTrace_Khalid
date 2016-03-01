#pragma once
#include <vector>
#include <stdio.h>

#include "../Header_Files/Condensation.h"
#include "../Header_Files/stdafx.h"
//#include "Console.cpp"
//#include "Form1.h";
//#include <vcclr.h> //K_I
//#include "statswriter.h"




using namespace std;
//using namespace RootTrace2;

const int max_roots = 70;



struct MyVertex : CvGraphVtx
{
	double prob;
	double x, y;
	bool end;
	int state;
	double dij_dist;  //shortest dist so far
	int dij_prev;	  //previous vertex
	double dij_prob;  // prob associated with dij (shortest) route
	double dij_dist_metric; //distance metric, weighted for prob
	double prob_non_normalized;
	int step; // iteration on which it was added to the graph. AKA level?
	double angle; //curvature angle at this point
	double curvature;  //using new metric
};
struct MyEdge : CvGraphEdge
{
	double dist;
};


struct MyPath
{
	vector<int> path;
	double prob;
	double dist;
};

int _vscprintf (const char * format, va_list pargs);
float max_float (float x, float y);
int max_int (int x, int y);

class CMainCode {

	public:
	IplImage* image, *originali, *previousi, *crop, *tempimage, *measuresi, *measuresiTaboo, *smoothedi, *output, *debuggraph, *preview_lengthsi, *i_copy, *i_copy2, *last_image, *base_image;
	CvPoint oldpoint;
	int clickx, clicky, cropx, cropy, crop_orig_x, crop_orig_y;
	CvSeq * lastVertices;
	CvPoint start_ps[60];
	int num_roots;

	 //  Asad
	IplImage* tabooMask;   
	CvPoint tabooPoints[1000];
	int tabooPointCounter;
	bool drawSubrootsToggle;
	//bool tabooPointsEnabled;
	int meanColour[1000][3];
	int motionMixture;
	// -Asad

	CCondensation cond;
	UCHAR idealR, idealG, idealB;

	CMainCode();
	
	void on_mouse( int event, int x, int y, int flags);
	void init(char filenames, UCHAR idealr, UCHAR idealg, UCHAR idealb, UCHAR tb_lr, double proSigmaX, double proSigmaY/*, double motionMixture*//*, System::Windows::Forms::Form^ mainWindow*/);//Motion mixture UNUSED locally??
	static char* myspf(char* strToFormat, ...);				// jon's version of sprintf =)
	static void imgTxt(IplImage *img, char *message, CvPoint pos, CvScalar colour);
	static void imgTxt(IplImage *img, char *message, CvPoint pos, CvScalar colour, float fontsize);

	void MyPutPixel(IplImage *m_image, int x, int y, UCHAR b, UCHAR g, UCHAR r);
	void MyGetPixel(IplImage *m_image, int x, int y, UCHAR *b, UCHAR *g, UCHAR *r); 
	CvPoint drawSample(IplImage* copy, int t, UCHAR r, UCHAR g, UCHAR b, bool drawsamples, int iters, int x_offset=20, int y_offset=20);
	void enhance(System::Windows::Forms::Form^ mainWindow, IplImage* i);
	void smooth(System::Windows::Forms::Form^ mainWindow, IplImage* i);
	void saveStartPoints(System::String^ filename);
	void loadStartPoints(System::String^ filename);
	IplImage* getMeasuresi(void);
	void setBGMask(System::String^ filename);

	void saveFileList(array<System::String^>^ filenames);
	array<System::String^>^ loadFileLists();
	void saveBGMask(System::String^ filename); 
	void setupCalib(System::String^ s, System::String^ s2, System::Windows::Forms::Form^ mainWindow);
	int getAngleQuardrant(int x1,int y1, int x2, int y2);





private:
	bool savedStartPoints;
	bool invert;
	gcroot<array<System::String^>^> m_filenames;
	//bool dragon; //debug variable
	bool enableEnhancing;
	bool evolveTipPoints;
	int tipPointsNumToFit;
	bool showLastImageToggle;
	//DWORD timer_val; //K_I
	bool calibration_mode;
	int calib_line_points, num_calib_traces;
	CvPoint calib_line1, calib_line2;
	double calib_dist;
	void doRootTracking( UCHAR idealr, UCHAR idealg, UCHAR idealb, double proSigmaX, double proSigmaY, UCHAR new_state, CvGraph* g, int rootnum, int filenum,  double prior_bg,  double upperHyst, double lowerHyst, System::Windows::Forms::Form^ mainWindow);
	int rootIndex;
	void doGraphTraversal(CvGraph* g, System::Windows::Forms::Form^ mainWindow);
	//double cum_sq_devs, st_dev;
	vector<MyPath> paths;
	//void mainWindowText(System::Windows::Forms::Form^ mainWindow, System::String ^string); //K_I
	double BG_mean_r;
	double BG_mean_g; 
	double BG_mean_b;
	double BG_st_dev_r;
	double BG_st_dev_g;
	double BG_st_dev_b;
	int rule2_count;
	bool rule_2_polarity;
	IplImage* loadImage(const char* loadpath);
	double evaluate_gaussian(double val, double sigma);
	bool getBGStats(IplImage* bg_mask, IplImage* image);
	//CStatsWriter prob_stats;
	double last_frame_euc_dist[max_roots];
	System::Windows::Forms::Form^ getMainWindow();
	gcroot<System::Windows::Forms::Form^> mainWindowGlobal;
	CvPoint bg_rect_click;
	bool removeBGflag; 

	UCHAR dataR[4000000];
	UCHAR dataG[4000000];
	UCHAR dataB[4000000];

	//double bg_mean[255][3];
	//double bg_sd[255][3];

	IplImage* bg_mask;
	IplImage* liveZoomImg;
	int liveZoomImg_width, liveZoomImg_height;

	bool recalcBGStats;
	bool multiple_BG_stats;
	bool doFixedDistMeasure;
	CvPoint fixedPoint1;
	CvPoint fixedPoint2;
	double fixedDistance;
	double calib_mmPerPix, calib_minPerImage;
	CvPoint crop1, crop2;
	bool doCrop;
	CvRect croprect;
	//IplImage* i_copy;
	char outputFolderString[512];
	char workingFolder[512];


	int BG_region_counter;

	void cropImages(CvRect rect);
	CvPoint bg_temp;
	CvPoint crop_temp;
	bool goButtonPressed;
	CvPoint manualAnglePoint1, manualAnglePoint2, manualAnglePoint3;
	int manualAngleCount;
	int numberOfManualAngleMeasures;
	bool doTipAngles;
	void drawTabooPoints();
	
	

	
protected:
	CvPoint lockToRoot(int clickx, int clicky);
public:
	void prepareCrop(void);
	void toggleGoButtonPressed(void);
	void setOutputFolderString(System::String^ folder);
	void setWorkingFolder(System::String^ folder);
private:
	//void startTimer(void); //K_I
	//void stopTimer(void); //K_I
	// //set up values for some parameters
	void setupParameters(void);
	int checkKeyPresses(int code);
	bool quitnow;
	bool doGravitropic;
	bool rotate_cw, rotate_ccw, flip_hor;
	bool tabooPointsEnabled;
	bool findLaterals;

	bool m_bilat_filter;
	bool m_med_filter;
	bool m_gaus_filter;
	bool m_suppressHor;
	bool m_bLocaLMaxFilter;
	bool m_bblowHighlights;
	bool m_blowHighlights;
	bool m_bgRemoval;

public:
	void setDoGravitropic(bool in);
	void setDoTipAngles(bool in);
	int doManualAngles(int x, int y);
	void setImageTranform(bool flipin, bool rotCWin, bool rotCCWin);
	void setTabooPointsEnabled(bool val);
	/*__int648*/ int findBarcodeInImage(void); //K_I
	void fitWindowToImage(void);
	void setTipPointsNumToFit(int in);
	void setEvolveTipPoints(bool in);
	void interactiveEnhance(bool bilat_filter, bool med_filter, bool gaus_filter, bool suppressHor, bool bLocalMaxFilter, bool bblowHighlights, int blowHighlights, bool cb_bg_removal);
	void setEnableEnhancing(bool in);
	void setIdealRGBinCond(int brightness);
	void setCountLaterals(bool m_countLaterals);
	void onClose(void);
	void saveComments();
	void setInvert(bool flag);
};
