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

// ToolBladeTGDoc.cpp : ���������� ������ CToolBladeTGDoc
//

#include "stdafx.h"
// SHARED_HANDLERS ����� ���������� � ������������ �������� ��������� ���������� ������� ATL, �������
// � ������; ��������� ��������� ������������ ��� ��������� � ������ �������.
#ifndef SHARED_HANDLERS
#include "ToolBladeTG.h"
#endif

#include "ToolBladeTGDoc.h"
//#include "d:\SB\ToolBlade\ToolBlade\ToolBlade.h"

#include <propkey.h>
#include "MainFrm.h"

#include "TB_DesDlg.h"
#include "CTB_OriDlg.h"
#include "ToolTypeDlg.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

// CToolBladeTGDoc

IMPLEMENT_DYNCREATE(CToolBladeTGDoc, CDocument)

BEGIN_MESSAGE_MAP(CToolBladeTGDoc, CDocument)
	ON_COMMAND(ID_NEW_CUTTER, &CToolBladeTGDoc::OnNewCutter)
	ON_COMMAND(ID_EDPARAMS, &CToolBladeTGDoc::OnEdparams)
	ON_COMMAND(ID_TOOLTYPE, &CToolBladeTGDoc::OnTooltype)
	ON_COMMAND(ID_SHOWTOOL, &CToolBladeTGDoc::OnShowtool)
END_MESSAGE_MAP()


// ��������/����������� CToolBladeTGDoc

CToolBladeTGDoc::CToolBladeTGDoc():VisPnt(0)
{
	for(int i=0; i<3; i++) MyCoolArrow[i]=0;
	IndInsParameters IndIns = GetDefaultInsert();
	IndInsOrientation IndOri = GetDefaultOrientation();
	IndInsData idt;
	idt.libdata=IndIns;
	idt.liboridata = IndOri;
	idt.diagdata.IsDisabled=false;
	idt.libcpptr=nullptr;
	CutterParams.push_back(idt);
	Instrument.ActualToothCount=Instrument.UsedToothCount=1;
	Instrument.CutDirection=false;
	Instrument.ToolType=0;
}

CToolBladeTGDoc::~CToolBladeTGDoc()
{
}

IndInsParameters CToolBladeTGDoc::GetDefaultInsert()
{
	IndInsParameters IndIns;
	IndIns.IGroup=0;//����� ������ ��������
	IndIns.IIForm=2;//����� ����� ��������
	IndIns.FormChar='P';
	IndIns.n=3;
	IndIns.RackAng=0;//�������� ������� ����
	IndIns.HT=CylHole;//������� � ����� ���������
	IndIns.Dim=6.35;//������
	IndIns.Thick=3.5;//�������
	IndIns.Dir=Dir_Right;//����������� �������
	IndIns.DHole=2.5;
	IndIns.VertForm=0;
	IndIns.B=IndIns.L=0;
	IndIns.eps=0;
	IndIns.r=0.1;
	//IndIns.ActEdge=0;
	return IndIns;
}

IndInsOrientation CToolBladeTGDoc::GetDefaultOrientation()
{
	IndInsOrientation IndOri;
	IndOri.Diameter = 0.1;
	IndOri.Dir = DirTool_Right;
	IndOri.Type = Turning_Cutter;
	IndOri.PointIndex = 0;
	IndOri.EdgePosition = 0;
	IndOri.Gamma = 0;
	IndOri.Lambda = 0;
	IndOri.Phi = 0;
	return IndOri;
}

BOOL CToolBladeTGDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �������� ��� ��������� �������������
	// (��������� SDI ����� �������� ������������ ���� ��������)

	return TRUE;
}




// ������������ CToolBladeTGDoc

void CToolBladeTGDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �������� ��� ����������
	}
	else
	{
		// TODO: �������� ��� ��������
	}
}

#ifdef SHARED_HANDLERS

