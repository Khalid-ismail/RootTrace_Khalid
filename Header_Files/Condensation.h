//#include "stdafx.h"
#include <cv.h>

#pragma once

typedef unsigned char UCHAR;

//-------------------------
  #define NSAMPLES  (25)      //was 25... but lowering to 10 drops dijk algorithm from 100s of ms to 10s of ms per root per image/ Fewer samples = noisier tracing, though
  #define numTs		(1)
//const int MODEL_NUM  = 3;		//1 = low, 2 = high, 3 = const
//-------------------------


//const int radius = 3; // measurement circle radius, was 3 - NOW SET IN GUI BOX

// NEW
//double ObsSigmaColR ;//=0.1; //0.1 fine. Larger values mean less good particles can exist longer
//double ObsSigmaColG ;//=0.1;  
//double ObsSigmaColB ;//=0.1;   
const double ObsSigmaColR_const = 0.07;//0.1. Changed 2/4/09
const double ObsSigmaColG_const = 0.07;//0.1
const double ObsSigmaColB_const = 0.07;//0.1

//const bool useGravProcessModel = false; //original, gravity y+1 motion model
//const bool usePolynomialMotion = true;
//const int numPointsToFit = 15;//5, 10
//const int futureStep = 5;
//const int polynomialOrder = 1;//2 good all-rounder, 3 ok with more points (25?)


// PROCESS SET IN form1.h!

const double transition_prob_0_1  = 0.0;  //was 0.2.  0 stays in state 0

const double transition_prob_stop = 0.0;  // 0 = don't detect stops.  Doesn't work 

//const int initial_state = 1; //0 down, 1 right. Now a parameter

const bool dont_use_uv = false;  // if true, u and v get set to 0 in process
const bool dont_use_acc = true; // if true, au and av get set to 0 in process (no acceleration variable)


const int Nsamples = NSAMPLES;  




typedef struct _Sample {
  double x[numTs], y[numTs], u[numTs], v[numTs], au[numTs], av[numTs], pi;
  int motion[numTs]; //discrete version, new May 08
  int state[numTs];
  //int radius;
} Sample;

class CCondensation
{
public:
	//Methods
	CCondensation(void);
	~CCondensation(void);
	void init(int target, int x, int y, double u_init, double v_init, UCHAR idealr, UCHAR idealg, UCHAR idealb, UCHAR tb_lr, double proSigmaX, double proSigmaY, int radius_in, double motMix);
	double gaussian_random(void);
	double uniform_random(void);
	void measurementPhase(IplImage* image);
	void eval_observation(double* measures); 
	void process_samples(CvPoint bestGuess_t_1, int step, IplImage* image);
	void pick_samples();
	double getHighestPi();
	double getHighestProb();
	Sample getAverageSample();
	Sample getWeightedAverageSample();
	int getBestIndex();
	void swap_arrays();
	double getBestX(int t);
	double getBestY(int t);
	double getBestU(int t);
	double getBestV(int t);
	double getBestAU(int t);
	double getBestAV(int t);
	int getBestState(int t);
	int getMajorityState(int t);
	double GetColourMeasureAtPos(int x, int y, double radius, IplImage * image, int flag);
	double GetColourMeasureAtPos_uniform(int x, int y, double radius, IplImage * image, int flag);
	double getStateProbability(int t);
	double getProbAroundXY(int target, double x, double y, double pix_range);
	int toggle_state();

	//Data structures
	double cumul_prob_array[NSAMPLES];
	double largest_cumulative_prob;
	Sample s_old[NSAMPLES];
	Sample s_new[NSAMPLES];

	void setObsModelParams(double r, double g, double b, double r_sd, double g_sd, double b_sd);
	void paintParticleSet(IplImage* img, IplImage* img2,  double r, double g, double b);

private:
	int polynomialOrder;
	int futureStep;
	int numPointsToFit;
	CvPoint bestPos[1000];
	int bestPosCounter;
	//ASAD
	int stepCount;
	//void paintParticleSet(IplImage* img, double r, double g, double b);
	//bool gravitropicTracking;
	int motionMixture;//was double
	int back[10000][2];
	// -ASAD

	double ObsSigmaColR ;//=0.1; //0.1 fine. Larger values mean less good particles can exist longer
	double ObsSigmaColG ;//=0.1;  
    double ObsSigmaColB ;//=0.1; 
	
	long int m_seed;
	int initial_state;
	//double *measures;
	double measures[NSAMPLES*3*numTs];
	
	void MyPutPixel(IplImage *m_image, int x, int y, UCHAR b, UCHAR g, UCHAR r);
	void MyGetPixel(IplImage *m_image, int x, int y, UCHAR *b, UCHAR *g, UCHAR *r);
	double evaluate_gaussian(double val, double sigma);
	UCHAR idealR, idealG, idealB;
	
	double ProcessSigmaX;
	double ProcessSigmaY;

	double rootFilterResponse(IplImage *M_image, int x, int y, int radius);

	int radius;
		
	
public:
	void incrementStepCount(void);
	void setBack(int i, int j, int val);
	int showSamples(IplImage* m_image);
	void updateBestPos(void);
	int getNumPointsToFit(void);
	int setIdealRGB(int R, int G, int B);
};
