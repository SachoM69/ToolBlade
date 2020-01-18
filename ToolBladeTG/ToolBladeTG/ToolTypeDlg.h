#pragma once
#include "afxwin.h"


// ToolTypeDlg dialog

class ToolTypeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ToolTypeDlg)

public:
	ToolTypeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ToolTypeDlg();

// Dialog Data
	enum { IDD = IDD_TOOLTYPEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox ToolTypeList;
};
