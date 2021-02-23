#pragma once
#include "ToolBlade.h"
class CInsertPreview :
	public IInsertPreview
{
	CInsertPreview(Handle_AIS_InteractiveContext thiscontext);
	// Унаследовано через IInsertPreview
	virtual void Preview(std::shared_ptr<IIndexableInsert>) override;
	virtual void Preview(std::shared_ptr<IIndexableInsertSeated>) override;
	virtual void Preview(std::shared_ptr<IIndexableInsertTool>) override;
	virtual void DestroyPreview() override;

protected:
	Handle_AIS_Shape PreviewShape;
	Handle_AIS_InteractiveContext PreviewContext;
};

