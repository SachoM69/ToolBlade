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
	CurrentlyUpdating = false;
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
	LoadKinematicParams(0.5, 0, 100, 30);
	DrawEdgePoint();
	InsertProvider->RefreshCutter(CurrentIndex, &ori);

	ImageList = new CImageList();
	ImageList->Create(16, 16, TRUE, 1, 1);
	ImageList->Add(AfxGetApp()->LoadIcon(IDI_ICONSHOWN));
	ImageList->Add(AfxGetApp()->LoadIcon(IDI_ICONHIDDEN));
	//IndInsListView.SetImageList(ImageList, LVSIL_SMALL);
	IndInsListView.SetExtendedStyle(LVS_EX_CHECKBOXES);

	UpdateInsertList();

	SetScale(1);

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
	DDX_Control(pDX, IDC_FEEDX, FeedXTB);
	DDX_Control(pDX, IDC_FEEDY, FeedYTB);
	DDX_Control(pDX, IDC_RPM, RPMTB);
	DDX_Control(pDX, IDC_CONTACTVELOCITY, VelocityTB);
	DDX_Control(pDX, IDC_SELECTRPM, RPMRadio);
	DDX_Control(pDX, IDC_SELECTVELOCITY, VelocityRadio);
	DDX_Control(pDX, IDC_SCALE, ScaleTB);
	DDX_Control(pDX, IDC_AXISROT, AxisRotTB);
	DDX_Control(pDX, IDC_ZOFFSET, ZOffsetTB);
}


BEGIN_MESSAGE_MAP(CTB_OriDlg, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_IILIST, &CTB_OriDlg::OnNMDblclkIilist)
	ON_EN_CHANGE(IDC_GAMMA, &CTB_OriDlg::OnEnChangeGamma)
	ON_EN_CHANGE(IDC_LAMBDA, &CTB_OriDlg::OnEnChangeLambda)
	ON_EN_CHANGE(IDC_PHI, &CTB_OriDlg::OnEnChangePhi)
	ON_EN_CHANGE(IDC_DIAMETER, &CTB_OriDlg::OnEnChangeDiameter)
	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(IDC_ACTIVEEDGE, &CTB_OriDlg::OnCbnSelchangeActiveedge)
	ON_BN_CLICKED(IDOK, &CTB_OriDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_DISPLAYRELIEFANGLES, &CTB_OriDlg::OnBnClickedDisplayreliefangles)
	ON_EN_CHANGE(IDC_FEEDX, &CTB_OriDlg::OnEnChangeFeedx)
	ON_EN_CHANGE(IDC_FEEDY, &CTB_OriDlg::OnEnChangeFeedy)
	ON_BN_CLICKED(IDC_DISPLAYKINEMATICRELIEFANGLES, &CTB_OriDlg::OnBnClickedDisplaykinematicreliefangles)
	ON_EN_CHANGE(IDC_RPM, &CTB_OriDlg::OnEnChangeRpm)
	ON_EN_CHANGE(IDC_CONTACTVELOCITY, &CTB_OriDlg::OnEnChangeVelocity)
	ON_EN_CHANGE(IDC_SCALE, &CTB_OriDlg::OnEnChangeScale)
	ON_EN_CHANGE(IDC_AXISROT, &CTB_OriDlg::OnEnChangeAxisrot)
	ON_EN_CHANGE(IDC_ZOFFSET, &CTB_OriDlg::OnEnChangeZoffset)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_IILIST, &CTB_OriDlg::OnLvnItemchangedIilist)
END_MESSAGE_MAP()


// Обработчики сообщений CTB_OriDlg

void CTB_OriDlg::StoreToParams(IndInsOrientation* IIt)
{
	CString s_diam, s_gamma, s_lambda, s_phi;
	DiamTB.GetWindowText(s_diam);
	LambdaTB.GetWindowText(s_lambda);
	PhiTB.GetWindowText(s_phi);
	GammaTB.GetWindowText(s_gamma);
	IIt->Diameter = (_wtof(s_diam));
	IIt->Gamma = RAD(_wtof(s_gamma));
	IIt->Phi = RAD(_wtof(s_phi));
	IIt->Lambda = RAD(_wtof(s_lambda));
	IIt->PointIndex = PointCB.GetCurSel();
	if (IIt->PointIndex == -1) IIt->PointIndex = 0;
	int curpos = PointSl.GetPos();
	int minpos = PointSl.GetRangeMin();
	int maxpos = PointSl.GetRangeMax();
	IIt->EdgePosition = double(curpos-minpos) / (maxpos-minpos);
	CString s_ar, s_zof;
	AxisRotTB.GetWindowText(s_ar);
	ZOffsetTB.GetWindowText(s_zof);
	IIt->AxisRotation= RAD(_wtof(s_ar));
	IIt->Zoffset = _wtof(s_zof);
}

