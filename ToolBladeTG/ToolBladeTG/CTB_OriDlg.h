#pragma once
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

	void StoreKinematicParams();
	void LoadKinematicParams(double FeedX_PS, double FeedY_PS, double defaultn, double defaultv);

	double GetScale();
	void SetScale(double scale);

#define UD_RELIEF_ANGLE 2
#define UD_KINEMATIC_RELIEF_ANGLE 4
#define UD_ACTIVEPOINT 8
#define UD_SHOWPLANES 16
#define UD_SHOWAXIS 32
	void UpdateDisplayDefault();
	void UpdateDisplayDefault(const IndInsOrientation* orientation_data);
	void UpdateDisplay(const IndInsOrientation* orientation_data, int flags);
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
	afx_msg void OnEnChangeFeedx();
	afx_msg void OnEnChangeFeedy();
	afx_msg void OnBnClickedDisplaykinematicreliefangles();
	afx_msg void OnEnChangeRpm();
	afx_msg void OnEnChangeVelocity();

	double FeedX_PS;
	double FeedY_PS;
	double Velocity_MPS;

	CEdit FeedXTB;
	CEdit FeedYTB;
	CEdit RPMTB;
	CEdit VelocityTB;
	CButton RPMRadio;
	CButton VelocityRadio;
	CEdit ScaleTB;
	CEdit AxisRotTB;
	CEdit ZOffsetTB;
	afx_msg void OnEnChangeScale();
	afx_msg void OnEnChangeAxisrot();
	afx_msg void OnEnChangeZoffset();
	afx_msg void OnLvnItemchangedIilist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedDisplayreliefanglescheck();
	afx_msg void OnBnClickedDisplaykinematicreliefanglescheck();
	BOOL ShowReliefGraph;
	BOOL ShowReliefKinematicGraph;
	CButton ShowReliefGraphCheck;
	CButton ShowReliefKinematicGraphCheck;
	CButton FPMRadio;
	CButton FPRRadio;
	afx_msg void OnBnClickedSelectfpm();
	afx_msg void OnBnClickedSelectfpr();
	afx_msg void OnBnClickedSelectrpm();
	afx_msg void OnBnClickedSelectvelocity();
	CButton ShowPlanes;
	afx_msg void OnBnClickedDisplayplanescheck();
	afx_msg void OnDestroy();
};
