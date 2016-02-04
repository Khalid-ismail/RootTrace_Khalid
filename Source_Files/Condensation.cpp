//#include "stdafx.h" //K_I


//#include <nr.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <iomanip>
#include <highgui.h>
#include "/home/osboxes/mnt/RT3/RootTrace/RootTrace2/Condensation.h"

using namespace std;
//using namespace System::Windows::Forms; //No need for forms

// #using <..\Release\MathNet.Iridium.dll> //K_I
// using namespace MathNet::Numerics::Interpolation; //K_I





#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))


int steps_before_transition_allowed = 10;


CCondensation::CCondensation(void)
{
	m_seed = (long int)time( NULL );
	//measures= new double[NSAMPLES*3*numTs];.
}

CCondensation::~CCondensation(void)
{
	//delete measures;
}

int CCondensation::toggle_state() {
	int new_state=-1;
	for (int i=0; i<Nsamples; i++) {

		if (s_old[i].state[0]==0) {
			s_old[i].state[0]=1;
			new_state= 1;
		}
		else if (s_old[i].state[0]==1) {
			s_old[i].state[0]=0;
			new_state= 0;
		}
	}

		return new_state;

	
}

void CCondensation::init(int t, int x, int y, double u_init, double v_init, UCHAR idealr, UCHAR idealg, UCHAR idealb, UCHAR tb_lr, double proSigmaX, double proSigmaY, int radius_in, double motMix)
{
	bestPosCounter = 0;
	motionMixture = motMix;
	stepCount = 0;
	radius = radius_in;

	ProcessSigmaX = proSigmaX;
	ProcessSigmaY = proSigmaY;

	ObsSigmaColR =ObsSigmaColR_const;
    ObsSigmaColG =ObsSigmaColG_const;
    ObsSigmaColB =ObsSigmaColB_const;

	initial_state = tb_lr;
	idealR = idealr;
	idealG = idealg;
	idealB = idealb;
	double x_init_covar = 2;//was 1


	double y_init_covar = 0;//was 0
	double u_init_covar = 0.1;
	double v_init_covar = 0.1;
	
for (int i=0; i<Nsamples; i++) {

			
			s_old[i].x[t]           = x  + (x_init_covar * gaussian_random());
			s_old[i].y[t]           = y  + (y_init_covar * gaussian_random());
			s_old[i].u[t]           = u_init + (u_init_covar * gaussian_random());
			s_old[i].v[t]           = v_init + (v_init_covar * gaussian_random());
			s_old[i].au[t]			= 0;
			s_old[i].av[t]			= 0;
			
			//cout <<"  Generated particle x, y = "<<s_old[i].x[t] <<", "<<s_old[i].y[t] <<endl;

			//state 0=normal, 1 = grav
			//double r = uniform_random();
			//if (r<1.1)  //always begin in normal state? 
				//s_old[i].state[t]=0;
				//else s_old[i].state[t] =1;

			s_old[i].state[t]=initial_state;

			//s_old[i].radius = 7/*+gaussian_random()*/;

			s_old[i].motion[t] = 5; //5 for down.  See p 97 lab book
		
			/* The probabilities are not normalised. */
			cumul_prob_array[i] = (double) i;
			s_old[i].pi = 1.0;
	}

   largest_cumulative_prob = (double) Nsamples;

}

__inline double CCondensation::gaussian_random(void)
{ 
  // NOT between -1 and +1; distibuted as per Gaussian
  static int next_gaussian = 0;
  static double saved_gaussian_value;

  double fac, rsq, v1, v2;

  if (next_gaussian == 0) {
    do {
      v1 = 2.0*uniform_random()-1.0;
      v2 = 2.0*uniform_random()-1.0;
      rsq = v1*v1+v2*v2;
    } while (rsq >= 1.0 || rsq == 0.0);
    fac = sqrt(-2.0*log(rsq)/rsq);
    saved_gaussian_value=v1*fac;
    next_gaussian=1;
    return v2*fac;
  } else {
    next_gaussian=0;
    return saved_gaussian_value;
  }
}


__inline double CCondensation::uniform_random(void)
{
    //this random number widget is tripe, apparently
	return (double) rand() / (double) RAND_MAX;
	
	// === PSEUDO RANDOM
	//return ran4(&m_seed);  // Numerical recipes - ran4 even better than ran2, but slower...


	// === QUASI-RANDOM
	// Do I need to re-init every time????? for every sample?
	/*
	int n=1;
	sobseq(&n, qx);
	return qx[1];
	*/

}

/*__inline*/ void CCondensation::measurementPhase(IplImage* image) {
	

	//MEAUREMENT PHASE 
	//Use Nummiaro et al's method
	for (int t=0; t<numTs; t++) {
		//int radius   = (int)cond->getAverageR(t);
		for (int n=0; n<NSAMPLES; n++) {
			
			
			// Next 3 lines make use of weighted circle measure
			measures[(t*NSAMPLES)+(n*3)]   = GetColourMeasureAtPos((int)s_new[n].x[t], image->height - (int)s_new[n].y[t], radius, image,  0);
			measures[(t*NSAMPLES)+(n*3+1)] = GetColourMeasureAtPos((int)s_new[n].x[t], image->height - (int)s_new[n].y[t], radius, image,  1);
			measures[(t*NSAMPLES)+(n*3+2)] = GetColourMeasureAtPos((int)s_new[n].x[t], image->height - (int)s_new[n].y[t], radius, image,  2);
			
			/*
			// all colours give same response to this at the moment
			measures[(t*NSAMPLES)+(n*3)] = rootFilterResponse(image, s_new[n].x[t],   image->height - s_new[n].y[t], s_new[n].radius);
			measures[(t*NSAMPLES)+(n*3+1)] = rootFilterResponse(image, s_new[n].x[t], image->height - s_new[n].y[t], s_new[n].radius);
			measures[(t*NSAMPLES)+(n*3+2)] = rootFilterResponse(image, s_new[n].x[t], image->height - s_new[n].y[t], s_new[n].radius);
			*/

		}
	}

	eval_observation(measures);

}

