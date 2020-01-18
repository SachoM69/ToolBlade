#pragma once
class CDocBase : public CDocument
{
public:
	CDocBase(void);
	~CDocBase(void);
protected:
  Handle_V3d_Viewer myViewer;
  Handle_AIS_InteractiveContext myAISContext;

// Атрибуты
public:
	Handle_AIS_InteractiveContext& GetAISContext(){ return myAISContext; };
	Handle_V3d_Viewer GetViewer()  { return myViewer; };
};

