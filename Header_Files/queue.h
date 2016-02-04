#include "cv.h"
#include "highgui.h"
#include <ctype.h>

#include <math.h>

#include "../Header_Files/ccnode.h"
class spque
{
public:
	ccnode * head;
	ccnode* tail;
	int numberOfccnodes;

	spque();
	void emptyQue();
	bool isEmpty();
	void enque(CvPoint pt);
	ccnode* deque();
	ccnode* dequemid(ccnode* ptr);
};
//*********************************************
spque::spque()
{
	head = 0;
	tail = 0;
	numberOfccnodes = 0;
}
//*********************************************
void spque::enque(CvPoint pt)
{

	ccnode *n = new ccnode(pt);

	if(numberOfccnodes ==0)
	{
		numberOfccnodes++;
		head = n;
		tail = n;
	}
	else
	{
		numberOfccnodes++;
		tail->next = n;
		tail = tail->next;
	}
}


ccnode* spque::deque()
{
	if(head == 0)
	{		
		return 0;
	}
	else if(numberOfccnodes == 1)
	{	
		ccnode* temp = head;
		tail = 0;
		head = 0;
		numberOfccnodes--;
		return temp;
	}
	else
	{
		ccnode* temp = head;
		head = head->next;		
		numberOfccnodes--;
		return temp;
	}
}




ccnode* spque::dequemid(ccnode* ptr)
{

	if(ptr == head)
	{	
		ccnode* temp = head;		
		head = head->next;
		numberOfccnodes--;
		return temp;
	}

	else if(ptr == tail)
	{
		ccnode* temp = head;
		while(temp->next != tail)
			temp = temp->next;
		tail = temp;
		tail->next = 0;
		
		numberOfccnodes--;
		return ptr;

	}

	else
	{
		ccnode* temp = head;
		while(temp->next != ptr)
			temp = temp->next;
		
		temp->next = ptr->next;
		numberOfccnodes--;
		return ptr;
	}
}
	
		



bool spque::isEmpty()
{
	if(head == 0)
		return true;
	else 
		return false;
}


void spque::emptyQue()
{
	ccnode* temp = head;
	while(!isEmpty())
		deque();

}
