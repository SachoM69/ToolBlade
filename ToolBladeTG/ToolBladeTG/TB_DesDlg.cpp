// TB_DesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ToolBladeTG.h"
#include "TB_DesDlg.h"
#include "afxdialogex.h"
//#include "d:\SB\ToolBlade\ToolBlade\ToolBlade.h"

#define deg M_PI/180
#define RAD(x) ((x)*M_PI/180)
#define DEG(x) ((x)*180/M_PI)

//массив стандартных размеров пластин
const double darr[] = {3.97,4.76,5.56,6.35,7.94,9.525,12.7,15.875,19.05,25.4,31.75};

//массив стандартных значений задних углов
const double RAarr[] = {3., 5., 7., 15., 20., 25., 30., 0., 11.};

//массив стандартных значений толщин
const double THarr[] ={1.59, 1.98, 2.38, 3.18, 3.97,4.76,5.56,6.35,7.94, 8.0, 9.525,12.7};
// CTB_DesDlg dialog

//массив стандартных значений диаметров отверстий
const double d1arr[]={2.5, 2.8, 3.84, 4.4, 5.16, 5.5, 6.35, 7.93};

const double rarr[]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25};

IMPLEMENT_DYNAMIC(CTB_DesDlg, CDialog)

CTB_DesDlg::CTB_DesDlg(IInstrInsList* myprov, CWnd* pParent /*=NULL*/)
	: CDialog(CTB_DesDlg::IDD, pParent)
{
	InsertProvider=myprov;
	CurrentIndex=0;
}

CTB_DesDlg::~CTB_DesDlg()
{
}

void CTB_DesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IGROUP, IGroupList);
	DDX_Control(pDX, IDC_IIFORM, IIFormList);
	DDX_Control(pDX, IDC_IIDIM, IIDimList);
	DDX_Control(pDX, IDC_INDINSRELIEFANG, IIReliefAnglList);
	DDX_Control(pDX, IDC_INDINSRACKANGLE, RackAngleTB);
	DDX_Control(pDX, IDC_INDINSKONSFEATURE, IIKonsFeatureList);
	DDX_Control(pDX, IDC_INDINSTOLCLASS, IITolClassList);
	DDX_Control(pDX, IDC_INDINSTHICKNESS, IIThicknessList);
	DDX_Control(pDX, IDC_INDINSVERTFORM, IIVertFormList);
	DDX_Control(pDX, IDC_INDINSVERTDIM, IIVertDimList);
	DDX_Control(pDX, IDC_INDINSDIR, IIDirList);
	DDX_Control(pDX, IDC_INDINSDHOLE, IIDimHoleList);
	DDX_Control(pDX, IDC_TEETHCNTSPIN, TeethCntSpin);
	DDX_Control(pDX, IDC_TOOLTYPE, ToolTypeList);
	DDX_Control(pDX, IDC_DIRTOOLTYPE, CuttingDirList);
	DDX_Control(pDX, IDC_ACTIVEEDGE, ActiveEdgeList);
	DDX_Control(pDX, IDC_IILIST, IndInsListView);
	DDX_Control(pDX, IDC_ACTIVEEDGEPOS, ActiveEdgePos);
}


