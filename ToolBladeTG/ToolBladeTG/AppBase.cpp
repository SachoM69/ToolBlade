#include "StdAfx.h"
#include "AppBase.h"

#include <OpenGl_GraphicDriver.hxx>

CAppBase::CAppBase(void)
{
	/*Standard_Transient st();
	st= new Standard_Transient();*/
	try
	{
		Handle(Aspect_DisplayConnection) aDisplayConnection;
		myGraphicDevice = new OpenGl_GraphicDriver (aDisplayConnection);
	}
	catch(Standard_Failure)
	{
		AfxMessageBox(LPCTSTR("Fatal error during graphic initialization"), MB_ICONSTOP);
		ExitProcess(1);
	}
}


CAppBase::~CAppBase(void)
{
}
