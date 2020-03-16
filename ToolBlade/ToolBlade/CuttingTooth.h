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

// класс, производ€щий расчеты
// считаем в конструкторе.
class CCuttingTooth abstract :public CCutTip //«уб режущего инструмента
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
	//ќрты координатного базиса, св€занного с выбранной точкой на главной режущей кромке
	gp_Ax3 GetIIMainEdgeAx0();
	void SetIIMainEdgeAx0();


	// рассчитать углы положени€ пластины в инструменте и запомнить их.
	// main_plane - основна€ плоскость, на которую проецируетс€ пластина
	// n - номер кромки
	// t - координата на кромке, 0..1
	void CalcCutterAngles(gp_Pln main_plane);

	// –асчет положени€ вершины пластины в инструменте
	void CalculateTipCoordinates();
	void CalculateTipMatrix();

	// вспомогательные функции из маткада
	virtual gp_Vec ft(int PointIndex, double ti) = 0;
	virtual gp_Pnt fK(int PointIndex, double ti) = 0;
	virtual double ftY(int iY0, int i) = 0;
	virtual void ContourProminentPoints(int& iK0maxX, int& iK0maxY, int index) = 0;

protected:
	// исходные данные
	double gamma, alpha, phi, phi1, lambda;
	Standard_Integer MainEdgeN;//номер режущей кромки, принин€той за главную
	Standard_Real MainEdgeT;//ѕараметр, определ€ющий расчетную точку главной режущей кромки
	ToolType Type;
	DirToolType Dir;
	double tool_diam;

	// рассчитаные вещи
	EdgeDir MainEdgeDir;//направление резани€ режущей кромки
	gp_Ax3 IIMainEdgeAx0;//ќрты координатного базиса, св€занного с выбранной точкой на главной режущей кромке
	gp_Dir A_gamma;//нормаль к передней поверхности зуба
	Standard_Real plx, ply; // координаты вершины
};

//
class CIndInsTooth :public CCuttingTooth
{
public:
	CIndInsTooth(double vgamma, double vphi, double vlambda, double diameter, ToolType TT, DirToolType DTT, CIndexableIns* vIndIns);
	~CIndInsTooth();
	gp_Trsf GetFI_Edge0();
	void SetFI_ii0(Standard_Integer n, Standard_Real t, gp_Ax3& Ax3);

	// рассчитать углы положени€ пластины в инструменте и запомнить их.
	// main_plane - основна€ плоскость, на которую проецируетс€ пластина
	// n - номер кромки
	// t - координата на кромке, 0..1
	virtual void CalcCutterAngles(gp_Pln main_plane, ToolType tt, DirToolType dtt) = 0;

	// вспомогательные функции из маткада
	virtual gp_Vec ft(int PointIndex, double ti) override;
	virtual gp_Pnt fK(int PointIndex, double ti) override;
					 // ”наследовано через CCuttingTooth
	virtual double ftY(int iY0, int i) override;
	virtual void ContourProminentPoints(double& iK0maxX, double& iK0maxY, int index) override;

protected:
	CIndexableIns* IndIns;

	Standard_Integer II_n;//номер главной режущей кромки пластины
	Standard_Real II_t;//параметр точки режущей кромки дл€ которой задаютс€ параметры режущей кромки
	gp_Ax3 Ax_Edge0;//расположение — , св€занной с началом — , оси которой параллельны
	gp_Ax3 IIAx;//расположение — , св€занной с выбранной точкой на режущей кромки
	gp_Trsf FI_Edge0;//ћатрица, задающа€ преобразование координат, перевод€щее пластину в положение, в котором ее оси параллельна ос€м ——  зуба инструмета

};