BEGIN_MESSAGE_MAP(CTB_DesDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_IIFORM, &CTB_DesDlg::OnCbnSelchangeIndinsform)
	ON_CBN_SELCHANGE(IDC_IGROUP, &CTB_DesDlg::OnCbnSelchangeIgroup)
	ON_CBN_SELCHANGE(IDC_IIDIM, &CTB_DesDlg::OnCbnSelchangeIidim)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &CTB_DesDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_INDINSVERTDIM, &CTB_DesDlg::OnCbnSelchangeIndinsvertdim)
	ON_CBN_SELCHANGE(IDC_INDINSVERTFORM, &CTB_DesDlg::OnCbnSelchangeIndinsvertform)
	ON_CBN_SELCHANGE(IDC_INDINSDIR, &CTB_DesDlg::OnCbnSelchangeIndinsdir)

	ON_NOTIFY(NM_DBLCLK, IDC_IILIST, &CTB_DesDlg::OnList_Dblclk)
	ON_CBN_SELCHANGE(IDC_ACTIVEEDGE, &CTB_DesDlg::OnCbnSelchangeActiveedge)
	ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_ACTIVEEDGEPOS, &CTB_DesDlg::OnTRBNThumbPosChangingActiveedgepos)
	//	ON_NOTIFY(NM_THEMECHANGED, IDC_ACTIVEEDGEPOS, &CTB_DesDlg::OnNMThemeChangedActiveedgepos)
	//	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_ACTIVEEDGEPOS, &CTB_DesDlg::OnNMReleasedcaptureActiveedgepos)
	ON_BN_CLICKED(IDC_REFRESH, &CTB_DesDlg::OnBnClickedRefresh)
	//	ON_EN_CHANGE(IDC_EDIT1, &CTB_DesDlg::OnEnChangeEdit1)
	ON_NOTIFY(NM_RCLICK, IDC_IILIST, &CTB_DesDlg::OnNMRClickIilist)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_IILIST, &CTB_DesDlg::OnLvnItemchangedIilist)
	ON_COMMAND(ID_INSERTLIST_DELETE, &CTB_DesDlg::MenuBtnDelete)
	ON_COMMAND(ID_INSERTLIST_EDIT, &CTB_DesDlg::MenuBtnEdit)
	ON_COMMAND(ID_INSERTLIST_NEW, &CTB_DesDlg::MenuBtnNew)
END_MESSAGE_MAP()


// CTB_DesDlg message handlers

BOOL CTB_DesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	ListViewContextMenu.LoadMenuW(IDR_INSERT_LIST_EDIT);
	IndInsParameters a;
	InsertProvider->QueryIndInsInformation(CurrentIndex, &a);
	LoadFromParams(&a);
	//for (int i=0; i<7; i++) SC[i].SetCurSel(0);
	//SC1.SetCurSel(0)
	UnpackDlgData();
	ToolData b;
	InsertProvider->QueryToolData(&b);
	TeethCntSpin.SetRange32(1, 20);
	TeethCntSpin.SetPos32(b.UsedToothCount);
	//IndInsListView.Create(LVS_SORTASCENDING, 
	ImageList = new CImageList();
	ImageList->Create(16, 16, TRUE, 1, 1);
    ImageList->Add(AfxGetApp()->LoadIcon(IDI_ICONSHOWN));
    ImageList->Add(AfxGetApp()->LoadIcon(IDI_ICONHIDDEN));
	//IndInsListView.SetImageList(ImageList, LVSIL_SMALL);
	IndInsListView.SetExtendedStyle(LVS_EX_CHECKBOXES);
	UpdateInsertList();
	ToolTypeList.SetCurSel(b.ToolType);
	CuttingDirList.SetCurSel(b.CutDirection);
//	int index; const IIndexableInsert* object;
//	InsertProvider->QueryIndInsObject(index, &object);
	ActiveEdgePos.SetRange(0, 100, 0);
	return true;
}

