struct IndInsParameters;
class IIndexableInsert;
struct IndInsOrientation;

__declspec(dllexport) IIndexableInsert* CreateInsert(const IndInsParameters*);
__declspec(dllexport) IIndexableInsert* CreateInsertAndPreview(Handle_AIS_InteractiveContext, const IndInsParameters*);
__declspec(dllexport) IIndexableInsert* OrientInsertAndPreview(Handle_AIS_InteractiveContext AISC, IIndexableInsert* ,const IndInsOrientation* IIt);
__declspec(dllexport) void DestroyInsert(const IIndexableInsert*);
//extern Handle_AIS_Shape CuttingPlate;

//enum GroupCapt { GCP_ESEA = '0', GCP_ESNA = '1', GCP_NSEA = '2', GCP_NSNA = '3' };
//enum GroupCapt { GCP_ESEA = 0, GCP_ESNA = 1, GCP_NSEA = 2, GCP_NSNA = 3 };

/*class CPart//присоединяемая деталь
{
public:
	CPart(gp_Ax3 vBase) {Base=vBase;};
	gp_Ax3 GetBase(){return Base;};
protected:
	gp_Ax3 Base;//СК посадочной поверхности в а
};*/

enum VertForm { VF_SHARP=0, VF_FILLET=1, VF_CHAMFER=2};
//тип отверстия в пластине
enum HoleType { NoHole=0, CylHole=1, TorHole14060=2, TorHole24060=3, TorHole17090=4, TorHole27090=5};
/*без отверстия
с цилиндрическим отверстием
с односторонним тороидальным отверстием для винта с потайной головкой с углом от 40 до 60 градусов
с двусторонним тороидальным отверстием для винта с потайной головкой с углом от 40 до 60 градусов
с односторонним тороидальным отверстием для винта с потайной головкой с углом от 70 до 90 градусов
с двусторонним тороидальным отверстием для винта с потайной головкой с углом от 70 до 90 градусов
Другое
*/

//Направление резания инструмента
enum DirToolType {DirTool_Right=0, DirTool_Left=1};

//Направление резания пластины СМП
enum CutDirection {Dir_Right=0, Dir_Left=1, Dir_Random};

enum EdgeDir {EdgeDir_Left=-1, EdgeDir_Right=1};

//Тип инструмента
enum ToolType {Turning_Cutter=0, Boring_Cutter=1, Drilling_Tool=2, Milling_Tool=3};
/*Резец, наружное точение;
Резец, растачивание;
Осевой инструмент(сверло, зенкер, развертка)
Фреза;
*/


struct IndInsParameters
{
	int IGroup;//номер группы пластины
	int IIForm;//номер формы пластины
	wchar_t FormChar;
	int n;//число вершин
	double eps;//угол при вершине
	double RackAng;//величина заднего угла
	int HT;//Наличие и форма отверстия
	int VertForm;//форма вершины
	double r;//радиус округления вершины
	double Dim;//Размер
	double Thick;//Толщина
	int Dir;//направление резания
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
	ToolType Type;
	DirToolType Dir;

	int PointIndex;
	double EdgePosition;
};

const double deg=M_PI/180;

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