#include "StdAfx.h"
#include "CuttingTooth.h"
#include <math.h>

////////////////////// CCuttingTooth ////////////////////////

CCuttingTooth::CCuttingTooth(double vgamma, double vphi, double vlambda, double diameter,ToolType TT , DirToolType DTT)
:gamma(vgamma), 
phi(vphi), 
lambda(vlambda),
tool_diam(diameter)
{
	SetMainEdgeDir(TT,DTT);
	SetA_gamma(vgamma, vphi, vlambda);
}

void CCuttingTooth::SetTipParameters(int PointIndex, double EdgePosition, double AxisRotation, double Zoffset)
{
	MainEdgeN = PointIndex;
	MainEdgeT = EdgePosition;

	axis_rot = AxisRotation;
	plz = Zoffset;
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
	default:
		throw std::exception("Invalid ToolType set in CCuttingTooth");
	}
}

// вычисление Фпл
void CCuttingTooth::CalculateMatrixOfPrincipalPlane()
{
	gp_Ax3 pseg, wseg;
	double majorKt = MainEdgeN + MainEdgeT;
	gp_Vec v = ft(MainEdgeN, MainEdgeT) * MainEdgeDir;

	gp_XYZ ip, jp, kp;
	gp_XYZ il, jl, kl;
	ip = v.XYZ();
	kp.SetCoord(0., 0., 1.);
	jp = kp.Crossed(ip);
	jp /= jp.Modulus();
	il.SetCoord(cos(lambda) * cos(phi), MainEdgeDir * cos(lambda) * sin(phi), sin(lambda));
	kl.SetX(-cos(gamma) * cos(phi) * sin(lambda) - sin(gamma) * cos(lambda) * sin(phi));
	kl.SetY(MainEdgeDir * (sin(gamma) * cos(phi) * cos(lambda) - cos(gamma) * sin(phi) * sin(lambda)));
	kl.SetZ(cos(gamma) * cos(lambda));
	kl.Normalize();
	jl = kl.Crossed(il);

	Mp0.SetCols(ip, jp, kp);
	Ml0.SetCols(il, jl, kl);
	auto mp0I = Mp0.Inverted();
	Fpl0 = Ml0 * mp0I;

	int iK0maxX, iK0maxY;
	ContourExtremities(iK0maxX, iK0maxY, 0);
	double ty = ftY(iK0maxY, 1);
	double tx = ftY(iK0maxX, 0);
	gp_XYZ pmaxY = fK(int(floor(ty)), fmod(ty, 1.)).XYZ() * Fpl0;
	gp_XYZ pmaxX = fK(int(floor(tx)), fmod(tx, 1.)).XYZ() * Fpl0;
	m_pmaxX = pmaxX;
	m_pmaxY = pmaxY;

	gp_Vec Fpl1;
	Fpl1.SetX((floor(tx) == floor(ty) || ceil(tx) == ceil(ty) ? -pmaxX.X(): -pmaxY.X()) + plx);
	Fpl1.SetY(-pmaxY.Y() + ply);
	Fpl1.SetZ(floor(tx) == floor(ty) || ceil(tx) == ceil(ty) ? -pmaxX.Z(): -pmaxY.Z());
	Fpl.SetTransformation(gp_Quaternion(Fpl0), Fpl1);
	//Fpl.SetTranslation(gp_Vec(0, 0, 0));
}

void CCuttingTooth::PlaneFromMatrixAndCoordinates()
{
	gp_Pln res;
	res.Transform(Fpl);
	PrincipalPlane = res;
}