void CTB_DesDlg::CollectDlgData()
{
	IGroup=IGroupList.GetCurSel();
	//GroupCapt aa;
/*	switch(IGroup)
	{
	case 0:
		aa=GCP_ESEA;
		break;
	case 1:
		aa=GCP_ESNA;
		break;
	case 2:
		aa=GCP_NSEA;
		break;
	case 3:
		aa=GCP_NSNA;
		break;
	}*/

	//OnCbnSelchangeIgroup();
	int sel = IIFormList.GetCurSel();
	CString S;
	IIFormList.GetLBText(sel,S);
	//первый символ - обозначение формы
	FormChar=S[0];
	wchar_t S1=S[0];
	II_n=0;
	II_eps=0;
	II_ReliefAng=0.;
	CString s_rack;
	RackAngleTB.GetWindowText(s_rack);
	II_RackAng = RAD(_wtof(s_rack));
	switch (IGroup)
	{
		case 0: switch (S1)
			{
				case 'R': II_n=0; break;
				case 'H' : II_n=6; break;
				case 'O' : II_n=8; break;
				case 'P' : II_n=5; break;
				case 'S' : II_n=4; break;
				case 'T' : II_n=3; break;
			}
			break;
		case 1: switch (S1)
			{
				case 'C' : II_n=2; II_eps=80*deg; break;
				case 'D' : II_n=2; II_eps=55*deg; break;
				case 'E' : II_n=2; II_eps=75*deg; break;
				case 'M' : II_n=2; II_eps=86*deg; break;
				case 'V' : II_n=2; II_eps=35*deg; break;
				case 'W' : II_n=3; II_eps=80*deg; break;
			}
				break;
		case 2: II_L=14;
				II_B=10;
				II_eps=M_PI/2;
				II_r=1;
				break;
		case 3: II_L=14;
				II_B=10;
				II_r=1;
				break;
			switch (S1)
				{
				case 'A' : II_eps=85+deg; break;
				case 'B' : II_eps=82*deg; break;
				case 'K' : II_eps=55*deg; break;
				case 'F' : II_eps=84*deg; break;
				}
	}

	sel = IIReliefAnglList.GetCurSel();	
	II_ReliefAng=RAarr[sel];//валичина заднего угла

	sel = IIKonsFeatureList.GetCurSel();
	II_HT=sel;//Наличие и форма отверстия

	sel = IIDimList.GetCurSel();
	II_Dim=darr[sel+d0];//Размер

	sel = IIThicknessList.GetCurSel();
	II_Thick=THarr[sel];//Толщина

	sel = IIDirList.GetCurSel();
	II_Dir=sel; //Направление

	OnCbnSelchangeIndinsvertform();
	if (II_VertForm==VF_SHARP)
		II_r=0;
	else
	{
		OnCbnSelchangeIndinsvertdim();
	}
//	II_Dir=IIDirList.GetCurSel();
	OnCbnSelchangeIndinsdir();
	IIn_DHole=IIDimHoleList.GetCurSel();
	II_DimHole=d1arr[IIn_DHole];

	// Tolerance
	sel=IITolClassList.GetCurSel();
	IITolClassList.GetLBText(sel, S);
	II_TolClass=S[0];

	II_ActiveEdge=ActiveEdgeList.GetCurSel();
}

void CTB_DesDlg::UnpackDlgData()
{
	IGroupList.SetCurSel(IGroup);
	OnCbnSelchangeIgroup();
	IIFormList.SetCurSel(IIForm);
	SetReliefAnglList();
	IIReliefAnglList.SetCurSel(nReliefAngle);
	CString RackText;
	RackText.Format(_T("%f"), DEG(II_RackAng));
	RackAngleTB.SetWindowText(RackText);
	SetTolClassList();
	SetKonsFeatureList();
	IIKonsFeatureList.SetCurSel(II_HT);
	SetThicknessList();
	IIThicknessList.SetCurSel(IIn_Thick);
	SetDimList();
	IIDimList.SetCurSel(IIn_d);
	SetVertFormList();
	IIVertFormList.SetCurSel(II_VertForm);
	SetVertDimList();
	IIVertDimList.SetCurSel(int(II_r*10)-1);
	SetDirList();
	IIDirList.SetCurSel(II_Dir);
	IIDimHoleList.SetCurSel(IIn_DHole);
	SetActiveEdgeList();
	ActiveEdgeList.SetCurSel(II_ActiveEdge);
	//DrawEdgePoint();
}

void CTB_DesDlg::StoreToParams(IndInsParameters* IIt)
{
	IndInsParameters &II = *IIt;
	II.IGroup=IGroup;
	II.IIForm=IIForm;
	II.FormChar=FormChar;
	II.eps=II_eps;
	II.n=II_n;

	II.ReliefAng=II_ReliefAng;
	II.RackAng = II_RackAng;

	II.HT=II_HT;
	II.Dim=II_Dim;

	II.Thick=II_Thick;
	II.VertForm=II_VertForm;
	if (II_VertForm==VF_SHARP)
		II.r=II_r=0;
	else II.r=II_r;
	II.Dir=II_Dir;//направление резания
	II.DHole=II_DimHole;
	II.TolClass=II_TolClass;
	//II.ActEdge=II_ActiveEdge;
}

