// StatsWriter.cpp: implementation of the StatsWriter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StatsWriter.h"
#include <iostream>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace std;
using namespace System::Windows::Forms;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/* Constructor:- Takes a CString as the path to the file to be opened for writing
	eg "C:\stats\torchtracker\results.csv" Currently csv is not appended this could
	be done in a future update
*/

CStatsWriter::CStatsWriter()
{
	m_fout.open("measures.csv");
	
	if (!m_fout.is_open()) 
	{
		cout << "Cannot open logfile measures.csv"<< endl;
		//MessageBox::Show("Cannot open logfile measures.csv. Is it open already??"); //K_I
	}
}

CStatsWriter::CStatsWriter(char* filepath)
{
	m_fout.open(filepath);
	char cfilepath[512];
	sprintf(cfilepath, "Cannot open logfile %s. Is it open already??", filepath);
	System::String ^s = gcnew System::String(cfilepath);
	

	if (!m_fout.is_open()) 
	{
		cout << "Cannot open logfile"<<filepath<< endl;
		MessageBox::Show(s);
	}
}

/* Destructor:- Closes the currently open file
*/

CStatsWriter::~CStatsWriter()
{
	if (m_fout.is_open()) {
		m_fout.close();
	}


}

/* WriteTitles:- Takes a CString of comma separated values that form the headers of
	columns in the spreadsheet eg "Frame_No, PosX, PosY"
*/

void CStatsWriter::WriteTitles(char* s)
{
	if (m_fout.is_open()) {
		m_fout << s << endl;
		//m_fout << endl;  //  extra blank row after titles
	} else {
		cout <<"WARNING: CANNOT WRITE TO FILE"<<endl;
		MessageBox::Show("Cannot write to logfile. Is it open already??");
	}
}

/* AppendDataRow:- Writes data as comma separated values that form the rows of
	the spreadsheet eg "7110, 125, 24"
	Example Use:- 
		Separate fields with commas
		To store integers etc.  The string must be formed as follows:
			char  st[40];
			sprintf(st, "test1, test2, test3, %i", intensity);
			stats.AppendDataRow(CString(st));
*/

void CStatsWriter::AppendDataRow(float* FPP, int arrsize)	// append an enire array of floats to file
{
	char st[40];

	if (m_fout.is_open() ) 
	{
		for (int x = 0; x < arrsize; x++)
		{
			sprintf(st, "%f,", FPP[x]);
			m_fout << st;
		}

		m_fout << endl;

	} else cout <<"WARNING: CANNOT WRITE TO FILE"<<endl;
}

void CStatsWriter::AppendDataRow(char* s)
{

	if (m_fout.is_open() ) 
	{
		m_fout << s << endl;
	} else {
		cout <<"WARNING: CANNOT WRITE TO FILE"<<endl;
		MessageBox::Show("Cannot write to logfile. Is it open already??");
	}
	
}

/* ChangeFile:- Allows writing to a different file (see constructor)
*/

void CStatsWriter::ChangeFile(char* s)
{
	if (m_fout.is_open()) {
		m_fout.close();
	}

	m_fout.open(s);

	char cfilepath[512];
	sprintf(cfilepath, "Cannot open logfile %s. Is it open already??", s);
	System::String ^ss = gcnew System::String(cfilepath);
	
	if (!m_fout.is_open()) 
	{
		cout <<"WARNING: CANNOT WRITE TO FILE"<<endl;
		MessageBox::Show(ss);
	}

}

void CStatsWriter::AppendDataRow_NoReturn(char *s)
{
	if (m_fout.is_open() ) 
	{
		m_fout << s <<",";
	} else {
		cout <<"WARNING: CANNOT WRITE TO FILE"<<endl;
		MessageBox::Show("Cannot write to logfile. Is it open already??");
	}
}

void CStatsWriter::CarriageReturn()
{
if (m_fout.is_open() ) 
	{
		m_fout << endl;
	} else {
		cout <<"WARNING: CANNOT WRITE TO FILE"<<endl;
		MessageBox::Show("Cannot write to logfile. Is it open already??");
	}
}