double CCuttingTooth::EffectiveReliefAngle(Standard_Integer n, Standard_Real t) const
{
	gp_Mat ProjectionMatrix = gp_Mat(0, 0, 0, 0, 1, 0, 0, 0, 1);
	gp_Vec ptt = ft(n, t);
	gp_XYZ tK = Ml0 * Mp0.Inverted() * ptt.XYZ();
	gp_XYZ tKxy = gp_Mat(1, 0, 0, 0, 1, 0, 0, 0, 0) *tK;
	double lambdaK = acos((tK * tKxy)/(tK.Modulus() * tKxy.Modulus())) * (tK.Z() < 0 ? -1 : 1);
	gp_XYZ icK = tKxy.Normalized();
	gp_XYZ kcK = gp_XYZ(0, 0, 1);
	gp_XYZ jcK = kcK.Crossed(icK);
	gp_Mat MAgamma;
	MAgamma.SetRows(icK, jcK, kcK);
	gp_XYZ NAgammaCK = ProjectionMatrix * MAgamma * A_gamma.XYZ();
	NAgammaCK.Normalize();
	gp_XYZ ycK = gp_XYZ(-icK.Y(), icK.X(), 0);

	gp_Mat2d signmat = gp_Mat2d(gp_XY(NAgammaCK.Y(), NAgammaCK.Z()), gp_XY(kcK.Y(), kcK.Z()));
	//if(ZeroRackAngle)
		double gammai = acos(NAgammaCK * kcK) * (signmat.Determinant() < 0 ? -1 : 1);
	//else
	{
		gp_XYZ NAgammaP = gp_XYZ(0, 0, 1) - gp_XYZ(ptt.Y(), -ptt.X(), 0) * tan(gammaP());
		NAgammaP.Normalize();
		gp_XYZ NAgammaPCK = ProjectionMatrix * MAgamma * Ml0 * Mp0.Inverted() * NAgammaP;
		gp_Mat2d signpmat = gp_Mat2d(gp_XY(NAgammaPCK.Y(), NAgammaPCK.Z()), gp_XY(kcK.Y(), kcK.Z()));
		double gammaPi = acos((NAgammaPCK*kcK)/ (kcK.Modulus()* NAgammaPCK.Modulus())) * (signpmat.Determinant() < 0 ? -1 : 1);
	}
	gp_XYZ NAalpha = gp_XYZ(ptt.Y(), -ptt.X(), 0) + gp_XYZ(0, 0, -tan(alphaP()));
	NAalpha.Normalize();
	gp_XYZ NAalphaCK = ProjectionMatrix * MAgamma * Ml0 * Mp0.Inverted() * NAalpha;

	gp_XYZ j = gp_XYZ(0, -1, 0);
	gp_Mat2d signalphamat = gp_Mat2d(gp_XY(j.Y(), j.X()), gp_XY(NAalphaCK.Y(), NAalphaCK.Z()));
	double alphac = acos((NAalphaCK * j) / (NAalphaCK.Modulus() * j.Modulus())) * (signalphamat.Determinant() < 0 ? -1 : 1);

	return alphac;
}

