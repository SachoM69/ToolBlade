#pragma once
#include "ToolBlade.h"

typedef unsigned int uint;

class CIndexableInsert : public IIndexableInsert//, CPart
{
protected:
	//int n; //число вершин
	//double diam, radius;
	//double eps, alpha, gamma;//углы
/*	char Group, Form, RackAng, Toler, Constr;
	Standard_CString Thickness, VertForm, CutEdge, Direct, Spec, Dimensions;*/
	IndInsParameters IInst;
	std::vector<gp_Pnt> node_p;//массив узловых точек
	std::vector<int>  d_order;//массив порядков сегментов кривой
	std::vector<double> weight; //массив весов узловых точек
	std::vector<Handle(Geom_BezierCurve)> curves;
	std::vector<int>  npmain;//массив номеров узловых точек, принадлежащих контуру пластины
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
	// Функции создания геометрии.
	void rrm();//Равносторонние равноугольные
	void rrrm();//круглая пластина
	void rtrm();//Равносторонние неравноугольные и ромбические с округленными вершинами
	void rtrm_sh();//Равносторонние неравноугольные и ромбические с острыми вершинами
	void rtrm_chamfer();//Равносторонние неравноугольные и ромбические с фаской
	void nn();//Неравносторонние неравноугольные

	// Внешние функции
	virtual int PointCount() const override;//число контрольных точек, лежащих на контуре пластины
	virtual void IIVertex(Standard_Integer n, Standard_Real t, gp_Pnt &P, gp_Vec &V, gp_Ax3 &Ax3) const override;//Координаты точки, лежащей на контуре пластины, n- номер точки в массиве узловых точек node_p
	void IIAx(Standard_Integer n, Standard_Real t, gp_Ax3 &Ax3) const;
	void swap(CIndexableInsert&);
	
	friend class CIndInsTooth;
};