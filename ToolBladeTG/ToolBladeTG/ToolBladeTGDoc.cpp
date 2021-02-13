// Этот исходный код MFC Samples демонстрирует функционирование пользовательского интерфейса Fluent на основе MFC в Microsoft Office
// ("Fluent UI") и предоставляется исключительно как справочный материал в качестве дополнения к
// справочнику по пакету Microsoft Foundation Classes и связанной электронной документации,
// включенной в программное обеспечение библиотеки MFC C++. 
// Условия лицензионного соглашения на копирование, использование или распространение Fluent UI доступны отдельно. 
// Для получения дополнительных сведений о нашей лицензионной программе Fluent UI посетите веб-узел
// http://msdn.microsoft.com/officeui.
//
// (C) Корпорация Майкрософт (Microsoft Corp.)
// Все права защищены.

// ToolBladeTGDoc.cpp : реализация класса CToolBladeTGDoc
//

#include "stdafx.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "ToolBladeTG.h"
#endif

#include "ToolBladeTGDoc.h"
//#include "d:\SB\ToolBlade\ToolBlade\ToolBlade.h"

#include <propkey.h>
#include "MainFrm.h"

#include "TB_DesDlg.h"
#include "CTB_OriDlg.h"
#include "ToolTypeDlg.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

// CToolBladeTGDoc

IMPLEMENT_DYNCREATE(CToolBladeTGDoc, CDocument)

BEGIN_MESSAGE_MAP(CToolBladeTGDoc, CDocument)
	ON_COMMAND(ID_EDPARAMS, &CToolBladeTGDoc::OnEdparams)
	ON_COMMAND(ID_TOOLTYPE, &CToolBladeTGDoc::OnTooltype)
	ON_COMMAND(ID_SHOWTOOL, &CToolBladeTGDoc::OnShowtool)
END_MESSAGE_MAP()


// создание/уничтожение CToolBladeTGDoc

CToolBladeTGDoc::CToolBladeTGDoc():VisPnt(0)
{
	for(int i=0; i<3; i++) MyCoolArrow[i]=0;
	IndInsParameters IndIns = GetDefaultInsert();
	IndInsOrientation IndOri = GetDefaultOrientation();
	IndInsData idt;
	idt.diagdata.IsDisabled=false;
	idt.libcpptr=nullptr;
	idt.liboriptr = nullptr;
	Instrument.ActualToothCount=Instrument.UsedToothCount=1;
	Instrument.CutDirection=false;
	Instrument.ToolType=0;
	IndOri.Dir = DirToolType(Instrument.CutDirection);
	IndOri.Type = ToolType(Instrument.ToolType);
	idt.libdata=IndIns;
	idt.liboridata = IndOri;
	CutterParams.push_back(idt);
}

CToolBladeTGDoc::~CToolBladeTGDoc()
{
}

IndInsParameters CToolBladeTGDoc::GetDefaultInsert()
{
	IndInsParameters IndIns;
	IndIns.IGroup=0;//номер группы пластины
	IndIns.IIForm=2;//номер формы пластины
	IndIns.FormChar='P';
	IndIns.n=3;
	IndIns.ReliefAng=0;//валичина заднего угла
	IndIns.HT=CylHole;//Наличие и форма отверстия
	IndIns.Dim=6.35;//Размер
	IndIns.Thick=3.5;//Толщина
	IndIns.Dir=Dir_Right;//направление резания
	IndIns.DHole=2.5;
	IndIns.VertForm=0;
	IndIns.B=IndIns.L=0;
	IndIns.eps=0;
	IndIns.r=0.1;
	IndIns.RackAng = 0;
	//IndIns.ActEdge=0;
	return IndIns;
}

