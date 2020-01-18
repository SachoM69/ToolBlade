#pragma once
#include "afxwinappex.h"
#include <Standard_Macro.hxx>
class CAppBase :
	public CWinAppEx
{
public:
	CAppBase(void);
	~CAppBase(void);
public:
	Handle(Graphic3d_GraphicDriver) GetGraphicDevice() const { return myGraphicDevice; } ;
protected:
	Handle(Graphic3d_GraphicDriver) myGraphicDevice;
};

