// ���� �������� ��� MFC Samples ������������� ���������������� ����������������� ���������� Fluent �� ������ MFC � Microsoft Office
// ("Fluent UI") � ��������������� ������������� ��� ���������� �������� � �������� ���������� �
// ����������� �� ������ Microsoft Foundation Classes � ��������� ����������� ������������,
// ���������� � ����������� ����������� ���������� MFC C++. 
// ������� ������������� ���������� �� �����������, ������������� ��� ��������������� Fluent UI �������� ��������. 
// ��� ��������� �������������� �������� � ����� ������������ ��������� Fluent UI �������� ���-����
// http://msdn.microsoft.com/officeui.
//
// (C) ���������� ���������� (Microsoft Corp.)
// ��� ����� ��������.

// ToolBladeTGDoc.h : ��������� ������ CToolBladeTGDoc
//


#pragma once
#include "DocBase.h"
#include "ISession_Direction.h"
#include <vector>
#include <map>
#include "ToolDataList.h"

class CToolBladeTGDoc : public CDocBase, IInstrInsList
{
protected: // ������� ������ �� ������������
	CToolBladeTGDoc();
	DECLARE_DYNCREATE(CToolBladeTGDoc)

// ��������
public:

//����������
private:
	enum GraphType
	{
		GTRelief = 0,
		GTKinematicRelief = 1,
		GTRack = 2
	};
	struct IndInsData
	{
		IndInsParameters libdata;
		IndInsOrientation liboridata;
		IndInsAttributes diagdata;
		std::shared_ptr<IIndexableInsert> libcpptr;
		std::shared_ptr<IIndexableInsertSeated> liboriptr;
		std::map<GraphType, Handle_AIS_Shape> graphs;
		
	};
	std::vector<IndInsData> CutterParams;
	ToolData Instrument;
	std::shared_ptr<IIndexableInsertTool> Tool;
	std::shared_ptr<IInsertPreview> Preview;
	Handle_ISession_Direction MyCoolArrow[3];
	Handle_ISession_Direction PntDirArrow;
	Handle_AIS_Point VisPnt;
	Handle_AIS_Shape StaticReliefAngles;

	static IndInsParameters GetDefaultInsert();
	static IndInsOrientation GetDefaultOrientation();

	// ��������
public:

// ���������������
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS
	virtual HRESULT QueryIndInsInformation(int index, IndInsParameters*) override;
	virtual HRESULT UpdateIndInsInformation(int index, const IndInsParameters*) override;
	virtual HRESULT QueryIndInsAttributes(int index, IndInsAttributes*) override;
	virtual HRESULT UpdateIndInsAttributes(int index, const IndInsAttributes*) override;
	virtual HRESULT QueryIndInsOrientation(int index, IndInsOrientation*) override;
	virtual HRESULT UpdateIndInsOrientation(int index, const IndInsOrientation*) override;
	virtual HRESULT QueryToolData(ToolData*) override;
	virtual HRESULT UpdateToolType(int type) override;
	virtual HRESULT UpdateToolDirection(bool dir) override;
	virtual HRESULT QueryIndInsObject(int index, const IIndexableInsert**) override;
	virtual HRESULT QueryIndInsObjectSeated(int index, const IIndexableInsertSeated**) override;
	virtual HRESULT RequestNewInsert(int* index_inout);
	virtual HRESULT RequestRemoveInsert(int index);
	virtual HRESULT ShowPoint(gp_Pnt, int index, bool) override;
	virtual HRESULT RefreshCutter(int index, const IndInsParameters*) override;
	virtual HRESULT RefreshCutter(int index, const IndInsOrientation*) override;
	virtual HRESULT UpdateDisplay() override;
	virtual HRESULT GraphReliefAngle(int index, const IIndexableInsertSeated*, double scale) override;
	virtual HRESULT HideReliefAngle(int index) override;
	virtual HRESULT GraphKinematicReliefAngle(int index, const IIndexableInsertSeated*, gp_Vec velocity, double scale) override;
	virtual HRESULT HideKinematicReliefAngle(int index) override;
	virtual HRESULT ShowPlane(gp_Pln, int planeindex, bool show) override;
	virtual HRESULT ShowAxis(gp_Vec, int vectorindex, bool show) override;
	virtual std::shared_ptr<IIndexableInsertTool> QueryToolObject() override;
	virtual HRESULT RefreshTool() override;

// ����������
public:
	virtual ~CToolBladeTGDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
   static void Fit();

   Handle_AIS_Shape GraphFunction(const IIndexableInsertSeated*, std::function<double(Standard_Integer n, Standard_Real f)> function, const Quantity_Color&, double scale);

protected:

// ��������� ������� ����� ���������
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ��������������� �������, �������� ���������� ������ ��� ����������� ������
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnEdparams();
	afx_msg void OnTooltype();
	afx_msg void OnShowtool();

};
