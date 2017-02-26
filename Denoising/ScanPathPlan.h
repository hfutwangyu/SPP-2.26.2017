#ifndef SCANPATHPLAN_H
#define SCANPATHPLAN_H

#include "../Algorithms/MeshDenoisingBase.h"
#include "Slice.h"
class ScanPathPlan : public MeshDenoisingBase
{
public:
	ScanPathPlan(DataManager *_data_manager, ParameterSet *_parameter_set);
	~ScanPathPlan();
private:
	void denoise();
	void initParameters();

	void getPlanPath();
};

#endif // SCANPATHPLAN_H