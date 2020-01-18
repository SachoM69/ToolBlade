// Этот исходный код MFC Samples демонстрирует функционирование пользовательского интерфейса Fluent на основе MFC в Microsoft Office
// ("Fluent UI") и предоставляется исключительно как справочный материал в качестве дополнения к
// справочнику по пакету Microsoft Foundation Classes и связанной электронной документации,
// включенной в программное обеспечение библиотеки MFC C++. 
// Условия лицензионного соглашения на копирование, использование или распространение Fluent UI доступны отдельно. 
// Для получения дополнительных сведений о нашей лицензионной программе Fluent UI посетите веб-узел
// http://msdn.microsoft.com/officeui.
//
// (C) Корпорация Майкрософт (Microsoft Corp.)
// Все права защищены.

// ToolBladeTGView.cpp : реализация класса CToolBladeTGView
//

#include "stdafx.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
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

// создание/уничтожение CToolBladeTGView

CToolBladeTGView::CToolBladeTGView()
{
	// TODO: добавьте код создания

}

CToolBladeTGView::~CToolBladeTGView()
{
		if ( myView )
		myView->Remove();
}

BOOL CToolBladeTGView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// рисование CToolBladeTGView

void CToolBladeTGView::OnDraw(CDC* /*pDC*/)
{
	CToolBladeTGDoc* pDoc = (CToolBladeTGDoc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	myView->Redraw();

	// TODO: добавьте здесь код отрисовки для собственных данных
}

CToolBladeTGDoc* CToolBladeTGView::GetDocument() const // встроена неотлаженная версия
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


// диагностика CToolBladeTGView

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