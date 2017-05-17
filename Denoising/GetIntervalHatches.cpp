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
		//for (int i = 0; (start_x = min_x - (max_y - min_y + 1.0) / sqrt(3.0) + i*parallel_line_spacing) <= max_x; end_x = max_x + i* parallel_line_spacing, i++)
		{
			end_x = max_x + i*2.0*parallel_line_spacing / sqrt(3.0);
			//end_x = end_x + parallel_line_spacing ;
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

		double //start_x = (max_y-min_y+1.0)/sqrt(3.0)+2*max_x-min_x,
			   start_x = 0.0,
			   start_y = min_y - 1.0,
			   end_x = max_x,
			   end_y = max_y + (max_x - min_x)*sqrt(3.0);
		Paths lines;
		for (int i = 0; (start_x = (max_y - min_y + 1.0) / sqrt(3.0) + 2 * max_x - min_x - 2.0*i*parallel_line_spacing / sqrt(3.0)) >= min_x; i++)
		//for (int i = 0; (start_x = (max_y - min_y + 1.0) / sqrt(3.0) + 2 * max_x - min_x - i*parallel_line_spacing) >= min_x; end_x = max_x - i*parallel_line_spacing, i++)
		{
			end_x = max_x - i*2.0*parallel_line_spacing / sqrt(3.0);
			//end_x = end_x - parallel_line_spacing;
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
		for (int i = 0; (vertical_y =  max_y + (max_x - min_x)*sqrt(3.0) - i*parallel_line_spacing) >= vertical_min; i++)
		//for (int i = 0; (vertical_y = max_y + (max_x - min_x)*sqrt(3.0) - i*sqrt(3.0)*parallel_line_spacing/2.0) >= vertical_min; i++)
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


void GetIntervalHatches::getY60ParallelLines(TriMesh::Slicing &slice_of_mesh_, int &scale, double &side_length_of_bounding_hexagon)
//5.16.2017
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
			end_y = max_y + sqrt(3.0);
		Paths lines;
		for (int i = 0; (start_x = min_x-side_length_of_bounding_hexagon - (max_y - min_y + 1.0) / sqrt(3.0) + i*side_length_of_bounding_hexagon) <= max_x; i++)//5.17.2017 -side_length_of_bounding_hexagon
		{
			end_x = min_x-side_length_of_bounding_hexagon +1.0 + i*side_length_of_bounding_hexagon;//5.17.2017 -side_length_of_bounding_hexagon
			Path line;
			line << IntPoint(start_x*scale, start_y*scale)
				<< IntPoint(end_x*scale, end_y*scale);
			lines << line;
		}
		Y_60_parallel_lines_.push_back(lines);
	}

}

void GetIntervalHatches::getY120ParallelLines(TriMesh::Slicing &slice_of_mesh_, int &scale, double &side_length_of_bounding_hexagon)
//5.16.2017
{
	for (auto contours_it = slice_of_mesh_.begin(); contours_it != slice_of_mesh_.end(); contours_it++)
	{

		double min_x = 1.0e8, max_x = 1.0e-9, min_y = 1.0e8, max_y = 1.0e-9;
		TriMesh::Contours layer_contours = *contours_it;
		GetHexagonalSubarea get_mesh_hexagonal_subarea_;
		get_mesh_hexagonal_subarea_.getMaxAndMinXYofLayer(layer_contours, min_x, max_x, min_y, max_y);

		double original_start_x = min_x-side_length_of_bounding_hexagon-1.0/sqrt(3.0),//5.17.2017 -side_length_of_bounding_hexagon
			start_y = max_y+1.0,
			original_end_x = min_x-side_length_of_bounding_hexagon + (max_y - min_y + 1.0) / sqrt(3.0),//5.17.2017 -side_length_of_bounding_hexagon
			end_y = min_y-1.0;

		do 
		{
			original_start_x += side_length_of_bounding_hexagon;
			original_end_x += side_length_of_bounding_hexagon;
		} while (original_start_x <= max_x);

		double start_x, end_x;
		Paths lines;
		for (int i = 0; (start_x = original_end_x - side_length_of_bounding_hexagon*i) >= min_x-side_length_of_bounding_hexagon; i++)//5.17.2017 -side_length_of_bounding_hexagon
		{
			end_x = original_start_x - side_length_of_bounding_hexagon*i;
			Path line;
			line << IntPoint(start_x*scale, end_y*scale)
				<< IntPoint(end_x*scale, start_y*scale);
			lines << line;
		}
		Y_120_parallel_lines_.push_back(lines);
	}
}

