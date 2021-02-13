#include "StdAfx.h"
#include "AppBase.h"
#include "ToolBladeTGDoc.h"
#include "ViewBase.h"
#include <Standard_MultiplyDefined.hxx>

static int rotCount = 0;

#define ZOOM_DIVIDER 8
#define ROTATE_MULT 3

/////////////////////////////////////////////////////////////////////////////
// CViewMan

IMPLEMENT_DYNCREATE(CViewBase, CView)

BEGIN_MESSAGE_MAP(CViewBase, CView)
	//{{AFX_MSG_MAP(CViewMan)
	ON_WM_SIZE()
	//ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
// CasCade

//ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


CViewBase::CViewBase(void)
{
	myXmin=0;
    myYmin=0;  
    myXmax=0;
    myYmax=0;

    m_Pen = NULL;

	myXmin=0;
    myYmin=0;  
    myXmax=0;
    myYmax=0;
}


CViewBase::~CViewBase(void)
{
}

CDocBase* CViewBase::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolBladeTGDoc)));
	return (CDocBase*)m_pDocument;
}

void CViewBase::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
    myView = GetDocument()->GetViewer()->CreateView();

    // set the default mode in wireframe ( not hidden line ! )
    //myView->SetDegenerateModeOn();

	//Handle(OpenGl_GraphicDriver) aDriver = 
    //Handle(OpenGl_GraphicDriver)::DownCast(((CAppBase*)AfxGetApp())->GetGraphicDevice());
    
    Handle(WNT_Window) aWNTWindow = new WNT_Window(GetSafeHwnd ());
	//Handle(WNT_Window) aWNTWindow = &
	
	myView->SetWindow(aWNTWindow);
	myView->Camera()->SetProjectionType (Graphic3d_Camera::Projection_Orthographic);
    if (!aWNTWindow->IsMapped()) aWNTWindow->Map();
	
}

void CViewBase::OnDraw(CDC* pDC)
{
	CDocBase* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	  myView->Redraw();

}

void CViewBase::OnMButtonDown(UINT nFlags, CPoint point) 
{
	myXmin=point.x;  myYmin=point.y;
  myXmax=point.x;  myYmax=point.y;
   if ( nFlags & MK_CONTROL ) 
	  {
      	// Button MB2 down Control : panning init  
        // SetCursor(AfxGetApp()->LoadStandardCursor());   
	  }else{
	//		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
			myView->StartRotation(point.x*ROTATE_MULT,point.y*ROTATE_MULT);
	//		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));

   }}

void CViewBase::OnMouseMove(UINT nFlags, CPoint point) 
{
    //   ============================  LEFT BUTTON =======================
  m_curx = point.x ;
  m_cury = point.y ;

  if ( nFlags & MK_LBUTTON)
    {
     if ( nFlags & MK_CONTROL ) 
	  {
	    // move with MB1 and Control : on the dynamic zooming  
	    // Do the zoom in function of mouse's coordinates  
	    myView->Zoom(myXmax,myYmax,point.x,point.y); 
	    // save the current mouse coordinate in min 
		myXmax = point.x; 
        myYmax = point.y;	
	  }
      // if ( nFlags & MK_CONTROL )  else 
    } else //   if ( nFlags & MK_LBUTTON) 
    //   ============================  MIDDLE BUTTON =======================
    if ( nFlags & MK_MBUTTON)
    {
     if ( nFlags & (MK_CONTROL|MK_RBUTTON) ) 
	  {
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
		myView->Pan(point.x-myXmax,myYmax-point.y); // Realize the panning
		myXmax = point.x; myYmax = point.y;	
		myView->StartRotation(myXmax * ROTATE_MULT,myYmax * ROTATE_MULT);

	 } else {
		rotCount++;
		myView->Rotation(point.x*ROTATE_MULT,point.y*ROTATE_MULT);
		myXmax = point.x; myYmax = point.y;
		}

    } else //  if ( nFlags & MK_MBUTTON)
    //   ============================  RIGHT BUTTON =======================
    if ( nFlags & MK_RBUTTON)
    {
     if ( nFlags & MK_CONTROL ) 
	  {
	     rotCount++;
      	 myView->Rotation(point.x,point.y);
	  }
    }else //if ( nFlags & MK_RBUTTON)
    //   ============================  NO BUTTON =======================
    {  // No buttons 
	  myXmax = point.x; myYmax = point.y;	
	  /*if (nFlags & MK_SHIFT)
		GetDocument()->ShiftMoveEvent(point.x,point.y,myView);
	  else
		GetDocument()->MoveEvent(point.x,point.y,myView);*/
	}
}

BOOL CViewBase::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	myView->StartZoomAtPoint(m_curx,m_cury);
	double mx=-(double(zDelta)/ZOOM_DIVIDER)/(double(m_cy)/double(m_cx));
	myView->ZoomAtPoint(0,0,int(mx),int(zDelta/ZOOM_DIVIDER));
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CViewBase::OnSize(UINT nType, int cx, int cy) 
{
	m_cx = cx ;
	m_cy = cy ;
	if (!myView.IsNull())
		myView->MustBeResized();
}