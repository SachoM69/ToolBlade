#pragma once
#include <vector>


typedef unsigned int uint;

class CIndexableIns : public CIndexableInsert//, CPart
{
protected:
	//int n; //число вершин
	//double diam, radius;
	//double eps, alpha, gamma;//углы
/*	char Group, Form, RackAng, Toler, Constr;
	Standard_CString Thickness, VertForm, CutEdge, Direct, Spec, Dimensions;*/
	IndInsert IInst;
	std::vector<gp_Pnt> node_p;//массив узловых точек
	std::vector<int>  d_order;//массив пор€дков сегментов кривой
	std::vector<double> weight; //массив весов узловых точек
	std::vector<Handle(Geom_BezierCurve)> curves;
	std::vector<int>  npmain;//массив номеров узловых точек, принадлежащих контуру пластины
public:
//	CIndexableIns(Standard_CString);
	CIndexableIns(const IndInsert*);
	CIndexableIns(const CIndexableIns&);
	CIndexableIns(CIndexableIns&&);
	~CIndexableIns(void);
//	void SetData(Standard_CString);
	TopoDS_Shape ConstructToolBlade();
//	void ReadRealVals(void);
	void rrm();//–авносторонние равноугольные
	void rrrm();//кругла€ пластина
	void rtrm();//–авносторонние неравноугольные и ромбические с округленными вершинами
	void rtrm_sh();//–авносторонние неравноугольные и ромбические с острыми вершинами
	void nn();//Ќеравносторонние неравноугольные
	virtual int NumPoint() const override;//число контрольных точек, лежащих на контуре пластины
	virtual void IIVertex(Standard_Integer n, Standard_Real t, gp_Pnt &P, gp_Vec &V, gp_Ax3 &Ax3) const override;// оординаты точки, лежащей на контуре пластины, n- номер точки в массиве узловых точек node_p
	void IIAx(Standard_Integer n, Standard_Real t, gp_Ax3 &Ax3);
	void swap(CIndexableIns&);
	
};

/*class CIndexableInsESEA : public CIndexableIns //равносторонние и равноугольные
{

public:
	CIndexableInsESEA(void);
	CIndexableInsESEA(Standard_CString);
	~CIndexableInsESEA(void);
	virtual TopoDS_Shape ConstructToolBlade() override;
};*/