#include "stdafx.h"
#include "InsertPreview.h"

CInsertPreview::CInsertPreview(Handle_AIS_InteractiveContext thiscontext) : PreviewContext(thiscontext)
{

}

void CInsertPreview::Preview(std::shared_ptr<IIndexableInsert> ins)
{
	DestroyPreview();
	PreviewShape.Append(new AIS_Shape(ins->GetResultingShape()));
	PreviewContext->SetDisplayMode(AIS_Shaded, false);
	for each (auto shape in PreviewShape)
	{
		PreviewContext->Display(shape, false);
	} 
}

void CInsertPreview::Preview(std::shared_ptr<IIndexableInsertSeated> ori)
{
	DestroyPreview();
	PreviewShape.Append(new AIS_Shape(ori->RotatedIntoPlace()));
	PreviewContext->SetDisplayMode(AIS_Shaded, false);
	for each (auto shape in PreviewShape)
	{
		PreviewContext->Display(shape, false);
	}
}

void CInsertPreview::Preview(std::shared_ptr<IIndexableInsertTool> tool)
{
	DestroyPreview();
	auto tool_shapes = tool->GetShape();
	for each (auto topo_shape in *tool_shapes)
		PreviewShape.Append(new AIS_Shape(topo_shape));
	for (int i = 0; i < tool->InsertCount(); i++)
	{
		PreviewShape.Append(new AIS_Shape(tool->GetInsert(i)->RotatedIntoPlace()));
	}
	for each (auto shape in PreviewShape)
	{
		PreviewContext->Display(shape, false);
	}
}

void CInsertPreview::DestroyPreview()
{
	for each (auto shape in PreviewShape)
	{
		if (PreviewContext->DisplayStatus(shape) == AIS_DS_Displayed) PreviewContext->Erase(shape, false);
	}
	PreviewShape.Clear();
}
