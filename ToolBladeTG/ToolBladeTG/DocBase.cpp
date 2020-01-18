#include "StdAfx.h"
#include "DocBase.h"
#include "AppBase.h"


CDocBase::CDocBase(void)
{
	Handle(Graphic3d_GraphicDriver) theGraphicDevice = 
		((CAppBase*)AfxGetApp())->GetGraphicDevice();
	
	myViewer = new V3d_Viewer(theGraphicDevice);
	myViewer->SetDefaultLights();
	myViewer->SetLightOn();
	myAISContext =new AIS_InteractiveContext(myViewer);
}


CDocBase::~CDocBase(void)
{
}
