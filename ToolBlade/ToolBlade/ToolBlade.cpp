// ToolBlade.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
//#include "ToolBlade.h"
#include <math.h>
#include "Degrees.hxx"
#include "IndexableIns.h"
//#include "IndexableIns.cpp"

Handle_AIS_Shape CuttingPlate;
//јбсолютно бесполезна. Ќе использовать.
__declspec(dllexport) void SetWndHandle(HWND hWnd)
{
	//Handle_WNT_Window BlWnd;
	//BlWnd=Handle_WNT_Window();
	//Handle_WNT_WClass TPaint = Handle_WNT_WClass();
	//WNT_Window ImgBl=WNT_Window(Handle_WNT_GraphicDevice(BlWnd->GraphicDevice()), "title", TPaint);
	/*//V3d_Viewer* UV=new V3d_Viewer(*(new Handle_Aspect_GraphicDevice()), Standard_ExtString("TViewer"), "", 1000., V3d_XposYposZpos, Quantity_NOC_NAVYBLUE, V3d_ZBUFFER, V3d_COLOR, V3d_ASAP, (Standard_Boolean)1, (Standard_Boolean)1, V3d_TEX_ALL);
	Handle(V3d_Viewer) *ViewerFTWnd = new Handle_V3d_Viewer();
	V3d_View* ViewFTWnd;// = new V3d_View(*ViewerFTWnd, V3d_PERSPECTIVE);
	Handle_Graphic3d_WNTGraphicDevice Painter;
	//Handle_WNT_GraphicDevice& Painter = Handle_WNT_GraphicDevice(&PrePainter);
	Handle_WNT_Window* ImgBladeInThere = new Handle_WNT_Window();
	WNT_Window ImgBl=WNT_Window(Painter, "title", TPaint);
	ImgBladeInThere = new Handle_WNT_Window(&ImgBl);
	(*Painter, 
		Standard_CString("Blade Window"),  
		*TPaint);
	ImgBladeInThere;*/
}

//ƒобавл€ет формы режущей пластины в контекст
/*	__declspec(dllexport) void InsertShape(Handle_AIS_InteractiveContext AISC, Standard_CString AllToolData, Standard_Boolean Reconstruct)
{
	if(AISC->DisplayStatus(CuttingPlate)==AIS_DS_Displayed) AISC->Erase(CuttingPlate);
	if (Reconstruct) 
	{
		AISC->SetDisplayMode(AIS_Shaded);
		CIndexableIns* PrefdIndIns = new CIndexableIns(AllToolData);
		PrefdIndIns->ReadRealVals();
		CuttingPlate = new AIS_Shape(PrefdIndIns->ConstructToolBlade());
		delete PrefdIndIns;
	}
	AISC->Display(CuttingPlate, true);
}*/

/*__declspec(dllexport) Handle_AIS_Shape ConstructShape(Standard_CString AllToolData, Standard_Boolean Reconstruct)
{
	if (Reconstruct)
	{
		CIndexableIns* PrefdIndIns = new CIndexableIns(AllToolData);
		PrefdIndIns->ReadRealVals();
		CuttingPlate = new AIS_Shape(PrefdIndIns->ConstructToolBlade());
		delete PrefdIndIns;
	}
	return CuttingPlate;
}*/

//ƒобавл€ет формы режущей пластины в контекст с данными, передаваемыми через запись
	__declspec(dllexport) CIndexableInsert* InsertShape(Handle_AIS_InteractiveContext AISC, const IndInsert* IIt, Standard_Boolean Reconstruct)
{
	if(Reconstruct && AISC->DisplayStatus(CuttingPlate)==AIS_DS_Displayed) AISC->Erase(CuttingPlate, true);
	CIndexableIns* PrefdIndIns = new CIndexableIns(IIt);
	if (Reconstruct) 
	{
		CuttingPlate = new AIS_Shape(PrefdIndIns->ConstructToolBlade());
		AISC->SetDisplayMode(AIS_Shaded, true);
		AISC->Display(CuttingPlate, true);
	} else
	{
		PrefdIndIns->ConstructToolBlade();
	}
	return PrefdIndIns;
}