                   

#ifndef ccnode_H
#define ccnode_H

#include "cv.h"
#include "highgui.h"



struct ccnode
{

	ccnode * next;
	CvPoint pt;
	

	ccnode(CvPoint pnt)
	{
		pt.x = pnt.x;
		pt.y = pnt.y;
		next = NULL;
		
	}
		
};

#endif