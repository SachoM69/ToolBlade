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
	//заполнение списка IIDimList. ¬ызываетс€ из SetDimList 
	void InitArr(CString a[], int n);
	CString *p_a;
	// √руппа форм пластин
	CComboBox IGroupList;
	int IGroup;//номер группы пластин
	// ‘орма пластины
	CComboBox IIFormList;
	int IIForm;//номер формы в списке
	double II_eps;
	int II_n;
	// –азмеры пластины
	CComboBox IIDimList;
	int IIn_d;//номер в списке размеров
	double II_Dim;//размер 
	// —писок обозначений задних углов
	CComboBox IIReliefAnglList;
	int nReliefAngle;
	double II_ReliefAng;
	// ¬еличина переднего угла
	CEdit RackAngleTB;
	double II_RackAng;

	wchar_t StandardShapeCode;
	// —писок обозначений класса допуска
	CComboBox IITolClassList;
	wchar_t II_TolClass;
	// —писок обозначений конструктивных особенностей пластины
	CComboBox IIKonsFeatureList;
	int II_HT; //наличие и форма отверсти€
	// “олщина пластины
	CComboBox IIThicknessList;
	double II_Thick;
	int IIn_Thick;
	CComboBox IIVertFormList;//форма вершины пластины
	VertForm II_VertForm;
	CComboBox IIVertDimList;
	int IIn_R;//номер радиуса в списке
	double II_r;//величина радиуса
	CComboBox IIDirList;//Ќаправление резани€
	CutDirection II_Dir;
	CComboBox IIDimHoleList;
	double II_DimHole;
	int IIn_DHole;
	CSpinButtonCtrl TeethCntSpin;
	CComboBox ToolTypeList;
	CComboBox CuttingDirList;
	CListCtrl IndInsListView;
	CComboBox ActiveEdgeList;
	int II_ActiveEdge; //номер кромки, выбранной в качестве рабочей
	CImageList* ImageList;
	CSliderCtrl ActiveEdgePos;// —кроллер выбора положени€ расчетной точки на лезвии инструмента
	CMenu ListViewContextMenu; // меню "удалить", "добавить", ...
	Standard_Integer EdgePos; //ѕоложение бегунка выбора положени€ точки на режущей кромке
	Standard_Real EdgePos_t;//
	int d0;//дл€ выбора первого размера из массива
	//диаметр пластины
//	double  d;
	double II_B, II_L;//размеры пластин 3 и 4 групп

	//подготовка данных и заполнение списка IIDimList
	void SetDimList();
	void SetReliefAnglList();
	void SetTolClassList();
	void SetKonsFeatureList();
	void SetThicknessList();
	void SetVertFormList();
	void SetVertDimList();
	void SetDirList();
	void SetActiveEdgeList();

	void DrawEdgePoint();//–исование расчетной точки на лезвии 

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

	// меню в списке пластин
	void CTB_DesDlg::MenuBtnDelete();
	void CTB_DesDlg::MenuBtnEdit();
	void CTB_DesDlg::MenuBtnNew();

	// перечитываем список пластин из документа
	void UpdateInsertList(void);
	// мен€ем заголовок активной пластины
	void UpdateInsertListSoft(void);
};
