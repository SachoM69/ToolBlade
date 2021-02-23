#pragma once
#include "afxwin.h"

#include "ToolDataList.h"
#include "afxcmn.h"

// CTB_DesDlg dialog

class CTB_DesDlg : public CDialog
{
	DECLARE_DYNAMIC(CTB_DesDlg)

public:
	CTB_DesDlg(IInstrInsList*, CWnd* pParent = NULL);   // standard constructor
	virtual ~CTB_DesDlg();

// Dialog Data
	enum { IDD = IDD_INDINS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//virtual void OnOK();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void StoreToParams(IndInsParameters* IIt);
	void LoadFromParams(const IndInsParameters* IIt);

	void StoreToolType();
	void StoreToolDir();
private:
	void CollectDlgData();
	void UnpackDlgData();
	bool HasDlgDataChanged();
	afx_msg void OnCbnSelchangeIndinsform();
	afx_msg void OnCbnSelchangeIgroup();
	afx_msg void OnCbnSelchangeIidim();
	afx_msg void OnCbnSelchangeIndinsvertdim();
	afx_msg void OnCbnSelchangeIndinsvertform();
	afx_msg void OnCbnSelchangeIndinsdir();
	afx_msg void OnList_Dblclk(NMHDR *pNMHDR, LRESULT *pResult);

	IInstrInsList* InsertProvider;
	int CurrentIndex;
	//���������� ������ IIDimList. ���������� �� SetDimList 
	void InitArr(CString a[], int n);
	CString *p_a;
	// ������ ���� �������
	CComboBox IGroupList;
	int IGroup;//����� ������ �������
	// ����� ��������
	CComboBox IIFormList;
	int IIForm;//����� ����� � ������
	double II_eps;
	int II_n;
	// ������� ��������
	CComboBox IIDimList;
	int IIn_d;//����� � ������ ��������
	double II_Dim;//������ 
	// ������ ����������� ������ �����
	CComboBox IIReliefAnglList;
	int nReliefAngle;
	double II_ReliefAng;
	// �������� ��������� ����
	CEdit RackAngleTB;
	double II_RackAng;

	wchar_t StandardShapeCode;
	// ������ ����������� ������ �������
	CComboBox IITolClassList;
	wchar_t II_TolClass;
	// ������ ����������� �������������� ������������ ��������
	CComboBox IIKonsFeatureList;
	int II_HT; //������� � ����� ���������
	// ������� ��������
	CComboBox IIThicknessList;
	double II_Thick;
	int IIn_Thick;
	CComboBox IIVertFormList;//����� ������� ��������
	VertForm II_VertForm;
	CComboBox IIVertDimList;
	int IIn_R;//����� ������� � ������
	double II_r;//�������� �������
	CComboBox IIDirList;//����������� �������
	CutDirection II_Dir;
	CComboBox IIDimHoleList;
	double II_DimHole;
	int IIn_DHole;
	CSpinButtonCtrl TeethCntSpin;
	CComboBox ToolTypeList;
	CComboBox CuttingDirList;
	CListCtrl IndInsListView;
	CComboBox ActiveEdgeList;
	int II_ActiveEdge; //����� ������, ��������� � �������� �������
	CImageList* ImageList;
	CSliderCtrl ActiveEdgePos;// �������� ������ ��������� ��������� ����� �� ������ �����������
	CMenu ListViewContextMenu; // ���� "�������", "��������", ...
	Standard_Integer EdgePos; //��������� ������� ������ ��������� ����� �� ������� ������
	Standard_Real EdgePos_t;//
	int d0;//��� ������ ������� ������� �� �������
	//������� ��������
//	double  d;
	double II_B, II_L;//������� ������� 3 � 4 �����

	//���������� ������ � ���������� ������ IIDimList
	void SetDimList();
	void SetReliefAnglList();
	void SetTolClassList();
	void SetKonsFeatureList();
	void SetThicknessList();
	void SetVertFormList();
	void SetVertDimList();
	void SetDirList();
	void SetActiveEdgeList();

	void DrawEdgePoint();//��������� ��������� ����� �� ������ 

	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnCbnSelchangeActiveedge();
	afx_msg void OnTRBNThumbPosChangingActiveedgepos(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnNMThemeChangedActiveedgepos(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnNMReleasedcaptureActiveedgepos(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnBnClickedRefresh();
//	afx_msg void OnEnChangeEdit1();
	afx_msg void OnNMRClickIilist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedIilist(NMHDR *pNMHDR, LRESULT *pResult);

	// ���� � ������ �������
	void CTB_DesDlg::MenuBtnDelete();
	void CTB_DesDlg::MenuBtnEdit();
	void CTB_DesDlg::MenuBtnNew();

	// ������������ ������ ������� �� ���������
	void UpdateInsertList(void);
	// ������ ��������� �������� ��������
	void UpdateInsertListSoft(void);
};