double CCondensation::rootFilterResponse(IplImage *m_image, int x, int y, int radius) {

	//int radius = 5;
	UCHAR r, g, b;
	double response=0;
	MyGetPixel(m_image, x, y, &r, &g, &b);
	response +=  24*(int)r + 24*(int)g + 24*(int)b;

	//MessageBox::Show("r, g, b = "+r+", "+g+", "+b);

	
	MyGetPixel(m_image, x-radius, y, &r, &g, &b);
	response +=  -3*(int)r + -3*(int)g + -3*(int)b;
	MyGetPixel(m_image, x+radius, y, &r, &g, &b);
	response +=  -3*(int)r + -3*(int)g + -3*(int)b;

	MyGetPixel(m_image, x, y-radius, &r, &g, &b);
	response +=  -3*(int)r + -3*(int)g + -3*(int)b;
	MyGetPixel(m_image, x, y+radius, &r, &g, &b);
	response +=  -3*(int)r + -3*(int)g + -3*(int)b;

	MyGetPixel(m_image, x-radius, y-radius, &r, &g, &b);
	response +=  -3*(int)r + -3*(int)g + -3*(int)b;
	MyGetPixel(m_image, x-radius, y+radius, &r, &g, &b);
	response +=  -3*(int)r + -3*(int)g + -3*(int)b;

	MyGetPixel(m_image, x+radius, y-radius, &r, &g, &b);
	response +=  -3*(int)r + -3*(int)g + -3*(int)b;
	MyGetPixel(m_image, x+radius, y+radius, &r, &g, &b);
	response +=  -3*(int)r + -3*(int)g + -3*(int)b;

	response = response/(24*3);
	if (response<0) response = 0;

	//MessageBox::Show("response = "+response);

	return response;
	


}

