// CTB_OriDlg.cpp: файл реализации
//


#include "stdafx.h"
#include "ToolBladeTG.h"
#include "CTB_OriDlg.h"
#include "afxdialogex.h"

#define RAD(x) ((x)*M_PI/180)
#define DEG(x) ((x)*180/M_PI)


// Диалоговое окно CTB_OriDlg

IMPLEMENT_DYNAMIC(CTB_OriDlg, CDialogEx)

CTB_OriDlg::CTB_OriDlg(IInstrInsList* myprov, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ORIENTATION_DLG, pParent)
{
	InsertProvider = myprov;
	CurrentIndex = 0;
}

CTB_OriDlg::~CTB_OriDlg()
{
	delete ImageList;
}

BOOL CTB_OriDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	IndInsOrientation ori;
	InsertProvider->QueryIndInsOrientation(CurrentIndex, &ori);
	LoadFromParams(&ori);
	DrawEdgePoint();

	ImageList = new CImageList();
	ImageList->Create(16, 16, TRUE, 1, 1);
	ImageList->Add(AfxGetApp()->LoadIcon(IDI_ICONSHOWN));
	ImageList->Add(AfxGetApp()->LoadIcon(IDI_ICONHIDDEN));
	IndInsListView.SetImageList(ImageList, LVSIL_SMALL);

	UpdateInsertList();

	return TRUE;
}

void CTB_OriDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GAMMA, GammaTB);
	DDX_Control(pDX, IDC_LAMBDA, LambdaTB);
	DDX_Control(pDX, IDC_PHI, PhiTB);
	DDX_Control(pDX, IDC_DIAMETER, DiamTB);
	DDX_Control(pDX, IDC_ACTIVEEDGE, PointCB);
	DDX_Control(pDX, IDC_ACTIVEEDGEPOS, PointSl);
	DDX_Control(pDX, IDC_IILIST, IndInsListView);
}


BEGIN_MESSAGE_MAP(CTB_OriDlg, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_IILIST, &CTB_OriDlg::OnNMDblclkIilist)
END_MESSAGE_MAP()


// Обработчики сообщений CTB_OriDlg

void CTB_OriDlg::StoreToParams(IndInsOrientation* IIt)
{
	CString s_diam, s_gamma, s_lambda, s_phi;
	DiamTB.GetWindowText(s_diam);
	LambdaTB.GetWindowText(s_lambda);
	PhiTB.GetWindowText(s_phi);
	GammaTB.GetWindowText(s_gamma);
	IIt->Diameter = (_wtof(s_diam))/1000;
	IIt->Gamma = RAD(_wtof(s_gamma));
	IIt->Phi = RAD(_wtof(s_phi));
	IIt->Lambda = RAD(_wtof(s_lambda));
	IIt->PointIndex = PointCB.GetCurSel();
	int curpos = PointSl.GetPos();
	int minpos = PointSl.GetRangeMin();
	int maxpos = PointSl.GetRangeMax();
	IIt->EdgePosition = double(curpos-minpos) / (maxpos-minpos);
}

void CTB_OriDlg::LoadFromParams(const IndInsOrientation* IIt)
{
	CString s_diam, s_gamma, s_lambda, s_phi;
	s_diam.Format(_T("%f"), (IIt->Diameter*1000));
	s_gamma.Format(_T("%f"), DEG(IIt->Gamma));
	s_lambda.Format(_T("%f"), DEG(IIt->Lambda));
	s_phi.Format(_T("%f"), DEG(IIt->Phi));
	DiamTB.SetWindowText(s_diam);
	LambdaTB.SetWindowText(s_lambda);
	PhiTB.SetWindowText(s_phi);
	GammaTB.SetWindowText(s_gamma);
	PointCB.ResetContent();
	CString sci;
	SetActiveEdgeList();
	PointCB.SetCurSel(IIt->PointIndex);
	PointSl.SetRange(0, 100);
	PointSl.SetPos(int(IIt->EdgePosition * 100));
}

void CTB_OriDlg::DrawEdgePoint()
{
	const IIndexableInsert* object;
	InsertProvider->QueryIndInsObject(CurrentIndex, &object);
	gp_Pnt edgept; gp_Vec V; gp_Ax3 Ax3;
	object->IIVertex(PointCB.GetCurSel(), PointSl.GetPos() / PointSl.GetRangeMax(), edgept, V, Ax3);
	InsertProvider->ShowPoint(edgept, true);
	//нарисовать стрелку из точки edgept в направлении V
}

void CTB_OriDlg::SetActiveEdgeList()
{
	const IIndexableInsert* object;
	InsertProvider->QueryIndInsObject(CurrentIndex, &object);
	CString S;
	PointCB.ResetContent();
	for (auto i = 0; i < object->NumPoint(); i++)
	{
		S.Format(L"%d", i);
		PointCB.AddString(S);
	}
}

// меняем заголовок активной пластины
void CTB_OriDlg::UpdateInsertListSoft(void)
{
	ToolData b;
	InsertProvider->QueryToolData(&b);
	for (auto i = 0; i < b.ActualToothCount; i++)
	{
		CString a;
		if (i != CurrentIndex) a.Format(_T("%d"), i);
		else a.Format(_T("%d (активна)"), i);
		IndInsListView.SetItemText(i, 0, a);
	}
}


// перечитываем список пластин из документа
void CTB_OriDlg::UpdateInsertList(void)
{
	ToolData b;
	InsertProvider->QueryToolData(&b);
	IndInsListView.DeleteAllItems();
	for (auto i = 0; i < b.ActualToothCount; i++)
	{
		IndInsAttributes c;
		InsertProvider->QueryIndInsAttributes(i, &c);
		CString a;
		if (i != CurrentIndex) a.Format(_T("%d"), i);
		else a.Format(_T("%d (активна)"), i);
		IndInsListView.InsertItem(i, a, c.IsDisabled);
	}
}

void CTB_OriDlg::OnNMDblclkIilist(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	IndInsOrientation a;
	//if (pNMItemActivate->iItem != CurrentIndex)
	{
		StoreToParams(&a);
		InsertProvider->UpdateIndInsOrientation(CurrentIndex, &a);
	}

	if (pNMItemActivate->iItem != -1)
	{
		CurrentIndex = pNMItemActivate->iItem;
		InsertProvider->QueryIndInsOrientation(CurrentIndex, &a);
		LoadFromParams(&a);
		UpdateInsertListSoft();
	}

	*pResult = 0;
}
