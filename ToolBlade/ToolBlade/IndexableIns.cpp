#include "StdAfx.h"
#include "IndexableIns.h"
#include <math.h>
#include "Degrees.hxx"

void CIndexableInsert::rrm() //TColgp_Array1OfPnt** ptarr), void** w, void** darr)
//ÏÐÎÔÈËÜ ÐÅÆÓÙÅÉ ÊÐÎÌÊÈ ÐÀÂÍÎÑÒÎÐÎÍÍÈÕ È ÐÀÂÍÎÓÃÎËÜÍÛÕ ÏËÀÑÒÈÍ ÌÍÎÃÎÃÐÀÍÍÎÉ ÔÎÐÌÛ
{
	double d=IInst.Dim, r=IInst.r;
	UINT len=(r<=0?1:3)*IInst.VertexCount+1;
	//î÷èñòêà è ïîäãîòîâêà ìàññèâîâ
	//ìàññèâ êîíòðîëüíûõ òî÷åê êðèâîé
	node_p.clear();
	node_p.reserve(len);
	//ìàññèâû âåñîâ è ïîðÿäêîâ
	weight.clear();
	weight.reserve(len);
	d_order.clear();
	d_order.resize((r<=0?1:2)*IInst.VertexCount);

	curves.clear();
	
	//TColgp_Array1OfPnt& p1=**ptarr;
	double eps2, psi, b, l, R, m;
	eps2=pi*(IInst.VertexCount-2)/(IInst.VertexCount*2);
	psi=2*pi/IInst.VertexCount;
	b=r/sin(eps2);
	l=d/tan(eps2);weight.push_back(1.);
	R=l/(2*cos(eps2));
	double wv=sin(eps2);

	m=R-b+r+(IInst.VertexCount/2-floor(double(IInst.VertexCount/2))>0)?(d/2):-(d/2);
	
	if (r<=0) //ïëàñòèíà ñ îñòðûìè âåðøèíàìè
	{
		node_p.push_back(gp_Pnt(0,R,0));
		weight.push_back(1);
		d_order[0]=1;
	}
	else //ïëàñòèíà ñ îêðóãëåííûìè âåðøèíàìè
	{
		double y=R-b+r*sin(eps2);
		node_p.push_back(gp_Pnt(r*cos(eps2),y,0));
		node_p.push_back(gp_Pnt(0,R,0));
		node_p.push_back(gp_Pnt(-r*cos(eps2),y,0));
		weight.push_back(1);
		weight.push_back(wv);
		weight.push_back(1);
		d_order[0]=2;
		d_order[1]=1;
	}
	for (int i=1; i<IInst.VertexCount; i++)
	{
		double psi_i=psi*i;
	if (r<=0) //ïëàñòèíà ñ îñòðûìè âåðøèíàìè
			{
				node_p.push_back(node_p[0].Rotated(gp::OZ(), psi_i));
				weight.push_back(1.);
				d_order[i]=1;
			}
		else //ïëàñòèíà ñ îêðóãëåííûìè âåðøèíàìè
			{
				node_p.push_back(node_p[0].Rotated(gp::OZ(), psi_i));
				node_p.push_back(node_p[1].Rotated(gp::OZ(), psi_i));
				node_p.push_back(node_p[2].Rotated(gp::OZ(), psi_i));
				weight.push_back(1);
				weight.push_back(wv);
				weight.push_back(1);
				d_order[i*2]=2;
				d_order[i*2+1]=1;
			}
	}
	node_p.push_back(node_p[0]);
	weight.push_back(1);
}
void CIndexableInsert::rrrm()
{
	double d=IInst.Dim;
	UINT len=8;
	//î÷èñòêà è ïîäãîòîâêà ìàññèâîâ
	//ìàññèâ êîíòðîëüíûõ òî÷åê êðèâîé
	node_p.clear();
	node_p.reserve(len);
	//ìàññèâû âåñîâ è ïîðÿäêîâ
	weight.clear();
	weight.reserve(len);

	curves.clear();

	d_order.clear();
	d_order.resize(4);
	double d24=d*sqrt(2.)/4;
	node_p.push_back(gp_Pnt(d24,d24,0.));
	weight.push_back(1.);
	node_p.push_back(gp_Pnt(0.,d/sqrt(2.),0.));
	weight.push_back(sin(pi/4));
	d_order[0]=2;
	double pi2=pi/2;
	for (int i=1; i<=3; i++)
	{
		node_p.push_back(node_p[0].Rotated(gp::OZ(), pi2*i));
		weight.push_back(1.);
		node_p.push_back(node_p[1].Rotated(gp::OZ(), pi2*i));
		weight.push_back(weight[1]);
		d_order[i]=2;
	}
	node_p.push_back(node_p[0]);
	weight.push_back(1);
}

