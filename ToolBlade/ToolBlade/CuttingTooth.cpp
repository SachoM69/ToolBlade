#include "StdAfx.h"
#include "CuttingTooth.h"
#include <math.h>

CCuttingTooth::CCuttingTooth(double vgamma, double valpha, double vphi, double vphi1, double vlambda,ToolType TT , DirToolType DTT)
:gamma(vgamma), 
alpha(valpha),
phi(vphi), 
phi1(vphi1), 
lambda(vlambda),
//CPart(gp_Ax3( gp_Pnt(0.,0.,0),gp_Dir(0.,0.,1.),gp_Dir(1.,0.,0.)))
CCutTip(gp_Ax3())
{
	SetMainEdgeDir(TT,DTT);
}

CCuttingTooth::~CCuttingTooth(void)
{
}

void CCuttingTooth::SetAngles(double vgamma, double valpha, double vphi, double vphi1, double vlambda)
{
	gamma=vgamma;
	alpha=valpha;
	phi=vphi;
	phi1=vphi1;
	lambda=vlambda;

}

//Определение направления главной режущей кромки
EdgeDir CCuttingTooth::SetMainEdgeDir(ToolType TT , DirToolType DTT)
{
	MainEdgeDir=EdgeDir_Right;
	if ((TT==Turning_Cutter)&(DTT==DirTool_Right)) //правый резец
	{MainEdgeDir=EdgeDir_Right;}
	else if ((TT==Turning_Cutter)&(DTT==DirTool_Left)) //левый резец
	{MainEdgeDir=EdgeDir_Left;}
	else if ((TT==Boring_Cutter)&(DTT==DirTool_Right)) //правый резец растачивание
	{MainEdgeDir=EdgeDir_Left;}
	else if ((TT==Boring_Cutter)&(DTT==DirTool_Left))//левый резец растачивание
	{MainEdgeDir=EdgeDir_Right;}
	else if ((TT==Drilling_Tool)&(DTT==DirTool_Right))//осевой инструмент праворежущий
	{MainEdgeDir=EdgeDir_Left;}	
	else if ((TT==Drilling_Tool)&(DTT==DirTool_Left))//осевой инструмент леворежущий
	{MainEdgeDir=EdgeDir_Right;}	
	else if ((TT==Milling_Tool)&(DTT==DirTool_Right))//фреза праворежущая
	{MainEdgeDir=EdgeDir_Right;}	
	else if ((TT==Milling_Tool)&(DTT==DirTool_Left))//фреза леворежущая
	{MainEdgeDir=EdgeDir_Left;};

	return MainEdgeDir;
}

//вектор нормали к передней поверхности
void CCuttingTooth::SetA_gamma(double vgamma,double vphi,double vlambda)
{
	Standard_Real x=-cos(gamma)*cos(phi)*sin(lambda)-sin(gamma)*cos(lambda)*sin(phi);
	Standard_Real y=MainEdgeDir*sin(gamma)*cos(phi)*cos(lambda)-cos(gamma)*sin(phi)*sin(lambda);
	Standard_Real z=cos(gamma)*cos(lambda);
	A_gamma=gp_Dir(x,y,z);
}

gp_Dir CCuttingTooth::GetA_Gamma()
{
	return A_gamma;
}

CRotCuttingTooth::CRotCuttingTooth(double vgamma, double valpha, double vphi, double vphi1, double vlambda, double vR, double vpsi,ToolType TT , DirToolType DTT)
	:CCuttingTooth(vgamma, valpha, vphi, vphi1, vlambda, TT, DTT),
	CRotationalTooth(vR,vpsi)
{
}

CRotCuttingTooth::~CRotCuttingTooth(void)
{
}


/*CIndInsCuttingTooth::CIndInsCuttingTooth(double vgamma, double valpha, double vphi, double vphi1, double vlambda, 
	gp_Ax3 vIIAx, ToolType TT , DirToolType DTT, CIndexableIns vIndIns)
	:CCuttingTooth(vgamma, valpha, vphi,  vphi1,  vlambda,  TT, DTT),
	IndIns(NULL)
{
	IIAx=vIIAx;
	IndIns=vIndIns;
}

CIndInsCuttingTooth::~CIndInsCuttingTooth(void)
{
}
*/

