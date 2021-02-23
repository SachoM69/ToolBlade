#pragma once
#include "ToolBlade.h"

typedef unsigned int uint;

class CIndexableInsert : public IIndexableInsert//, CPart
{
protected:
	//int n; //����� ������
	//double diam, radius;
	//double eps, alpha, gamma;//����
/*	char Group, Form, RackAng, Toler, Constr;
	Standard_CString Thickness, VertForm, CutEdge, Direct, Spec, Dimensions;*/
	IndInsParameters IInst;
	std::vector<gp_Pnt> node_p;//������ ������� �����
	std::vector<int>  d_order;//������ �������� ��������� ������
	std::vector<double> weight; //������ ����� ������� �����
	std::vector<Handle(Geom_BezierCurve)> curves;
	std::vector<int>  npmain;//������ ������� ������� �����, ������������� ������� ��������
	TopoDS_Shape ToolBladeShape;

public:
//	CIndexableIns(Standard_CString);
	CIndexableInsert(const IndInsParameters*);
	CIndexableInsert(const CIndexableInsert&);
	CIndexableInsert(CIndexableInsert&&);
	~CIndexableInsert(void);
//	void SetData(Standard_CString);
	void CIndexableInsert::ConstructToolBlade();
	virtual TopoDS_Shape GetResultingShape() const override;
//	void ReadRealVals(void);
	// ������� �������� ���������.
	void rrm();//�������������� �������������
	void rrrm();//������� ��������
	void rtrm();//�������������� ��������������� � ����������� � ������������ ���������
	void rtrm_sh();//�������������� ��������������� � ����������� � ������� ���������
	void rtrm_chamfer();//�������������� ��������������� � ����������� � ������
	void nn();//���������������� ���������������

	// ������� �������
	virtual int PointCount() const override;//����� ����������� �����, ������� �� ������� ��������
	virtual void IIVertex(Standard_Integer n, Standard_Real t, gp_Pnt &P, gp_Vec &V, gp_Ax3 &Ax3) const override;//���������� �����, ������� �� ������� ��������, n- ����� ����� � ������� ������� ����� node_p
	void IIAx(Standard_Integer n, Standard_Real t, gp_Ax3 &Ax3) const;
	void swap(CIndexableInsert&);
	
	friend class CIndInsTooth;
};