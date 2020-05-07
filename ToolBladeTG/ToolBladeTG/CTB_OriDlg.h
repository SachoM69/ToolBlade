﻿#pragma once
#include "ToolDataList.h"


// Диалоговое окно CTB_OriDlg

class CTB_OriDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTB_OriDlg)

public:
	CTB_OriDlg(IInstrInsList*, CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~CTB_OriDlg();
	virtual BOOL OnInitDialog();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ORIENTATION_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()

	void StoreToParams(IndInsOrientation* IIt);
	void LoadFromParams(const IndInsOrientation*);
	void DrawEdgePoint();
	void SetActiveEdgeList();
	void UpdateInsertList(void);
	void UpdateInsertListSoft(void);

private:
	IInstrInsList* InsertProvider;
	int CurrentIndex;

	bool CurrentlyUpdating;
public:
	CEdit GammaTB;
	CEdit LambdaTB;
	CEdit PhiTB;
	CEdit DiamTB;
	CComboBox PointCB;
	CSliderCtrl PointSl;
	afx_msg void OnNMDblclkIilist(NMHDR* pNMHDR, LRESULT* pResult);
	CListCtrl IndInsListView;
	CImageList* ImageList;
	afx_msg void OnEnChangeGamma();
	afx_msg void OnEnChangeLambda();
	afx_msg void OnEnChangePhi();
	afx_msg void OnEnChangeDiameter();
	afx_msg void OnHScroll(UINT, UINT, CScrollBar*);
	afx_msg void OnCbnSelchangeActiveedge();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedDisplayreliefangles();
};