void GetIntervalHatches::getY0ParallelLines(TriMesh::Slicing &slice_of_mesh_, int &scale, double &side_length_of_bounding_hexagon)
//5.16.2017
{
	for (auto contours_it = slice_of_mesh_.begin(); contours_it != slice_of_mesh_.end(); contours_it++)
	{
		double min_x = 1.0e8, max_x = 1.0e-9, min_y = 1.0e8, max_y = 1.0e-9;
		TriMesh::Contours layer_contours = *contours_it;
		GetHexagonalSubarea get_mesh_hexagonal_subarea_;
		get_mesh_hexagonal_subarea_.getMaxAndMinXYofLayer(layer_contours, min_x, max_x, min_y, max_y);

		double horizontal_min = min_x-side_length_of_bounding_hexagon - 1.0,//5.17.2017 -side_length_of_bounding_hexagon
			horizontal_max = max_x + 1.0,
			vertical_y = 0.0,
			vertical_min = min_y;
		Paths lines;
		for (int i = 0; (vertical_y = max_y  - i*side_length_of_bounding_hexagon*sqrt(3.0)/2.0) >= vertical_min; i++)
		{
			Path line;
			line << IntPoint(horizontal_min*scale, vertical_y*scale)
				<< IntPoint(horizontal_max*scale, vertical_y*scale);
			lines << line;
		}
		Y_0_parallel_lines_.push_back(lines);
	}
}

void GetIntervalHatches::getIntervalHatchYConnection(TriMesh &mesh, std::vector<Paths> &layers_integer_, std::vector<Paths> &bounding_hexagons_in_layers_interger_)
//add Y Connection 5.16.2017
{
	if ((Y_0_parallel_lines_.size() == Y_60_parallel_lines_.size())
		&& (Y_0_parallel_lines_.size() == Y_120_parallel_lines_.size())
		&&(Y_0_parallel_lines_.size()==bounding_hexagons_in_layers_interger_.size())
		&& (Y_0_parallel_lines_.size() == layers_integer_.size()))
	{
		for (int i = 0; i < bounding_hexagons_in_layers_interger_.size();i++)
		{
			Paths y_0_lines = Y_0_parallel_lines_[i],
				y_60_lines = Y_60_parallel_lines_[i],
				y_120_lines = Y_120_parallel_lines_[i];
			Paths layer_contours = layers_integer_[i];
			Paths hexagons = bounding_hexagons_in_layers_interger_[i];

			std::vector<Paths> layer_solution_;


			Clipper hexagons_intersection;
			Paths solution_hexagons_intersection;
			hexagons_intersection.AddPaths(layer_contours,ptSubject, true);
			hexagons_intersection.AddPaths(hexagons,ptClip,true);
			hexagons_intersection.Execute(ctIntersection, solution_hexagons_intersection, pftEvenOdd, pftEvenOdd);
			layer_solution_.push_back(solution_hexagons_intersection);//hexaghons

		    Paths solution_hexagons_difference;
			Clipper c_difference;
			c_difference.AddPaths(layer_contours, ptSubject, true);
			c_difference.AddPaths(solution_hexagons_intersection, ptClip, true);
			c_difference.Execute(ctDifference, solution_hexagons_difference, pftEvenOdd, pftEvenOdd);///get intervals between hexagonal subareas

			Clipper y_0_;
			PolyTree solution_polytree_0_;
			y_0_.AddPaths(y_0_lines, ptSubject, false);
			y_0_.AddPaths(solution_hexagons_difference, ptClip, true);
			y_0_.Execute(ctIntersection, solution_polytree_0_, pftEvenOdd, pftEvenOdd);
			Paths solution_paths_0_;
			OpenPathsFromPolyTree(solution_polytree_0_, solution_paths_0_);
			layer_solution_.push_back(solution_paths_0_);

			Clipper y_60_;
			PolyTree solution_polytree_60_;
			y_60_.AddPaths(y_60_lines, ptSubject, false);
			y_60_.AddPaths(solution_hexagons_difference, ptClip, true);
			y_60_.Execute(ctIntersection, solution_polytree_60_, pftEvenOdd, pftEvenOdd);
			Paths solution_paths_60_;
			OpenPathsFromPolyTree(solution_polytree_60_, solution_paths_60_);
			layer_solution_.push_back(solution_paths_60_);
			
			Clipper y_120_;
			PolyTree solution_polytree_120_;
			y_120_.AddPaths(y_120_lines, ptSubject, false);
			y_120_.AddPaths(solution_hexagons_difference, ptClip, true);
			y_120_.Execute(ctIntersection, solution_polytree_120_, pftEvenOdd, pftEvenOdd);
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