// ��������� ��� �������
void CToolBladeTGDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �������� ���� ��� ��� ����������� ������ ���������
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ��������� ������������ ������
void CToolBladeTGDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������� ���������� ������ �� ������ ���������. 
	// ����� ����������� ������ ����������� ������ � ������� ";"

	// ��������:  strSearchContent = _T("�����;�������������;����;������ ole;");
	SetSearchContent(strSearchContent);
}

void CToolBladeTGDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// ����������� CToolBladeTGDoc

#ifdef _DEBUG
void CToolBladeTGDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CToolBladeTGDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// ������� CToolBladeTGDoc

HRESULT CToolBladeTGDoc::RefreshCutter(int index, const IndInsParameters *a)
{
	try
	{
		if(CutterParams[index].libcpptr) DestroyInsert(CutterParams[index].libcpptr);
		CutterParams[index].libcpptr = CreateInsertAndPreview(myAISContext, a);
		for each(auto& mca in MyCoolArrow) if(!mca.IsNull()) myAISContext->Remove(mca, Standard_False);
		MyCoolArrow[0] = new ISession_Direction(gp_Pnt(0, 0, 0), gp_Vec(10, 0, 0), 1);
		MyCoolArrow[1] = new ISession_Direction(gp_Pnt(0, 0, 0), gp_Vec(0, 10, 0), 1);
		MyCoolArrow[2] = new ISession_Direction(gp_Pnt(0, 0, 0), gp_Vec(0, 0, 10), 1);
		//for (int i=0; i<3; i++) 
		//for each(auto& mca in MyCoolArrow)
			//myAISContext->Display(mca);

	}
	catch(Standard_OutOfRange& e)
	{
		MessageBoxA(NULL, e.GetMessageString(), "Exception in drawing routine", MB_OK);
	}
	catch(Standard_NullObject& e)
	{
		MessageBoxA(NULL, e.GetMessageString(), "Exception in drawing routine", MB_OK);
	}
	catch(Standard_NoSuchObject& e)
	{
		MessageBoxA(NULL, e.GetMessageString(), "Exception in drawing routine", MB_OK);
	}
	catch(Standard_Failure &e)
	{
		MessageBoxA(NULL, e.GetMessageString(), "Exception in drawing routine", MB_OK);
	}
	return S_OK;
}

HRESULT CToolBladeTGDoc::RefreshCutter(int index, const IndInsOrientation* a)
{
	try
	{
		if (!CutterParams[index].libcpptr)
			CutterParams[index].libcpptr = CreateInsert(&CutterParams[index].libdata);
		CutterParams[index].libcpptr = OrientInsertAndPreview(myAISContext, CutterParams[index].libcpptr, a);
		for each (auto & mca in MyCoolArrow) if (!mca.IsNull()) myAISContext->Remove(mca, Standard_False);
		MyCoolArrow[0] = new ISession_Direction(gp_Pnt(0, 0, 0), gp_Vec(10, 0, 0), 1);
		MyCoolArrow[1] = new ISession_Direction(gp_Pnt(0, 0, 0), gp_Vec(0, 10, 0), 1);
		MyCoolArrow[2] = new ISession_Direction(gp_Pnt(0, 0, 0), gp_Vec(0, 0, 10), 1);
	}
	catch (Standard_OutOfRange& e)
	{
		MessageBoxA(NULL, e.GetMessageString(), "Exception in drawing routine", MB_OK);
	}
	catch (Standard_NullObject& e)
	{
		MessageBoxA(NULL, e.GetMessageString(), "Exception in drawing routine", MB_OK);
	}
	catch (Standard_NoSuchObject& e)
	{
		MessageBoxA(NULL, e.GetMessageString(), "Exception in drawing routine", MB_OK);
	}
	catch (Standard_Failure& e)
	{
		MessageBoxA(NULL, e.GetMessageString(), "Exception in drawing routine", MB_OK);
	}
	return S_OK;
}