void CIndexableInsert::rtrm()//, void** w, void** darr)
//ÏÐÎÔÈËÜ ÐÅÆÓÙÅÉ ÊÐÎÌÊÈ ÐÀÂÍÎÑÒÎÐÎÍÍÈÕ È ÍÅÐÀÂÍÎÓÃÎËÜÍÛÕ ÏËÀÑÒÈÍ ÌÍÎÃÎÃÐÀÍÍÎÉ ÔÎÐÌÛ ñ îêðóãëåííûìè âåðøèíàìè
{
	double d=IInst.Dim, r=IInst.r;
	double r1=r;
	int n=IInst.VertexCount;
	UINT len=7*n;
	//î÷èñòêà è ïîäãîòîâêà ìàññèâîâ
	//ìàññèâ êîíòðîëüíûõ òî÷åê êðèâîé
	node_p.clear();
	node_p.reserve(len);
	node_p.resize(6);
	//ìàññèâû âåñîâ è ïîðÿäêîâ
	weight.clear();
	weight.reserve(len);

	curves.clear();

	d_order.clear();
	d_order.reserve(n*4);
	double eps2, m, ksi, eps1, /*ksi1,*/ seps2, ceps2;
	eps2=IInst.eps/2;
	seps2=sin(eps2);
	ceps2=cos(eps2);
	m=(d-2*r)*0.5/sin(eps2)+r-0.5*d;
	ksi=2*pi/n-pi+IInst.eps;
	eps1=pi-ksi;

	node_p[0]=gp_Pnt(r*sin(pi/2-eps2),(d*0.5-r)/seps2+r*cos(0.5*pi-eps2),0.);
	node_p[1]=gp_Pnt(0.,0.5*d/seps2,0);

	node_p[2]=gp_Pnt(-r*sin(pi/2-eps2),m-r+0.5*d+r*cos(pi/2-eps2),0);
	node_p[4]=gp_Pnt(-0.5*d*sin(pi/n)/cos(0.5*ksi),0.5*d*cos(pi/n)/cos(0.5*ksi),0);
	gp_Pnt p3=gp_Pnt(-0.5*d*sin(0.5*pi-eps2),0.5*d*cos(0.5*pi-eps2),0);
	Standard_Real dist=p3.Distance(node_p[4]);
	gp_Pnt p5=gp_Pnt(-0.5*d*sin(pi/n+0.5*ksi),0.5*d*cos(pi/n+0.5*ksi),0);
	Standard_Real t=r1/tan(eps1*0.5)/dist;
	gp_Vec v34=gp_Vec(node_p[4],p3);
	v34.Scale(t);
	node_p[3]=node_p[4].Translated(v34);
	gp_Vec v54=gp_Vec(node_p[4],p5);
	v54.Scale(t);
	node_p[5]=node_p[4].Translated(v54);
		
	weight.push_back(1.);
	weight.push_back(seps2);
	weight.push_back(1.);
	weight.push_back(1.);
	weight.push_back(sin(0.5*eps1));
	weight.push_back(1.);

	d_order.push_back(2);
	d_order.push_back(1);
	d_order.push_back(2);
	d_order.push_back(1);

	Standard_Real psi=2*pi/n;

	for (int i=1; i<=n-1; i++)
	{
		double psii=psi*i;
		for (int j=0; j<=5; j++)
		{
			node_p.push_back(node_p[j].Rotated(gp::OZ(), psii));
			weight.push_back(weight[j]);
		}
		d_order.push_back(2);
		d_order.push_back(1);
		d_order.push_back(2);
		d_order.push_back(1);
	}
	node_p.push_back(node_p[0]);
	weight.push_back(1.);
}