IndInsOrientation CToolBladeTGDoc::GetDefaultOrientation()
{
	IndInsOrientation IndOri;
	IndOri.Diameter = 100;
	IndOri.Dir = DirTool_Right;
	IndOri.Type = Turning_Cutter;
	IndOri.PointIndex = 0;
	IndOri.EdgePosition = 0;
	IndOri.Gamma = 0;
	IndOri.Lambda = 0;
	IndOri.Phi = 0;
	IndOri.AxisRotation = 0;
	IndOri.Zoffset = 0;
	return IndOri;
}

#include <AIS_Trihedron.hxx>

BOOL CToolBladeTGDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	//for each (auto & mca in MyCoolArrow) if (!mca.IsNull()) myAISContext->Remove(mca, Standard_False);
	/*MyCoolArrow[0] = new ISession_Direction(gp_Pnt(0, 0, 0), gp_Vec(10, 0, 0), 1);
	MyCoolArrow[1] = new ISession_Direction(gp_Pnt(0, 0, 0), gp_Vec(0, 10, 0), 1);
	MyCoolArrow[2] = new ISession_Direction(gp_Pnt(0, 0, 0), gp_Vec(0, 0, 10), 1);
	for each (auto & mca in MyCoolArrow)
		myAISContext->Display(mca, true);*/

	gp_Ax2 ax = gp_Ax2(gp_Pnt(0, 0, 0), gp_Vec(10, 0, 0));

	Handle(Geom_Axis2Placement) myTrihedronAxis = new Geom_Axis2Placement(gp_Pnt(0, 0, 0), gp_Vec(0, 0, 10), gp_Vec(10, 0, 0));
	AIS_Trihedron* tri = new AIS_Trihedron(myTrihedronAxis);
	tri->SetSize(10);
	myAISContext->Display(tri, Standard_True);

	return TRUE;
}




// сериализация CToolBladeTGDoc

void CToolBladeTGDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: добавьте код сохранения
	}
	else
	{
		// TODO: добавьте код загрузки
	}
}

#ifdef SHARED_HANDLERS

// Поддержка для эскизов
void CToolBladeTGDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Измените этот код для отображения данных документа
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Поддержка обработчиков поиска
void CToolBladeTGDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Задайте содержимое поиска из данных документа. 
	// Части содержимого должны разделяться точкой с запятой ";"

	// Например:  strSearchContent = _T("точка;прямоугольник;круг;объект ole;");
	SetSearchContent(strSearchContent);
}

void CToolBladeTGDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// диагностика CToolBladeTGDoc

#ifdef _DEBUG
void CToolBladeTGDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CToolBladeTGDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// команды CToolBladeTGDoc

HRESULT CToolBladeTGDoc::RefreshCutter(int index, const IndInsParameters *a)
{
	try
	{
		size_t indop(index);
		if (index < 0 || indop >= CutterParams.size()) return E_INVALIDARG;
		if(CutterParams[index].libcpptr) DestroyInsert(CutterParams[index].libcpptr);
		CutterParams[index].libcpptr = CreateInsertAndPreview(myAISContext, a);

	}
	catch(Standard_OutOfRange& e)
	{
		MessageBoxA(NULL, e.GetMessageString(), "Exception in drawing routine", MB_OK);
	}
	catch(Standard_NullObject& e)
	{
		MessageBoxA(NULL, e.GetMessageString(), "Exception in drawing routine", MB_OK);
	}
	catch(Standard_NoSuchObject& e)
	{
		MessageBoxA(NULL, e.GetMessageString(), "Exception in drawing routine", MB_OK);
	}
	catch(Standard_Failure &e)
	{
		MessageBoxA(NULL, e.GetMessageString(), "Exception in drawing routine", MB_OK);
	}
	return S_OK;
}