void CTB_DesDlg::LoadFromParams(const IndInsParameters* IIt)
{
	IGroup=IIt->IGroup;
	IIForm=IIt->IIForm;
	int sel=0;
	for (int i=0; i<(sizeof(RAarr)/sizeof(double)); i++)
	{
		if (RAarr[i]==IIt->ReliefAng) 
		{
			sel=i; 
			break;
		}
	}
	nReliefAngle=sel;
	II_RackAng = IIt->RackAng;
		//Толщина
	sel=0;
	for (int i=0; i<(sizeof(THarr)/sizeof(double)); i++)
	{
		if (THarr[i]==IIt->Thick) 
		{
			sel=i; 
			break;
		}
	}
	IIn_Thick=sel;

	//диаметр отверстия
	sel=0;
	for (int i=0; i<(sizeof(d1arr)/sizeof(double)); i++)
	{
		if (d1arr[i]==IIt->DHole) 
		{
			sel=i; 
			break;
		}
	}
	IIn_DHole=sel;

	II_HT=IIt->HT;

	sel=0;
	for (int i=0; i<(sizeof(darr)/sizeof(double)); i++)
	{
		if (darr[i]==IIt->Dim) 
		{
			sel=i; 
			break;
		}
	}
	IIn_d=sel;

	FormChar=IIt->FormChar;
	switch (FormChar)
	{
/*	case 'H':
	case 'O':
	case 'P':
	case 'R': d0=3; break;*/
	case 'S': 
	case 'T': 
	case 'C': d0=0; break;
	case 'D': 
	case 'E':
	case 'M':
	case 'V' : d0=1; break;
	case 'W' : d0=2; break;
	default: d0=3;
	}
	IIn_d=sel-d0;
	IIn_d=IIn_d<0?0:IIn_d;

	II_VertForm=IIt->VertForm;
	sel=0;
	if (II_VertForm==VF_SHARP)
		II_r=0;
	else 
		{II_r=IIt->r;
			for (int i=0; i<(sizeof(rarr)/sizeof(double)); i++)
			{
				if ((rarr[i]/10)==IIt->r) 
				{
					sel=i; 
					break;
				}
			}

		}
	IIn_R=sel;
	II_Dir=IIt->Dir;
	II_TolClass=IIt->TolClass;
	//II_ActiveEdge=IIt->ActEdge;
	EdgePos_t = 0;
	EdgePos = 0;

}

void CTB_DesDlg::StoreToolType()
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

void CTB_DesDlg::StoreToolDir()
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


void CTB_DesDlg::OnCbnSelchangeIndinsform()
{
	int sel=IIFormList.GetCurSel();
	ASSERT(sel>=0);
	IIForm=sel;
	OnCbnSelchangeIgroup();
}


void CTB_DesDlg::OnCbnSelchangeIgroup()
{
	//IDC_INDINSFORM
	int i=IIFormList.GetCurSel();
	int iG = IGroupList.GetCurSel();
		if (iG==0) 
		{
		IIFormList.ResetContent();
		IIFormList.AddString (L"H 11 Шестигранная");
		IIFormList.AddString (L"O -- Восьмигранная");
		IIFormList.AddString (L"P 10 Пятигранная");
		IIFormList.AddString (L"R 12 Круглая");
		IIFormList.AddString (L"S 03 Квадратная");
		IIFormList.AddString (L"T 01 Трехгранная");
		}
	else if (iG == 1)
	{
		IIFormList.ResetContent();
		IIFormList.AddString (L"C 05 Ромбическая с углом 80");
		IIFormList.AddString (L"D 13 Ромбическая с углом 55");
		IIFormList.AddString (L"E -- Ромбическая с углом 75");
		IIFormList.AddString (L"M -- Ромбическая с углом 86");
		IIFormList.AddString (L"V -- Ромбическая с углом 35");
		IIFormList.AddString (L"W 02 Шестигранная с углом 80");
		}
	else if (iG == 2)
	{
		IIFormList.ResetContent();
		IIFormList.AddString (L"L 09 Прямоугольная");
	}
	else if (iG == 3)  		
	{
		IIFormList.ResetContent();
		IIFormList.AddString (L"A -- Параллелограммная с углом 85");
		IIFormList.AddString (L"B -- Параллелограммная с углом 82");
		IIFormList.AddString (L"K 08 Параллелограммная с углом 55");
		IIFormList.AddString (L"F 07 Параллелограммная с углом 84");
	}
	else
		{
			IIFormList.ResetContent();
			IIFormList.AddString (L"- - Нестандартная форма");
	}
	IGroup=iG;
	CString S;
#ifdef WELPSHIT
	if (i<0)  i=0;
	IIFormList.SetCurSel(i);
	IIFormList.GetLBText(i, S);
#else
	if(IIForm>=IIFormList.GetCount()) IIForm=0;
	IIFormList.SetCurSel(IIForm);
	IIFormList.GetLBText(IIForm, S);
	IIDimList.SetCurSel(0);
#endif
	//LPTSTR TT = LPTSTR("_");
	FormChar=S.GetAt(0);
	SetDimList();
}

