struct IndInsert;
class CIndexableInsert;
__declspec(dllexport) void SetWndHandle(HWND hWnd);
__declspec(dllexport) CIndexableInsert* InsertShape(Handle_AIS_InteractiveContext, const IndInsert*, Standard_Boolean RebuildAnyway = Standard_True);
extern Handle_AIS_Shape CuttingPlate;

//enum GroupCapt { GCP_ESEA = '0', GCP_ESNA = '1', GCP_NSEA = '2', GCP_NSNA = '3' };
//enum GroupCapt { GCP_ESEA = 0, GCP_ESNA = 1, GCP_NSEA = 2, GCP_NSNA = 3 };

/*class CPart//�������������� ������
{
public:
	CPart(gp_Ax3 vBase) {Base=vBase;};
	gp_Ax3 GetBase(){return Base;};
protected:
	gp_Ax3 Base;//�� ���������� ����������� � �
};*/

enum VertForm { VF_SHARP=0, VF_FILLET=1, VF_CHAMFER=2};
//��� ��������� � ��������
enum HoleType { NoHole=0, CylHole=1, TorHole14060=2, TorHole24060=3, TorHole17090=4, TorHole27090=5};
/*��� ���������
� �������������� ����������
� ������������� ������������ ���������� ��� ����� � �������� �������� � ����� �� 40 �� 60 ��������
� ������������ ������������ ���������� ��� ����� � �������� �������� � ����� �� 40 �� 60 ��������
� ������������� ������������ ���������� ��� ����� � �������� �������� � ����� �� 70 �� 90 ��������
� ������������ ������������ ���������� ��� ����� � �������� �������� � ����� �� 70 �� 90 ��������
������
*/

//����������� ������� �����������
enum DirToolType {DirTool_Right=0, DirTool_Left=1};

//����������� ������� �������� ���
enum CutDirection {Dir_Right=0, Dir_Left=1, Dir_Random};

enum EdgeDir {EdgeDir_Left=-1, EdgeDir_Right=1};

//��� �����������
enum ToolType {Turning_Cutter=0, Boring_Cutter=1, Drilling_Tool=2, Milling_Tool=3};
/*�����, �������� �������;
�����, ������������;
������ ����������(������, ������, ���������)
�����;
*/


struct IndInsert
{
	int IGroup;//����� ������ ��������
	int IIForm;//����� ����� ��������
	wchar_t FormChar;
	int n;//����� ������
	double eps;//���� ��� �������
	double RackAng;//�������� ������� ����
	int HT;//������� � ����� ���������
	int VertForm;//����� �������
	double r;//������ ���������� �������
	double Dim;//������
	double Thick;//�������
	int Dir;//����������� �������
	double DHole;//������� ���������
	double B, L; //������� ������� 3� � 4� ����� 
	wchar_t TolClass; //����������� ������ �������
	int ActEdge; //�������� ������
};

const double deg=M_PI/180;

class CIndexableInsert
{
public:
	virtual int NumPoint() const = 0;//����� ����������� �����, ������� �� ������� ��������
	//���������� �����, ������� �� ������� ��������, 
	// n- ����� ����� � ������� ������� ����� node_p
	// t - ���������� ����� �� ������, 0..1
	// P - ������������ �����
	// V - ������ �����������
	// Ax3 - ������� ��������� � �����
	virtual void IIVertex(Standard_Integer n, Standard_Real t, gp_Pnt &P, gp_Vec &V, gp_Ax3 &Ax3) const = 0;
};