HRESULT CToolBladeTGDoc::RefreshCutter(int index, const IndInsOrientation* a)
{
	try
	{
		size_t indop(index);
		if (index < 0 || indop >= CutterParams.size()) return E_INVALIDARG;
		if (!CutterParams[index].libcpptr)
			CutterParams[index].libcpptr = CreateInsert(&CutterParams[index].libdata);
		CutterParams[index].liboriptr = OrientInsertAndPreview(myAISContext, CutterParams[index].libcpptr, a);
	}
	catch (Standard_OutOfRange& e)
	{
		MessageBoxA(NULL, e.GetMessageString(), "Exception in drawing routine", MB_OK);
	}
	catch (Standard_NullObject& e)
	{
		MessageBoxA(NULL, e.GetMessageString(), "Exception in drawing routine", MB_OK);
	}
	catch (Standard_NoSuchObject& e)
	{
		MessageBoxA(NULL, e.GetMessageString(), "Exception in drawing routine", MB_OK);
	}
	catch (Standard_Failure& e)
	{
		MessageBoxA(NULL, e.GetMessageString(), "Exception in drawing routine", MB_OK);
	}
	return S_OK;
}


void CToolBladeTGDoc::OnEdparams()
{
	CTB_DesDlg* NPGetter = new CTB_DesDlg(this);
	NPGetter->DoModal();
	delete NPGetter;
}

HRESULT CToolBladeTGDoc::QueryIndInsInformation(int index, IndInsParameters* res)
{
	size_t indop(index);
	if(index<0 || indop>=CutterParams.size()) return E_INVALIDARG;
	*res = CutterParams[index].libdata;
	return S_OK;
}
HRESULT CToolBladeTGDoc::UpdateIndInsInformation(int index, const IndInsParameters* res)
{
	size_t indop(index);
	if(index<0 || indop>=CutterParams.size()) return E_INVALIDARG;
	CutterParams[index].libdata = *res;
	if(CutterParams[index].libcpptr) DestroyInsert(CutterParams[index].libcpptr);
	CutterParams[index].libcpptr = CreateInsert(res);
	return S_OK;
}


HRESULT CToolBladeTGDoc::QueryIndInsAttributes(int index, IndInsAttributes* res)
{
	size_t indop(index);
	if(index<0 || indop>=CutterParams.size()) return E_INVALIDARG;
	*res = CutterParams[index].diagdata;
	return S_OK;
}

HRESULT CToolBladeTGDoc::UpdateIndInsAttributes(int index, const IndInsAttributes* res)
{
	size_t indop(index);
	if(index<0 || indop>=CutterParams.size()) return E_INVALIDARG;
	CutterParams[index].diagdata = *res;
	return S_OK;
}

HRESULT CToolBladeTGDoc::QueryIndInsOrientation(int index, IndInsOrientation* res)
{
	size_t indop(index);
	if (index < 0 || indop >= CutterParams.size()) return E_INVALIDARG;
	*res = CutterParams[index].liboridata;
	return S_OK;
}

HRESULT CToolBladeTGDoc::UpdateIndInsOrientation(int index, const IndInsOrientation* res)
{
	size_t indop(index);
	if (index < 0 || indop >= CutterParams.size()) return E_INVALIDARG;
	CutterParams[index].liboridata = *res;
	return S_OK;
}

HRESULT CToolBladeTGDoc::QueryToolData(ToolData* tda)
{
	*tda = Instrument;
	return S_OK;
}

HRESULT CToolBladeTGDoc::UpdateToolType(int type)
{
	Instrument.ToolType = type;
	for (int i = 0; i < CutterParams.size(); i++)
	{
		CutterParams[i].liboridata.Type = ToolType(Instrument.ToolType);
	}
	return S_OK;
}

HRESULT CToolBladeTGDoc::UpdateToolDirection(bool dir)
{
	Instrument.CutDirection = dir;
	for (int i = 0; i < CutterParams.size(); i++)
	{
		CutterParams[i].liboridata.Dir = DirToolType(Instrument.CutDirection);
	}
	return S_OK;
}