double CCuttingTooth::EffectiveKinematicReliefAngle(Standard_Integer n, Standard_Real t, gp_Vec velocity) const
{
	gp_Mat ProjectionMatrix = gp_Mat(0, 0, 0, 0, 1, 0, 0, 0, 1);
	gp_Vec ptt = ft(n, t);
	gp_XYZ kd = velocity.Normalized().XYZ();
	gp_XYZ tK = Ml0 * Mp0.Inverted() * ptt.XYZ();
	gp_XYZ Pnk = tK ^ kd;
	gp_XYZ ikK = kd ^ Pnk;
	ikK.Normalize();
	gp_XYZ kkK = kd;
	gp_XYZ jkK = kkK ^ ikK;
	gp_Mat MAgammak;
	MAgammak.SetRows(ikK, jkK, kkK);
	// calculate MAgamma
	gp_XYZ tKxy = gp_Mat(1, 0, 0, 0, 1, 0, 0, 0, 0) * tK;
	gp_XYZ icK = tKxy.Normalized();
	gp_XYZ kcK = gp_XYZ(0, 0, 1);
	gp_XYZ jcK = kcK.Crossed(icK);
	gp_Mat MAgamma;
	MAgamma.SetRows(icK, jcK, kcK);
	//
	//gp_Mat NAgammaKK = ProjectionMatrix * MAgamma * Agamma;
	//NAgammaKK.Normalize();
	gp_XYZ yKK = gp_XYZ(-ikK.Y(), ikK.X(), 0);

	gp_XYZ NAalpha = gp_XYZ(ptt.Y(), -ptt.X(), 0) + gp_XYZ(0, 0, -tan(alphaP()));
	NAalpha.Normalize();

	gp_XYZ NAalphaKK = ProjectionMatrix * MAgammak * Ml0 * Mp0.Inverted() * NAalpha;
	gp_XYZ j = gp_XYZ(0, -1, 0);
	gp_Mat2d signalphamat = gp_Mat2d(gp_XY(j.Y(), j.X()), gp_XY(NAalphaKK.Y(), NAalphaKK.Z()));
	double alphaK = acos( (NAalphaKK * j) / (NAalphaKK.Modulus() * j.Modulus())) * (signalphamat.Determinant() < 0 ? -1 : 1);
	return alphaK;
}

gp_Pnt CCuttingTooth::XExtremityPoint() const
{
	gp_Pnt oP;
	oP = m_pmaxX;
	oP.Translate(Fpl.TranslationPart());
	oP.Rotate(gp_Ax1(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1)), axis_rot);
	oP.Translate(gp_Pnt(0, 0, 0), gp_Pnt(0, 0, plz));
	return oP;
}

gp_Pnt CCuttingTooth::YExtremityPoint() const
{
	gp_Pnt oP;
	oP = m_pmaxY;
	oP.Translate(Fpl.TranslationPart());
	oP.Rotate(gp_Ax1(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1)), axis_rot);
	oP.Translate(gp_Pnt(0, 0, 0), gp_Pnt(0, 0, plz));
	return oP;
}

void CCuttingTooth::CalcCutterAngles()
{
	CalculateTipCoordinates();
	CalculateMatrixOfPrincipalPlane();
	PlaneFromMatrixAndCoordinates();
	ProjectContourToPlane();
}

////////////////////// CIndInsTooth ////////////////////////

CIndInsTooth::CIndInsTooth
	(double vgamma, double vphi, double vlambda, double diameter,
	ToolType TT, DirToolType DTT, CIndexableInsert* vIndIns)
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

gp_Vec CIndInsTooth::ft(int PointIndex, double ti) const
{
	gp_Pnt pt_on_cv;
	gp_Vec tan_to_cv;
	IndIns->curves[PointIndex]->D1(ti, pt_on_cv, tan_to_cv);
	tan_to_cv.Normalize();
	return tan_to_cv;
}

gp_Pnt CIndInsTooth::fK(int PointIndex, double ti) const
{
	gp_Pnt pt_on_cv;
	IndIns->curves[PointIndex]->D0(ti, pt_on_cv);
	return pt_on_cv;
}

#define dK 9

class ftrot : public math_Function
{
public:
	ftrot(const CIndInsTooth* mt, int crd, int idd) : mytooth(mt), coord(crd), id(idd)
	{
	};
	Standard_EXPORT virtual Standard_Boolean Value(const Standard_Real X, Standard_Real& F) override
	{
		gp_Vec ft = mytooth->ft(id, X);
		gp_XYZ ft_trans =  ft.XYZ() * mytooth->Fpl0;
		F = ft_trans.Coord(coord + 1);
		if (abs(F) < 1e-10)
		{
			// edge case workaround
			double oppositeval;
			Value(1 - X, oppositeval);
			F = Sign(Epsilon(oppositeval), -oppositeval);
		}
		return Standard_True;
	};
private:
	const CIndInsTooth* mytooth;
	int id;
	int coord;
};

