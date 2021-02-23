// ќсновной включаемый файл библиотеки.
// ѕозвол€ет создать геометрию пластины, сориентировать еЄ и вставить в инструмент.
// ¬се углы в радианах
#pragma once
#include <memory>
#include <functional>

struct IndInsParameters;
class IIndexableInsert;
struct IndInsOrientation;
class IIndexableInsertSeated;
class IIndexableInsertTool;
class IInsertPreview;
struct IndInsToolParams;

__declspec(dllexport) std::shared_ptr<IIndexableInsert> CreateInsert(const IndInsParameters*);
__declspec(dllexport) std::shared_ptr<IIndexableInsertSeated> OrientInsert(std::shared_ptr<IIndexableInsert>, const IndInsOrientation* IIt);

__declspec(dllexport) std::shared_ptr<IIndexableInsertTool> CreateTool(const IndInsToolParams*);
// —оздать инструмент по правилу
typedef std::function<void(int index, double* rotation_angle, double* z_position)> PPatterningRule;
__declspec(dllexport) std::shared_ptr<IIndexableInsertTool> CreateToolPatterned(const IndInsToolParams*, const IndInsOrientation* IIt,
	std::shared_ptr<IIndexableInsert>, PPatterningRule, int count);

__declspec(dllexport) std::shared_ptr<IInsertPreview> CreateInsertPreview(Handle_AIS_InteractiveContext);

enum VertForm { VF_SHARP=0, VF_FILLET=1, VF_CHAMFER=2};
//тип отверсти€ в пластине
/*без отверсти€
с цилиндрическим отверстием
с односторонним тороидальным отверстием дл€ винта с потайной головкой с углом от 40 до 60 градусов
с двусторонним тороидальным отверстием дл€ винта с потайной головкой с углом от 40 до 60 градусов
с односторонним тороидальным отверстием дл€ винта с потайной головкой с углом от 70 до 90 градусов
с двусторонним тороидальным отверстием дл€ винта с потайной головкой с углом от 70 до 90 градусов
ƒругое
*/
enum HoleType { NoHole=0, CylHole=1, TorHole14060=2, TorHole24060=3, TorHole17090=4, TorHole27090=5};

//Ќаправление резани€ инструмента
enum DirToolType {DirTool_Right=0, DirTool_Left=1};

//Ќаправление резани€ пластины —ћѕ
enum CutDirection {Dir_Right=0, Dir_Left=1};

enum EdgeDir {EdgeDir_Left=-1, EdgeDir_Right=1};

//“ип инструмента
/*–езец, наружное точение;
–езец, растачивание;
ќсевой инструмент(сверло, зенкер, развертка)
‘реза;
*/
enum ToolType {Turning_Cutter=0, Boring_Cutter=1, Drilling_Tool=2, Milling_Tool=3};

struct IndInsParameters
{
	int IGroup;//номер группы пластины
	int IIForm;//номер формы пластины
	wchar_t StandardShapeCode; //костыль
	int VertexCount;//число вершин
	double eps;//угол при вершине
	double ReliefAng;//величина заднего угла
	double RackAng; //величина переднего угла
	int HT;//Ќаличие и форма отверсти€
	VertForm VertForm;//форма вершины
	double r;//радиус округлени€ вершины
	double Dim;//–азмер
	double Thick;//“олщина
	CutDirection Dir;//направление резани€
	double DHole;//диаметр отверсти€
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

struct IndInsToolParams
{
	double Diameter;
	ToolType Type;
	DirToolType Dir;
};

class IIndexableInsert
{
public:
	virtual int PointCount() const = 0;//число контрольных точек, лежащих на контуре пластины
	// оординаты точки, лежащей на контуре пластины, 
	// n- номер точки в массиве узловых точек node_p
	// t - координата точки на кромке, 0..1
	// P - получивша€с€ точка
	// V - вектор касательной
	// Ax3 - система координат в точке
	virtual void IIVertex(Standard_Integer n, Standard_Real t, gp_Pnt &P, gp_Vec &V, gp_Ax3 &Ax3) const = 0;
	virtual TopoDS_Shape GetResultingShape() const = 0;
};

class IIndexableInsertSeated
{
public:
	virtual int PointCount() const = 0;//число контрольных точек, лежащих на контуре пластины
	virtual void IIVertex(Standard_Integer n, Standard_Real t, gp_Pnt& P, gp_Vec& V, gp_Ax3& Ax3) const = 0;
	virtual gp_Dir NormalToReferencePlane() const = 0;
	virtual double EffectiveReliefAngle(Standard_Integer n, Standard_Real t) const = 0;
	virtual double EffectiveKinematicReliefAngle(Standard_Integer n, Standard_Real t, gp_Vec velocity) const = 0;
	virtual gp_Pnt XExtremityPoint() const = 0;
	virtual gp_Pnt YExtremityPoint() const = 0;
	virtual TopoDS_Shape RotatedIntoPlace() const = 0;
};

class IIndexableInsertTool
{
public:
	virtual int InsertCount() const = 0;
	virtual void AppendInsert(std::shared_ptr<IIndexableInsertSeated>) = 0;
	virtual void RemoveInsert(int index) = 0;
	virtual std::shared_ptr<IIndexableInsertSeated> GetInsert(int index) const = 0;
	virtual void SwapInsert(int index, std::shared_ptr<IIndexableInsertSeated>) = 0;
	virtual gp_Vec ToolAxis() const = 0;
	virtual ToolType Type() const = 0;
	virtual DirToolType CutDirection() const = 0;
	virtual std::shared_ptr<IIndexableInsertSeated> operator [](int index) const = 0;
	virtual IFSelect_ReturnStatus LoadShapeFromSTEP(const char* path) = 0;
	virtual Handle(TopTools_HSequenceOfShape) GetShape() const = 0;
};

class IInsertPreview
{
public:
	virtual void Preview(std::shared_ptr<IIndexableInsert>) = 0;
	virtual void Preview(std::shared_ptr<IIndexableInsertSeated>) = 0;
	virtual void Preview(std::shared_ptr<IIndexableInsertTool>) = 0;
	virtual void DestroyPreview() = 0;
};