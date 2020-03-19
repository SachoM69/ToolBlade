#pragma once

//#include "ToolBlade.h"
#include "IndexableIns.h"
#include "math.h"

// класс с вершиной
class CCutTip //вершина инструмента
{
public:
	CCutTip(gp_Ax3 Ax3);
	~CCutTip();
protected:
	gp_Ax3 CutTip; //вершина зуба

};

// класс, производящий расчеты
// считаем в конструкторе.
class CCuttingTooth abstract :public CCutTip //Зуб режущего инструмента
{
public:
	//углы зуба

	~CCuttingTooth(void);
	CCuttingTooth(double vgamma, double vphi, double vlambda, double diameter, ToolType TT , DirToolType DTT);
	void SetTipParameters(int PointIndex, double EdgePosition);


	void SetAngles(double vgamma, double valpha, double vphi, double vphi1, double vlambda);
	void SetEdgeDir(int vEdgeDir);
	EdgeDir SetMainEdgeDir(ToolType TT , DirToolType DTT);
	void SetA_gamma(double vgamma,double vphi,double vlambda);
	gp_Dir GetA_Gamma();
	//Орты координатного базиса, связанного с выбранной точкой на главной режущей кромке
	gp_Ax3 GetIIMainEdgeAx0();
	void SetIIMainEdgeAx0();


	// рассчитать углы положения пластины в инструменте и запомнить их.
	// main_plane - основная плоскость, на которую проецируется пластина
	// n - номер кромки
	// t - координата на кромке, 0..1
	void CalcCutterAngles();
	virtual TopoDS_Shape RotatedIntoPlace() = 0;

	// Расчет положения вершины пластины в инструменте
	void CalculateTipCoordinates();
	void CalculateMatrixOfPrincipalPlane();
	void PlaneFromMatrixAndCoordinates();
	virtual void ProjectContourToPlane() = 0;

	// вспомогательные функции из маткада
	virtual gp_Vec ft(int PointIndex, double ti) = 0;
	virtual gp_Pnt fK(int PointIndex, double ti) = 0;
	virtual double ftY(int iY0, int i) = 0;
	virtual void ContourProminentPoints(int& iK0maxX, int& iK0maxY, int index) = 0;

protected:
	// исходные данные
	double gamma, alpha, phi, phi1, lambda;
	Standard_Integer MainEdgeN;//номер режущей кромки, прининятой за главную
	Standard_Real MainEdgeT;//Параметр, определяющий расчетную точку главной режущей кромки
	ToolType Type;
	DirToolType Dir;
	double tool_diam;

	// рассчитаные вещи
	EdgeDir MainEdgeDir;//направление резания режущей кромки
	gp_Ax3 IIMainEdgeAx0;//Орты координатного базиса, связанного с выбранной точкой на главной режущей кромке
	gp_Dir A_gamma;//нормаль к передней поверхности зуба
	Standard_Real plx, ply; // координаты вершины
	gp_Mat Fpl0; // матрица поворота в положение в инструменте
	gp_Trsf Fpl;
	gp_Pln PrincipalPlane;
	std::vector<gp_Pnt> ProjectedPts;
};

//
class CIndInsTooth :public CCuttingTooth
{
public:
	CIndInsTooth(double vgamma, double vphi, double vlambda, double diameter, ToolType TT, DirToolType DTT, CIndexableIns* vIndIns);
	~CIndInsTooth();
	gp_Trsf GetFI_Edge0();
	void SetFI_ii0(Standard_Integer n, Standard_Real t, gp_Ax3& Ax3);

	// вспомогательные функции из маткада
	virtual gp_Vec ft(int PointIndex, double ti) override;
	virtual gp_Pnt fK(int PointIndex, double ti) override;
					 // Унаследовано через CCuttingTooth
	virtual double ftY(int iY0, int i) override;
	virtual void ContourProminentPoints(int& iK0maxX, int& iK0maxY, int index) override;
	virtual void ProjectContourToPlane() override;
	gp_Pnt K0(int index);
	gp_Pnt K0r(int index);
	virtual TopoDS_Shape RotatedIntoPlace() override;

protected:
	CIndexableIns* IndIns;

	Standard_Integer II_n;//номер главной режущей кромки пластины
	Standard_Real II_t;//параметр точки режущей кромки для которой задаются параметры режущей кромки
	gp_Ax3 Ax_Edge0;//расположение СК, связанной с началом СК, оси которой параллельны
	gp_Ax3 IIAx;//расположение СК, связанной с выбранной точкой на режущей кромки
	gp_Trsf FI_Edge0;//Матрица, задающая преобразование координат, переводящее пластину в положение, в котором ее оси параллельна осям ССК зуба инструмета

};