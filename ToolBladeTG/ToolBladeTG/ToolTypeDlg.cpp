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

}

ToolTypeDlg::~ToolTypeDlg()
{
}

void ToolTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TOOLTYPELIST, ToolTypeList);
}


BEGIN_MESSAGE_MAP(ToolTypeDlg, CDialogEx)
	ON_BN_CLICKED(IDC_LOADMODEL, &ToolTypeDlg::OnBnClickedLoadmodel)
	ON_BN_CLICKED(IDC_TOOLPREVIEW, &ToolTypeDlg::OnBnClickedToolpreview)
END_MESSAGE_MAP()


// ToolTypeDlg message handlers


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
}


void ToolTypeDlg::OnBnClickedToolpreview()
{
	InsertProvider->RefreshTool();
}