void CTB_DesDlg::SetDimList()
{
	int sel = IIDimList.GetCurSel();
	if (sel<0) sel=0;

	IIDimList.ResetContent();
	d0=0;
	if (FormChar == 'H') 
	{
		CString a[8]={L"03", L"04", L"05", L"07", L"09", L"11", L"14", L"18"};
		d0=3;
		InitArr(a,8);
	}
	else if (FormChar == 'O')
	{
		CString a[8]={L"02",	L"03",	L"04",	L"05",	L"06", L"07", L"10",	L"13"};
		InitArr(a,8);
		d0=3;
	}
	else if (FormChar == 'P')
	{
		CString a[]={L"04", L"05", L"07", L"09", L"11", L"13", L"18", L"23"};
		InitArr(a,8);
		d0=3;
	}
	else if (FormChar == 'S') 
	{
		CString a[]={L"03", L"04", L"05", L"06", L"07", L"09", L"12", L"15", L"19", L"25", L"31"};
		InitArr(a,11);
		d0=0;
	}
	else if (FormChar == 'T')
	{
		CString a[]={L"06", L"08", L"09", L"11", L"13", L"16", L"22", L"27", L"33", L"44", L"54"};
		InitArr(a,11);
		d0=0;
	}
	else if (FormChar == 'C')
	{
		CString a[]={L"03", L"04", L"05", L"06", L"08", L"09", L"12", L"16", L"19", L"25", L"32"};
		InitArr(a,11);
		d0=0;
	}
	else if (FormChar == 'D') 
	{
		CString a[]={L"05", L"06", L"07", L"09", L"11", L"15",  L"19", L"23", L"31", L"38"};
		InitArr(a,10);
		d0=1;
	}
	else if (FormChar == 'E')
	{
		CString a[]={L"04", L"05", L"06", L"08", L"09", L"13",  L"16", L"19", L"26", L"32"};
		InitArr(a,10);
		d0=1;
	}
	else if (FormChar == 'M')
	{
		CString a[]={L"04", L"05", L"06", L"08", L"09", L"12",  L"15", L"19", L"25", L"31"};
		InitArr(a,10);
		d0=1;
	}
	else if (FormChar == 'V')
	{
		CString a[]={L"08", L"09", L"11", L"13", L"16", L"22",  L"27", L"33", L"44", L"54"};
		InitArr(a,10);
		d0=1;
	}
	else if (FormChar == 'W')
	{
		CString a[]={L"03",	L"04",	L"05",	L"06",	L"08",	L"10",	L"13",	L"17", L"21"};
		InitArr(a,9);
		d0=2;
	}
	else if (FormChar == 'R')
	{
		CString a[]={L"06",	L"07",	L"09",	L"12",	L"15",	L"19",	L"25", L"31"};
		InitArr(a,8);
		d0=3;
	}
	else
		IIDimList.AddString (L"Нестандартная форма");

  IIDimList.SetCurSel(sel);
}

void CTB_DesDlg::InitArr(CString a[],int n)
{
//	int II=_countof(a);
	for (int i=0; i<n; i++)
	IIDimList.AddString (a[i]);
	p_a=a;
}

void CTB_DesDlg::OnCbnSelchangeIidim()
{
	int sel = IIDimList.GetCurSel();
	II_Dim=darr[sel+d0];
}


