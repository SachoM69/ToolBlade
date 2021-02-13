struct IndInsParameters;
class IIndexableInsert;
struct IndInsOrientation;
class IIndexableInsertSeated;

__declspec(dllexport) IIndexableInsert* CreateInsert(const IndInsParameters*);
__declspec(dllexport) IIndexableInsert* CreateInsertAndPreview(Handle_AIS_InteractiveContext, const IndInsParameters*);
__declspec(dllexport) IIndexableInsertSeated* OrientInsert(IIndexableInsert*, const IndInsOrientation* IIt);
__declspec(dllexport) IIndexableInsertSeated* OrientInsertAndPreview(Handle_AIS_InteractiveContext AISC, IIndexableInsert* ,const IndInsOrientation* IIt);
__declspec(dllexport) void DestroyInsert(const IIndexableInsert*);
__declspec(dllexport) void DestroyInsert(const IIndexableInsertSeated*);

enum VertForm { VF_SHARP=0, VF_FILLET=1, VF_CHAMFER=2};
//тип отверстия в пластине
/*без отверстия
с цилиндрическим отверстием
с односторонним тороидальным отверстием для винта с потайной головкой с углом от 40 до 60 градусов
с двусторонним тороидальным отверстием для винта с потайной головкой с углом от 40 до 60 градусов
с односторонним тороидальным отверстием для винта с потайной головкой с углом от 70 до 90 градусов
с двусторонним тороидальным отверстием для винта с потайной головкой с углом от 70 до 90 градусов
Другое
*/
enum HoleType { NoHole=0, CylHole=1, TorHole14060=2, TorHole24060=3, TorHole17090=4, TorHole27090=5};

//Направление резания инструмента
enum DirToolType {DirTool_Right=0, DirTool_Left=1};

//Направление резания пластины СМП
enum CutDirection {Dir_Right=0, Dir_Left=1};

enum EdgeDir {EdgeDir_Left=-1, EdgeDir_Right=1};

//Тип инструмента
/*Резец, наружное точение;
Резец, растачивание;
Осевой инструмент(сверло, зенкер, развертка)
Фреза;
*/
enum ToolType {Turning_Cutter=0, Boring_Cutter=1, Drilling_Tool=2, Milling_Tool=3};


struct IndInsParameters
{
	int IGroup;//номер группы пластины
	int IIForm;//номер формы пластины
	wchar_t FormChar;
	int VertexCount;//число вершин
	double eps;//угол при вершине
	double ReliefAng;//величина заднего угла
	double RackAng; //величина переднего угла
	int HT;//Наличие и форма отверстия
	VertForm VertForm;//форма вершины
	double r;//радиус округления вершины
	double Dim;//Размер
	double Thick;//Толщина
	CutDirection Dir;//направление резания
	double DHole;//диаметр отверстия
	double B, L; //размеры пластин 3й и 4й групп 
	wchar_t TolClass; //обозначение класса допуска
};

struct IndInsOrientation
{
	double Gamma;
	double Phi;
	double Lambda;
	double Diameter;

	double AxisRotation;
	double Zoffset;

	ToolType Type;
	DirToolType Dir;

	int EdgeIndex;
	double EdgePosition;
};

class IIndexableInsert
{
public:
	virtual int NumPoint() const = 0;//число контрольных точек, лежащих на контуре пластины
	//Координаты точки, лежащей на контуре пластины, 
	// n- номер точки в массиве узловых точек node_p
	// t - координата точки на кромке, 0..1
	// P - получившаяся точка
	// V - вектор касательной
	// Ax3 - система координат в точке
	virtual void IIVertex(Standard_Integer n, Standard_Real t, gp_Pnt &P, gp_Vec &V, gp_Ax3 &Ax3) const = 0;
	virtual TopoDS_Shape ConstructToolBlade() = 0;
};

class IIndexableInsertSeated
{
public:
	virtual int NumPoint() const = 0;//число контрольных точек, лежащих на контуре пластины
	virtual void IIVertex(Standard_Integer n, Standard_Real t, gp_Pnt& P, gp_Vec& V, gp_Ax3& Ax3) const = 0;
	virtual gp_Dir NormalToReferencePlane() const = 0;
	virtual double EffectiveReliefAngle(Standard_Integer n, Standard_Real t) const = 0;
	virtual double EffectiveKinematicReliefAngle(Standard_Integer n, Standard_Real t, gp_Vec velocity) const = 0;
	virtual gp_Pnt XExtremityPoint() const = 0;
	virtual gp_Pnt YExtremityPoint() const = 0;
	virtual gp_Vec ToolAxis() const = 0;
};