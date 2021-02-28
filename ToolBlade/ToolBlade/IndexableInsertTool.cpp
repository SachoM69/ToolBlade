#include "stdafx.h"
#include "IndexableInsertTool.h"

CIndexableInsertTool::CIndexableInsertTool(double diameter, ToolType TT, DirToolType DTT):
	tool_diam(diameter), type(TT), Dir(DTT)
{
	aHSequenceOfShape = new TopTools_HSequenceOfShape();
}

int CIndexableInsertTool::InsertCount() const
{
	return int(IndInserts.size());
}

void CIndexableInsertTool::AppendInsert(std::shared_ptr<IIndexableInsertSeated> new_insert)
{
	IndInserts.push_back(new_insert);
}

void CIndexableInsertTool::RemoveInsert(int index)
{
	IndInserts.erase(IndInserts.begin());

}

std::shared_ptr<IIndexableInsertSeated> CIndexableInsertTool::GetInsert(int index) const
{
	return IndInserts[index];
}

void CIndexableInsertTool::SwapInsert(int index, std::shared_ptr<IIndexableInsertSeated> new_insert)
{
	IndInserts[index] = new_insert;
}

gp_Vec CIndexableInsertTool::ToolAxis() const
{
	switch (type)
	{
	case Drilling_Tool:
		return gp_Vec(-tool_diam, 0, 0);
	case Milling_Tool:
		return gp_Vec(0, tool_diam * (Dir == DirTool_Right ? 1 : -1), 0);
	default:
		return gp_Vec();
	}
}

std::shared_ptr<IIndexableInsertSeated> CIndexableInsertTool::operator[](int index) const
{
	return GetInsert(index);
}

ToolType CIndexableInsertTool::Type() const
{
	return type;
}

DirToolType CIndexableInsertTool::CutDirection() const
{
	return Dir;
}

IFSelect_ReturnStatus CIndexableInsertTool::LoadShapeFromSTEP(const char* path)
{
	aHSequenceOfShape->Clear();

	// create additional log file
	STEPControl_Reader aReader;
	IFSelect_ReturnStatus status = aReader.ReadFile(path);
	if (status != IFSelect_RetDone)
		return status;

	aReader.WS()->TransferReader()->TransientProcess()->SetTraceLevel(2); // increase default trace level

	Standard_Boolean failsonly = Standard_False;
	aReader.PrintCheckLoad(failsonly, IFSelect_ItemsByEntity);

	// Root transfers
	Standard_Integer nbr = aReader.NbRootsForTransfer();
	aReader.PrintCheckTransfer(failsonly, IFSelect_ItemsByEntity);
	for (Standard_Integer n = 1; n <= nbr; n++) {
		/*Standard_Boolean ok =*/ aReader.TransferRoot(n);
	}

	// Collecting resulting entities
	Standard_Integer nbs = aReader.NbShapes();
	if (nbs == 0) {
		return IFSelect_RetVoid;
	}
	for (Standard_Integer i = 1; i <= nbs; i++) {
		aHSequenceOfShape->Append(aReader.Shape(i));
	}

	return status;
}

Handle(TopTools_HSequenceOfShape) CIndexableInsertTool::GetShape() const
{
	return aHSequenceOfShape;
}

IFSelect_ReturnStatus CIndexableInsertTool::LoadShapeFromSTEP(const wchar_t* path)
{
	return IFSelect_ReturnStatus();
}

void CIndexableInsertTool::SetType(ToolType t)
{
	type = t;
}

void CIndexableInsertTool::SetCutDirection(DirToolType d)
{
	Dir = d;
}
