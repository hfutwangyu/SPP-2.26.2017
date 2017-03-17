#include "GetHexagonalSubarea.h"


GetHexagonalSubarea::GetHexagonalSubarea()
{
}


GetHexagonalSubarea::~GetHexagonalSubarea()
{
}


void GetHexagonalSubarea::transformLayersDataTypeToInteger(TriMesh::Slicing &slice_of_mesh_)
//////////////change layers data type from double to cInt
{
	for (auto contours_it = slice_of_mesh_.begin(); contours_it != slice_of_mesh_.end();contours_it++)
	{
		TriMesh::Contours layer_contours = *contours_it;
		Paths contours_integer;
		for (auto polylines_it = layer_contours.begin(); polylines_it != layer_contours.end();polylines_it++)
		{
			TriMesh::Polylines polyline_in_the_contours = *polylines_it;
			Path polyline_integer;
			for (auto points_it = polyline_in_the_contours.begin(); points_it != polyline_in_the_contours.end();points_it++)
			{
				TriMesh::Point pt_double = *points_it;
				polyline_integer << IntPoint(scale*pt_double[0],scale*pt_double[1]);
			}
			contours_integer << polyline_integer;
		}
		layers_integer_.push_back(contours_integer);
	}
}


void GetHexagonalSubarea::getHexagons(TriMesh::Slicing &slice_of_mesh_)
//////////////get hexagons in each layer
{
	for (auto contours_it = slice_of_mesh_.begin(); contours_it != slice_of_mesh_.end();contours_it++)
	{
		double min_x = 1.0e8, max_x = 1.0e-9, min_y = 1.0e8, max_y = 1.0e-9;
		TriMesh::Contours layer_contours = *contours_it;
		getMaxAndMinXYofLayer(layer_contours, min_x, max_x, min_y, max_y);

		double horizontal_x=0.0, vertical_y=0.0;
		Paths hexagons;
		for (int j = 0; (vertical_y = min_y + (0.5*sqrt(3.0)*side_length_of_bounding_hexagon*j)) <= max_y; j++)
		{
			for (int i = 0; (horizontal_x = min_x + (1.5*side_length_of_bounding_hexagon*i)) <= max_x; i++)
			{
				if ((i%2 == 1 && j%2 == 1)||
					(i%2 == 0 && j%2 == 0))//if both i and j are odd or even
				{   
					
					Path hexagon;
					hexagon << IntPoint((horizontal_x + side_length_of_hexagon)*scale, (vertical_y)*scale)
						<< IntPoint((horizontal_x + 0.5*side_length_of_hexagon)*scale, (vertical_y + 0.5*sqrt(3.0)*side_length_of_hexagon)*scale)
						<< IntPoint((horizontal_x - 0.5*side_length_of_hexagon)*scale, (vertical_y + 0.5*sqrt(3.0)*side_length_of_hexagon)*scale)
						<< IntPoint((horizontal_x - side_length_of_hexagon)*scale, (vertical_y)*scale)
						<< IntPoint((horizontal_x - 0.5*side_length_of_hexagon)*scale, (vertical_y - 0.5*sqrt(3.0)*side_length_of_hexagon)*scale)
						<< IntPoint((horizontal_x + 0.5*side_length_of_hexagon)*scale, (vertical_y - 0.5*sqrt(3.0)*side_length_of_hexagon)*scale);
					hexagons << hexagon;
				}
			}
		}

		hexagons_in_layers_interger_.push_back(hexagons);
	}
}


void GetHexagonalSubarea::getMaxAndMinXYofLayer(TriMesh::Contours &layer_contours, double &min_x, double &max_x, double &min_y, double &max_y)
/////////////////get the max & min x&y in one layer 
{

	for (auto polylines_it = layer_contours.begin(); polylines_it != layer_contours.end();polylines_it++)
	{
		TriMesh::Polylines polyline_in_the_contours = *polylines_it;
		for (auto points_it = polyline_in_the_contours.begin(); points_it != polyline_in_the_contours.end(); points_it++)
		{
			TriMesh::Point point_in_polyline = *points_it;
			if (point_in_polyline[0] <= min_x) min_x = point_in_polyline[0];
			if (point_in_polyline[0] >= max_x) max_x = point_in_polyline[0];

			if (point_in_polyline[1] <= min_y) min_y = point_in_polyline[1];
			if (point_in_polyline[1] >= max_y) max_y = point_in_polyline[1];
		}
	}
}


void GetHexagonalSubarea::segmenLayersIntoHexagonalSubareas(TriMesh &mesh, TriMesh::Slicing &slice_of_mesh_)
{	
	transformLayersDataTypeToInteger(slice_of_mesh_);
	getHexagons(slice_of_mesh_);///////
	if ((layers_integer_.size())==(hexagons_in_layers_interger_.size()))
	{
		int n_layers = layers_integer_.size();
		for (int i = 0; i < n_layers; i++)
		{
			Paths contours_integer = layers_integer_[i];
			Paths hexagon = hexagons_in_layers_interger_[i];
			Paths layer_solution_intersection_;
			Clipper c_intersection;
			c_intersection.AddPaths(contours_integer, ptSubject, true);
			c_intersection.AddPaths(hexagon, ptClip, true);
			c_intersection.Execute(ctIntersection, layer_solution_intersection_, pftEvenOdd, pftEvenOdd);
			mesh.mesh_hexagoned_hexagons_int_paths_.push_back(layer_solution_intersection_);
			
			Paths layer_solution_difference_;
			Clipper c_difference;
			c_difference.AddPaths(contours_integer, ptSubject, true);
			c_difference.AddPaths(layer_solution_intersection_, ptClip, true);
			c_difference.Execute(ctDifference,layer_solution_difference_,pftEvenOdd,pftEvenOdd);
			mesh.mesh_areas_betweent_hexagons_int_paths_.push_back(layer_solution_difference_);
		}
	}
	else
	{
		exit(0);////
	}


}
