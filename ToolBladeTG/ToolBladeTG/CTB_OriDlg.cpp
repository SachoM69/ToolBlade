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
	, ShowReliefGraph(FALSE)
	, ShowReliefKinematicGraph(FALSE)
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
	UpdateDisplayDefault(&ori);

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
	DDX_Check(pDX, IDC_DISPLAYRELIEFANGLESCHECK, ShowReliefGraph);
	DDX_Check(pDX, IDC_DISPLAYKINEMATICRELIEFANGLESCHECK, ShowReliefKinematicGraph);
	DDX_Control(pDX, IDC_DISPLAYRELIEFANGLESCHECK, ShowReliefGraphCheck);
	DDX_Control(pDX, IDC_DISPLAYKINEMATICRELIEFANGLESCHECK, ShowReliefKinematicGraphCheck);
	DDX_Control(pDX, IDC_SELECTFPM, FPMRadio);
	DDX_Control(pDX, IDC_SELECTFPR, FPRRadio);
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
	ON_BN_CLICKED(IDC_DISPLAYRELIEFANGLESCHECK, &CTB_OriDlg::OnBnClickedDisplayreliefanglescheck)
	ON_BN_CLICKED(IDC_DISPLAYKINEMATICRELIEFANGLESCHECK, &CTB_OriDlg::OnBnClickedDisplaykinematicreliefanglescheck)
	ON_BN_CLICKED(IDC_SELECTFPM, &CTB_OriDlg::OnBnClickedSelectfpm)
	ON_BN_CLICKED(IDC_SELECTFPR, &CTB_OriDlg::OnBnClickedSelectfpr)
	ON_BN_CLICKED(IDC_SELECTRPM, &CTB_OriDlg::OnBnClickedSelectrpm)
	ON_BN_CLICKED(IDC_SELECTVELOCITY, &CTB_OriDlg::OnBnClickedSelectvelocity)
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
	
	object->IIVertex(PointCB.GetCurSel(), pos, edgept, V, Ax3);
	InsertProvider->ShowPoint(edgept, 0, true);
	//InsertProvider->ShowPoint(object->XExtremityPoint(), 1, true);
	//InsertProvider->ShowPoint(object->YExtremityPoint(), 2, true);
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
		UpdateDisplayDefault(&a);
	}

	*pResult = 0;
}


void CTB_OriDlg::OnEnChangeGamma()
{
	if (CurrentlyUpdating) return;
	UpdateDisplayDefault();
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
	StoreKinematicParams();
	OnEnChangeGamma();
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
	UpdateDisplayDefault();
}

void CTB_OriDlg::OnCbnSelchangeActiveedge()
{
	UpdateDisplayDefault();
}


void CTB_OriDlg::OnBnClickedOk()
{
	IndInsOrientation a;
	InsertProvider->QueryIndInsOrientation(CurrentIndex, &a);
	StoreToParams(&a);
	InsertProvider->UpdateIndInsOrientation(CurrentIndex, &a);
	CDialogEx::OnOK();
}



void CTB_OriDlg::OnEnChangeFeedx()
{
	StoreKinematicParams();
	UpdateDisplayDefault();
}


void CTB_OriDlg::OnEnChangeFeedy()
{
	StoreKinematicParams();
	UpdateDisplayDefault();
}


void CTB_OriDlg::OnEnChangeRpm()
{
	this->CheckRadioButton(RPMRadio.GetDlgCtrlID(), VelocityRadio.GetDlgCtrlID(), RPMRadio.GetDlgCtrlID());
	StoreKinematicParams();
	UpdateDisplayDefault();
}


void CTB_OriDlg::OnEnChangeVelocity()
{
	this->CheckRadioButton(RPMRadio.GetDlgCtrlID(), VelocityRadio.GetDlgCtrlID(), VelocityRadio.GetDlgCtrlID());
	StoreKinematicParams();
	UpdateDisplayDefault();
}