void CTB_OriDlg::LoadFromParams(const IndInsOrientation* IIt)
{
	CurrentlyUpdating = true;
	CString s_diam, s_gamma, s_lambda, s_phi;
	s_diam.Format(_T("%f"), (IIt->Diameter));
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
	PointSl.SetRange(0, 101);
	PointSl.SetPos(int(IIt->EdgePosition * 101));
	CString s_ar, s_zof;
	s_ar.Format(_T("%f"), DEG(IIt->AxisRotation));
	s_zof.Format(_T("%f"), (IIt->Zoffset));
	AxisRotTB.SetWindowText(s_ar);
	ZOffsetTB.SetWindowText(s_zof);
	CurrentlyUpdating = false;
}

void CTB_OriDlg::DrawEdgePoint()
{
	const IIndexableInsertSeated* object;
	InsertProvider->QueryIndInsObjectSeated(CurrentIndex, &object);
	gp_Pnt edgept; gp_Vec V; gp_Ax3 Ax3;
	int curpos = PointSl.GetPos();
	int minpos = PointSl.GetRangeMin();
	int maxpos = PointSl.GetRangeMax();
	double pos  = double(curpos - minpos) / (maxpos - minpos);
	std::vector<gp_Pnt> x_pnt;
	IndInsOrientation a;
	InsertProvider->QueryIndInsOrientation(CurrentIndex, &a);
	
	/*for (a.EdgePosition = 0.53; a.EdgePosition <= 0.55; a.EdgePosition += 0.01)
	{
		InsertProvider->UpdateIndInsOrientation(CurrentIndex, &a);
		InsertProvider->RefreshCutter(CurrentIndex, &a);
		const IIndexableInsertSeated* object;
		InsertProvider->QueryIndInsObjectSeated(CurrentIndex, &object);
		object->IIVertex(PointCB.GetCurSel(), a.EdgePosition, edgept, V, Ax3);
		x_pnt.push_back(object->XExtremityPoint());
	}*/
	object->IIVertex(PointCB.GetCurSel(), pos, edgept, V, Ax3);
	InsertProvider->ShowPoint(edgept, 0, true);
	InsertProvider->ShowPoint(object->XExtremityPoint(), 1, true);
	InsertProvider->ShowPoint(object->YExtremityPoint(), 2, true);
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
		IndInsListView.SetCheck(i, c.IsDisabled ? FALSE : TRUE);
	}
}

void CTB_OriDlg::OnNMDblclkIilist(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	IndInsOrientation a;
	//if (pNMItemActivate->iItem != CurrentIndex)
	{
		InsertProvider->QueryIndInsOrientation(CurrentIndex, &a);
		StoreToParams(&a);
		InsertProvider->UpdateIndInsOrientation(CurrentIndex, &a);
	}

	if (pNMItemActivate->iItem != -1)
	{
		CurrentIndex = pNMItemActivate->iItem;
		InsertProvider->QueryIndInsOrientation(CurrentIndex, &a);
		LoadFromParams(&a);
		UpdateInsertListSoft();
		DrawEdgePoint();
		InsertProvider->RefreshCutter(CurrentIndex, &a);
	}

	*pResult = 0;
}


void CTB_OriDlg::OnEnChangeGamma()
{
	if (CurrentlyUpdating) return;
	IndInsOrientation a;
	InsertProvider->QueryIndInsOrientation(CurrentIndex, &a);
	StoreToParams(&a);
	InsertProvider->RefreshCutter(CurrentIndex, &a);
}


void CTB_OriDlg::OnEnChangeLambda()
{
	OnEnChangeGamma();
}


void CTB_OriDlg::OnEnChangePhi()
{
	OnEnChangeGamma();
}


void CTB_OriDlg::OnEnChangeDiameter()
{
	OnEnChangeGamma();
	StoreKinematicParams();
}

void CTB_OriDlg::OnEnChangeAxisrot()
{
	OnEnChangeGamma();
}


void CTB_OriDlg::OnEnChangeZoffset()
{
	OnEnChangeGamma();
}


void CTB_OriDlg::OnHScroll(UINT, UINT, CScrollBar*)
{
	// TEMP
	IndInsOrientation a;
	InsertProvider->QueryIndInsOrientation(CurrentIndex, &a);
	StoreToParams(&a);
	InsertProvider->UpdateIndInsOrientation(CurrentIndex, &a);
	InsertProvider->RefreshCutter(CurrentIndex, &a);
	DrawEdgePoint();
}

void CTB_OriDlg::OnCbnSelchangeActiveedge()
{
	OnEnChangeGamma();
	DrawEdgePoint();
}


void CTB_OriDlg::OnBnClickedOk()
{
	IndInsOrientation a;
	InsertProvider->QueryIndInsOrientation(CurrentIndex, &a);
	StoreToParams(&a);
	InsertProvider->UpdateIndInsOrientation(CurrentIndex, &a);
	CDialogEx::OnOK();
}