void CIndexableInsert::rtrm_sh()//, void** w, void** darr)
//ÏÐÎÔÈËÜ ÐÅÆÓÙÅÉ ÊÐÎÌÊÈ ÐÀÂÍÎÑÒÎÐÎÍÍÈÕ È ÍÅÐÀÂÍÎÓÃÎËÜÍÛÕ ÏËÀÑÒÈÍ ÌÍÎÃÎÃÐÀÍÍÎÉ ÔÎÐÌÛ ñ îñòðûìè âåðøèíàìè
{
	double d=IInst.Dim, r=IInst.r;
	double r1=r;
	int n=IInst.VertexCount;
	UINT len=2*n+1;
	//î÷èñòêà è ïîäãîòîâêà ìàññèâîâ
	//ìàññèâ êîíòðîëüíûõ òî÷åê êðèâîé
	node_p.clear();
	node_p.reserve(len);
	node_p.resize(2);
	//ìàññèâû âåñîâ è ïîðÿäêîâ
	weight.clear();
	weight.reserve(len);
	d_order.clear();
	d_order.reserve(n*2);

	curves.clear();

	double eps2, ksi, eps1, /*ksi1,*/ seps2, ceps2;
	eps2=IInst.eps/2;
	seps2=sin(eps2);
	ceps2=cos(eps2);
	//m=(d-2*r)*0.5/sin(eps2)+r-0.5*d;
	ksi=2*pi/n-pi+IInst.eps;
	eps1=pi-ksi;

	node_p[0]=gp_Pnt(0.,0.5*d/seps2,0);
	node_p[1]=gp_Pnt(-0.5*d*sin(pi/n)/cos(0.5*ksi),0.5*d*cos(pi/n)/cos(0.5*ksi),0);

		
	weight.push_back(1.);
	weight.push_back(1.);

	d_order.push_back(1);

	Standard_Real psi=2*pi/n;

	for (int i=1; i<=n-1; i++)
	{
		double psii=psi*i;
		for (int j=0; j<=1; j++)
		{
			node_p.push_back(node_p[j].Rotated(gp::OZ(), psii));
			weight.push_back(weight[j]);
		}
		d_order.push_back(1);
		d_order.push_back(1);
	}
	node_p.push_back(node_p[0]);
	weight.push_back(1.);
		d_order.push_back(1);
}

void CIndexableInsert::rtrm_chamfer()
//ÏÐÎÔÈËÜ ÐÅÆÓÙÅÉ ÊÐÎÌÊÈ ÐÀÂÍÎÑÒÎÐÎÍÍÈÕ È ÍÅÐÀÂÍÎÓÃÎËÜÍÛÕ ÏËÀÑÒÈÍ ÌÍÎÃÎÃÐÀÍÍÎÉ ÔÎÐÌÛ ñ ôàñêîé
{
	double d = IInst.Dim, r = IInst.r;
	double r1 = r;
	int n = IInst.VertexCount;
	int pts_per_edge = 2;
	UINT len = pts_per_edge * n + 1;
	//î÷èñòêà è ïîäãîòîâêà ìàññèâîâ
	//ìàññèâ êîíòðîëüíûõ òî÷åê êðèâîé
	std::vector<gp_Pnt> control_pts;
	control_pts.reserve(len);
	control_pts.resize(pts_per_edge);
	//ìàññèâû âåñîâ è ïîðÿäêîâ
	weight.clear();
	weight.reserve(len);
	d_order.clear();
	d_order.reserve(n * pts_per_edge);

	curves.clear();

	double eps2, ksi, eps1, /*ksi1,*/ seps2, ceps2;
	eps2 = IInst.eps / 2;
	seps2 = sin(eps2);
	ceps2 = cos(eps2);
	//m=(d-2*r)*0.5/sin(eps2)+r-0.5*d;
	ksi = 2 * pi / n - pi + IInst.eps;
	eps1 = pi - ksi;

	control_pts[1] = gp_Pnt(0., 0.5 * d / seps2, 0);
	control_pts[0] = gp_Pnt(-0.5 * d * sin(pi / n) / cos(0.5 * ksi), 0.5 * d * cos(pi / n) / cos(0.5 * ksi), 0);


	Standard_Real psi = 2 * pi / n;

	for (int i = 1; i <= n - 1; i++)
	{
		double psii = psi * i;
		for (int j = 0; j < pts_per_edge; j++)
		{
			control_pts.push_back(control_pts[j].Rotated(gp::OZ(), psii));
		}
	}
	control_pts.push_back(control_pts[0]);

	for (int i = 1; i < control_pts.size(); i++)
	{
		gp_Vec lin_dir = gp_Vec(control_pts[i - 1], control_pts[i]);
		lin_dir.Normalize();
		lin_dir *= r;
		node_p.push_back(gp_Pnt((lin_dir + control_pts[i].XYZ()).XYZ()));
		node_p.push_back(gp_Pnt((lin_dir - control_pts[i - 1].XYZ()).XYZ()));
		d_order.push_back(1);
		d_order.push_back(1);
		weight.push_back(1.);
		weight.push_back(1.);
	}

		weight.push_back(1.);
	node_p.push_back(node_p[0]);
}

