#include "ToolBlade.h"
#include <list>

class CIndexableInsertTool :
	public IIndexableInsertTool
{
public:
	CIndexableInsertTool(double diameter, ToolType TT, DirToolType DTT);
	// Унаследовано через IIndexableInsertTool
	virtual int InsertCount() const override;
	virtual void AppendInsert(std::shared_ptr<IIndexableInsertSeated>) override;
	virtual void RemoveInsert(int index) override;
	virtual std::shared_ptr<IIndexableInsertSeated> GetInsert(int index) const override;
	virtual void SwapInsert(int index, std::shared_ptr<IIndexableInsertSeated>) override;
	virtual gp_Vec ToolAxis() const override;
	virtual std::shared_ptr<IIndexableInsertSeated> operator[](int index) const override;
	virtual ToolType Type() const override;
	virtual DirToolType CutDirection() const override;

	IFSelect_ReturnStatus LoadShapeFromSTEP(const char* path);
	virtual Handle(TopTools_HSequenceOfShape) GetShape() const override;

private:
	ToolType type;
	DirToolType Dir;
	double tool_diam;
	std::vector<std::shared_ptr<IIndexableInsertSeated>> IndInserts;
	Handle(TopTools_HSequenceOfShape) aHSequenceOfShape;

	// Унаследовано через IIndexableInsertTool
	virtual IFSelect_ReturnStatus LoadShapeFromSTEP(const wchar_t* path) override;
	virtual void SetType(ToolType) override;
	virtual void SetCutDirection(DirToolType) override;
};
