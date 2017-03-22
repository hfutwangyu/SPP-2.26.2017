#include "GetIntervalHatches.h"


GetIntervalHatches::GetIntervalHatches() :parallel_line_spacing(0.03)
{
}


GetIntervalHatches::~GetIntervalHatches()
{
}


void GetIntervalHatches::get60ParallelLines(TriMesh::Slicing &slice_of_mesh_, int &scale)
{
	for (auto contours_it = slice_of_mesh_.begin(); contours_it != slice_of_mesh_.end(); contours_it++)
	{
		double min_x = 1.0e8, max_x = 1.0e-9, min_y = 1.0e8, max_y = 1.0e-9;
		TriMesh::Contours layer_contours = *contours_it;
		GetHexagonalSubarea get_mesh_hexagonal_subarea_;
		get_mesh_hexagonal_subarea_.getMaxAndMinXYofLayer(layer_contours, min_x, max_x, min_y, max_y);

		double start_x = 0.0, 
			   start_y = min_y - 1.0,
	           end_x = max_x, 
			   end_y = max_y+(max_x-min_x)*sqrt(3.0);
		Paths lines;
		for (int i = 0; (start_x = min_x - (max_y - min_y + 1.0) / sqrt(3.0) + i*2.0*parallel_line_spacing / sqrt(3.0)) <= max_x;i++)
		{
			end_x = end_x + 2.0*parallel_line_spacing / sqrt(3.0);
			Path line;
			line << IntPoint(start_x*scale, start_y*scale)
				<< IntPoint(end_x*scale, end_y*scale);
			lines << line;
		}
		_60_parallel_lines_.push_back(lines);
	}

}


void GetIntervalHatches::get120ParallelLines(TriMesh::Slicing &slice_of_mesh_, int &scale)
{
	for (auto contours_it = slice_of_mesh_.begin(); contours_it != slice_of_mesh_.end(); contours_it++)
	{

		double min_x = 1.0e8, max_x = 1.0e-9, min_y = 1.0e8, max_y = 1.0e-9;
		TriMesh::Contours layer_contours = *contours_it;
		GetHexagonalSubarea get_mesh_hexagonal_subarea_;
		get_mesh_hexagonal_subarea_.getMaxAndMinXYofLayer(layer_contours, min_x, max_x, min_y, max_y);

		double start_x = (max_y-min_y+1.0)/sqrt(3.0)+2*max_x-min_x,
			   start_y = min_y - 1.0,
			   end_x = max_x,
			   end_y = max_y + (max_x - min_x)*sqrt(3.0);
		Paths lines;
		for (int i = 0; (start_x  = start_x - 2.0*parallel_line_spacing / sqrt(3.0)) >= min_x; i++)
		{
			end_x = end_x - 2.0*parallel_line_spacing / sqrt(3.0);
			Path line;
			line << IntPoint(start_x*scale, start_y*scale)
				<< IntPoint(end_x*scale, end_y*scale);
			lines << line;
		}
		_120_parallel_lines_.push_back(lines);
	}
}

void GetIntervalHatches::get0ParallelLines(TriMesh::Slicing &slice_of_mesh_, int &scale)
{
	for (auto contours_it = slice_of_mesh_.begin(); contours_it != slice_of_mesh_.end(); contours_it++)
	{
		double min_x = 1.0e8, max_x = 1.0e-9, min_y = 1.0e8, max_y = 1.0e-9;
		TriMesh::Contours layer_contours = *contours_it;
		GetHexagonalSubarea get_mesh_hexagonal_subarea_;
		get_mesh_hexagonal_subarea_.getMaxAndMinXYofLayer(layer_contours, min_x, max_x, min_y, max_y);

		double horizontal_min = min_x - 1.0,
			horizontal_max = max_x + 1.0,
			vertical_y = 0.0,
			vertical_min = min_y;
		Paths lines;
		for (int i = 0; (vertical_y = max_y - i*parallel_line_spacing) >= vertical_min; i++)
		{
			Path line;
			line << IntPoint(horizontal_min*scale, vertical_y*scale)
				<< IntPoint(horizontal_max*scale, vertical_y*scale);
			lines << line;
		}
		_0_parallel_lines_.push_back(lines);
	}
}

void GetIntervalHatches::getIntervalHatchTriangles(TriMesh &mesh)
{
	if ((_0_parallel_lines_.size()==_60_parallel_lines_.size())
		&&(_0_parallel_lines_.size()==_120_parallel_lines_.size()))
	{
		for (int i = 0; i < _0_parallel_lines_.size();i++)
		{
		  Paths _0_lines = _0_parallel_lines_[i],
			    _60_lines = _60_parallel_lines_[i],
			    _120_lines = _120_parallel_lines_[i];
		  Paths one_layer_intervals_ = mesh.mesh_areas_betweent_hexagons_int_paths_[i];

		  std::vector<Paths> layer_solution_;
	      Clipper c_0_;
		  PolyTree solution_polytree_0_;
	      c_0_.AddPaths(_0_lines, ptSubject, false);
		  c_0_.AddPaths(one_layer_intervals_, ptClip, true);
		  c_0_.Execute(ctIntersection, solution_polytree_0_, pftEvenOdd, pftEvenOdd);
		  Paths solution_paths_0_;
		  OpenPathsFromPolyTree(solution_polytree_0_, solution_paths_0_);
		  layer_solution_.push_back(solution_paths_0_);

		  Clipper c_60_;
		  PolyTree solution_polytree_60_;
		  c_60_.AddPaths(_60_lines, ptSubject, false);
		  c_60_.AddPaths(one_layer_intervals_, ptClip, true);
		  c_60_.Execute(ctIntersection, solution_polytree_60_, pftEvenOdd, pftEvenOdd);
		  Paths solution_paths_60_;
		  OpenPathsFromPolyTree(solution_polytree_60_, solution_paths_60_);
		  layer_solution_.push_back(solution_paths_60_);

		  Clipper c_120_;
		  PolyTree solution_polytree_120_;
		  c_120_.AddPaths(_120_lines, ptSubject, false);
		  c_120_.AddPaths(one_layer_intervals_, ptClip, true);
		  c_120_.Execute(ctIntersection, solution_polytree_120_, pftEvenOdd, pftEvenOdd);
		  Paths solution_paths_120_;
		  OpenPathsFromPolyTree(solution_polytree_120_, solution_paths_120_);
		  layer_solution_.push_back(solution_paths_120_);

		  mesh.mesh_interval_hatches_int_.push_back(layer_solution_);
		}
	}
	else
	{
		exit(1);
	}
}



