#pragma once

//#include "ToolBlade.h"
#include "IndexableIns.h"
#include "math.h"

// ����� � ��������
class CCutTip //������� �����������
{
public:
	CCutTip(gp_Ax3 Ax3);
	~CCutTip();
protected:
	gp_Ax3 CutTip; //������� ����

};

// �����, ������������ �������
// ������� � ������������.
class CCuttingTooth abstract :public CCutTip //��� �������� �����������
{
public:
	//���� ����

	~CCuttingTooth(void);
	CCuttingTooth(double vgamma, double vphi, double vlambda, double diameter, ToolType TT , DirToolType DTT);
	void SetTipParameters(int PointIndex, double EdgePosition);


	void SetAngles(double vgamma, double valpha, double vphi, double vphi1, double vlambda);
	void SetEdgeDir(int vEdgeDir);
	EdgeDir SetMainEdgeDir(ToolType TT , DirToolType DTT);
	void SetA_gamma(double vgamma,double vphi,double vlambda);
	gp_Dir GetA_Gamma();
	//���� ������������� ������, ���������� � ��������� ������ �� ������� ������� ������
	gp_Ax3 GetIIMainEdgeAx0();
	void SetIIMainEdgeAx0();


	// ���������� ���� ��������� �������� � ����������� � ��������� ��.
	// main_plane - �������� ���������, �� ������� ������������ ��������
	// n - ����� ������
	// t - ���������� �� ������, 0..1
	void CalcCutterAngles();
	virtual TopoDS_Shape RotatedIntoPlace() = 0;

	// ������ ��������� ������� �������� � �����������
	void CalculateTipCoordinates();
	void CalculateMatrixOfPrincipalPlane();
	void PlaneFromMatrixAndCoordinates();
	virtual void ProjectContourToPlane() = 0;

	// ��������������� ������� �� �������
	virtual gp_Vec ft(int PointIndex, double ti) = 0;
	virtual gp_Pnt fK(int PointIndex, double ti) = 0;
	virtual double ftY(int iY0, int i) = 0;
	virtual void ContourProminentPoints(int& iK0maxX, int& iK0maxY, int index) = 0;

protected:
	// �������� ������
	double gamma, alpha, phi, phi1, lambda;
	Standard_Integer MainEdgeN;//����� ������� ������, ���������� �� �������
	Standard_Real MainEdgeT;//��������, ������������ ��������� ����� ������� ������� ������
	ToolType Type;
	DirToolType Dir;
	double tool_diam;

	// ����������� ����
	EdgeDir MainEdgeDir;//����������� ������� ������� ������
	gp_Ax3 IIMainEdgeAx0;//���� ������������� ������, ���������� � ��������� ������ �� ������� ������� ������
	gp_Dir A_gamma;//������� � �������� ����������� ����
	Standard_Real plx, ply; // ���������� �������
	gp_Mat Fpl0; // ������� �������� � ��������� � �����������
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

	// ��������������� ������� �� �������
	virtual gp_Vec ft(int PointIndex, double ti) override;
	virtual gp_Pnt fK(int PointIndex, double ti) override;
					 // ������������ ����� CCuttingTooth
	virtual double ftY(int iY0, int i) override;
	virtual void ContourProminentPoints(int& iK0maxX, int& iK0maxY, int index) override;
	virtual void ProjectContourToPlane() override;
	gp_Pnt K0(int index);
	gp_Pnt K0r(int index);
	virtual TopoDS_Shape RotatedIntoPlace() override;

protected:
	CIndexableIns* IndIns;

	Standard_Integer II_n;//����� ������� ������� ������ ��������
	Standard_Real II_t;//�������� ����� ������� ������ ��� ������� �������� ��������� ������� ������
	gp_Ax3 Ax_Edge0;//������������ ��, ��������� � ������� ��, ��� ������� �����������
	gp_Ax3 IIAx;//������������ ��, ��������� � ��������� ������ �� ������� ������
	gp_Trsf FI_Edge0;//�������, �������� �������������� ���������, ����������� �������� � ���������, � ������� �� ��� ����������� ���� ��� ���� ����������

};