void CTB_OriDlg::OnBnClickedDisplayreliefangles()
{
	IndInsOrientation a;
	InsertProvider->QueryIndInsOrientation(CurrentIndex, &a);
	StoreToParams(&a);
	InsertProvider->UpdateIndInsOrientation(CurrentIndex, &a);
	const IIndexableInsertSeated* iis;
	InsertProvider->QueryIndInsObjectSeated(CurrentIndex, &iis);
	InsertProvider->GraphReliefAngle(CurrentIndex, iis, GetScale());
}

void CTB_OriDlg::OnBnClickedDisplaykinematicreliefangles()
{
	IndInsOrientation a;
	InsertProvider->QueryIndInsOrientation(CurrentIndex, &a);
	StoreToParams(&a);
	InsertProvider->UpdateIndInsOrientation(CurrentIndex, &a);
	const IIndexableInsertSeated* iis;
	InsertProvider->QueryIndInsObjectSeated(CurrentIndex, &iis);
	gp_Vec vel = gp_Vec(feedx, feedy, relvel);
	InsertProvider->GraphKinematicReliefAngle(CurrentIndex, iis, vel, GetScale());
}


void CTB_OriDlg::OnEnChangeFeedx()
{
	StoreKinematicParams();
}


void CTB_OriDlg::OnEnChangeFeedy()
{
	StoreKinematicParams();
}


void CTB_OriDlg::OnEnChangeRpm()
{
	this->CheckRadioButton(RPMRadio.GetDlgCtrlID(), VelocityRadio.GetDlgCtrlID(), RPMRadio.GetDlgCtrlID());
	StoreKinematicParams();
}


void CTB_OriDlg::OnEnChangeVelocity()
{
	this->CheckRadioButton(RPMRadio.GetDlgCtrlID(), VelocityRadio.GetDlgCtrlID(), VelocityRadio.GetDlgCtrlID());
	StoreKinematicParams();
}

void CTB_OriDlg::StoreKinematicParams()
{
	if (CurrentlyUpdating) return;
	CString s_fx, s_fy, s_v, s_rpm;
	FeedXTB.GetWindowText(s_fx);
	feedx = _wtof(s_fx);
	FeedYTB.GetWindowText(s_fy);
	feedy = _wtof(s_fy);
	int rpmcheck = RPMRadio.GetCheck();
	int velcheck = VelocityRadio.GetCheck();
	if (rpmcheck)
	{
		RPMTB.GetWindowText(s_rpm);
		double rpm = _wtof(s_rpm);
		CString s_diam;
		DiamTB.GetWindowText(s_diam);
		double diam = _wtof(s_diam);
		relvel = M_PI * diam * rpm / 60;
		feedx *= rpm / 60;
		feedy *= rpm / 60;
	}
	else if (velcheck)
	{
		VelocityTB.GetWindowText(s_v);
		relvel = _wtof(s_v) / 60;
	}
}

void CTB_OriDlg::LoadKinematicParams(double l_feedx, double l_feedy, double defaultn, double defaultv)
{
	CurrentlyUpdating = true;
	CString s_fx, s_fy, s_v, s_rpm;
	s_fx.Format(_T("%f"), l_feedx);
	s_fy.Format(_T("%f"), l_feedy);
	s_v.Format(_T("%f"), defaultv);
	s_rpm.Format(_T("%f"), defaultn);
	FeedXTB.SetWindowText(s_fx);
	FeedYTB.SetWindowText(s_fy);
	VelocityTB.SetWindowText(s_v);
	RPMTB.SetWindowText(s_rpm);
	feedx = l_feedx;
	feedy = l_feedy;
	CString s_diam;
	DiamTB.GetWindowText(s_diam);
	double diam = _wtof(s_diam);
	relvel = M_PI * diam;
	CurrentlyUpdating = false;
}

double CTB_OriDlg::GetScale()
{
	CString s_s;
	ScaleTB.GetWindowText(s_s);
	double scale = (_wtof(s_s));
	return scale;
}

void CTB_OriDlg::SetScale(double scale)
{
	CString s_s; s_s.Format(L"%f", scale);
	ScaleTB.SetWindowText(s_s);
}
void CTB_OriDlg::OnEnChangeScale()
{
}


void CTB_OriDlg::OnLvnItemchangedIilist(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int index = pNMLV->iItem;
	if (pNMLV->uNewState & LVIS_STATEIMAGEMASK && pNMLV->iItem >= 0)
	{
		IndInsAttributes atrs;
		InsertProvider->QueryIndInsAttributes(index, &atrs);
		atrs.IsDisabled = IndInsListView.GetCheck(pNMLV->iItem) == FALSE;
		InsertProvider->UpdateIndInsAttributes(index, &atrs);
	}
	*pResult = 0;
}
