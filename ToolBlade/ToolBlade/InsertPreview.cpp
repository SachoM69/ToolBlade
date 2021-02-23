#include "stdafx.h"
#include "InsertPreview.h"

CInsertPreview::CInsertPreview(Handle_AIS_InteractiveContext thiscontext) : PreviewContext(thiscontext)
{

}

void CInsertPreview::Preview(std::shared_ptr<IIndexableInsert> ins)
{
	if (PreviewContext->DisplayStatus(PreviewShape) == AIS_DS_Displayed) PreviewContext->Erase(PreviewShape, false);
	PreviewShape = new AIS_Shape(ins->GetResultingShape());
	PreviewContext->SetDisplayMode(AIS_Shaded, false);
	PreviewContext->Display(PreviewShape, false);
}

void CInsertPreview::Preview(std::shared_ptr<IIndexableInsertSeated> ori)
{
	if (PreviewContext->DisplayStatus(PreviewShape) == AIS_DS_Displayed) PreviewContext->Erase(PreviewShape, false);
	PreviewShape = new AIS_Shape(ori->RotatedIntoPlace());
	PreviewContext->SetDisplayMode(AIS_Shaded, false);
	PreviewContext->Display(PreviewShape, false);
}

void CInsertPreview::Preview(std::shared_ptr<IIndexableInsertTool> tool)
{
}

void CInsertPreview::DestroyPreview()
{
	PreviewContext->Erase(PreviewShape, false);
}