void CTB_OriDlg::StoreKinematicParams()
{
	if (CurrentlyUpdating) return;
	CString s_fx, s_fy, s_v, s_rpm;
	CString s_diam;
	DiamTB.GetWindowText(s_diam);
	double diam = _wtof(s_diam);
	double velocity = -1;
	double rpm = -1;
	int rpmcheck = RPMRadio.GetCheck();
	int velcheck = VelocityRadio.GetCheck();
	if (rpmcheck)
	{
		RPMTB.GetWindowText(s_rpm);
		rpm = _wtof(s_rpm);
		velocity = M_PI * diam * rpm / 60;
	}
	else if (velcheck)
	{
		VelocityTB.GetWindowText(s_v);
		velocity = _wtof(s_v) / 60;
		rpm = velocity * 60 / M_PI / diam;
	}
	FeedXTB.GetWindowText(s_fx);
	double feedx = _wtof(s_fx);
	FeedYTB.GetWindowText(s_fy);
	double feedy = _wtof(s_fy);
	int mmpscheck = FPMRadio.GetCheck();
	int mmperrevcheck = FPRRadio.GetCheck();
	if (mmpscheck)
	{
		feedx /= 60;
		feedy /= 60;
	}
	else if (mmperrevcheck)
	{
		feedx *= rpm / 60;
		feedy *= rpm / 60;
	}
	FeedX_PS = feedx;
	FeedY_PS = feedy;
	Velocity_MPS = velocity;
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
	FeedX_PS = l_feedx;
	FeedY_PS = l_feedy;
	CString s_diam;
	DiamTB.GetWindowText(s_diam);
	double diam = _wtof(s_diam);
	Velocity_MPS = M_PI * diam;
	this->CheckRadioButton(FPMRadio.GetDlgCtrlID(), FPRRadio.GetDlgCtrlID(), FPMRadio.GetDlgCtrlID());
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
	UpdateDisplayDefault();
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


void CTB_OriDlg::OnBnClickedDisplayreliefanglescheck()
{
	UpdateDisplayDefault();
}


void CTB_OriDlg::OnBnClickedDisplaykinematicreliefanglescheck()
{
	UpdateDisplayDefault();
}


void CTB_OriDlg::OnBnClickedDisplayreliefangles()
{
}

void CTB_OriDlg::OnBnClickedDisplaykinematicreliefangles()
{
}

void CTB_OriDlg::UpdateDisplayDefault()
{
	IndInsOrientation a;
	InsertProvider->QueryIndInsOrientation(CurrentIndex, &a);
	StoreToParams(&a);
	InsertProvider->UpdateIndInsOrientation(CurrentIndex, &a);
	UpdateDisplayDefault(&a);
}

void CTB_OriDlg::UpdateDisplayDefault(const IndInsOrientation* orientation_data)
{
	int flags = UD_ACTIVEPOINT;
	if (ShowReliefGraphCheck.GetCheck() == TRUE) flags |= UD_RELIEF_ANGLE;
	if (ShowReliefKinematicGraphCheck.GetCheck() == TRUE) flags |= UD_KINEMATIC_RELIEF_ANGLE;
	UpdateDisplay(orientation_data, flags);
}

void CTB_OriDlg::UpdateDisplay(const IndInsOrientation* orientation_data, int flags)
{
	InsertProvider->RefreshCutter(CurrentIndex, orientation_data);
	if (flags & UD_ACTIVEPOINT)
	{
		DrawEdgePoint();
	}
	if (flags & UD_RELIEF_ANGLE)
	{
		const IIndexableInsertSeated* iis;
		InsertProvider->QueryIndInsObjectSeated(CurrentIndex, &iis);
		InsertProvider->GraphReliefAngle(CurrentIndex, iis, GetScale());
	}
	else InsertProvider->HideReliefAngle(CurrentIndex);
	if (flags & UD_KINEMATIC_RELIEF_ANGLE)
	{
		const IIndexableInsertSeated* iis;
		InsertProvider->QueryIndInsObjectSeated(CurrentIndex, &iis);
		gp_Vec vel = gp_Vec(FeedX_PS, FeedY_PS, Velocity_MPS);
		InsertProvider->GraphKinematicReliefAngle(CurrentIndex, iis, vel, GetScale());
	}
	else InsertProvider->HideKinematicReliefAngle(CurrentIndex);
	InsertProvider->UpdateDisplay();
}

void CTB_OriDlg::OnBnClickedSelectfpm()
{
	StoreKinematicParams();
	UpdateDisplayDefault();
}


void CTB_OriDlg::OnBnClickedSelectfpr()
{
	StoreKinematicParams();
	UpdateDisplayDefault();
}


void CTB_OriDlg::OnBnClickedSelectrpm()
{
	StoreKinematicParams();
	UpdateDisplayDefault();
}


void CTB_OriDlg::OnBnClickedSelectvelocity()
{
	StoreKinematicParams();
	UpdateDisplayDefault();
}
