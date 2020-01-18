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
	void GetComplexStruct(IndInsert* IIt);
	void SetComplexStruct(const IndInsert* IIt);
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
	//заполнение списка IIDimList. Вызывается из SetDimList 
	void InitArr(CString a[], int n);
	CString *p_a;
	// Группа форм пластин
	CComboBox IGroupList;
	int IGroup;//номер группы пластин
	// Форма пластины
	CComboBox IIFormList;
	int IIForm;//номер формы в списке
	double II_eps;
	int II_n;
	// Размеры пластины
	CComboBox IIDimList;
	int IIn_d;//номер в списке размеров
	double II_Dim;//размер 
	// Список обозначений задних углов
	CComboBox IIRackAnglList;
	int nRackAngle;
	double II_RackAng;
	wchar_t FormChar;
	// Список обозначений класса допуска
	CComboBox IITolClassList;
	wchar_t II_TolClass;
	// Список обозначений конструктивных особенностей пластины
	CComboBox IIKonsFeatureList;
	int II_HT; //наличие и форма отверстия
	// Толщина пластины
	CComboBox IIThicknessList;
	double II_Thick;
	int IIn_Thick;
	CComboBox IIVertFormList;//форма вершины пластины
	int II_VertForm;
	CComboBox IIVertDimList;
	int IIn_R;//номер радиуса в списке
	double II_r;//величина радиуса
	CComboBox IIDirList;//Направление резания
	int II_Dir;
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
	CSliderCtrl ActiveEdgePos;// Скроллер выбора положения расчетной точки на лезвии инструмента
	Standard_Integer EdgePos; //Положение бегунка выбора положения точки на режущей кромке
	Standard_Real EdgePos_t;//
	int d0;//для выбора первого размера из массива
	//диаметр пластины
//	double  d;
	double II_B, II_L;//размеры пластин 3 и 4 групп

	//подготовка данных и заполнение списка IIDimList
	void SetDimList();
	void SetRackAnglList();
	void SetTolClassList();
	void SetKonsFeatureList();
	void SetThicknessList();
	void SetVertFormList();
	void SetVertDimList();
	void SetDirList();
	void SetActiveEdgeList();

	void DrawEdgePoint();//Рисование расчетной точки на лезвии 

	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnCbnSelchangeActiveedge();
	afx_msg void OnTRBNThumbPosChangingActiveedgepos(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnNMThemeChangedActiveedgepos(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnNMReleasedcaptureActiveedgepos(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnBnClickedRefresh();
	afx_msg void OnEnChangeEdit1();
};
