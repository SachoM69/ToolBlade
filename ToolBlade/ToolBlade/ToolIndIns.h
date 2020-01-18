#pragma once

//#include <Degrees.hxx>
#include <vector>
#include "IndexableIns.h"
#include "CuttingTooth.h"

class CToolIndIns
{
public:
	CToolIndIns(void);
	~CToolIndIns(void);
	std::vector<CCuttingTooth> IndInserts;
};

