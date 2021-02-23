// ToolBlade.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <math.h>
#include "Degrees.hxx"
#include "IndexableIns.h"
#include "CuttingTooth.h"

__declspec(dllexport) std::shared_ptr<IIndexableInsert> CreateInsert(const IndInsParameters* IIt)
{
	std::shared_ptr<IIndexableInsert> PrefdIndIns = std::make_shared<CIndexableInsert>(IIt);
	return PrefdIndIns;
}

__declspec(dllexport) std::shared_ptr<IIndexableInsertSeated> OrientInsert(IIndexableInsert* II, const IndInsOrientation* IIo)
{
	CIndexableInsert* PrefdIndIns = dynamic_cast<CIndexableInsert*>(II);
	std::shared_ptr<CIndInsTooth> mi = std::make_shared<CIndInsTooth>(IIo->Gamma, IIo->Phi, IIo->Lambda, IIo->Diameter, IIo->Type, IIo->Dir, PrefdIndIns);
	//CIndInsTooth* mi = new CIndInsTooth(IIo->Gamma, IIo->Phi, IIo->Lambda, IIo->Diameter, IIo->Type, IIo->Dir, PrefdIndIns);
	mi->SetTipParameters(IIo->EdgeIndex, IIo->EdgePosition, IIo->AxisRotation, IIo->Zoffset);
	mi->CalcCutterAngles();
	return mi;
}