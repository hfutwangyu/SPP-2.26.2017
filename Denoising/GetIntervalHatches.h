#ifndef GETINTERVALHATCHES
#define GETINTERVALHATCHES

#include "GetHexagonHatches.h"

class GetIntervalHatches
{
public:
	GetIntervalHatches();
	~GetIntervalHatches();

public:
	void get60ParallelLines(TriMesh::Slicing &slice_of_mesh_, int &scale);
	void get120ParallelLines(TriMesh::Slicing &slice_of_mesh_, int &scale);
	void get0ParallelLines(TriMesh::Slicing &slice_of_mesh_, int &scale);
    void getIntervalHatchTriangles(TriMesh &mesh);

public:
	double parallel_line_spacing;
	std::vector<Paths> _0_parallel_lines_, _60_parallel_lines_, _120_parallel_lines_;
	
};

#endif // !GETINTERVALHATCHES