void CTB_DesDlg::SetReliefAnglList()
{
	IIReliefAnglList.ResetContent();
	IIReliefAnglList.AddString(L"A 7 3°");
	IIReliefAnglList.AddString(L"B 8 5°");
	IIReliefAnglList.AddString(L"C 2 7°");
	IIReliefAnglList.AddString(L"D 6 15°");
	IIReliefAnglList.AddString(L"E 4 20°");
	IIReliefAnglList.AddString(L"F 5 25°");
	IIReliefAnglList.AddString(L"G 9 30°");
	IIReliefAnglList.AddString(L"N 1 0°");
	IIReliefAnglList.AddString(L"P 3 11°");
	IIReliefAnglList.AddString(L"O 0 Спец.");
}

void CTB_DesDlg::SetTolClassList()
{
	IITolClassList.ResetContent();
	IITolClassList.AddString(L"A 6");
	IITolClassList.AddString(L"F -");
	IITolClassList.AddString(L"C 4");
	IITolClassList.AddString(L"H -");
	IITolClassList.AddString(L"E 5");
	IITolClassList.AddString(L"G 3");
	IITolClassList.AddString(L"J -");
	IITolClassList.AddString(L"K 7");
	IITolClassList.AddString(L"L -");
	IITolClassList.AddString(L"M 2");
	IITolClassList.AddString(L"N -");
	IITolClassList.AddString(L"U 1");
	// Set selection
	CString S;
	int i;
	for(i=0; i<IITolClassList.GetCount(); i++)
	{
		IITolClassList.GetLBText(i, S);
		if(II_TolClass==S[0])
		{
			IITolClassList.SetCurSel(i);
			break;
		}
	}
	if(i==IITolClassList.GetCount()) IITolClassList.SetCurSel(0);
}

void CTB_DesDlg::SetKonsFeatureList()
{
	IIKonsFeatureList.ResetContent();
	IIKonsFeatureList.AddString(L"N 1");
	IIKonsFeatureList.AddString(L"R 2");
	IIKonsFeatureList.AddString(L"F 6");
	IIKonsFeatureList.AddString(L"A 3");
	IIKonsFeatureList.AddString(L"M 4");
	IIKonsFeatureList.AddString(L"G 5");
	IIKonsFeatureList.AddString(L"W 8");
	IIKonsFeatureList.AddString(L"T 9");
	IIKonsFeatureList.AddString(L"Q -");
	IIKonsFeatureList.AddString(L"U -");
	IIKonsFeatureList.AddString(L"B -");
	IIKonsFeatureList.AddString(L"H -");
	IIKonsFeatureList.AddString(L"C -");
	IIKonsFeatureList.AddString(L"J -");
	IIKonsFeatureList.AddString(L"X 7");
}

void CTB_DesDlg::SetThicknessList()
{
	CString a[12]={L"01", L"T1", L"02", L"03", L"T3", L"04", L"05", L"06", L"07", L"08", L"09", L"12"};

	IIThicknessList.ResetContent();
	for (int i=0; i<12; i++)
	IIThicknessList.AddString (a[i]);
}

void CTB_DesDlg::SetVertDimList()
{
	if (II_VertForm==VF_SHARP)
	{
		IIVertDimList.SetCurSel(-1);
		IIVertDimList.EnableWindow(FALSE);
	}
	else
	{
		IIVertDimList.EnableWindow(TRUE);
		IIVertDimList.SetCurSel(int(II_r*10)-1);
	}
}

void CTB_DesDlg::SetVertFormList()
{
}

void CTB_DesDlg::SetDirList()
{
}

void CTB_DesDlg::SetActiveEdgeList()
{
	const IIndexableInsert* object;
	InsertProvider->QueryIndInsObject(CurrentIndex, &object);
	CString S;
	ActiveEdgeList.ResetContent();
	for(auto i=0; i<object->NumPoint(); i++)
	{
		S.Format(L"%d", i);
		ActiveEdgeList.AddString(S);
	}
	if(II_ActiveEdge>=object->NumPoint()) II_ActiveEdge=object->NumPoint()-1;
}

