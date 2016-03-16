// StatsWriter.h: interface for the StatsWriter class.
//
//////////////////////////////////////////////////////////////////////

#include <fstream>
//#include "../Header_Files/stdafx.h"
#include <iostream>


using namespace std;
class CStatsWriter  
{
public:
	void CarriageReturn();
	void AppendDataRow_NoReturn(char* s);
	void ChangeFile(char* s);			//DOES NOT WORK at present
	void AppendDataRow(float* FPP, int arrsize);
	void AppendDataRow(char* s);		//Add a row of comma-delimited data
	void WriteTitles(char* s);		//Add the row of coloumn headers.  This should be called first.  Headers should be comma-delimited.
	CStatsWriter();
	CStatsWriter(char* filepath);
	virtual ~CStatsWriter();
	ofstream m_fout;
};


