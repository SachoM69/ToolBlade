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
	struct IndInsData
	{
		IndInsert libdata;
		IndInsAttributes diagdata;
		CIndexableInsert* libcpptr;
	};
	std::vector<IndInsData> CutterParams;
	ToolData Instrument;
	Handle_ISession_Direction MyCoolArrow[3];
	Handle_ISession_Direction PntDirArrow;
	Handle_AIS_Point VisPnt;

	IndInsert GetDefaultInsert() const;
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
	virtual HRESULT QueryIndInsertInformation(int index, IndInsert*) override;
	virtual HRESULT UpdateIndInsertInformation(int index, const IndInsert*) override;
	virtual HRESULT QueryIndInsertAttributes(int index, IndInsAttributes*) override;
	virtual HRESULT UpdateIndInsertAttributes(int index, const IndInsAttributes*) override;
	virtual HRESULT QueryToolData(ToolData*) override;
	virtual HRESULT QueryIndInsertObject(int index, const CIndexableInsert**) override;
	virtual HRESULT RequestNewInsert(int* index_inout);
	virtual HRESULT RequestRemoveInsert(int index);
	virtual HRESULT ShowPoint(gp_Pnt, bool) override;
	virtual HRESULT RefreshCutter(int index, IndInsert*) override;

// ����������
public:
	virtual ~CToolBladeTGDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
   static void Fit();

protected:

// ��������� ������� ����� ���������
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ��������������� �������, �������� ���������� ������ ��� ����������� ������
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnNewCutter();
	afx_msg void OnEdparams();
	afx_msg void OnTooltype();
	afx_msg void OnShowtool();
};
