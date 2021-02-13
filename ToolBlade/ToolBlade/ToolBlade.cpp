// ToolBlade.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
//#include "ToolBlade.h"
#include <math.h>
#include "Degrees.hxx"
#include "IndexableIns.h"
#include "CuttingTooth.h"
//#include "IndexableIns.cpp"
Handle_AIS_Shape DisplayedShape;
Handle_AIS_InteractiveContext thisAISC;
IIndexableInsert* DisplayedInsert;

__declspec(dllexport) IIndexableInsert* CreateInsert(const IndInsParameters* IIt)
{
	CIndexableInsert* PrefdIndIns = new CIndexableInsert(IIt);
	PrefdIndIns->ConstructToolBlade();
	return dynamic_cast<IIndexableInsert*>(PrefdIndIns);
}

//Добавляет формы режущей пластины в контекст с данными, передаваемыми через запись
__declspec(dllexport) IIndexableInsert* CreateInsertAndPreview(Handle_AIS_InteractiveContext AISC, const IndInsParameters* IIt)
{
	IIndexableInsert* iii = CreateInsert(IIt);
	if(AISC->DisplayStatus(DisplayedShape)==AIS_DS_Displayed) AISC->Erase(DisplayedShape, true);
	DisplayedShape = new AIS_Shape(iii->ConstructToolBlade());
	AISC->SetDisplayMode(AIS_Shaded, true);
	AISC->Display(DisplayedShape, true);
	thisAISC = AISC;
	return iii;
}

__declspec(dllexport) IIndexableInsertSeated* OrientInsert(IIndexableInsert* II, const IndInsOrientation* IIo)
{
	CIndexableInsert* PrefdIndIns = dynamic_cast<CIndexableInsert*>(II);
	//CIndInsTooth* mi = new CIndInsTooth(DEG(20), DEG(40), DEG(0), IIo->Diameter, IIo->Type, IIo->Dir, PrefdIndIns);
	CIndInsTooth* mi = new CIndInsTooth(IIo->Gamma, IIo->Phi, IIo->Lambda, IIo->Diameter, IIo->Type, IIo->Dir, PrefdIndIns);
	mi->SetTipParameters(IIo->PointIndex, IIo->EdgePosition, IIo->AxisRotation, IIo->Zoffset);
	mi->CalcCutterAngles();
	return mi;
}

//Добавляет формы режущей пластины в контекст с данными, передаваемыми через запись
__declspec(dllexport) IIndexableInsertSeated* OrientInsertAndPreview(Handle_AIS_InteractiveContext AISC, IIndexableInsert* II, const IndInsOrientation* IIo)
{
	if (AISC->DisplayStatus(DisplayedShape) == AIS_DS_Displayed) AISC->Erase(DisplayedShape, true);
	CIndInsTooth* mi = dynamic_cast<CIndInsTooth*>(OrientInsert(II, IIo));
	DisplayedShape = new AIS_Shape(mi->RotatedIntoPlace());
	AISC->SetDisplayMode(AIS_Shaded, true);
	AISC->Display(DisplayedShape, true);
	thisAISC = AISC;
	return mi;
}


__declspec(dllexport) void DestroyInsert(const IIndexableInsert* II)
{
	const CIndexableInsert* PrefdIndIns = dynamic_cast<const CIndexableInsert*>(II);
	if (DisplayedInsert == II)
	{
		if (!thisAISC.IsNull() && thisAISC->DisplayStatus(DisplayedShape) == AIS_DS_Displayed) thisAISC->Erase(DisplayedShape, true);
		thisAISC.Nullify();
		DisplayedShape.Nullify();
		DisplayedInsert = nullptr;
	}
	delete PrefdIndIns;
}

__declspec(dllexport) void DestroyInsert(const IIndexableInsertSeated* IIo)
{
	const CIndInsTooth* PrefdIndIns = dynamic_cast<const CIndInsTooth*>(IIo);
	if (DisplayedInsert == PrefdIndIns->IndIns)
	{
		if (!thisAISC.IsNull() && thisAISC->DisplayStatus(DisplayedShape) == AIS_DS_Displayed) thisAISC->Erase(DisplayedShape, true);
		thisAISC.Nullify();
		DisplayedShape.Nullify();
		DisplayedInsert = nullptr;
	}
	delete PrefdIndIns;
}