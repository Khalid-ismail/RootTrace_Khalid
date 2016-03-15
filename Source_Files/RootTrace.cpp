// RootTrace2.cpp : main project file.

//#include "stdafx.h"

#include "RootTrace.h"
#include "ReadFile.h"
#include "Form1.h"

using namespace RootTrace2;

// [STAThreadAttribute] //K_I
int main(int argc, char *argv[]/*array<System::String ^> ^args*/) //K_I
{
	
	// Enabling Windows XP visual effects before any controls are created
	//Application::EnableVisualStyles(); //K_I
	//Application::SetCompatibleTextRenderingDefault(false); //K_I

	// Create the main window and run it
	//Application::Run(gcnew Form1()); //K_I
	int i;
	UCHAR tb_lr = 0;

	char filename[100];
	for(i=0;i<100;i++)
	{
		if(argv[1][i] == '.') {break;} // chop off file extension (if found)
		else if(argv[1][i] == '\0'){break;}
		else {filename[i] = argv[1][i];}
	}

	read_file(argv[1]);

	//m_main.init(filename, idealr, idealg, idealb, tb_lr, processX, processY/*, this->motionMixture*//*, this*/); //K_I
	m_main.init(filename, idealr, idealg, idealb, tb_lr, processX, processY);

	return 0;
}
