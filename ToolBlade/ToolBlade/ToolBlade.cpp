// ToolBlade.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <math.h>
#include "Degrees.hxx"
#include "ToolBlade.h"
#include "IndexableIns.h"
#include "CuttingTooth.h"
#include "IndexableInsertTool.h"
#include "InsertPreview.h"

__declspec(dllexport) std::shared_ptr<IIndexableInsert> CreateInsert(const IndInsParameters* IIt)
{
	std::shared_ptr<IIndexableInsert> PrefdIndIns = std::make_shared<CIndexableInsert>(IIt);
	return PrefdIndIns;
}

__declspec(dllexport) std::shared_ptr<IIndexableInsertSeated> OrientInsert(std::shared_ptr<IIndexableInsert> II, const IndInsOrientation* IIo)
{
	std::shared_ptr<CIndexableInsert> PrefdIndIns = std::dynamic_pointer_cast<CIndexableInsert>(II);
	std::shared_ptr<CIndInsTooth> mi = std::make_shared<CIndInsTooth>(IIo->Gamma, IIo->Phi, IIo->Lambda, IIo->Diameter, IIo->Type, IIo->Dir, PrefdIndIns);
	//CIndInsTooth* mi = new CIndInsTooth(IIo->Gamma, IIo->Phi, IIo->Lambda, IIo->Diameter, IIo->Type, IIo->Dir, PrefdIndIns);
	mi->SetTipParameters(IIo->EdgeIndex, IIo->EdgePosition, IIo->AxisRotation, IIo->Zoffset);
	mi->CalcCutterAngles();
	return mi;
}

std::shared_ptr<IIndexableInsertTool> CreateTool(const IndInsToolParams* params)
{
	std::shared_ptr<IIndexableInsertTool> tool = std::make_shared<CIndexableInsertTool>(params->Diameter, params->Type, params->Dir);
	return tool;
}

std::shared_ptr<IIndexableInsertTool> CreateToolPatterned(const IndInsToolParams* params, const IndInsOrientation* IIt,
	std::shared_ptr<IIndexableInsert> basic_insert, PPatterningRule rule, int count)
{
	IndInsOrientation orient_copy = *IIt;
	std::shared_ptr<IIndexableInsertTool> tool = std::make_shared<CIndexableInsertTool>(params->Diameter, params->Type, params->Dir);
	for (int i = 0; i < count; i++)
	{
		rule(i, &orient_copy.AxisRotation, &orient_copy.Zoffset);
		tool->AppendInsert(OrientInsert(basic_insert, &orient_copy));
	}
	return tool;
}

std::shared_ptr<IInsertPreview> CreateInsertPreview(Handle_AIS_InteractiveContext aisc)
{
	std::shared_ptr<IInsertPreview> preview = std::make_shared<CInsertPreview>(aisc);
	return preview;
}