HRESULT CToolBladeTGDoc::QueryIndInsObject(int index, const IIndexableInsert** optr)
{
	size_t indop(index);
	if(index<0 || indop>=CutterParams.size()) return E_INVALIDARG;
	if(CutterParams[index].libcpptr) DestroyInsert(CutterParams[index].libcpptr);
	*optr = CutterParams[index].libcpptr = CreateInsert(&CutterParams[index].libdata);
	return S_OK;
}

HRESULT CToolBladeTGDoc::QueryIndInsObjectSeated(int index, const IIndexableInsertSeated** optr)
{
	size_t indop(index);
	if (index < 0 || indop >= CutterParams.size()) return E_INVALIDARG;
	if(!CutterParams[index].libcpptr) CutterParams[index].libcpptr = CreateInsert(&CutterParams[index].libdata);
	if(CutterParams[index].liboriptr) DestroyInsert(CutterParams[index].liboriptr);
	CutterParams[index].liboridata.Dir = DirToolType(Instrument.CutDirection);
	CutterParams[index].liboridata.Type = ToolType(Instrument.ToolType);
	*optr = CutterParams[index].liboriptr = OrientInsert(CutterParams[index].libcpptr , &CutterParams[index].liboridata);
	return S_OK;
}

void CToolBladeTGDoc::OnTooltype()
{
	ToolTypeDlg* TTD = new ToolTypeDlg;

	TTD->DoModal();

	delete TTD;
}

HRESULT CToolBladeTGDoc::RequestNewInsert(int* index)
{
	size_t indop(*index);
	IndInsData idt;
	
	if (*index < 0 || indop >= CutterParams.size())
	{
		idt.libdata = GetDefaultInsert();
		idt.liboridata = GetDefaultOrientation();
		idt.liboridata.Dir = DirToolType(Instrument.CutDirection);
		idt.liboridata.Type = ToolType(Instrument.ToolType);
	}
	else
	{
		idt.libdata = CutterParams[*index].libdata;
		idt.liboridata = CutterParams[*index].liboridata;

	}
	idt.diagdata.IsDisabled=false;
	idt.libcpptr=nullptr;
	idt.liboriptr = nullptr;
	CutterParams.push_back(idt);
	Instrument.ActualToothCount++; Instrument.UsedToothCount++;
	*index = int(CutterParams.size()-1);
	return S_OK;
}

HRESULT CToolBladeTGDoc::RequestRemoveInsert(int index)
{
	size_t indop(index);
	if(index<0 || indop>=CutterParams.size()) return E_INVALIDARG;
	if(CutterParams[index].libcpptr) DestroyInsert(CutterParams[index].libcpptr);
	Instrument.ActualToothCount--;
	if(!CutterParams[index].diagdata.IsDisabled) Instrument.UsedToothCount--;
	CutterParams.erase(index+CutterParams.begin());
	return S_OK;
}

HRESULT CToolBladeTGDoc::ShowPoint(gp_Pnt a, int index, bool show)
{
	static std::vector<Handle_AIS_Point> ppp;
	if (ppp.size() <= index) ppp.push_back(0);
	myAISContext->Erase(ppp[index], true);
	if (show)
	{
		Handle_Geom_CartesianPoint c = new Geom_CartesianPoint(a);
		ppp[index] = new AIS_Point(c);
		myAISContext->Display(ppp[index], true);
	}
	/*myAISContext->Erase(VisPnt, true);
	if(show)
	{
		Handle_Geom_CartesianPoint c = new Geom_CartesianPoint(a);
		VisPnt = new AIS_Point(c);
		myAISContext->Display(VisPnt, true);
	}*/
	return S_OK;
}