void CTB_DesDlg::OnClose()
{

	CDialog::OnClose();
}


void CTB_DesDlg::OnDestroy()
{
	CDialog::OnDestroy();
	InsertProvider->ShowPoint(gp_Pnt(), 0, false);
	delete ImageList;
}


void CTB_DesDlg::OnBnClickedOk()
{
	CollectDlgData();

	StoreToolType();
	StoreToolDir();

	IndInsParameters a;
	StoreToParams(&a);
	InsertProvider->UpdateIndInsInformation(CurrentIndex, &a);
	InsertProvider->RefreshCutter(CurrentIndex, &a);
	InsertProvider->UpdateDisplay();

	CDialog::OnOK();
}


void CTB_DesDlg::OnCbnSelchangeIndinsvertdim()
{
	int sel = IIVertDimList.GetCurSel();
	if(sel<0) return;
	CString S;
	IIVertDimList.GetLBText(sel,S);
	II_r=_wtof(S.GetBuffer())*0.1;
}


void CTB_DesDlg::OnCbnSelchangeIndinsvertform()
{
	int sel = IIVertFormList.GetCurSel();
	ASSERT(sel>=0);
	switch(sel)
	{

		case 1 : II_VertForm=VF_FILLET;
			IIVertDimList.EnableWindow(TRUE);
			if(II_r==0) II_r=0.1;
			IIVertDimList.SetCurSel(int(II_r*10)-1);
			break;
		case 2 : II_VertForm=VF_CHAMFER;
			IIVertDimList.EnableWindow(TRUE);
			if(II_r==0) II_r=0.1;
			IIVertDimList.SetCurSel(int(II_r*10)-1);
			break;
		default : 
			{
			II_VertForm=VF_SHARP;
			IIVertDimList.EnableWindow(FALSE);
			IIVertDimList.SetCurSel(-1);
			}

	}
}



void CTB_DesDlg::OnCbnSelchangeIndinsdir()
{
}

void CTB_DesDlg::OnList_Dblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	IndInsParameters a;
	if(pNMItemActivate->iItem!=CurrentIndex)
	{
		CollectDlgData();
		StoreToParams(&a);
		InsertProvider->UpdateIndInsInformation(CurrentIndex, &a);
	}

	if(pNMItemActivate->iItem!=-1)
	{
		CurrentIndex=pNMItemActivate->iItem;
		InsertProvider->QueryIndInsInformation(CurrentIndex, &a);
		LoadFromParams(&a);
		UnpackDlgData();
		UpdateInsertListSoft();
	} else
	{
		int index = CurrentIndex;
		InsertProvider->RequestNewInsert(&index);
		InsertProvider->UpdateIndInsInformation(index, &a);
		CurrentIndex = index;
		UpdateInsertList();
	}
	InsertProvider->RefreshCutter(CurrentIndex, &a);
	InsertProvider->UpdateDisplay();

	*pResult = 0;
}

void CTB_DesDlg::OnNMRClickIilist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	POINT cl_pos = pNMItemActivate->ptAction;
	IndInsListView.ClientToScreen(&cl_pos);
	int click_x, click_y;
	click_x = cl_pos.x;
	click_y = cl_pos.y;
	ListViewContextMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN, click_x, click_y, this);
	*pResult = 0;
}



void CTB_DesDlg::OnCbnSelchangeActiveedge()
{
	II_ActiveEdge = ActiveEdgeList.GetCurSel();
	ASSERT(II_ActiveEdge>=0);
	
	DrawEdgePoint();
}


void CTB_DesDlg::OnTRBNThumbPosChangingActiveedgepos(NMHDR *pNMHDR, LRESULT *pResult)
{
	// This feature requires Windows Vista or greater.
	// The symbol _WIN32_WINNT must be >= 0x0600.
	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
	EdgePos=Standard_Integer(pNMTPC->dwPos);
	EdgePos_t=Standard_Real(EdgePos)/ActiveEdgePos.GetRangeMax();
	DrawEdgePoint();
	*pResult = 0;
}

