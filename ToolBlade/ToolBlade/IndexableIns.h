#pragma once
#include <vector>


typedef unsigned int uint;

class CIndexableIns : public CIndexableInsert//, CPart
{
protected:
	//int n; //����� ������
	//double diam, radius;
	//double eps, alpha, gamma;//����
/*	char Group, Form, RackAng, Toler, Constr;
	Standard_CString Thickness, VertForm, CutEdge, Direct, Spec, Dimensions;*/
	IndInsert IInst;
	std::vector<gp_Pnt> node_p;//������ ������� �����
	std::vector<int>  d_order;//������ �������� ��������� ������
	std::vector<double> weight; //������ ����� ������� �����
	std::vector<Handle(Geom_BezierCurve)> curves;
	std::vector<int>  npmain;//������ ������� ������� �����, ������������� ������� ��������
public:
//	CIndexableIns(Standard_CString);
	CIndexableIns(const IndInsert*);
	CIndexableIns(const CIndexableIns&);
	CIndexableIns(CIndexableIns&&);
	~CIndexableIns(void);
//	void SetData(Standard_CString);
	TopoDS_Shape ConstructToolBlade();
//	void ReadRealVals(void);
	void rrm();//�������������� �������������
	void rrrm();//������� ��������
	void rtrm();//�������������� ��������������� � ����������� � ������������ ���������
	void rtrm_sh();//�������������� ��������������� � ����������� � ������� ���������
	void nn();//���������������� ���������������
	virtual int NumPoint() const override;//����� ����������� �����, ������� �� ������� ��������
	virtual void IIVertex(Standard_Integer n, Standard_Real t, gp_Pnt &P, gp_Vec &V, gp_Ax3 &Ax3) const override;//���������� �����, ������� �� ������� ��������, n- ����� ����� � ������� ������� ����� node_p
	void IIAx(Standard_Integer n, Standard_Real t, gp_Ax3 &Ax3);
	void swap(CIndexableIns&);
	
};

/*class CIndexableInsESEA : public CIndexableIns //�������������� � �������������
{

public:
	CIndexableInsESEA(void);
	CIndexableInsESEA(Standard_CString);
	~CIndexableInsESEA(void);
	virtual TopoDS_Shape ConstructToolBlade() override;
};*/