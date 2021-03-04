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
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedLoadmodel();
	afx_msg void OnBnClickedToolpreview();
	void StoreToolType();
	void StoreToolDir();
//	afx_msg void OnCbnEditchangeTooltype();
//	afx_msg void OnCbnEditchangeDirtooltype();
#define UD_RELIEF_ANGLE 2
#define UD_KINEMATIC_RELIEF_ANGLE 4
#define UD_ACTIVEPOINT 8
#define UD_SHOWPLANES 16
#define UD_SHOWAXIS 32
	void UpdateDisplayDefault();
	void UpdateDisplay(int flags);
	CComboBox ToolTypeList;
	CComboBox CuttingDirList;
	afx_msg void OnCbnSelendokTooltypett();
	afx_msg void OnCbnSelendokDirtooltypett();
};