void CIndexableInsert::nn()//, void** w, void** darr)
//ÏÐÎÔÈËÜ ÐÅÆÓÙÅÉ ÊÐÎÌÊÈ ÍÅÐÀÂÍÎÑÒÎÐÎÍÍÈÕ È ÍÅÐÀÂÍÎÓÃÎËÜÍÛÕ ÏËÀÑÒÈÍ ÌÍÎÃÎÃÐÀÍÍÎÉ ÔÎÐÌÛ
{
	double seps=sin(IInst.eps), ceps=cos(IInst.eps);
	double seps2=sin(IInst.eps/2);
	double l=IInst.L/seps, b=IInst.B/seps;
	double D=sqrt(l*l+b*b+2*l*b*ceps);
	double d=sqrt(l*l+b*b-2*l*b*ceps);
	double psi=acos((l*l-b*b)/(D*d));
	double eps2=0.5*IInst.eps;
	double y0=0.5*D-IInst.r*0.5*(D-d*sin(0.5*pi-psi))/(b*tan(eps2));
	//î÷èñòêà è ïîäãîòîâêà ìàññèâîâ
	//ìàññèâ êîíòðîëüíûõ òî÷åê êðèâîé
	int len=9;
	node_p.clear();
	//node_p.reserve(len);
	node_p.resize(len);
	//ìàññèâû âåñîâ è ïîðÿäêîâ
	weight.clear();
	weight.resize(len);
	d_order.clear();
	d_order.reserve(6);

	curves.clear();

	node_p[0]=gp_Pnt(0.5*IInst.r*d*cos(0.5*pi-psi)/(b*tan(eps2)), y0,0);
	node_p[1]=gp_Pnt(0, 0.5*D,0);
	double y2=0.5*D-IInst.r*0.5*(D+d*sin(0.5*pi-psi))/(l*tan(eps2));
	node_p[2]=gp_Pnt(-0.5*IInst.r*d*cos(0.5*pi-psi)/(l*tan(eps2)), y2, 0);
	node_p[3]=gp_Pnt( -0.5*d*cos(0.5*pi-psi), -0.5*d*sin(0.5*pi-psi), 0);
	node_p[4]=node_p[0].Mirrored(gp_Pnt( 0, 0, 0));
	node_p[5]=node_p[1].Mirrored(gp_Pnt( 0, 0, 0));
	node_p[6]=node_p[2].Mirrored(gp_Pnt( 0, 0, 0));
	node_p[7]=node_p[3].Mirrored(gp_Pnt( 0, 0, 0));
	node_p[8]=node_p[0];
		
	weight[0]=(1.);
	weight[1]=(seps2);
	weight[2]=(1.);
	weight[3]=(1.);
	weight[4]=(1.);
	weight[5]=(seps2);
	weight[6]=(1.);
	weight[7]=(1.);
	weight[8]=(1.);

	d_order.push_back(2);
	d_order.push_back(1);
	d_order.push_back(1);
	d_order.push_back(2);
	d_order.push_back(1);
	d_order.push_back(1);
}


/*CIndexableIns::CIndexableIns(Standard_CString rep)
{
	filled=true;
	decided=false;
	n=5;
	if (rep!="")	
	{
		filled = Standard_True;
		int i=0;
		Group=rep[i++];
		Form=rep[i++];
		RackAng=rep[i++];
		Toler=rep[i++];
		Constr=rep[i++];
		Dimensions=&rep[i++]; Dimensions=Dimensions+rep[i++];
		Thickness=&rep[i++]; Thickness=Thickness+rep[i++];
		VertForm=&rep[i++]; VertForm=VertForm+rep[i++];
		Spec=rep+i;
	} else {
		throw Standard_NullObject("Õðåíîâûé àðãóìåíò ïîñòàâëåí â êà÷åñòâå ñòðîêè ïðåäñòàâëåíèÿ âñåõ ïàðàìåòðîâ");
	}
}*/

CIndexableInsert::CIndexableInsert(const IndInsParameters* IIt)
//:CPart(gp_Ax3( gp_Pnt(0.,0.,0),gp_Dir(0.,0.,1.),gp_Dir(1.,0.,0.)))
{
	if(IIt) // êîíñòðóêòîð CIndInsCuttingTooth
		IInst=*IIt;
	else memset(&IInst, 0, sizeof(IInst)); // TODO óñòàíàâëèâàòü êàæäîå ïîëå
	ConstructToolBlade();
}

