#pragma once
#include "afxwin.h"
#include "ToolDataList.h"


// ToolTypeDlg dialog

class ToolTypeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ToolTypeDlg)

public:
	ToolTypeDlg(IInstrInsList* myprov, CWnd* pParent = NULL);   // standard constructor
	virtual ~ToolTypeDlg();

// Dialog Data
	enum { IDD = IDD_TOOLTYPEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	IInstrInsList* InsertProvider;
public:
	CComboBox ToolTypeList;
	afx_msg void OnBnClickedLoadmodel();
	afx_msg void OnBnClickedToolpreview();
};
