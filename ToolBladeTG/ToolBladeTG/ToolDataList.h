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
	virtual HRESULT QueryIndInsInformation(int index, IndInsParameters*) = 0;
	virtual HRESULT UpdateIndInsInformation(int index, const IndInsParameters*) = 0;
	virtual HRESULT QueryIndInsAttributes(int index, IndInsAttributes*) = 0;
	virtual HRESULT UpdateIndInsAttributes(int index, const IndInsAttributes*) = 0;
	virtual HRESULT QueryIndInsOrientation(int index, IndInsOrientation*) = 0;
	virtual HRESULT UpdateIndInsOrientation(int index, const IndInsOrientation*) = 0;
	virtual HRESULT QueryIndInsObject(int index, const IIndexableInsert**) = 0;
	virtual HRESULT QueryIndInsObjectSeated(int index, const IIndexableInsertSeated**) = 0;
	virtual HRESULT QueryToolData(ToolData*) = 0;
	virtual HRESULT UpdateToolType(int type) = 0;
	virtual HRESULT UpdateToolDirection(bool dir) = 0;
	virtual HRESULT RequestNewInsert(int* index_inout) = 0;
	virtual HRESULT RequestRemoveInsert(int index) = 0;
	virtual HRESULT ShowPoint(gp_Pnt, int index, bool) = 0;
	virtual HRESULT RefreshCutter(int index, const IndInsParameters*) = 0;
	virtual HRESULT RefreshCutter(int index, const IndInsOrientation*) = 0;

	virtual HRESULT GraphReliefAngle (int index, const IIndexableInsertSeated*, double scale) = 0;
	virtual HRESULT GraphKinematicReliefAngle(int index, const IIndexableInsertSeated*, gp_Vec velocity, double scale) = 0;
};