void CTB_DesDlg::DrawEdgePoint()
{
	const IIndexableInsert* object;
	InsertProvider->QueryIndInsObject(CurrentIndex, &object);
	gp_Pnt edgept; gp_Vec V; gp_Ax3 Ax3;
	object->IIVertex(II_ActiveEdge,EdgePos_t,edgept,V, Ax3);
	InsertProvider->ShowPoint(edgept,0,true);
	//нарисовать стрелку из точки edgept в направлении V
}


void CTB_DesDlg::OnBnClickedRefresh()
{
	CollectDlgData();

	IndInsParameters a;
	StoreToParams(&a);
	InsertProvider->RefreshCutter(CurrentIndex, &a);
	InsertProvider->UpdateDisplay();
}

void CTB_DesDlg::OnLvnItemchangedIilist(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int index = pNMLV->iItem;
	if (pNMLV->uNewState & LVIS_STATEIMAGEMASK && pNMLV->iItem >= 0)
	{
		IndInsAttributes atrs;
		InsertProvider->QueryIndInsAttributes(index, &atrs);
		atrs.IsDisabled = IndInsListView.GetCheck(pNMLV->iItem)==FALSE;
		InsertProvider->UpdateIndInsAttributes(index, &atrs);
	}
	*pResult = 0;
}

// меняем заголовок активной пластины
void CTB_DesDlg::UpdateInsertListSoft(void)
{
	ToolData b;
	InsertProvider->QueryToolData(&b);
	for(auto i=0; i<b.ActualToothCount; i++)
	{
		CString a;
		if(i!=CurrentIndex) a.Format(_T("%d"), i);
		else a.Format(_T("%d (активна)"), i);
		IndInsListView.SetItemText(i, 0, a);
	}
}


// перечитываем список пластин из документа
void CTB_DesDlg::UpdateInsertList(void)
{
	ToolData b;
	InsertProvider->QueryToolData(&b);
	IndInsListView.DeleteAllItems();
	for(auto i=0; i<b.ActualToothCount; i++)
	{
		IndInsAttributes c;
		InsertProvider->QueryIndInsAttributes(i, &c);
		CString a;
		if(i!=CurrentIndex) a.Format(_T("%d"), i);
		else a.Format(_T("%d (активна)"), i);
		IndInsListView.InsertItem(i, a, c.IsDisabled);
		IndInsListView.SetCheck(i, c.IsDisabled?FALSE:TRUE);
	}
}

void CTB_DesDlg::MenuBtnDelete()
{
	int sel = IndInsListView.GetSelectionMark();
	if(sel!=-1)
	{
		InsertProvider->RequestRemoveInsert(sel);
		if(sel<CurrentIndex) CurrentIndex--;
		
		int closest_index = -1;
		IndInsParameters a;
		for(int i=CurrentIndex; i>=0; i--)
		{
			if(InsertProvider->QueryIndInsInformation(i, &a)==S_OK)
			{
				closest_index=i; break;
			}
		}
		if(closest_index==-1)
		{
			InsertProvider->RequestNewInsert(&closest_index);
		}
		CurrentIndex=closest_index;
		InsertProvider->QueryIndInsInformation(CurrentIndex, &a);
		LoadFromParams(&a);
		UnpackDlgData();
		UpdateInsertList();
	}
}

void CTB_DesDlg::MenuBtnEdit()
{
	IndInsParameters a;
	CollectDlgData();
	StoreToParams(&a);
	InsertProvider->UpdateIndInsInformation(CurrentIndex, &a);
	UpdateInsertListSoft();

	int sel = IndInsListView.GetSelectionMark();
	if(sel!=-1)
	{
		CurrentIndex=sel;
		InsertProvider->QueryIndInsInformation(CurrentIndex, &a);
		LoadFromParams(&a);
		UnpackDlgData();
		UpdateInsertListSoft();
	}
}

void CTB_DesDlg::MenuBtnNew()
{
	IndInsParameters a;
	CollectDlgData();
	StoreToParams(&a);
	InsertProvider->UpdateIndInsInformation(CurrentIndex, &a);
	
	int index = CurrentIndex;
	InsertProvider->RequestNewInsert(&index);
	InsertProvider->UpdateIndInsInformation(index, &a);
	CurrentIndex = index;
	UpdateInsertList();
}