void CToolBladeTGDoc::OnShowtool()
{
	try
	{
		/*if(!CutterParams)
			InsertShape(myAISContext);
		else */
		/*
		IndInsOrientation a; a.PointIndex = 0; a.Diameter = 0.02; a.Dir = DirTool_Right; a.EdgePosition = 0; a.Phi = 0;
		a.Gamma = 0; a.Lambda = 0; a.Type = Turning_Cutter;
		CutterParams[0].libcpptr = OrientInsertAndPreview(myAISContext, CutterParams.begin()->libcpptr, &a); // не самый красивый способ
		for each (auto & mca in MyCoolArrow) if (!mca.IsNull()) myAISContext->Remove(mca, Standard_False);
		MyCoolArrow[0] = new ISession_Direction(gp_Pnt(0, 0, 0), gp_Vec(10, 0, 0), 1);
		MyCoolArrow[1] = new ISession_Direction(gp_Pnt(0, 0, 0), gp_Vec(0, 10, 0), 1);
		MyCoolArrow[2] = new ISession_Direction(gp_Pnt(0, 0, 0), gp_Vec(0, 0, 10), 1);
		//for (int i=0; i<3; i++) 
		for each (auto & mca in MyCoolArrow)
			myAISContext->Display(mca, true);*/

		CTB_OriDlg* NPGetter = new CTB_OriDlg(this);
		NPGetter->DoModal();
		delete NPGetter;

	}
	catch (Standard_OutOfRange & e)
	{
		MessageBoxA(NULL, e.GetMessageString(), "Exception in drawing routine", MB_OK);
	}
	catch (Standard_NullObject & e)
	{
		MessageBoxA(NULL, e.GetMessageString(), "Exception in drawing routine", MB_OK);
	}
	catch (Standard_NoSuchObject & e)
	{
		MessageBoxA(NULL, e.GetMessageString(), "Exception in drawing routine", MB_OK);
	}
	catch (Standard_Failure & e)
	{
		MessageBoxA(NULL, e.GetMessageString(), "Exception in drawing routine", MB_OK);
	}
}

HRESULT CToolBladeTGDoc::GraphReliefAngle(int index, const IIndexableInsertSeated* iis, double scale)
{
	size_t indop(index);
	if (index < 0 || indop >= CutterParams.size()) return E_INVALIDARG;
	Handle_AIS_Shape oldgraph = CutterParams[index].graphs[GTRelief];
	if (!oldgraph.IsNull()) myAISContext->Erase(oldgraph, Standard_False);
	auto graph = GraphFunction(iis, [=](Standard_Integer n, Standard_Real f)->double {return iis->EffectiveReliefAngle(n, f); }, Quantity_NOC_BLUE1, scale);
	CutterParams[index].graphs[GTRelief] = graph;
	myAISContext->Display(graph, Standard_True);
	return S_OK;
}

HRESULT CToolBladeTGDoc::HideReliefAngle(int index)
{
	size_t indop(index);
	if (index < 0 || indop >= CutterParams.size()) return E_INVALIDARG;
	Handle_AIS_Shape oldgraph = CutterParams[index].graphs[GTRelief];
	if (!oldgraph.IsNull()) myAISContext->Erase(oldgraph, Standard_False);
	return S_OK;
}

HRESULT CToolBladeTGDoc::GraphKinematicReliefAngle(int index, const IIndexableInsertSeated* iis, gp_Vec velocity, double scale)
{
	size_t indop(index);
	if (index < 0 || indop >= CutterParams.size()) return E_INVALIDARG;
	Handle_AIS_Shape oldgraph = CutterParams[index].graphs[GTKinematicRelief];
	if (!oldgraph.IsNull()) myAISContext->Erase(oldgraph, Standard_False);
	auto graph = GraphFunction(iis, [=](Standard_Integer n, Standard_Real f)->double {return iis->EffectiveKinematicReliefAngle(n, f, velocity); }, Quantity_NOC_HONEYDEW, scale);
	CutterParams[index].graphs[GTKinematicRelief] = graph;
	myAISContext->Display(graph, Standard_True);
	return S_OK;
}