void CToolBladeTGDoc::OnNewCutter()
{
	try
	{
		if(CutterParams[0].libcpptr) DestroyInsert(CutterParams[0].libcpptr);
		CutterParams[0].libcpptr = CreateInsertAndPreview(myAISContext, &CutterParams.begin()->libdata); // �� ����� �������� ������
		for each(auto& mca in MyCoolArrow) if(!mca.IsNull()) myAISContext->Remove(mca, Standard_False);
		MyCoolArrow[0] = new ISession_Direction(gp_Pnt(0, 0, 0), gp_Vec(10, 0, 0), 1);
		MyCoolArrow[1] = new ISession_Direction(gp_Pnt(0, 0, 0), gp_Vec(0, 10, 0), 1);
		MyCoolArrow[2] = new ISession_Direction(gp_Pnt(0, 0, 0), gp_Vec(0, 0, 10), 1);
		//for (int i=0; i<3; i++) 
		for each(auto& mca in MyCoolArrow)
		myAISContext->Display(mca, true);

	}
	catch(Standard_OutOfRange& e)
	{
		MessageBoxA(NULL, e.GetMessageString(), "Exception in drawing routine", MB_OK);
	}
	catch(Standard_NullObject& e)
	{
		MessageBoxA(NULL, e.GetMessageString(), "Exception in drawing routine", MB_OK);
	}
	catch(Standard_NoSuchObject& e)
	{
		MessageBoxA(NULL, e.GetMessageString(), "Exception in drawing routine", MB_OK);
	}
	catch(Standard_Failure &e)
	{
		MessageBoxA(NULL, e.GetMessageString(), "Exception in drawing routine", MB_OK);
	}
}

void CToolBladeTGDoc::OnEdparams()
{
	CTB_DesDlg* NPGetter = new CTB_DesDlg(this);
	NPGetter->DoModal();
	delete NPGetter;
}

HRESULT CToolBladeTGDoc::QueryIndInsInformation(int index, IndInsParameters* res)
{
	size_t indop(index);
	if(index<0 || indop>=CutterParams.size()) return E_INVALIDARG;
	*res = CutterParams[index].libdata;
	return S_OK;
}
HRESULT CToolBladeTGDoc::UpdateIndInsInformation(int index, const IndInsParameters* res)
{
	size_t indop(index);
	if(index<0 || indop>=CutterParams.size()) return E_INVALIDARG;
	CutterParams[index].libdata = *res;
	if(CutterParams[index].libcpptr) DestroyInsert(CutterParams[index].libcpptr);
	CutterParams[index].libcpptr = CreateInsertAndPreview(myAISContext, res);
	return S_OK;
}


HRESULT CToolBladeTGDoc::QueryIndInsAttributes(int index, IndInsAttributes* res)
{
	size_t indop(index);
	if(index<0 || indop>=CutterParams.size()) return E_INVALIDARG;
	*res = CutterParams[index].diagdata;
	return S_OK;
}

HRESULT CToolBladeTGDoc::UpdateIndInsAttributes(int index, const IndInsAttributes* res)
{
	size_t indop(index);
	if(index<0 || indop>=CutterParams.size()) return E_INVALIDARG;
	CutterParams[index].diagdata = *res;
	return S_OK;
}

HRESULT CToolBladeTGDoc::QueryIndInsOrientation(int index, IndInsOrientation* res)
{
	size_t indop(index);
	if (index < 0 || indop >= CutterParams.size()) return E_INVALIDARG;
	*res = CutterParams[index].liboridata;
	return S_OK;
}

HRESULT CToolBladeTGDoc::UpdateIndInsOrientation(int index, const IndInsOrientation* res)
{
	size_t indop(index);
	if (index < 0 || indop >= CutterParams.size()) return E_INVALIDARG;
	CutterParams[index].liboridata = *res;
	return S_OK;
}

HRESULT CToolBladeTGDoc::QueryToolData(ToolData* tda)
{
	*tda = Instrument;
	return S_OK;
}

