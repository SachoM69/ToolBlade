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

// ToolBladeTGDoc.h : интерфейс класса CToolBladeTGDoc
//


#pragma once
#include "DocBase.h"
#include "ISession_Direction.h"
#include <vector>
#include "ToolDataList.h"

class CToolBladeTGDoc : public CDocBase, IInstrInsList
{
protected: // создать только из сериализации
	CToolBladeTGDoc();
	DECLARE_DYNCREATE(CToolBladeTGDoc)

// Атрибуты
public:

//Переменные
private:
	struct IndInsData
	{
		IndInsert libdata;
		IndInsAttributes diagdata;
		CIndexableInsert* libcpptr;
	};
	std::vector<IndInsData> CutterParams;
	ToolData Instrument;
	Handle_ISession_Direction MyCoolArrow[3];
	Handle_ISession_Direction PntDirArrow;
	Handle_AIS_Point VisPnt;

	IndInsert GetDefaultInsert() const;
	// Операции
public:

// Переопределение
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS
	virtual HRESULT QueryIndInsertInformation(int index, IndInsert*) override;
	virtual HRESULT UpdateIndInsertInformation(int index, const IndInsert*) override;
	virtual HRESULT QueryIndInsertAttributes(int index, IndInsAttributes*) override;
	virtual HRESULT UpdateIndInsertAttributes(int index, const IndInsAttributes*) override;
	virtual HRESULT QueryToolData(ToolData*) override;
	virtual HRESULT QueryIndInsertObject(int index, const CIndexableInsert**) override;
	virtual HRESULT RequestNewInsert(int* index_inout);
	virtual HRESULT RequestRemoveInsert(int index);
	virtual HRESULT ShowPoint(gp_Pnt, bool) override;
	virtual HRESULT RefreshCutter(int index, IndInsert*) override;

// Реализация
public:
	virtual ~CToolBladeTGDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
   static void Fit();

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Вспомогательная функция, задающая содержимое поиска для обработчика поиска
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnNewCutter();
	afx_msg void OnEdparams();
	afx_msg void OnTooltype();
	afx_msg void OnShowtool();
};