double CIndInsTooth::ftY(int iY0, int i)
{
	double troot = 0;
	double t = double(iY0);
	int t0 = iY0;
	int t1 = t0 + 1;
	if (IndIns->d_order[t0] == 1) troot = t;
	else
	{
		ftrot ftrot(this, i, t0);
		double lower, upper;
		ftrot.Value(0, lower);
		ftrot.Value(1, upper);
		troot = math_BracketedRoot(ftrot, 0, 1, 1e-10).Root();
	}
	troot += iY0;
	if (troot < 0)  troot = IndIns->NumPoint() + troot;
	return troot;
}
void CIndInsTooth::ContourExtremities(int& iK0maxX, int& iK0maxY, int index)
{
	int count = IndIns->NumPoint();
	gp_Pnt K0max = K0(0);
	iK0maxX = 0;
	iK0maxY = 0;
	int ix = -1;
	int iy = -1;
	int sg = MainEdgeDir;
	double Kois[12][3];
	for (int i = 0; i < count; i++)
	{
		int li = i - 1; if (li < 0) li += count;
		int ni = i + 1; if (ni >= count) ni -= count;
		double KOi = K0(i).Y() * sg;
		double KOli = K0(li).Y() * sg;
		double KOni = K0(ni).Y() * sg;
		Kois[i][0] = K0(li).Y() * sg; Kois[i][1] = KOi;  Kois[i][2] = K0(ni).Y() * sg;
		if (KOli >= KOi&& KOni >= KOi)
		{
			iy++;
			if (!iy)
			{
				if (IndIns->d_order[li] > 1) iK0maxY = li;
				else if (IndIns->d_order[i] > 1) iK0maxY = i;
				else iK0maxY = i;
			}
		}
		KOi = K0(i).X();
		if (K0(li).X() <= KOi && KOi > K0(ni).X())
		{
			ix++;
			if (!ix)
			{
				if (IndIns->d_order[li] > 1) iK0maxX = li;
				else if (IndIns->d_order[i] > 1) iK0maxX = i;
				else iK0maxX = i;
			}
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
	res = Fpl0 * IndIns->node_p[IndIns->npmain[index]].XYZ();
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
	gp_Trsf inst_rot_part, z_offset_part;
	inst_rot_part.SetRotation(gp_Ax1(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1)), axis_rot);
	z_offset_part.SetTranslation(gp_Pnt(0, 0, 0), gp_Pnt(0, 0, plz));
	shp.Move(TopLoc_Location(z_offset_part * inst_rot_part * Fpl ));
	return shp;
}

void CIndInsTooth::IIVertex(Standard_Integer n, Standard_Real t, gp_Pnt& P, gp_Vec& V, gp_Ax3& Ax3) const
{
	gp_Pnt oP;
	gp_Vec oV;
	gp_Ax3 oA;
	IndIns->IIVertex(n, t, oP, oV, oA);
	oP.Transform(Fpl);
	oV.Transform(Fpl);
	oA.Transform(Fpl);
	oP.Rotate(gp_Ax1(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1)), axis_rot);
	oV.Rotate(gp_Ax1(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1)), axis_rot);
	oA.Rotate(gp_Ax1(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1)), axis_rot);
	oP.Translate(gp_Pnt(0, 0, 0), gp_Pnt(0, 0, plz));
	oA.Translate(gp_Pnt(0, 0, 0), gp_Pnt(0, 0, plz));
	P = oP;
	V = oV;
	Ax3 = oA;
}

int CIndInsTooth::NumPoint() const
{
	return IndIns->NumPoint();
}

gp_Dir CIndInsTooth::NormalToReferencePlane() const
{
	return A_gamma; // TODO к передней или к плоскости резания?
}

double CIndInsTooth::gammaP() const
{
	return IndIns->IInst.RackAng;
}

double CIndInsTooth::alphaP() const
{
	return IndIns->IInst.ReliefAng;
}

#undef ROUND_TO_5