CIndexableInsert::~CIndexableInsert(void)
{
}

CIndexableInsert::CIndexableInsert(const CIndexableInsert& src)
//:CPart(gp_Ax3( gp_Pnt(0.,0.,0),gp_Dir(0.,0.,1.),gp_Dir(1.,0.,0.)))
{
	IInst=src.IInst;
	for each(auto& p in src.node_p) node_p.push_back(p);
	for each(auto& p in src.d_order) d_order.push_back(p);
	for each(auto& p in src.weight) weight.push_back(p);
	for each(auto& p in src.npmain) npmain.push_back(p);
	for each(auto& p in src.curves)
	{
		auto b = p->Copy();
		curves.push_back(*(Handle_Geom_BezierCurve*)&b); // Åñëè óïàäåò, òî íàäî ÷óòü ïåðåïðàâèòü
	}
}

CIndexableInsert::CIndexableInsert(CIndexableInsert&& src)
//:CPart(gp_Ax3( gp_Pnt(0.,0.,0),gp_Dir(0.,0.,1.),gp_Dir(1.,0.,0.)))
{
	swap(src);
}

void CIndexableInsert::swap(CIndexableInsert& src)
{
	auto b=IInst;
	IInst=src.IInst;
	src.IInst=b;
	node_p.swap(src.node_p);
	d_order.swap(src.d_order);
	weight.swap(src.weight);
	npmain.swap(src.npmain);
	curves.swap(src.curves);
}
/*void CIndexableIns::SetData(Standard_CString rep)
{
	if (rep!="")	
	{
		filled = Standard_True;
		int i=0;
		Group=rep[i++];
		Form=rep[i++];
		RackAng=rep[i++];
		Toler=rep[i++];
		Constr=rep[i++];
		Dimensions=&rep[i++]; Dimensions=Dimensions+rep[i++];
		Thickness=&rep[i++]; Thickness=Thickness+rep[i++];
		VertForm=&rep[i++]; VertForm=VertForm+rep[i++];
		Spec=rep+i;
	} else {
		throw Standard_NullObject("Õðåíîâûé àðãóìåíò ïîñòàâëåí â êà÷åñòâå ñòðîêè ïðåäñòàâëåíèÿ âñåõ ïàðàìåòðîâ");
	}
}*/

//Ñîçäàåò ôîðìó ëåçâèÿ èíñòðóìåíòà
/*TopoDS_Shape CIndexableIns::GetResultingShape()//÷èñëî âåðøèí
{
throw Standard_NoSuchObject("Âûçâàí àáñòðàêòíûé ìåòîä ðèñîâàíèÿ ïëàñòèíû");
	
}*/

//ìàññèâ ñòàíäàðòíûõ ðàçìåðîâ ïëàñòèí
/*const double darr[] = {3.97,4.76,5.56,6.35,7.94,9.525,12.7,15.875,19.05,25.4,31.75};
void CIndexableIns::ReadRealVals(void)
{
	if(!filled)  throw Standard_NoSuchObject("Îòñóòñòâóþò ñóðñû äëÿ îáíàðóæåíèÿ ðåàëüíîãî çíà÷åíèÿ");
	
	int degrees_eps;
	switch(Group)
	{
	case GCP_ESEA:
		switch(Form)
		{
		case 'H':
			n=6;
			break;
		case 'O':
			n=8;
			break;
		case 'P':
			n=5;
			break;
		case 'S':
			n=4;
			break;
		case 'T':
			n=3;
			break;
		}
		break;
	case GCP_ESNA:
		n=2;
		switch(Form)
		{
		case 'W':
			n=3;
		case 'C':
			degrees_eps=80;
			break;
		case 'D':
			degrees_eps=55;
			break;
		case 'E':
			degrees_eps=75;
			break;
		case 'M':
			degrees_eps=86;
			break;
		case 'V':
			degrees_eps=35;
			break;
		}
		eps = DEG(degrees_eps);
		break;
	}
	decided=true;
}*/


/*CIndexableInsESEA::CIndexableInsESEA(void)
{
CIndexableIns::CIndexableIns();
};

CIndexableInsESEA::CIndexableInsESEA(Standard_CString rep)
{
	CIndexableIns::CIndexableIns(rep);
}

CIndexableInsESEA::~CIndexableInsESEA(void)
{
}*/

