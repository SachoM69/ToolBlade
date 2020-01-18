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

// ToolBladeTGView.cpp : ���������� ������ CToolBladeTGView
//

#include "stdafx.h"
// SHARED_HANDLERS ����� ���������� � ������������ �������� ��������� ���������� ������� ATL, �������
// � ������; ��������� ��������� ������������ ��� ��������� � ������ �������.
#ifndef SHARED_HANDLERS
#include "ToolBladeTG.h"
#endif

#include "ToolBladeTGDoc.h"
#include "ToolBladeTGView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolBladeTGView

IMPLEMENT_DYNCREATE(CToolBladeTGView, CView)

BEGIN_MESSAGE_MAP(CToolBladeTGView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_MBUTTONDOWN()
END_MESSAGE_MAP()

// ��������/����������� CToolBladeTGView

CToolBladeTGView::CToolBladeTGView()
{
	// TODO: �������� ��� ��������

}

CToolBladeTGView::~CToolBladeTGView()
{
		if ( myView )
		myView->Remove();
}

BOOL CToolBladeTGView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �������� ����� Window ��� ����� ����������� ���������
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// ��������� CToolBladeTGView

void CToolBladeTGView::OnDraw(CDC* /*pDC*/)
{
	CToolBladeTGDoc* pDoc = (CToolBladeTGDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	myView->Redraw();

	// TODO: �������� ����� ��� ��������� ��� ����������� ������
}

CToolBladeTGDoc* CToolBladeTGView::GetDocument() const // �������� ������������ ������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolBladeTGDoc)));
	return (CToolBladeTGDoc*)m_pDocument;
}

void CToolBladeTGView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	//OnContextMenu(this, point);
}

void CToolBladeTGView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// ����������� CToolBladeTGView

#ifdef _DEBUG
void CToolBladeTGView::AssertValid() const
{
	CView::AssertValid();
}

void CToolBladeTGView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

#endif //_DEBUG

void CToolBladeTGView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CViewBase::OnMouseMove(nFlags, point);
}

BOOL CToolBladeTGView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	return CViewBase::OnMouseWheel(nFlags, zDelta, pt);
}

void CToolBladeTGView::OnMButtonDown(UINT nFlags, CPoint point) 
{
	CViewBase::OnMButtonDown(nFlags, point);
}

void CToolBladeTGView::OnSize(UINT nType, int cx, int cy) 
{
	CViewBase::OnSize(nType, cx, cy);
}