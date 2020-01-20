#pragma once

struct ToolData
{
	int ToolType;
	int UsedToothCount;
	int ActualToothCount;
	bool CutDirection;
};

struct IndInsAttributes
{
	bool IsDisabled;
};

interface IInstrInsList abstract
{
	virtual HRESULT QueryIndInsertInformation(int index, IndInsert*) = 0;
	virtual HRESULT UpdateIndInsertInformation(int index, const IndInsert*) = 0;
	virtual HRESULT QueryIndInsertAttributes(int index, IndInsAttributes*) = 0;
	virtual HRESULT UpdateIndInsertAttributes(int index, const IndInsAttributes*) = 0;
	virtual HRESULT QueryIndInsertObject(int index, const CIndexableInsert**) = 0;
	virtual HRESULT QueryToolData(ToolData*) = 0;
	virtual HRESULT RequestNewInsert(int* index_inout) = 0;
	virtual HRESULT RequestRemoveInsert(int index) = 0;
	virtual HRESULT ShowPoint(gp_Pnt, bool) = 0;
	virtual HRESULT RefreshCutter(int index, IndInsert*) = 0;
};