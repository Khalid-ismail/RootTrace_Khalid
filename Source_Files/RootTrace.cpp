// RootTrace2.cpp : main project file.

#include "stdafx.h"
#include "Form1.h"

using namespace RootTrace2;

// [STAThreadAttribute] //K_I
int main(/*array<System::String ^> ^args*/) //K_I
{
	
	// Enabling Windows XP visual effects before any controls are created
	//Application::EnableVisualStyles(); //K_I
	//Application::SetCompatibleTextRenderingDefault(false); //K_I

	// Create the main window and run it
	Application::Run(gcnew Form1());
	return 0;
}
