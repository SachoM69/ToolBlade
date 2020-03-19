#include "StdAfx.h"
#include "CuttingTooth.h"
#include <math.h>

////////////////////// CCutTip ////////////////////////

CCutTip::CCutTip(gp_Ax3 Ax3)
{
	CutTip = Ax3;
};

CCutTip::~CCutTip()
{
}

////////////////////// CCuttingTooth ////////////////////////

CCuttingTooth::CCuttingTooth(double vgamma, double vphi, double vlambda, double diameter,ToolType TT , DirToolType DTT)
:gamma(vgamma), 
phi(vphi), 
lambda(vlambda),
tool_diam(diameter),
CCutTip(gp_Ax3())
{
	SetMainEdgeDir(TT,DTT);
}

void CCuttingTooth::SetTipParameters(int PointIndex, double EdgePosition)
{
	MainEdgeN = PointIndex;
	MainEdgeT = EdgePosition;
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
	Type = TT;
	Dir = DTT;
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

void CCuttingTooth::SetIIMainEdgeAx0()
{
	gp_Pnt zero = gp_Pnt(0., 0., 0.);
	gp_Dir edge = gp_Dir(cos(lambda) * cos(phi), MainEdgeDir * cos(lambda) * sin(phi), sin(lambda));
	IIMainEdgeAx0 = gp_Ax3(zero, A_gamma, edge);
}

gp_Ax3 CCuttingTooth::GetIIMainEdgeAx0() { return IIMainEdgeAx0; }


void CCuttingTooth::CalculateTipCoordinates()
{
	switch (Type)
	{
	case Turning_Cutter:
	case Boring_Cutter:
		plx = ply = 0.;
		break;
	case Drilling_Tool:
		plx = 0;
		ply = -MainEdgeDir * tool_diam / 2;
		break;
	case Milling_Tool:
		plx = tool_diam / 2;
		ply = 0;
		break;
	}
}

// вычисление Фпл
void CCuttingTooth::CalculateMatrixOfPrincipalPlane()
{
	gp_Ax3 pseg, wseg;
	double majorKt = MainEdgeN + MainEdgeT;
	gp_Vec v = ft(MainEdgeN, MainEdgeT)* MainEdgeDir;

	gp_XYZ ip, jp, kp;
	gp_XYZ il, jl, kl;
	ip = v.XYZ();
	kp.SetCoord(0., 0., 1.);
	jp = kp.Multiplied(ip) / (kp.CrossMagnitude(ip));
	il.SetCoord(cos(lambda) * cos(phi), MainEdgeDir * cos(lambda) * sin(phi), sin(lambda));
	kl.SetX(-cos(gamma) * cos(phi) * sin(lambda) - sin(gamma) * cos(lambda) * sin(phi));
	kl.SetY(MainEdgeDir * (sin(gamma) * cos(phi) * cos(lambda) - cos(gamma) * sin(phi) * sin(lambda)));
	kl.SetZ(cos(gamma) * cos(lambda));
	kl.Normalize();
	jl = kl.Multiplied(il);

	gp_Mat Mp0, Ml0;
	Mp0.SetRows(ip, jp, kp);
	Ml0.SetRows(il, jl, kl);
	auto mp0I = Mp0.Inverted();
	Fpl0 = Ml0 * mp0I;

	int iK0maxX, iK0maxY;
	ContourProminentPoints(iK0maxX, iK0maxY, 0);
	double ty = ftY(iK0maxY, 1);
	double tx = ftY(iK0maxX, 0);
	gp_XYZ pmaxY = fK(int(floor(ty)), remainder(ty, 1.)).XYZ() * Fpl0;
	gp_XYZ pmaxX = fK(int(floor(tx)), remainder(tx, 1.)).XYZ() * Fpl0;

	gp_Vec Fpl1;
	Fpl1.SetX((floor(tx) == floor(ty) || ceil(tx) == ceil(ty) ? -pmaxX.X(): -pmaxY.X()) + plx);
	Fpl1.SetY(-pmaxY.Y() + ply);
	Fpl1.SetZ(floor(tx) == floor(ty) || ceil(tx) == ceil(ty) ? -pmaxX.Z(): -pmaxY.Z());
	Fpl.SetTransformation(gp_Quaternion(Fpl0), Fpl1);
}

void CCuttingTooth::PlaneFromMatrixAndCoordinates()
{
	gp_Pln res;
	res.Transform(Fpl);
	PrincipalPlane = res;
}

void CCuttingTooth::CalcCutterAngles()
{
	CalculateMatrixOfPrincipalPlane();
	CalculateTipCoordinates();
	PlaneFromMatrixAndCoordinates();
	ProjectContourToPlane();
}

////////////////////// CIndInsTooth ////////////////////////

CIndInsTooth::CIndInsTooth
	(double vgamma, double vphi, double vlambda, double diameter,
	ToolType TT, DirToolType DTT, CIndexableIns* vIndIns)
	:CCuttingTooth(vgamma, vphi, vlambda, diameter, TT, DTT),
	IndIns(vIndIns), IIAx(), FI_Edge0(), Ax_Edge0()
{
}

CIndInsTooth::~CIndInsTooth()
{
}

gp_Trsf CIndInsTooth::GetFI_Edge0()
{
	return FI_Edge0;
};

void CIndInsTooth::SetFI_ii0(Standard_Integer n, Standard_Real t, gp_Ax3& Ax3)
{
	//Установка СК, связанной с режущей кромкой пластины
	IndIns->IIAx(II_n, II_t, IIAx);
	//Установка СК, параллельной СК пластины в инструменте
	SetA_gamma(gamma, phi, lambda);
	SetIIMainEdgeAx0();
}

gp_Vec CIndInsTooth::ft(int PointIndex, double ti)
{
	gp_Pnt pt_on_cv;
	gp_Vec tan_to_cv;
	IndIns->curves[PointIndex]->D1(ti, pt_on_cv, tan_to_cv);
	return tan_to_cv;
}

gp_Pnt CIndInsTooth::fK(int PointIndex, double ti)
{
	gp_Pnt pt_on_cv;
	IndIns->curves[PointIndex]->D0(ti, pt_on_cv);
	return pt_on_cv;
}

#define dK 9

double CIndInsTooth::ftY(int iY0, int i)
{
	double troot = 0;
	int t0 = iY0/dK;
	int t1 = t0 + 1;
	if (IndIns->d_order[t0] == 2) troot = double(iY0) / dK;
	else troot = sqrt(0);
	return troot;
}
void CIndInsTooth::ContourProminentPoints(int& iK0maxX, int& iK0maxY, int index)
{
	int count = IndIns->NumPoint();
	gp_Pnt K0max = K0(0);
	iK0maxX = 0;
	iK0maxY = 0;
	int ix = -1;
	int iy = -1;
	int sg = MainEdgeDir;
	for (int i = 0; i < count; i++)
	{
		int li = i - 1; if (li < 0) li += count;
		int ni = i + 1; if (ni >= count) ni -= count;
		double KOi = K0(i).Y() * sg;
		if (iy < index && K0(li).Y() * sg > KOi && KOi < K0(ni).Y() * sg)
		{
			iy++;
			iK0maxY = i;
		}
		KOi = K0r(i).X();
		if (K0r(li).X() < KOi && KOi >= K0r(ni).X())
		{
			ix++;
			iK0maxX = i;
		}
	}
}
void CIndInsTooth::ProjectContourToPlane()
{
	Handle_Geom_Surface srf = new Geom_Plane(PrincipalPlane);
	gp_Vec tangent;
	gp_Ax3 path_params;
	for each (const gp_Pnt & vert_pt in IndIns->node_p)
	{
		gp_Pnt project = GeomAPI_ProjectPointOnSurf(vert_pt, srf);
		ProjectedPts.push_back(project);
	}
}
gp_Pnt CIndInsTooth::K0(int index)
{
	gp_Pnt res;
	res = Fpl0 * IndIns->node_p[index].XYZ();
	return res;
}

#define ROUND_TO_5(x) (round((x)*100000)/100000)

gp_Pnt CIndInsTooth::K0r(int index)
{
	gp_Pnt res = K0(index);
	gp_XYZ& pc = res.ChangeCoord();
	pc.SetCoord(ROUND_TO_5(pc.X()), ROUND_TO_5(pc.Y()), ROUND_TO_5(pc.Z()));
	return res;
}

TopoDS_Shape CIndInsTooth::RotatedIntoPlace()
{
	TopoDS_Shape shp = IndIns->ConstructToolBlade();
	shp.Move(TopLoc_Location(Fpl));
	return shp;
}

#undef ROUND_TO_5