HRESULT CToolBladeTGDoc::HideKinematicReliefAngle(int index)
{
	size_t indop(index);
	if (index < 0 || indop >= CutterParams.size()) return E_INVALIDARG;
	Handle_AIS_Shape oldgraph = CutterParams[index].graphs[GTKinematicRelief];
	if (!oldgraph.IsNull()) myAISContext->Erase(oldgraph, Standard_False);
	return S_OK;
}

Handle_AIS_Shape CToolBladeTGDoc::GraphFunction(const IIndexableInsertSeated* iis, std::function<double(Standard_Integer n, Standard_Real f)> function, const Quantity_Color& clr, double scale)
{
	int pt_cnt = iis->NumPoint();
	gp_XYZ norm = iis->NormalToReferencePlane().XYZ();

	TopoDS_Shape ToolBlade;
	TopoDS_Wire aW;

	for (Standard_Integer i = 0; i < pt_cnt; i++)
	{
		//массив контрольных точек кривой
		TColgp_Array1OfPnt CPs(1, 12);
		//массив весов точек
		TColStd_Array1OfReal wi(1, 12);
		for (Standard_Integer j = 2; j <= 11; j++)
		{
			Standard_Real f = Standard_Real(j-2) / 9.;
			gp_Pnt point;
			gp_Vec tangent;
			gp_Ax3 u;
			iis->IIVertex(i, f, point, tangent, u);
			double magn = function(i, f) * scale;
			gp_XYZ nrm = tangent.XYZ(); nrm ^= norm;
			gp_Pnt waypoint = point.Translated(nrm.Normalized() * magn);
			//waypoint.SetZ(waypoint.Z() - 1);

			//создание массивов точек и весов для сегментов кривых
			CPs(j) = waypoint;
			wi(j) = 1;
		}
		gp_Vec tangent;
		gp_Ax3 u;
		iis->IIVertex(i, 0., CPs(1), tangent, u);
		iis->IIVertex(i, 1., CPs(12), tangent, u);
		//CPs(1).SetZ(CPs(1).Z() - 1);
		//CPs(12).SetZ(CPs(12).Z() - 1);
		wi(1) = 1;
		wi(12) = 1;
		for (Standard_Integer j = 1; j <= 11; j++)
		{
			TColgp_Array1OfPnt CPLine(1, 2);
			TColStd_Array1OfReal wiLine(1, 2);
			CPLine[1] = CPs[j];
			CPLine[2] = CPs[j+1];
			wiLine[1] = wi[j];
			wiLine[2] = wi[j+1];
			//создание кривых
			Handle(Geom_BezierCurve) curve = new Geom_BezierCurve(CPLine, wiLine);
			TopoDS_Edge aEdge = BRepBuilderAPI_MakeEdge(curve);
			aW = BRepBuilderAPI_MakeWire(aW, aEdge);
		}
		for (Standard_Integer j = 2; j <= 11; j++)
		{
			TColgp_Array1OfPnt CPLine(1, 2);
			TColStd_Array1OfReal wiLine(1, 2);
			CPLine[1] = CPs[j];
			Standard_Real f = Standard_Real(j - 2) / 9.;
			gp_Pnt point;
			gp_Vec tangent;
			gp_Ax3 u;
			iis->IIVertex(i, f, point, tangent, u);
			CPLine[2] = point;
			wiLine[1] = wi[j];
			wiLine[2] = 1;
			//создание кривых
			Handle(Geom_BezierCurve) curve = new Geom_BezierCurve(CPLine, wiLine);
			TopoDS_Edge aEdge = BRepBuilderAPI_MakeEdge(curve);
			aW = BRepBuilderAPI_MakeWire(aW, aEdge);
		}
	}
	TopoDS_Face aF = BRepBuilderAPI_MakeFace(aW);
	StaticReliefAngles = new AIS_Shape(aW);
	StaticReliefAngles->SetColor(clr);
	return StaticReliefAngles;
}