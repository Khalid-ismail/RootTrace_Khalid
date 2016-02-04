 #include "cv.h"
#include "highgui.h"
#include <ctype.h>

#include <math.h>

#include "../Header_Files/ccnode.h"




class spstack
{
public:
	ccnode * MasterPtr;
	long numberOfccnodes;

	spstack();
	void emptyspstack();
	bool isEmpty();
	void push(ccnode*);	
	ccnode* pop();

};
//*********************************************

spstack::spstack()
{
	MasterPtr = 0;
	numberOfccnodes = 0;
}
//*********************************************
void spstack::push(ccnode* n1)
{

	ccnode *n = new ccnode(n1->pt);
	numberOfccnodes++;
	if(MasterPtr == 0)
		MasterPtr = n;
	else 
	{
		n->next = MasterPtr;
		MasterPtr = n;
	}
}


//*********************************************

ccnode* spstack::pop()
{
	if(MasterPtr == 0)
		return 0;

	if(numberOfccnodes ==1)
	{
		ccnode *temp = MasterPtr;
		MasterPtr = NULL;
		numberOfccnodes--;
		return temp;
	}

	if(MasterPtr != NULL && numberOfccnodes>1)
	{
		numberOfccnodes--;
		ccnode * temp = MasterPtr;
		MasterPtr = MasterPtr->next;
		return temp;
	
	}

		

}
//*********************************************
bool spstack::isEmpty()
{
	if(MasterPtr == 0 ||numberOfccnodes == 0 )
		return true;
	else
		return false;
}


void spstack::emptyspstack()
{
	while(MasterPtr != 0)
		pop();

}



