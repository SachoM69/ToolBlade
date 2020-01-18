#pragma once

//#include "ToolBlade.h"
#include "IndexableIns.h"
#include "math.h"

class CCutTip //вершина инструмента
{
public:
	CCutTip(gp_Ax3 Ax3)
	{CutTip=Ax3;};
	~CCutTip(){};
protected:
	gp_Ax3 CutTip; //вершина зуба

};

class CCuttingTooth :public CCutTip /*CPart*/ //Зуб режущего инструмента
{
public:
	//углы зуба

	~CCuttingTooth(void);
	CCuttingTooth(double vgamma, double valpha, double vphi, double vphi1, double vlambda,ToolType TT , DirToolType DTT);
	void SetAngles(double vgamma, double valpha, double vphi, double vphi1, double vlambda);
	void SetEdgeDir(int vEdgeDir);
	EdgeDir SetMainEdgeDir(ToolType TT , DirToolType DTT);
	void SetA_gamma(double vgamma,double vphi,double vlambda);
	gp_Dir GetA_Gamma();
	gp_Ax3 GetIIMainEdgeAx0(){return IIMainEdgeAx0;}//Орты координатного базиса, связанного с выбранной точкой на главной режущей кромке};//Орты координатного базиса, связанного с выбранной точкой на главной режущей кромке
	void SetIIMainEdgeAx0(){IIMainEdgeAx0=gp_Ax3(gp_Pnt(0.,0.,0.),A_gamma,gp_Dir(cos(lambda)*cos(phi), MainEdgeDir*cos(lambda)*sin(phi), sin(lambda)));}
protected:
	double gamma, alpha, phi, phi1, lambda;
	Standard_Integer MainEdgeN;//номер режущей кромки, прининятой за главную
	Standard_Real MainEdgeT;//Параметр, определяющий расчетную точку главной режущей кромки
	EdgeDir MainEdgeDir;//направление резания режущей кромки
	gp_Ax3 IIMainEdgeAx0;//Орты координатного базиса, связанного с выбранной точкой на главной режущей кромке
	gp_Dir A_gamma;//нормаль к передней поверхности зуба
};


class CIndInsTooth :public CCuttingTooth
{
public:
	CIndexableIns IndIns;
	CIndInsTooth(double vgamma, double valpha, double vphi, double vphi1, double vlambda, 
	 ToolType TT , DirToolType DTT, CIndexableIns vIndIns)
		//const IndInsert* h)
		:CCuttingTooth(vgamma, valpha, vphi,  vphi1,  vlambda,  TT, DTT),
	IndIns(vIndIns), IIAx(), FI_Edge0(), Ax_Edge0()
	{};
	~CIndInsTooth() { };
	gp_Trsf GetFI_Edge0() {return FI_Edge0;};
	void SetFI_ii0(Standard_Integer n, Standard_Real t, gp_Ax3 &Ax3) 
	{	
		//Установка СК, связанной с режущей кромкой пласттны
		IndIns.IIAx(II_n,II_t,IIAx);
		//Установка СК, параллельной СК пластины в инструменте
		SetA_gamma(gamma,phi,lambda);
		SetIIMainEdgeAx0();
	};

protected:
	Standard_Integer II_n;//номер главной режущей кромки пластины
	Standard_Real II_t;//параметр точки режущей кромки для которой задаются параметры режущей кромки
	gp_Ax3 Ax_Edge0;//расположение СК, связанной с началом СК, оси которой параллельны
	gp_Ax3 IIAx;//расположение СК, связанной с выбранной точкой на режущей кромки
	gp_Trsf FI_Edge0;//Матрица, задающая преобразование координат, переводящее пластину в положение, в котором ее оси параллельна осям ССК зуба инструмета

};

class CRotationalTooth :public CCutTip
{
public:
 	CRotationalTooth(double R, double TAng)
		:Radius(R),ToothAng(TAng), CCutTip(gp_Ax3())
	{
		Standard_Real x=R*sin(TAng);
		Standard_Real y=cos(TAng);
	};
	CRotationalTooth(gp_Ax3 Ax3)
		:CCutTip(Ax3)
	{
	//Вычисление радиуса инструмента как расстояния от вершины инструмента до оси вращения инструмента.
	//Ось вращения инструмента совпадает с осью Z его СК.
		//Точка, принятая за вершину зуба
		gp_Pnt pt=Ax3.Location();
		Standard_Real x=pt.X();
		Standard_Real y=pt.Y();
		Radius=sqrt(x*x+y*y);
		ToothAng=atan2(y,x);
	};
	~CRotationalTooth(){};
protected:
	double Radius;//радиус расположения зуба относительно оси вращения
	double ToothAng;//угловое расположение вершины зуба
};


class CRotCuttingTooth : public CCuttingTooth, CRotationalTooth//Зуб вращающегося инструмента
{
public:
	CRotCuttingTooth(double vgamma, double valpha, double vphi, double vphi1, double vlambda, double vR, double vTAng, ToolType TT , DirToolType DTT);
	~CRotCuttingTooth(void);
};

/*class CIndInsCuttingTooth : public CCuttingTooth//Зуб режущего инструмента
{
public:
	CIndexableIns IndIns;
	~CIndInsCuttingTooth(void);
	CIndInsCuttingTooth(double vgamma, double valpha, double vphi, double vphi1, double vlambda, 
		 gp_Ax3 vIIAx,ToolType TT , DirToolType DTT, CIndexableIns vIndIns);
protected:

};
*/