HRESULT CToolBladeTGDoc::QueryIndInsObject(int index, const IIndexableInsert** optr)
{
	size_t indop(index);
	if(index<0 || indop>=CutterParams.size()) return E_INVALIDARG;
	if(CutterParams[index].libcpptr) DestroyInsert(CutterParams[index].libcpptr);
	*optr = CutterParams[index].libcpptr = CreateInsertAndPreview(myAISContext, &CutterParams[index].libdata);
	return S_OK;
}

void CToolBladeTGDoc::OnTooltype()
{
	ToolTypeDlg* TTD = new ToolTypeDlg;

	TTD->DoModal();

	delete TTD;
}

HRESULT CToolBladeTGDoc::RequestNewInsert(int* index)
{
	size_t indop(*index);
	IndInsData idt;
	
	if(*index<0 || indop>=CutterParams.size()) idt.libdata=GetDefaultInsert();
	else idt.libdata=CutterParams[*index].libdata;
	idt.diagdata.IsDisabled=false;
	idt.libcpptr=nullptr;
	CutterParams.push_back(idt);
	Instrument.ActualToothCount++; Instrument.UsedToothCount++;
	*index = int(CutterParams.size()-1);
	return S_OK;
}

HRESULT CToolBladeTGDoc::RequestRemoveInsert(int index)
{
	size_t indop(index);
	if(index<0 || indop>=CutterParams.size()) return E_INVALIDARG;
	if(CutterParams[index].libcpptr) DestroyInsert(CutterParams[index].libcpptr);
	Instrument.ActualToothCount--;
	if(!CutterParams[index].diagdata.IsDisabled) Instrument.UsedToothCount--;
	CutterParams.erase(index+CutterParams.begin());
	return S_OK;
}

HRESULT CToolBladeTGDoc::ShowPoint(gp_Pnt a, bool show)
{
	myAISContext->Erase(VisPnt, true);
	if(show)
	{
		Handle_Geom_CartesianPoint c = new Geom_CartesianPoint(a);
		VisPnt = new AIS_Point(c);
		myAISContext->Display(VisPnt, true);
	}
	return S_OK;
}

void CToolBladeTGDoc::OnShowtool()
{
	try
	{
		/*if(!CutterParams)
			InsertShape(myAISContext);
		else */
		/*
		IndInsOrientation a; a.PointIndex = 0; a.Diameter = 0.02; a.Dir = DirTool_Right; a.EdgePosition = 0; a.Phi = 0;
		a.Gamma = 0; a.Lambda = 0; a.Type = Turning_Cutter;
		CutterParams[0].libcpptr = OrientInsertAndPreview(myAISContext, CutterParams.begin()->libcpptr, &a); // �� ����� �������� ������
		for each (auto & mca in MyCoolArrow) if (!mca.IsNull()) myAISContext->Remove(mca, Standard_False);
		MyCoolArrow[0] = new ISession_Direction(gp_Pnt(0, 0, 0), gp_Vec(10, 0, 0), 1);
		MyCoolArrow[1] = new ISession_Direction(gp_Pnt(0, 0, 0), gp_Vec(0, 10, 0), 1);
		MyCoolArrow[2] = new ISession_Direction(gp_Pnt(0, 0, 0), gp_Vec(0, 0, 10), 1);
		//for (int i=0; i<3; i++) 
		for each (auto & mca in MyCoolArrow)
			myAISContext->Display(mca, true);*/

		CTB_OriDlg* NPGetter = new CTB_OriDlg(this);
		NPGetter->DoModal();
		delete NPGetter;

	}
	catch (Standard_OutOfRange & e)
	{
		MessageBoxA(NULL, e.GetMessageString(), "Exception in drawing routine", MB_OK);
	}
	catch (Standard_NullObject & e)
	{
		MessageBoxA(NULL, e.GetMessageString(), "Exception in drawing routine", MB_OK);
	}
	catch (Standard_NoSuchObject & e)
	{
		MessageBoxA(NULL, e.GetMessageString(), "Exception in drawing routine", MB_OK);
	}
	catch (Standard_Failure & e)
	{
		MessageBoxA(NULL, e.GetMessageString(), "Exception in drawing routine", MB_OK);
	}
}
