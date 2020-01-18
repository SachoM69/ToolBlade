#pragma once
#include "afxwin.h"
#include "DocBase.h"
class CViewBase :
	public CView
{
public:
	CViewBase(void);
	~CViewBase(void);
	//Атрибуты
public:
	CDocBase* GetDocument() const;
	Handle_V3d_View& GetView() { return myView; }
protected:
	Handle_V3d_View		 myView;
	CPen*  m_Pen;
	int m_cx , m_cy ;
	int m_curx , m_cury ;
    Standard_Integer     myXmin;
    Standard_Integer     myYmin;  
    Standard_Integer     myXmax;
    Standard_Integer     myYmax;
	//Реализация
public:
	void FitAll() {   if ( !myView.IsNull() ) myView->FitAll();  myView->ZFitAll(); };
	void Redraw() {   if ( !myView.IsNull() ) myView->Redraw(); };

	void SetZoom ( const Standard_Real& Coef  ) {   myView->SetZoom ( Coef  );  };
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNCREATE(CViewBase)
	//Перегрузка
public:
	virtual void OnInitialUpdate();
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

