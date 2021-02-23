// �������� ���������� ���� ����������.
// ��������� ������� ��������� ��������, �������������� � � �������� � ����������.
// ��� ���� � ��������
#include <memory>
#include <functional>

struct IndInsParameters;
class IIndexableInsert;
struct IndInsOrientation;
class IIndexableInsertSeated;
class IIndexableInsertTool;
class IInsertPreview;

__declspec(dllexport) std::shared_ptr<IIndexableInsert> CreateInsert(const IndInsParameters*);
__declspec(dllexport) std::shared_ptr<IIndexableInsertSeated> OrientInsert(std::shared_ptr<IIndexableInsert>, const IndInsOrientation* IIt);

__declspec(dllexport) std::shared_ptr<IIndexableInsertTool> CreateTool();
// ������� ���������� �� �������
typedef std::function<void(int index, double* rotation_angle, double* z_position)> PPatterningRule;
__declspec(dllexport) std::shared_ptr<IIndexableInsertTool> CreateToolPatterned(std::shared_ptr<IIndexableInsert>, PPatterningRule);

__declspec(dllexport) std::shared_ptr<IInsertPreview> CreateInsertPreview(Handle_AIS_InteractiveContext);

enum VertForm { VF_SHARP=0, VF_FILLET=1, VF_CHAMFER=2};
//��� ��������� � ��������
/*��� ���������
� �������������� ����������
� ������������� ������������ ���������� ��� ����� � �������� �������� � ����� �� 40 �� 60 ��������
� ������������ ������������ ���������� ��� ����� � �������� �������� � ����� �� 40 �� 60 ��������
� ������������� ������������ ���������� ��� ����� � �������� �������� � ����� �� 70 �� 90 ��������
� ������������ ������������ ���������� ��� ����� � �������� �������� � ����� �� 70 �� 90 ��������
������
*/
enum HoleType { NoHole=0, CylHole=1, TorHole14060=2, TorHole24060=3, TorHole17090=4, TorHole27090=5};

//����������� ������� �����������
enum DirToolType {DirTool_Right=0, DirTool_Left=1};

//����������� ������� �������� ���
enum CutDirection {Dir_Right=0, Dir_Left=1};

enum EdgeDir {EdgeDir_Left=-1, EdgeDir_Right=1};

//��� �����������
/*�����, �������� �������;
�����, ������������;
������ ����������(������, ������, ���������)
�����;
*/
enum ToolType {Turning_Cutter=0, Boring_Cutter=1, Drilling_Tool=2, Milling_Tool=3};

struct IndInsParameters
{
	int IGroup;//����� ������ ��������
	int IIForm;//����� ����� ��������
	wchar_t StandardShapeCode; //�������
	int VertexCount;//����� ������
	double eps;//���� ��� �������
	double ReliefAng;//�������� ������� ����
	double RackAng; //�������� ��������� ����
	int HT;//������� � ����� ���������
	VertForm VertForm;//����� �������
	double r;//������ ���������� �������
	double Dim;//������
	double Thick;//�������
	CutDirection Dir;//����������� �������
	double DHole;//������� ���������
	double B, L; //������� ������� 3� � 4� ����� 
	wchar_t TolClass; //����������� ������ �������
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
	virtual int PointCount() const = 0;//����� ����������� �����, ������� �� ������� ��������
	//���������� �����, ������� �� ������� ��������, 
	// n- ����� ����� � ������� ������� ����� node_p
	// t - ���������� ����� �� ������, 0..1
	// P - ������������ �����
	// V - ������ �����������
	// Ax3 - ������� ��������� � �����
	virtual void IIVertex(Standard_Integer n, Standard_Real t, gp_Pnt &P, gp_Vec &V, gp_Ax3 &Ax3) const = 0;
	virtual TopoDS_Shape GetResultingShape() const = 0;
};

class IIndexableInsertSeated
{
public:
	virtual int PointCount() const = 0;//����� ����������� �����, ������� �� ������� ��������
	virtual void IIVertex(Standard_Integer n, Standard_Real t, gp_Pnt& P, gp_Vec& V, gp_Ax3& Ax3) const = 0;
	virtual gp_Dir NormalToReferencePlane() const = 0;
	virtual double EffectiveReliefAngle(Standard_Integer n, Standard_Real t) const = 0;
	virtual double EffectiveKinematicReliefAngle(Standard_Integer n, Standard_Real t, gp_Vec velocity) const = 0;
	virtual gp_Pnt XExtremityPoint() const = 0;
	virtual gp_Pnt YExtremityPoint() const = 0;
	virtual gp_Vec ToolAxis() const = 0;
	virtual TopoDS_Shape RotatedIntoPlace() const = 0;
};

class IIndexableInsertTool
{
public:
	virtual int InsertCount() const = 0;
	virtual void AppendInsert(std::shared_ptr<IIndexableInsertSeated>) = 0;
};

class IInsertPreview
{
public:
	virtual void Preview(std::shared_ptr<IIndexableInsert>) = 0;
	virtual void Preview(std::shared_ptr<IIndexableInsertSeated>) = 0;
	virtual void Preview(std::shared_ptr<IIndexableInsertTool>) = 0;
	virtual void DestroyPreview() = 0;
};