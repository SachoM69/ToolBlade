// ToolTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ToolBladeTG.h"
#include "ToolTypeDlg.h"
#include "afxdialogex.h"


// ToolTypeDlg dialog

IMPLEMENT_DYNAMIC(ToolTypeDlg, CDialogEx)

ToolTypeDlg::ToolTypeDlg(IInstrInsList* myprov, CWnd* pParent /*=NULL*/)
	: CDialogEx(ToolTypeDlg::IDD, pParent), InsertProvider(myprov)
{
	Create(ToolTypeDlg::IDD);
	ShowWindow(SW_SHOW);
}

ToolTypeDlg::~ToolTypeDlg()
{
}

void ToolTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TOOLTYPETT, ToolTypeList);
	DDX_Control(pDX, IDC_DIRTOOLTYPETT, CuttingDirList);
}


BEGIN_MESSAGE_MAP(ToolTypeDlg, CDialogEx)
	ON_BN_CLICKED(IDC_LOADMODEL, &ToolTypeDlg::OnBnClickedLoadmodel)
	ON_BN_CLICKED(IDC_TOOLPREVIEW, &ToolTypeDlg::OnBnClickedToolpreview)
//	ON_CBN_EDITCHANGE(IDC_TOOLTYPETT, &ToolTypeDlg::OnCbnEditchangeTooltype)
//	ON_CBN_EDITCHANGE(IDC_DIRTOOLTYPETT, &ToolTypeDlg::OnCbnEditchangeDirtooltype)
	ON_CBN_SELENDOK(IDC_TOOLTYPETT, &ToolTypeDlg::OnCbnSelendokTooltypett)
	ON_CBN_SELENDOK(IDC_DIRTOOLTYPETT, &ToolTypeDlg::OnCbnSelendokDirtooltypett)
END_MESSAGE_MAP()


// ToolTypeDlg message handlers


BOOL ToolTypeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	ToolData b;
	InsertProvider->QueryToolData(&b);
	ToolTypeList.SetCurSel(b.ToolType);
	CuttingDirList.SetCurSel(b.CutDirection);
	return true;
}

void ToolTypeDlg::OnBnClickedLoadmodel()
{
	CFileDialog fileDialog(TRUE, NULL, L"*.stp;*.step");
	INT_PTR result = fileDialog.DoModal();
	if (result == IDOK)
	{
		auto tool = InsertProvider->QueryToolObject();
		auto path = fileDialog.GetPathName();
		tool->LoadShapeFromSTEP(path.GetBuffer());
	}
	UpdateDisplayDefault();
}


void ToolTypeDlg::OnBnClickedToolpreview()
{
	UpdateDisplayDefault();
}


void ToolTypeDlg::StoreToolType()
{
	int sel = ToolTypeList.GetCurSel();
	ToolType tt;
	switch (sel)
	{
	case 1:
		tt = Boring_Cutter;
		break;
	case 2:
		tt = Drilling_Tool;
		break;
	case 3:
		tt = Milling_Tool;
		break;
	default:
		tt = Turning_Cutter;
		break;
	}
	InsertProvider->UpdateToolType(tt);
}

void ToolTypeDlg::StoreToolDir()
{
	int sel = CuttingDirList.GetCurSel();
	DirToolType dtt;
	switch (sel)
	{
	case 1:
		dtt = DirTool_Left;
		break;
	default:
		dtt = DirTool_Right;
		break;
	}
	InsertProvider->UpdateToolDirection(dtt);
}

//void ToolTypeDlg::OnCbnEditchangeTooltype()
//{
//	StoreToolType();
//	UpdateDisplayDefault();
//}


//void ToolTypeDlg::OnCbnEditchangeDirtooltype()
//{
//	StoreToolDir();
//	UpdateDisplayDefault();
//}

void ToolTypeDlg::UpdateDisplayDefault()
{
	int flags = UD_SHOWAXIS;
	//if (ShowReliefGraphCheck.GetCheck() == TRUE) flags |= UD_RELIEF_ANGLE;
	//if (ShowReliefKinematicGraphCheck.GetCheck() == TRUE) flags |= UD_KINEMATIC_RELIEF_ANGLE;
	//if (ShowPlanes.GetCheck() == TRUE) flags |= UD_SHOWPLANES;
	UpdateDisplay(flags);
}

void ToolTypeDlg::UpdateDisplay(int flags)
{
	InsertProvider->RefreshTool();
	for (int i = 0; i < InsertProvider->QueryToolObject()->InsertCount(); i++)
	{
		IndInsOrientation orientation_data;
		InsertProvider->QueryIndInsOrientation(i, &orientation_data);
		if (flags & UD_RELIEF_ANGLE)
		{
			// TODO
			/*
			const IIndexableInsertSeated* iis;
			InsertProvider->QueryIndInsObjectSeated(i, &iis);
			InsertProvider->GraphReliefAngle(i, iis, GetScale());
			*/
		}
		else InsertProvider->HideReliefAngle(i);
		if (flags & UD_KINEMATIC_RELIEF_ANGLE)
		{
			// TODO
			/*
			const IIndexableInsertSeated* iis;
			InsertProvider->QueryIndInsObjectSeated(i, &iis);
			gp_Vec vel = gp_Vec(FeedX_PS, FeedY_PS, Velocity_MPS);
			InsertProvider->GraphKinematicReliefAngle(i, iis, vel, GetScale());
			*/
		}
		else InsertProvider->HideKinematicReliefAngle(i);
		if (flags & UD_SHOWPLANES)
		{
			const IIndexableInsertSeated* iis;
			InsertProvider->QueryIndInsObjectSeated(i, &iis);
			gp_Pnt point_on_edge;
			gp_Vec tn;
			gp_Ax3 ax;
			iis->IIVertex(orientation_data.EdgeIndex, orientation_data.EdgePosition, point_on_edge, tn, ax);
			gp_Pln norm = gp_Pln(point_on_edge, iis->NormalToReferencePlane());
			gp_Pln tan = gp_Pln(point_on_edge, tn);
			InsertProvider->ShowPlane(norm, 0, true);
			InsertProvider->ShowPlane(tan, 1, true);
		}
		else
		{
			InsertProvider->ShowPlane(gp_Pln(), 0, false);
			InsertProvider->ShowPlane(gp_Pln(), 1, false);
		}
		if (flags & UD_SHOWAXIS)
		{
			auto tool = InsertProvider->QueryToolObject();
			InsertProvider->ShowAxis(tool->ToolAxis(), 0, true);
		} else InsertProvider->ShowAxis(gp_Vec(), 0, false);
		InsertProvider->UpdateDisplay();
	}
}


void ToolTypeDlg::OnCbnSelendokTooltypett()
{
	StoreToolType();
	UpdateDisplayDefault();
}


void ToolTypeDlg::OnCbnSelendokDirtooltypett()
{
	StoreToolDir();
	UpdateDisplayDefault();
}