void CIndexableInsert::ConstructToolBlade()
{
	npmain.clear();
	switch(IInst.IGroup)
	{
	case 0:
		if (IInst.VertexCount==0) rrrm();
		else rrm();
		break;
	case 1:
		if (IInst.VertForm==0)
		{
			rtrm_sh();
		} else if (IInst.VertForm==1)
		{
			rtrm();
		}
		else if (IInst.VertForm == 2)
		{
			rtrm_chamfer();
		}
		break;
	case 2:
		nn();
		break;
	case 3:
		nn();
		break;
	}

	TopoDS_Shape ToolBlade;
	ToolBlade = TopoDS_Shape();
	TopoDS_Wire aW;

	int j=0;
	for (uint i=0; i<=d_order.size()-1; i++)
	{
		//ïîðÿäîê êðèâîé
		int di=d_order[i];
		//ìàññèâ êîíòðîëüíûõ òî÷åê êðèâîé
		TColgp_Array1OfPnt CPs(1, di+1);
		//ìàññèâ âåñîâ òî÷åê
		TColStd_Array1OfReal wi(1, di+1);

		//ñîçäàíèå ìàññèâîâ òî÷åê è âåñîâ äëÿ ñåãìåíòîâ êðèâûõ
		for (int ii=0; ii<=(di); ii++)
		{
			CPs(ii+1)=node_p[j+ii];
			wi(ii+1)=weight[j+ii];
		}
		//ñîçäàíèå êðèâûõ
		Handle(Geom_BezierCurve) curve = new Geom_BezierCurve(CPs,wi);
		TopoDS_Edge aEdge = BRepBuilderAPI_MakeEdge(curve);
		aW = BRepBuilderAPI_MakeWire(aW, aEdge);
		curves.push_back(curve);

		//çàïîëíåíèå ìàññèâà íîìåðîâ óçëîâûõ òî÷åê, ïðèíàäëåæàùèõ êîíòóðó
		npmain.push_back(j);

		j+=di;

	}

	TopoDS_Face aF = BRepBuilderAPI_MakeFace(aW );
	gp_Vec aV(0, 0, IInst.Thick);
	ToolBlade = BRepPrimAPI_MakePrism(aF, aV);
	if (IInst.HT!=NoHole)
	{
 		TopoDS_Shape CB = BRepPrimAPI_MakeCylinder (IInst.DHole, IInst.Thick); 
		ToolBlade = BRepAlgoAPI_Cut(ToolBlade,CB); 
	}
	//Êîíåö ñîçäàíèÿ òåëà ïëàñòèíû
	ToolBladeShape = ToolBlade;
}

TopoDS_Shape CIndexableInsert::GetResultingShape() const
{
	return ToolBladeShape;
}

int CIndexableInsert::PointCount() const
{
	return int(npmain.size());
}

void CIndexableInsert::IIVertex(Standard_Integer n, Standard_Real t, gp_Pnt &P, gp_Vec &V, gp_Ax3 &Ax3) const//Êîîðäèíàòû òî÷êè, ëåæàùåé íà êîíòóðå ïëàñòèíû, n- íîìåð òî÷êè â ìàññèâå óçëîâûõ òî÷åê node_p
{
	//Êîîðäèíàòû ðàñ÷åòíîé òî÷êè è êàñàòåëüíàÿ ê ðåæóùåé êðîìêå
	curves[n]->D1(t,P,V);
	gp_Dir N(0.,0.,1.);
	//Îðòû êîîðäèíàòíîãî áàçèñà, ñâÿçàííîãî ñ âûáðàííîé òî÷êîé íà êîíòóðå ðåæóùèõ êðîìîê ïëàñòèíû
	Ax3=gp_Ax3(P,N,V);
}

void CIndexableInsert::IIAx(Standard_Integer n, Standard_Real t, gp_Ax3 &Ax3) const//Êîîðäèíàòû òî÷êè, ëåæàùåé íà êîíòóðå ïëàñòèíû, n- íîìåð òî÷êè â ìàññèâå óçëîâûõ òî÷åê node_p
{

	gp_Pnt P; 
	gp_Vec V;
	//Êîîðäèíàòû ðàñ÷åòíîé òî÷êè è êàñàòåëüíàÿ ê ðåæóùåé êðîìêå
	curves[n]->D1(t,P,V);
	gp_Dir N(0.,0.,1.);
	//Îðòû êîîðäèíàòíîãî áàçèñà, ñâÿçàííîãî ñ âûáðàííîé òî÷êîé íà êîíòóðå ðåæóùèõ êðîìîê ïëàñòèíû
	Ax3=gp_Ax3(P,N,V);
}

