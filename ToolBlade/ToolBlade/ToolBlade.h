struct IndInsert;
class CIndexableInsert;
__declspec(dllexport) void SetWndHandle(HWND hWnd);
__declspec(dllexport) CIndexableInsert* InsertShape(Handle_AIS_InteractiveContext, const IndInsert*, Standard_Boolean RebuildAnyway = Standard_True);
extern Handle_AIS_Shape CuttingPlate;

//enum GroupCapt { GCP_ESEA = '0', GCP_ESNA = '1', GCP_NSEA = '2', GCP_NSNA = '3' };
//enum GroupCapt { GCP_ESEA = 0, GCP_ESNA = 1, GCP_NSEA = 2, GCP_NSNA = 3 };

/*class CPart//присоедин€ема€ деталь
{
public:
	CPart(gp_Ax3 vBase) {Base=vBase;};
	gp_Ax3 GetBase(){return Base;};
protected:
	gp_Ax3 Base;//—  посадочной поверхности в а
};*/

enum VertForm { VF_SHARP=0, VF_FILLET=1, VF_CHAMFER=2};
//тип отверсти€ в пластине
enum HoleType { NoHole=0, CylHole=1, TorHole14060=2, TorHole24060=3, TorHole17090=4, TorHole27090=5};
/*без отверсти€
с цилиндрическим отверстием
с односторонним тороидальным отверстием дл€ винта с потайной головкой с углом от 40 до 60 градусов
с двусторонним тороидальным отверстием дл€ винта с потайной головкой с углом от 40 до 60 градусов
с односторонним тороидальным отверстием дл€ винта с потайной головкой с углом от 70 до 90 градусов
с двусторонним тороидальным отверстием дл€ винта с потайной головкой с углом от 70 до 90 градусов
ƒругое
*/

//Ќаправление резани€ инструмента
enum DirToolType {DirTool_Right=0, DirTool_Left=1};

//Ќаправление резани€ пластины —ћѕ
enum CutDirection {Dir_Right=0, Dir_Left=1, Dir_Random};

enum EdgeDir {EdgeDir_Left=-1, EdgeDir_Right=1};

//“ип инструмента
enum ToolType {Turning_Cutter=0, Boring_Cutter=1, Drilling_Tool=2, Milling_Tool=3};
/*–езец, наружное точение;
–езец, растачивание;
ќсевой инструмент(сверло, зенкер, развертка)
‘реза;
*/


struct IndInsert
{
	int IGroup;//номер группы пластины
	int IIForm;//номер формы пластины
	wchar_t FormChar;
	int n;//число вершин
	double eps;//угол при вершине
	double RackAng;//величина заднего угла
	int HT;//Ќаличие и форма отверсти€
	int VertForm;//форма вершины
	double r;//радиус округлени€ вершины
	double Dim;//–азмер
	double Thick;//“олщина
	int Dir;//направление резани€
	double DHole;//диаметр отверсти€
	double B, L; //размеры пластин 3й и 4й групп 
	wchar_t TolClass; //обозначение класса допуска
	int ActEdge; //активна€ кромка
};

const double deg=M_PI/180;

class CIndexableInsert
{
public:
	virtual int NumPoint() const = 0;//число контрольных точек, лежащих на контуре пластины
	// оординаты точки, лежащей на контуре пластины, 
	// n- номер точки в массиве узловых точек node_p
	// t - координата точки на кромке, 0..1
	// P - получивша€с€ точка
	// V - вектор касательной
	// Ax3 - система координат в точке
	virtual void IIVertex(Standard_Integer n, Standard_Real t, gp_Pnt &P, gp_Vec &V, gp_Ax3 &Ax3) const = 0;
};