void CCondensation::MyPutPixel(IplImage *m_image, int x, int y, UCHAR b, UCHAR g, UCHAR r)
{
       CvPoint point;
       point.x = x;
       point.y = y;

       if ((x<0||x>m_image->width)||(y<1||y>m_image->height)) {
           //cout <<" ERROR in MyPutPixel. x = " <<x <<", y = "<<y<<endl;
		    //MessageBox::Show("WARNING trying to write value off image");
           //exit(1);
	   }else{

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

void CCondensation::MyGetPixel(IplImage *m_image, int x, int y, UCHAR *b, UCHAR *g, UCHAR *r)
{
       CvPoint point;
       point.x = x;
       point.y = y;

       if ((x<0||x>m_image->width)||(y<1||y>m_image->height)) {
           //cout <<" ERROR in MyGetPixel. x = " <<x <<", y = "<<y<<endl;
           //exit(1);
		   //MessageBox::Show("WARNING trying to read value off image");
		   r=0;
		   g=0;
		   b=0;
	   } else{

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

double CCondensation::evaluate_gaussian(double val, double sigma)
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

void CCondensation::pick_samples() {
	int low, middle, high;
	


	for (int i=0; i<Nsamples; i++) {
		double choice = uniform_random() * largest_cumulative_prob;
		// binary search cumulative probs
		low = 0;
		high = Nsamples;

		while (high>(low+1)) {
			middle = (high+low)/2;
			if (choice > cumul_prob_array[middle])
			low = middle;
		else high = middle;
		}
		

		for (int t=0; t<numTs; t++) {
		// Copy across to new
		s_new[i].x[t] = s_old[low].x[t];
		s_new[i].y[t] = s_old[low].y[t];
		s_new[i].u[t] = s_old[low].u[t];
		s_new[i].v[t] = s_old[low].v[t];
		s_new[i].au[t] = s_old[low].au[t];
		s_new[i].av[t] = s_old[low].av[t];
		s_new[i].state[t] = s_old[low].state[t];
		s_new[i].motion[t] = s_old[low].motion[t];
		//cout<<"  Copying to new "<<i<<", x "<<s_new[i].x[t]<<", "<<s_new[i].y[t]<<endl;
		
		s_new[i].pi = s_old[low].pi;  // <<<<< should I carry forward the pi? Gets over-written on measurement anyway
		}

	}
}

Sample CCondensation::getAverageSample()
{
	Sample info;
	for (int t = 0; t<numTs; t++){
	info.x[t]=0;
	info.y[t]=0;
	info.u[t]=0;
	info.v[t]=0;
	info.au[t]=0;
	info.av[t]=0;
	}
	info.pi=0;
	
	for (int t = 0; t<numTs; t++){
		for (int i = 0; i<Nsamples; i++) {
			
			info.x[t]+=s_new[i].x[t];
			info.y[t]+=s_new[i].y[t];
			info.u[t]+=s_new[i].u[t];
			info.v[t]+=s_new[i].v[t];
			info.au[t]+=s_new[i].au[t];
			info.av[t]+=s_new[i].av[t];
			
			
		}

	info.x[t] /= Nsamples;
	info.y[t] /= Nsamples;
	info.u[t] /= Nsamples;
	info.v[t] /= Nsamples;
	info.au[t] /= Nsamples;
	info.av[t] /= Nsamples;
	}// t
	
	return info;
}

Sample CCondensation::getWeightedAverageSample()
{
	Sample info;
	for (int t = 0; t<numTs; t++){
		info.x[t]=0;
		info.y[t]=0;
		info.u[t]=0;
		info.v[t]=0;
		info.au[t]=0;
		info.av[t]=0;
	}
	info.pi=0;
	double totalpi=0;

	for (int t = 0; t<numTs; t++){
		for (int i = 0; i<Nsamples; i++) {

			info.x[t]+=s_new[i].x[t]*s_new[i].pi;
			info.y[t]+=s_new[i].y[t]*s_new[i].pi;
			info.u[t]+=s_new[i].u[t]*s_new[i].pi;
			info.v[t]+=s_new[i].v[t]*s_new[i].pi;
			info.au[t]+=s_new[i].au[t]*s_new[i].pi;
			info.av[t]+=s_new[i].av[t]*s_new[i].pi;
			totalpi += s_new[i].pi;
		}

		
		
		info.x[t] /= totalpi;
		info.y[t] /= totalpi;
		info.u[t] /= totalpi;
		info.v[t] /= totalpi;
		info.au[t] /= totalpi;
		info.av[t] /= totalpi;
	}// t

	return info;
}
int CCondensation::getBestIndex()
{
	double highestProb = 0;
	int index = -1;

	for (int i = 0; i<Nsamples; i++) {
		if (s_new[i].pi>highestProb) {  
			highestProb = s_new[i].pi;
			index = i;
		}
	}

	return index;
}

double CCondensation::getBestX(int t) {
	double highestProb = 0;
	int index = -1;

	for (int i = 0; i<Nsamples; i++) {
		if (s_new[i].pi>highestProb) {  //s_new or s_old?!?
			highestProb = s_new[i].pi;
			index = i;
		}
	}

	return s_new[index].x[t];

}

double CCondensation::getBestY(int t) {
	double highestProb = 0;
	int index = -1;

	for (int i = 0; i<Nsamples; i++) {
		if (s_new[i].pi>highestProb) {  //s_new or s_old?!?
			highestProb = s_new[i].pi;
			index = i;
		}
	}

	return s_new[index].y[t];

}

double CCondensation::getBestU(int t) {
	double highestProb = 0;
	int index = -1;

	for (int i = 0; i<Nsamples; i++) {
		if (s_new[i].pi>highestProb) {  //s_new or s_old?!?
			highestProb = s_new[i].pi;
			index = i;
		}
	}

	return s_new[index].u[t];

}

double CCondensation::getBestV(int t) {
	double highestProb = 0;
	int index = -1;

	for (int i = 0; i<Nsamples; i++) {
		if (s_new[i].pi>highestProb) {  //s_new or s_old?!?
			highestProb = s_new[i].pi;
			index = i;
		}
	}

	return s_new[index].v[t];

}

double CCondensation::getBestAU(int t) {
	double highestProb = 0;
	int index = -1;

	for (int i = 0; i<Nsamples; i++) {
		if (s_new[i].pi>highestProb) {  //s_new or s_old?!?
			highestProb = s_new[i].pi;
			index = i;
		}
	}

	return s_new[index].au[t];

}

double CCondensation::getBestAV(int t) {
	double highestProb = 0;
	int index = -1;

	for (int i = 0; i<Nsamples; i++) {
		if (s_new[i].pi>highestProb) {  //s_new or s_old?!?
			highestProb = s_new[i].pi;
			index = i;
		}
	}

	return s_new[index].av[t];

}



int CCondensation::getBestState(int t) {
	double highestProb = 0;
	int index = -1;

	for (int i = 0; i<Nsamples; i++) {
		if (s_new[i].pi>highestProb) {  //s_new or s_old?!?
			highestProb = s_new[i].pi;
			index = i;
		}
	}

	return s_new[index].state[t];

}

double CCondensation::getProbAroundXY(int target, double xval, double yval, double pix_range) {
	// find probability of being within 'range' of x,y...?
		
	double prob = 0;
	double cum_prob=0;
	

	for (int i = 0; i<Nsamples; i++) {
		cum_prob+=s_new[i].pi;

		if (s_new[i].x[target]>xval-pix_range && s_new[i].x[target]<xval+pix_range && 
			s_new[i].y[target]>yval-pix_range && s_new[i].y[target]<yval+pix_range) {
			prob+=s_new[i].pi;

		}
	}

	prob = prob/cum_prob;

	if (numTs!=1) prob = -1;

	return prob;

}

int CCondensation::getMajorityState(int t) {
	
	double state0counter = 0;
	double state1counter = 0;

	for (int i = 0; i<Nsamples; i++) {
		if (s_new[i].state[t]==0) state0counter+=s_new[i].pi;
		if (s_new[i].state[t]==1) state1counter+=s_new[i].pi;
		}
	
	
	//cout<<"State 0: "<< state0counter<<", State 1: "<<state1counter<<endl;
	if (state0counter>state1counter) 
		return 0;
	else
		return 1;

}

double CCondensation::getStateProbability(int t) {
	
	double state0counter = 0;
	double state1counter = 0;

	for (int i = 0; i<Nsamples; i++) {
		if (s_new[i].state[t]==0) state0counter+=s_new[i].pi;
		if (s_new[i].state[t]==1) state1counter+=s_new[i].pi;
		}
	
	double prob = state0counter/(state0counter+state1counter);
	//cout<<"Prob State 0: "<<setprecision(2)<<prob<<endl;
	
	return prob;

}

double CCondensation::getHighestPi() {
	// un normalised prob, see getHighestProb
	double highestProb = 0;
	for (int i = 0; i<Nsamples; i++) {
		if (s_new[i].pi>highestProb) highestProb = s_new[i].pi;
	}
	return highestProb;
}

double CCondensation::getHighestProb() {
	// Normalized prob
	// BUT! if lots of samples with this prob then normalized prob decreases...this is unwanted behaviour
	double highestProb = 0;
	for (int i = 0; i<Nsamples; i++) {
		if (s_new[i].pi>highestProb) highestProb = s_new[i].pi;
	}

	highestProb = highestProb/largest_cumulative_prob;
	return highestProb;
}

void CCondensation::process_samples(CvPoint bestGuess_t_1, int step, IplImage* image) {
	// NOT using best guess plan yet

	//const int sliderValue = motionMixture;
	

	//double bestu = getAverageSample().u[0];
	//double bestv = getAverageSample().v[0];
	//double random_component = 0;

    numPointsToFit = 25-motionMixture * 2; //high when 'straight'
	futureStep     = 5;//5 - floor((double)(motionMixture/3)); //5 when straight, 2 when bendy?
	if (motionMixture==10) futureStep = 4;
	if (motionMixture==1) polynomialOrder = 1;
	else polynomialOrder = 2;

	for (int i=0; i<Nsamples; i++) {
		// *** Should be real processes here (plus drift?) ***

		double x=0, y=0, u=0, v=0, r=0, rv=0, col=0, theta=0, au=0, av=0;
		bool mdebug=false;
		
		for (int t=0; t<numTs; t++) {
		//prior
		x = s_new[i].x[t];
		y = s_new[i].y[t];
		u = s_new[i].u[t];
		v = s_new[i].v[t];
		au = s_new[i].au[t];
		av = s_new[i].av[t];



		
		
		double r = uniform_random();
		double r2 = uniform_random();


		
		if (dont_use_uv) {
			u=0;
			v=0;
		}

		if (dont_use_acc){
			au=0;
			av=0;
		}

		double maxvel = 5.0;
		
		
		  // the old way
		if (s_new[i].state[t]==0) {		//top to bottom
			

			//NOTE only really need to do fit once per time step, then use the fit to produce each sample? Hence condition below
			/*if (/*usePolynomialMotion &&/ i==Nsamples-1 &&motionMixture>0) {
				bool displayTestGraphs=false;
				IplImage* rootspacei;
				if (displayTestGraphs) {
					rootspacei= cvCreateImage(cvSize(200, image->height), 8, 3);
					cvZero(rootspacei);
				}
				
				//numPointsToFit = 30;//5 , 10 good. 6.
				if(stepCount>max(numPointsToFit+1, 15)) {



					array<double>^ arrayy= gcnew array<double>(numPointsToFit);
					for(int j=0; j < numPointsToFit; j++)
						arrayy[j] = bestPos[bestPosCounter-(numPointsToFit-j)].x;
						
					

					// Build the matrix for the least-squares fitting
					//array<System::Double,2>^ m;
					//m = (array<System::Double,2>^)MathNet::Numerics::LinearAlgebra::Matrix::CreateMatrixData(10, polynomialOrder+1);

					//double m[10][];
					//for (int ii = 0; ii < 10; ii++)
					//{
					//	m[ii] = new double[ polynomialOrder+1];
					//}
					array<double,2>^ m = gcnew array<double, 2>(numPointsToFit,polynomialOrder+1);

					
					double distance = 0;
					for(int ii=0; ii < numPointsToFit; ii++)
					{
						double xi = distance;//was ii
						//MessageBox::Show("Dist = "+distance);
						//array<double>^ xrow;
						m[ii,0] = 1.0;
						for(int j=1; j < polynomialOrder+1; j++) {
							m[ii,j]  = xi * m[ii,j-1];
						}

						if (ii<numPointsToFit-1){ //can't do this last time through. Must be a better way of writing this?
						double xdis = abs(bestPos[bestPosCounter-(numPointsToFit-ii)].x - bestPos[bestPosCounter-(numPointsToFit-ii-1)].x);
						double ydis = abs(bestPos[bestPosCounter-(numPointsToFit-ii)].y - bestPos[bestPosCounter-(numPointsToFit-ii-1)].y);
						distance = distance + sqrt(xdis*xdis + ydis*ydis);

						}

						

					}

					if (displayTestGraphs && i==Nsamples-1){
						//test display
						double testdist=0;
						cvNamedWindow("Rootspace");
										
						for(int ii=0; ii < numPointsToFit; ii++)
						{
						
							cvCircle(rootspacei, cvPoint(testdist,bestPos[bestPosCounter-(numPointsToFit-ii)].x), 1, cvScalar(255, 255, 255), -1);
							cvCircle(rootspacei, cvPoint(testdist,bestPos[bestPosCounter-(numPointsToFit-ii)].y), 1, cvScalar(0, 255, 255), -1);
							if (ii<numPointsToFit-1){ //can't do this last time through. Must be a better way of writing this?
								double xdis = abs(bestPos[bestPosCounter-(numPointsToFit-ii)].x - bestPos[bestPosCounter-(numPointsToFit-ii-1)].x);
								double ydis = abs(bestPos[bestPosCounter-(numPointsToFit-ii)].y - bestPos[bestPosCounter-(numPointsToFit-ii-1)].y);
								testdist = testdist + sqrt(xdis*xdis + ydis*ydis);
							}
						}
						cvShowImage("Rootspace", rootspacei);
						cvWaitKey(1);
						//End test display
					}

					// Find the least-squares solution
					MathNet::Numerics::LinearAlgebra::Matrix matrix = gcnew MathNet::Numerics::LinearAlgebra::Matrix(m);
					MathNet::Numerics::LinearAlgebra::Matrix solution = matrix.Solve(gcnew MathNet::Numerics::LinearAlgebra::Matrix(arrayy, arrayy->Length));

					// Extract the values (in our case into a polynomial for fast evaluation)
					MathNet::Numerics::Polynomial polynomial = gcnew MathNet::Numerics::Polynomial(solution.GetColumnVector(0));

					//Now do for y
					array<double>^ arrayy2= gcnew array<double>(numPointsToFit);
					for(int j=0; j < numPointsToFit; j++)
						arrayy2[j] = bestPos[bestPosCounter-(numPointsToFit-j)].y;

					// Find the least-squares solution
					MathNet::Numerics::LinearAlgebra::Matrix matrix2 = gcnew MathNet::Numerics::LinearAlgebra::Matrix(m);
					MathNet::Numerics::LinearAlgebra::Matrix solution2 = matrix2.Solve(gcnew MathNet::Numerics::LinearAlgebra::Matrix(arrayy2, arrayy2->Length));

					// Extract the values (in our case into a polynomial for fast evaluation)
					MathNet::Numerics::Polynomial polynomial2 = gcnew MathNet::Numerics::Polynomial(solution2.GetColumnVector(0));

					double future = distance+futureStep;

					array<double>^ out;//= gcnew array<double>(2);
					array<double>^ out2;//= gcnew array<double>(2);
					
					
					out  =polynomial.Evaluate (future, 1);
					out2 =polynomial2.Evaluate(future, 1);

					double newx = out[0];
					double newy = out2[0];
					double der = out[1];
					double der2 = out2[1];

					// Tangent equations
					// y=mx+c. Don't forget this is in root coordinates! So x = distance, y = x or y
					// c = y-mx. m we know 
					double c  = newx - (der *future);
					double c2 = newy - (der2*future);

					if (i==Nsamples-1) {

						//x tang line
						CvPoint start = cvPoint(0, 0.0*der+c);
						CvPoint end = cvPoint(distance, distance*der+c);
						if (displayTestGraphs) cvLine(rootspacei, start, end, cvScalar(128, 128,128), 1);
						//Perp
						double dertest = -1.0/der;
						double ctest  = newx - (dertest *future);
						CvPoint startp = cvPoint(distance-20, (distance-20.0)*dertest+ctest);
						CvPoint endp = cvPoint(distance+20, (distance+20.0)*dertest+ctest);
						if (displayTestGraphs) cvLine(rootspacei, startp, endp, cvScalar(128, 128,128), 1);
						//cvCircle(rootspacei, startp, 2, CV_RGB(255, 0, 0), -1);
						

						//y tang line
						CvPoint start2 = cvPoint(0, 0.0*der2+c2);
						CvPoint end2 = cvPoint(distance, distance*der2+c2);
						if (displayTestGraphs) cvLine(rootspacei, start2, end2, cvScalar(0, 128,128), 1);
						//Perp
						double dertest2 = -1.0/der2;
						double ctest2  = newy - (dertest2 *future);
						CvPoint startp2 = cvPoint(distance-20, (distance-20.0)*dertest2+ctest2);
						CvPoint endp2 = cvPoint(distance+20, (distance+20.0)*dertest2+ctest2);
						if (displayTestGraphs) cvLine(rootspacei, startp2, endp2, cvScalar(0, 128,128), 1);
						//cvCircle(rootspacei, startp2, 2, CV_RGB(0, 0, 255), -1);



						//BELOW: Tangent line appears correct but NOT the perp. one!
						//Tang
						CvPoint ispaceStart =  cvPoint(start.y, start2.y);
						CvPoint ispaceEnd = cvPoint(end.y, end2.y);
						//cvLine(image, ispaceStart, ispaceEnd, cvScalar(255,0,255), 1);
						//cvCircle(image, ispaceStart, 2, CV_RGB(255, 0, 255), -1);
						//Perp
						CvPoint ispaceStartp =  cvPoint(startp.y, startp2.y);//swapped start and end. Now works...? Why?
						CvPoint ispaceEndp = cvPoint(endp.y, endp2.y);
						//cvLine(image, ispaceStartp, ispaceEndp, cvScalar(255,255,0), 1);
						//cvCircle(image, ispaceStartp, 2, CV_RGB(255, 0, 255), -1);

						//CvPoint ispaceStartp_ =  cvPoint(endp.y, startp2.y);
						//CvPoint ispaceEndp_ = cvPoint(startp.y, endp2.y);
						//cvLine(image, ispaceStartp_, ispaceEndp_, cvScalar(128,128,0), 1);

						
						// New idea. Try and calculate from the image-space tangent line, as thats correct
						// added safegaurds to prevent div 0 and infinity, but actually this is handled in special vertical case below
						double iTangGrad;
						if (ispaceEnd.y-ispaceStart.y==0) {
							iTangGrad=0.00001;  //not zero, to prevent div 0
						} 
						else if (ispaceEnd.x-ispaceStart.x==0 && ispaceEnd.y-ispaceStart.y>0){
							iTangGrad=9999999; // big, but can't be calculated due to div 0
						}
						else if (ispaceEnd.x-ispaceStart.x==0 && ispaceEnd.y-ispaceStart.y<0){
							iTangGrad=-9999999; // big, but can't be calculated due to div 0
						}
						else iTangGrad = (double)(ispaceEnd.y-ispaceStart.y)/(ispaceEnd.x-ispaceStart.x);
						double iPerpGrad = -1.0/iTangGrad;
						//MessageBox::Show("iPerpGrad = "+iPerpGrad);
					

						//invert 1st try at perp?
						//double iPerp1Grad= (double)(ispaceEndp.y-ispaceStartp.y)/(ispaceEndp.x-ispaceStartp.x);
						//double iPerp2Grad= -1.0/iPerp1Grad;
						double ipc  = newy - (iPerpGrad *newx);
						//now plot little bit either side of newx, newy;
						double pdist = 40;//start distance, 40
						double size = 30; //target distance, 30
						bool doloop=true;
						int x1, x2, y1, y2;

						if (ispaceEnd.y == ispaceStart.y ) { //Vertical special case, as this is tangent not perp?
							//MessageBox::Show("VERTICAL");
							x1 = newx;
							x2 = newx;
							y1 = newy - size/2;
							y2 = newy + size/2;
							cvLine(image, cvPoint(x1, y1), cvPoint(x2, y2), CV_RGB(128, 128, 0), 1);


						}else
						//int dd=0;
						while (doloop){
							//dd++;
							x1 = newx-pdist;
							x2 = newx+pdist;
							y1 = iPerpGrad*x1 + ipc;//if iPerpGrad is massive, so is y1.....BUG
							y2 = iPerpGrad*x2 + ipc;
							if ((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) > size*size) pdist/=2;
							else {
									doloop = false;
									//if (abs(iPerpGrad)<0.0001||abs(iPerpGrad)>99999) MessageBox::Show("newy = "+newy+", iPerpGrad = " + iPerpGrad +", iTangGrad = "+iTangGrad);
									if (displayTestGraphs) cvLine(image, cvPoint(x1, y1), cvPoint(x2, y2), CV_RGB(0, 128, 0), 1); // THIS IS THE LINE (AND BELOW) that gets drawn in an odd place
								}
							if (pdist<5) {
								doloop = false;
								x1 = newx;
								x2 = newx;
								y1 = newy - size/2;
								y2 = newy + size/2;
								if (displayTestGraphs) cvLine(image, cvPoint(x1, y1), cvPoint(x2, y2), CV_RGB(128, 128, 0), 1); //near vertical 

							}
							
						}


						//This code only happens for one i (Nsamples-1), so fill the rest of the samples
						// Create samples in a line along future perpendicular. Line is NSamples long???
						CvPoint jpoint = cvPoint(x1, y1);
						double tempx = jpoint.x;
						double tempy = jpoint.y;
						//MessageBox::Show("jpoint = "+x+","+y+"\n dd="+dd); //the y here is wrong = max -ve int
						for (int j=0; j<Nsamples; j++){
							s_new[j].x[t] = jpoint.x;
							s_new[j].y[t] = jpoint.y;

							tempx += (double)(x2-x1)/Nsamples;
							tempy += (double)(y2-y1)/Nsamples;
							jpoint.y = tempy; //double to int
							jpoint.x = tempx; //double to int
							if (displayTestGraphs) MyPutPixel(image, jpoint.x, image->height-jpoint.y, 255, 0, 255);

						}
						

						if (displayTestGraphs) {
							cvShowImage("Image", image);
							cvShowImage("Rootspace", rootspacei);
							cvWaitKey(1);
						}
					}

					// Tangent line in image coordinates defined by moving distance along root with 2 new root coordinate equations...
					/*for (int q=future-40; q<future+40; q++) {
						//y=mx+c;
						int imagex = der*(double)q + c;
						int imagey = der2*(double)q + c2;
						MyPutPixel(image, imagex, image->height-imagey, 255, 0, 255);
					}
					*/

					//perpendicular
					/*der  = -1.0/der;//div 0??
					der2 = -1.0/der2;
					c  = newx - (der *future);
					c2 = newy - (der2*future);
					for (int q=future-30; q<future+5; q++) {
						//y=mx+c;. Could just draw a cvLine as in displayTestGraphs above
						int imagex = der *(double)q + c;
						int imagey = der2*(double)q + c2;
						MyPutPixel(image, imagex, image->height-imagey, 64, 255, 255);
					}*/
			
					
					
					
					// Display fit...This best done below in the debug code...?
					/*for (int tt=0; tt<numPointsToFit*10+50; tt++){
						double ts=(double)tt/10;
						if (ts<numPointsToFit)
							MyPutPixel(image,  polynomial.Evaluate(ts), image->height-polynomial2.Evaluate(ts), 255, 255, 255);
						else
							MyPutPixel(image,  polynomial.Evaluate(ts), image->height-polynomial2.Evaluate(ts), 255, 0, 0);
					}
					cvShowImage("Image", image);/ //"*" removed from this position
					
				}
				else{
					//MessageBox::Show("G");
					//Don't have enough points to fit yet, so just go down. This only happens at Nsamples-1 in overarching loop so fill other samples
					for (int j=0; j<Nsamples; j++){
						s_new[j].x[t] = x + max(min(u + ProcessSigmaX * gaussian_random(), maxvel), -maxvel);	//old way
						s_new[j].y[t] = y + 1;
						s_new[j].u[t] = s_new[j].x[t] - x;
						s_new[j].v[t] = s_new[j].y[t] - y;
						s_new[j].au[t] = s_new[j].u[t] - u;
						s_new[j].av[t] = s_new[j].v[t] - v;
					}
				}


				if (displayTestGraphs) cvReleaseImage(&rootspacei);
			}*/

			
			/*else*/ if (/*useGravProcessModel*/motionMixture==0){
					// The original way
					s_new[i].x[t] = x + u+	   (ProcessSigmaX * gaussian_random());		//always advance 1 pix
					s_new[i].y[t] = y + 1;
					s_new[i].u[t] = s_new[i].x[t] - x;
					s_new[i].v[t] = s_new[i].y[t] - y;

				}
			/*else{
				//Asads new way

				// X
				if(stepCount>15 )
					{
						if(sliderValue == 0) //andy's model
							s_new[i].x[t] = x + u + ProcessSigmaX * gaussian_random();		
			   
						else if(sliderValue == 1) {// no momentum
							double gr = gaussian_random();
							s_new[i].x[t] = x + max(min(au+u + ProcessSigmaX * gr, maxvel), -maxvel);	//old way	
							//s_new[i].x[t] = x + u + ProcessSigmaX * gaussian_random();
							
							//random_component = ProcessSigmaX * gaussian_random();
							//s_new[i].x[t] = x + max(min(bestu + random_component, maxacc), -maxacc);
							

						}

			   
						else if(sliderValue > 1)// momentum
						{
							s_new[i].x[t] = x +  ( back[stepCount-2][0]- back[stepCount-sliderValue][0])/(sliderValue-1)  + ProcessSigmaX * gaussian_random() * 4.0/sliderValue;		
					}

						//else
						//	s_new[i].x[t] = x ;		//always advance 1 pix
					}
					else
						{
							//s_new[i].x[t] = x + u +  (ProcessSigmaX * gaussian_random());
							s_new[i].x[t] = x + max(min(u + ProcessSigmaX * gaussian_random(), maxvel), -maxvel);	//old way
				}

			
					
    			
    			
				// Y
				if(stepCount>15 )
					{

						if(sliderValue == 0){ //andy's model
							s_new[i].y[t] = y + 1;		
						}
			   
						else if(sliderValue == 1){// no momentum
							double gr = gaussian_random();
							s_new[i].y[t] = y + max(min(av+v + ProcessSigmaY * gr, maxvel), -maxvel);//old way	
							//s_new[i].y[t] = y + v + ProcessSigmaY * gaussian_random();//old way	
							
							//s_new[i].y[t] = y + max(min((bestv/bestu * random_component), maxacc), -maxacc);	

						}
			   
						else if(sliderValue > 1)// momentum
							s_new[i].y[t] = y +  ( back[stepCount-2][1]- back[stepCount-sliderValue][1])/(sliderValue-1)  + ProcessSigmaY * gaussian_random() * 4.0/sliderValue;		
			   
						
					

					
					
			        
					}
					else{
						s_new[i].y[t] = y + 1;
						//MessageBox::Show("<15, y+1");
					}
    			
    			
    			
					s_new[i].u[t] = s_new[i].x[t] - x;
					s_new[i].v[t] = s_new[i].y[t] - y;
					s_new[i].au[t] = s_new[i].u[t] - u;
					s_new[i].av[t] = s_new[i].v[t] - v;
					//if (s_new[i].v[t]>maxacc) MessageBox::Show("v = "+s_new[i].v[t], "step="+stepCount);

					

			}//end Asads way
			*/


		}//state==0 

		

		
		
		

		





		}//t

	} // i

	bool testPolynomialfit = false;//ie display it
	/*if (motionMixture>0 && testPolynomialfit){
		//Test polynomial fitting
		//const int polynomialOrder = 2;//2 good, 3 ok with more points (25?)
		//const int numPointsToFit = 30;//5 , 10 good
		if(stepCount>max(numPointsToFit+1, 15)) {



			array<double>^ arrayy= gcnew array<double>(numPointsToFit);
			for(int inp=0; inp < numPointsToFit; inp++)
				arrayy[inp] = bestPos[bestPosCounter-(numPointsToFit-inp)].x;

			// Build the matrix for the least-squares fitting
			//array<System::Double,2>^ m;
			//m = (array<System::Double,2>^)MathNet::Numerics::LinearAlgebra::Matrix::CreateMatrixData(10, polynomialOrder+1);

			//double m[10][];
			//for (int ii = 0; ii < 10; ii++)
			//{
			//	m[ii] = new double[ polynomialOrder+1];
			//}
			array<double,2>^ m = gcnew array<double, 2>(numPointsToFit,polynomialOrder+1);

			double distance = 0;
			for(int ii=0; ii < numPointsToFit; ii++)
			{
				double xi = distance;//was ii
				//MessageBox::Show("Dist = "+distance);
				//array<double>^ xrow;
				m[ii,0] = 1.0;
				for(int j=1; j < polynomialOrder+1; j++) {
					m[ii,j]  = xi * m[ii,j-1];
				}

				if (ii<numPointsToFit-1){ //can't do this last time through. Must be a better way of writing this?
					double xdis = abs(bestPos[bestPosCounter-(numPointsToFit-ii)].x - bestPos[bestPosCounter-(numPointsToFit-ii-1)].x);
					double ydis = abs(bestPos[bestPosCounter-(numPointsToFit-ii)].y - bestPos[bestPosCounter-(numPointsToFit-ii-1)].y);
					distance = distance + sqrt(xdis*xdis + ydis*ydis);

				}



			}

			// Find the least-squares solution
			MathNet::Numerics::LinearAlgebra::Matrix matrix = gcnew MathNet::Numerics::LinearAlgebra::Matrix(m);
			MathNet::Numerics::LinearAlgebra::Matrix solution = matrix.Solve(gcnew MathNet::Numerics::LinearAlgebra::Matrix(arrayy, arrayy->Length));

			// Extract the values (in our case into a polynomial for fast evaluation)
			MathNet::Numerics::Polynomial polynomial = gcnew MathNet::Numerics::Polynomial(solution.GetColumnVector(0));

			//Now do for y
			array<double>^ arrayy2= gcnew array<double>(numPointsToFit);
			for(int inp=0; inp < numPointsToFit; inp++)
				arrayy2[inp] = bestPos[bestPosCounter-(numPointsToFit-inp)].y;

			// Find the least-squares solution
			MathNet::Numerics::LinearAlgebra::Matrix matrix2 = gcnew MathNet::Numerics::LinearAlgebra::Matrix(m);
			MathNet::Numerics::LinearAlgebra::Matrix solution2 = matrix2.Solve(gcnew MathNet::Numerics::LinearAlgebra::Matrix(arrayy2, arrayy2->Length));

			// Extract the values (in our case into a polynomial for fast evaluation)
			MathNet::Numerics::Polynomial polynomial2 = gcnew MathNet::Numerics::Polynomial(solution2.GetColumnVector(0));



			//// Verify that the polynomial fits with less than 10% error for all given value pairs.
			//for(int i = 0; i < 10; i++)
			//{
			//	MathNet::Numerics::NumericAssert::AreAlmostEqual(y[i], polynomial.Evaluate(x[i]), 0.1, i.ToString());
			//}
			for (int tt=0; tt<distance*10*2; tt++){
				double ts=(double)tt/10;
				if (ts<distance)
					MyPutPixel(image,  polynomial.Evaluate(ts), image->height-polynomial2.Evaluate(ts), 255, 255, 255);
				else
					MyPutPixel(image,  polynomial.Evaluate(ts), image->height-polynomial2.Evaluate(ts), 255, 0, 0);
			}
			cvShowImage("Image", image);
			//if (i==0) MessageBox::Show("x = "+polynomial.Evaluate(12) );
			//cvWaitKey(0);
		}

	}*/ //test poly fit
}

// this method paints NSAMPLES * 4 times the particles on the screeen.
void CCondensation::paintParticleSet(IplImage* img, IplImage* img2, double r, double g, double b)
{
	int noiseX, noiseY;
	bool useCircle = false;
	bool breedSamples = false;

	for(int i=0; i<NSAMPLES; i++)
	{ 
		if (useCircle){
			cvCircle(img, cvPoint(s_new[i].x[0], s_new[i].y[0]), 2, CV_RGB(0,0,0), -1);
			if (img2!=NULL) cvCircle(img2, cvPoint(s_new[i].x[0], s_new[i].y[0]), 2, CV_RGB(0,0,0), -1);
		
		}else
		{
			/*img->imageData[((int)s_new[i].y[0]) * img->widthStep +  ((int)s_new[i].x[0]) * 3 +0] = b;
			img->imageData[((int)s_new[i].y[0]) * img->widthStep +  ((int)s_new[i].x[0]) * 3 +1] = g;
			img->imageData[((int)s_new[i].y[0]) * img->widthStep +  ((int)s_new[i].x[0]) * 3 +2] = r;*/
			MyPutPixel(img, s_new[i].x[0], img->height-s_new[i].y[0], r, g, b);
		if (img2!=NULL){ //draw on second image too, eg for display
			/*img2->imageData[((int)s_new[i].y[0]) * img2->widthStep +  ((int)s_new[i].x[0]) * 3 +0] = b;
			img2->imageData[((int)s_new[i].y[0]) * img2->widthStep +  ((int)s_new[i].x[0]) * 3 +1] = g;
			img2->imageData[((int)s_new[i].y[0]) * img2->widthStep +  ((int)s_new[i].x[0]) * 3 +2] = r;*/
			MyPutPixel(img2, s_new[i].x[0], img2->height-s_new[i].y[0], r, g, b);
		}
		}
	}

	// painting the extra particles, i am going to keep noise in the
	// y-direction negative, so i dont mess up the unprocessed pixels.
	// this seems to work fine :-)
	if (breedSamples)
	for(int j=0; j<3; j++)
	{
		for(int i=0; i< NSAMPLES; i++)
		{
			noiseX = (3.0 * uniform_random()-1.5);
			noiseY = (3.0 * uniform_random()-1.5);

			MyPutPixel(img, s_new[i].x[0]+noiseX, img->height-s_new[i].y[0]+noiseY, r, g, b);

			if (img2!=NULL){ //draw on second image too, eg for display
				MyPutPixel(img2, s_new[i].x[0]+noiseX, img2->height-s_new[i].y[0]+noiseY, r, g, b);
			}
		}
	} 	
}

void CCondensation::eval_observation(double* measures) {
	double cumul_total = 0;
	largest_cumulative_prob = 0;

    //cout << "eval_observation is wrong version for MCMC! "<<endl;
	//cout<<setprecision(15);
	for (int i=0; i<Nsamples; i++) {
		
		//likelyhood of target being underneath sample based on colour?
		s_new[i].pi = 1;
		for (int t=0; t<numTs; t++) {	// evaluates all t measures simultaneously
			s_new[i].pi  *= evaluate_gaussian((double)(measures[(t*Nsamples)+(i*3)]   - idealR)/255  , ObsSigmaColR) *
							evaluate_gaussian((double)(measures[(t*Nsamples)+(i*3+1)] - idealG)/255, ObsSigmaColG) *
							evaluate_gaussian((double)(measures[(t*Nsamples)+(i*3+2)] - idealB)/255 , ObsSigmaColB);
			//s_new[i].pi = s_new[i].pi*s_new[i].pi;
						
		}

		/*
		// BLOCKING: this code takes a long time to run!
		int xdist, ydist, xdiff, ydiff;
		for (int t=0; t<numTs; t++) {
			
			// new interaction weight
			
			int dist_thresh = (int)(s_new[i].r[t]*s_new[i].r[t]);//was +2
			for (int s=0; s<Nsamples; s++) { 
				for (int t2=0; t2<numTs; t2++) {
					if (t!=t2) { //don't compare with self
						xdiff = (int)s_new[i].x[t] - (int)s_new[s].x[t2];
						ydiff = (int)s_new[i].y[t] - (int)s_new[s].y[t2];
						xdist = xdiff*xdiff;
						ydist = ydiff*ydiff;
						if (xdist<dist_thresh&&ydist<dist_thresh) {				// a square for speed
							s_new[i].pi = 0;
							//cout <<"Blocking... xdist "<<xdist<<"; ydist "<<ydist<<endl;
						}
					}
				}//t2

			}//s
		}//t
		*/
		
		
		
	
		
		cumul_prob_array[i] = cumul_total;
		cumul_total += s_new[i].pi;
	} //i
	//cout<<setprecision(6);
	largest_cumulative_prob = cumul_total;
	//cout << "==largest_cumulative_prob = " << largest_cumulative_prob << endl;;
}

void CCondensation::swap_arrays() {
//swap ready for next cycle
	for (int i = 0; i<Nsamples; i++) {
		for (int t=0; t<numTs; t++) {
			s_old[i].x[t] = s_new[i].x[t];
			s_old[i].y[t] = s_new[i].y[t];
			s_old[i].u[t] = s_new[i].u[t];
			s_old[i].v[t] = s_new[i].v[t];
			s_old[i].au[t] = s_new[i].au[t];
			s_old[i].av[t] = s_new[i].av[t];
			s_old[i].state[t] = s_new[i].state[t];
			s_old[i].motion[t]= s_new[i].motion[t];
			//s_old[i].radius   = s_new[i].radius;
			
			s_old[i].pi= s_new[i].pi;
		}
	}

}

__inline double CCondensation::GetColourMeasureAtPos(int x, int y, double radius, IplImage * image, int flag) {
	// *Original*
	// Work out average colour, weighted to the centre of the radius
	// this is used by MCMC

	double colourweight[1000][2];
	int counter = 0;
	double average=0, sum=0, totalweights=0;
	unsigned char r, g, b;
	int rad = (int)ceil(radius);
	int rs = rad*rad; //max

	for (int i=-rad; i<rad; i++) {
		for (int j=-rad; j<rad; j++){
			
			if ((i*i+j*j)<rs) {
				// Within radius so use colour
				MyGetPixel(image, x+i, y+j, &r, &g, &b);
				if (flag==0) colourweight[counter][0] = (double)(r);		  // Colour
				if (flag==1) colourweight[counter][0] = (double)(g);
				if (flag==2) colourweight[counter][0] = (double)(b);

				colourweight[counter][1] = 1.0 - ((double)(i*i+j*j)/rs);      //weight
				
						
				totalweights+=colourweight[counter][1];
				counter++;
			}


		}
	}
	
	for (int i=0; i<counter; i++) {
		//calc weighted average
		sum += colourweight[i][0]*colourweight[i][1];
	}
	average = sum / totalweights;

	return average;
	
}

__inline double CCondensation::GetColourMeasureAtPos_uniform(int x, int y, double radius, IplImage * image, int flag) {
	// *Original*
	// Work out average colour, weighted to the centre of the radius
	// this is used by MCMC

	double colourweight[1000][2];
	int counter = 0;
	double average=0, sum=0, totalweights=0;
	unsigned char r, g, b;
	int rad = (int)ceil(radius);
	int rs = rad*rad; //max

	for (int i=-rad; i<rad; i++) {
		for (int j=-rad; j<rad; j++){
			
			if ((i*i+j*j)<rs) {
				// Within radius so use colour
				MyGetPixel(image, x+i, y+j, &r, &g, &b);
				if (flag==0) colourweight[counter][0] = (double)(r);		  // Colour
				if (flag==1) colourweight[counter][0] = (double)(g);
				if (flag==2) colourweight[counter][0] = (double)(b);

				colourweight[counter][1] = 1.0 /*- ((double)(i*i+j*j)/rs)*/;      //weight
						
				totalweights+=colourweight[counter][1];
				counter++;
			}


		}
	}
	
	for (int i=0; i<counter; i++) {
		//calc weighted average
		sum += colourweight[i][0]*colourweight[i][1];
	}
	average = sum / totalweights;

	return average;
	
}
 void CCondensation::setObsModelParams(double r, double g, double b, double r_sd, double g_sd, double b_sd)
{
	// Update appearance model stats.  Params in range 0-255.

	ObsSigmaColR = r_sd/255;
	ObsSigmaColG = g_sd/255;
	ObsSigmaColB = b_sd/255;

	idealR = (UCHAR)r;
	idealG = (UCHAR)g;
	idealB = (UCHAR)b;

	
}


 void CCondensation::incrementStepCount(void)
 {
	 stepCount++;
 }


 void CCondensation::setBack(int i, int j, int val)
 {
	 back[i][j] = val;

 }


 int CCondensation::showSamples(IplImage* m_image)
 {
	 for (int i=0; i<Nsamples; i++){
		 MyPutPixel(m_image, s_new[i].x[0], m_image->height - s_new[i].y[0], 255, 255, 0);
		 
	 }
	 
	 return 0;
 }


 void CCondensation::updateBestPos(void)
 {
	 bestPos[bestPosCounter].x = getWeightedAverageSample().x[0];//getBestX(0);
	 bestPos[bestPosCounter].y = getWeightedAverageSample().y[0];//getBestY(0);
	 bestPosCounter++;

 }


 int CCondensation::getNumPointsToFit(void)
 {
	 return numPointsToFit;
 }


 int CCondensation::setIdealRGB(int R, int G, int B)
 {
	 idealR = R;
	 idealG = G;
	 idealB = B;

	 return 0;
 }
