// ToolTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ToolBladeTG.h"
#include "ToolTypeDlg.h"
#include "afxdialogex.h"


// ToolTypeDlg dialog

IMPLEMENT_DYNAMIC(ToolTypeDlg, CDialogEx)

ToolTypeDlg::ToolTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ToolTypeDlg::IDD, pParent)
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
END_MESSAGE_MAP()


// ToolTypeDlg message handlers
