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
	layers_integer_.swap(std::vector<Paths>());
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
		Paths bounding_hexagons;//5.16.2017

		std::vector<int> temp_one_layer_hexagon_column_order_;//5.12.2017

		//for (int j = 0; (vertical_y = min_y + (0.5*sqrt(3.0)*side_length_of_bounding_hexagon*j)) <= (max_y + 0.5*sqrt(3.0)*side_length_of_bounding_hexagon); j++)
		for (int j = 0; (vertical_y = max_y - (0.5*sqrt(3.0)*side_length_of_bounding_hexagon*j)) >= (min_y - 0.5*sqrt(3.0)*side_length_of_bounding_hexagon); j++)//5.15.2017//5.17.2017
			//max_y + 0.5*sqrt(3.0)*side_length_of_bounding_hexagon  revise large blank at the outside contour boundary 4.15.2017
		{
			for (int i = 0; (horizontal_x = min_x-side_length_of_bounding_hexagon + (1.5*side_length_of_bounding_hexagon*i)) <= (max_x + 1.5*side_length_of_bounding_hexagon); i++)//5.17.2017//5.17.2017 -side_length_of_bounding_hexagon
				//max_x + 1.5*side_length_of_bounding_hexagon revise large blank at the outside contour boundary 4.15.2017
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

					temp_one_layer_hexagon_column_order_.push_back(j+1);//5.12.2017

					Path bounding_hexagon;//5.16.2017
					bounding_hexagon << IntPoint((horizontal_x + (side_length_of_bounding_hexagon - parallel_line_spacing * 2 / sqrt(3.0)))*scale, (vertical_y)*scale)
						<< IntPoint((horizontal_x + 0.5*(side_length_of_bounding_hexagon - parallel_line_spacing * 2 / sqrt(3.0)))*scale, (vertical_y + 0.5*sqrt(3.0)*(side_length_of_bounding_hexagon - parallel_line_spacing * 2 / sqrt(3.0)))*scale)
						<< IntPoint((horizontal_x - 0.5*(side_length_of_bounding_hexagon - parallel_line_spacing * 2 / sqrt(3.0)))*scale, (vertical_y + 0.5*sqrt(3.0)*(side_length_of_bounding_hexagon - parallel_line_spacing * 2 / sqrt(3.0)))*scale)
						<< IntPoint((horizontal_x - (side_length_of_bounding_hexagon - parallel_line_spacing * 2 / sqrt(3.0)))*scale, (vertical_y)*scale)
						<< IntPoint((horizontal_x - 0.5*(side_length_of_bounding_hexagon - parallel_line_spacing * 2 / sqrt(3.0)))*scale, (vertical_y - 0.5*sqrt(3.0)*(side_length_of_bounding_hexagon - parallel_line_spacing * 2 / sqrt(3.0)))*scale)
						<< IntPoint((horizontal_x + 0.5*(side_length_of_bounding_hexagon - parallel_line_spacing * 2 / sqrt(3.0)))*scale, (vertical_y - 0.5*sqrt(3.0)*(side_length_of_bounding_hexagon - parallel_line_spacing * 2 / sqrt(3.0)))*scale);
					bounding_hexagons << bounding_hexagon;//////////////
				}
			}
		}

		hexagons_in_layers_interger_.push_back(hexagons);

		temp_hexagon_column_order_.push_back(temp_one_layer_hexagon_column_order_);//5.12.2017

		interior_hexagons_in_layers_interger_.push_back(bounding_hexagons);//5.16.2017
	}
}


void GetHexagonalSubarea::getHexagonsStaggeredBetweenLayers(TriMesh::Slicing &slice_of_mesh_)
//get hexagons starggered between layers 4.22.2017
{
	int n_layers = slice_of_mesh_.size();
	for (int k = 0; k < n_layers; k++)
	{
		double min_x = 1.0e8, max_x = 1.0e-9, min_y = 1.0e8, max_y = 1.0e-9;
		TriMesh::Contours layer_contours = slice_of_mesh_[k];
		getMaxAndMinXYofLayer(layer_contours, min_x, max_x, min_y, max_y);

		double horizontal_x = 0.0, vertical_y = 0.0;
		Paths hexagons;
		Paths bounding_hexagons;//5.16.2017

		std::vector<int> temp_one_layer_hexagon_column_order_;//5.12.2017

		if (k%2==1)
		{
			//for (int j = 0; (vertical_y = min_y + (0.5*sqrt(3.0)*side_length_of_bounding_hexagon*j)) <= (max_y + 0.5*sqrt(3.0)*side_length_of_bounding_hexagon); j++)
			for (int j = 0; (vertical_y = max_y - (0.5*sqrt(3.0)*side_length_of_bounding_hexagon*j)) >= (min_y - 2*0.5*sqrt(3.0)*side_length_of_bounding_hexagon); j++)//5.15.2017//5.17.2017
				//max_y + 0.5*sqrt(3.0)*side_length_of_bounding_hexagon  revise large blank at the outside contour boundary 4.15.2017
			{
				for (int i = 0; (horizontal_x = min_x-side_length_of_bounding_hexagon + (1.5*side_length_of_bounding_hexagon*i)) <= (max_x + 2*1.5*side_length_of_bounding_hexagon); i++)//5.17.2017//5.17.2017 -side_length_of_bounding_hexagon
					//max_x + 1.5*side_length_of_bounding_hexagon revise large blank at the outside contour boundary 4.15.2017
				{
					if ((i % 2 == 1 && j % 2 == 1) ||
						(i % 2 == 0 && j % 2 == 0))//if both i and j are odd or even
					{

						Path hexagon;
						hexagon << IntPoint((horizontal_x + side_length_of_hexagon)*scale, (vertical_y)*scale)
							<< IntPoint((horizontal_x + 0.5*side_length_of_hexagon)*scale, (vertical_y + 0.5*sqrt(3.0)*side_length_of_hexagon)*scale)
							<< IntPoint((horizontal_x - 0.5*side_length_of_hexagon)*scale, (vertical_y + 0.5*sqrt(3.0)*side_length_of_hexagon)*scale)
							<< IntPoint((horizontal_x - side_length_of_hexagon)*scale, (vertical_y)*scale)
							<< IntPoint((horizontal_x - 0.5*side_length_of_hexagon)*scale, (vertical_y - 0.5*sqrt(3.0)*side_length_of_hexagon)*scale)
							<< IntPoint((horizontal_x + 0.5*side_length_of_hexagon)*scale, (vertical_y - 0.5*sqrt(3.0)*side_length_of_hexagon)*scale);
						hexagons << hexagon;

						temp_one_layer_hexagon_column_order_.push_back(j + 1);//5.12.2017

						Path bounding_hexagon;//5.16.2017
						bounding_hexagon << IntPoint((horizontal_x + (side_length_of_bounding_hexagon-parallel_line_spacing*2/sqrt(3.0)))*scale, (vertical_y)*scale)
							<< IntPoint((horizontal_x + 0.5*(side_length_of_bounding_hexagon - parallel_line_spacing * 2 / sqrt(3.0)))*scale, (vertical_y + 0.5*sqrt(3.0)*(side_length_of_bounding_hexagon - parallel_line_spacing * 2 / sqrt(3.0)))*scale)
							<< IntPoint((horizontal_x - 0.5*(side_length_of_bounding_hexagon - parallel_line_spacing * 2 / sqrt(3.0)))*scale, (vertical_y + 0.5*sqrt(3.0)*(side_length_of_bounding_hexagon - parallel_line_spacing * 2 / sqrt(3.0)))*scale)
							<< IntPoint((horizontal_x - (side_length_of_bounding_hexagon - parallel_line_spacing * 2 / sqrt(3.0)))*scale, (vertical_y)*scale)
							<< IntPoint((horizontal_x - 0.5*(side_length_of_bounding_hexagon - parallel_line_spacing * 2 / sqrt(3.0)))*scale, (vertical_y - 0.5*sqrt(3.0)*(side_length_of_bounding_hexagon - parallel_line_spacing * 2 / sqrt(3.0)))*scale)
							<< IntPoint((horizontal_x + 0.5*(side_length_of_bounding_hexagon - parallel_line_spacing * 2 / sqrt(3.0)))*scale, (vertical_y - 0.5*sqrt(3.0)*(side_length_of_bounding_hexagon - parallel_line_spacing * 2 / sqrt(3.0)))*scale);
						bounding_hexagons << bounding_hexagon;//////////////
					}
				}
			}
		} 
		else if (k%2==0)
		{
			//for (int j = 0; (vertical_y = min_y + (0.5*sqrt(3.0)*side_length_of_bounding_hexagon*j)) <= (max_y + 0.5*sqrt(3.0)*side_length_of_bounding_hexagon); j++)
			for (int j = 0; (vertical_y = max_y - (0.5*sqrt(3.0)*side_length_of_bounding_hexagon*j)) >= (min_y - 2*0.5*sqrt(3.0)*side_length_of_bounding_hexagon); j++)//5.15.2017
				//max_y + 0.5*sqrt(3.0)*side_length_of_bounding_hexagon  revise large blank at the outside contour boundary 4.15.2017
			{
				for (int i = 0; (horizontal_x = min_x-side_length_of_bounding_hexagon + side_length_of_bounding_hexagon + (1.5*side_length_of_bounding_hexagon*i)) <= (max_x + 2 * 1.5*side_length_of_bounding_hexagon); i++)//5.17.2017 -side_length_of_bounding_hexagon
					//max_x + 1.5*side_length_of_bounding_hexagon revise large blank at the outside contour boundary 4.15.2017
				{
					if ((i % 2 == 1 && j % 2 == 1) ||
						(i % 2 == 0 && j % 2 == 0))//if both i and j are odd or even
					{

						Path hexagon;
						hexagon << IntPoint((horizontal_x + side_length_of_hexagon)*scale, (vertical_y)*scale)
							<< IntPoint((horizontal_x + 0.5*side_length_of_hexagon)*scale, (vertical_y + 0.5*sqrt(3.0)*side_length_of_hexagon)*scale)
							<< IntPoint((horizontal_x - 0.5*side_length_of_hexagon)*scale, (vertical_y + 0.5*sqrt(3.0)*side_length_of_hexagon)*scale)
							<< IntPoint((horizontal_x - side_length_of_hexagon)*scale, (vertical_y)*scale)
							<< IntPoint((horizontal_x - 0.5*side_length_of_hexagon)*scale, (vertical_y - 0.5*sqrt(3.0)*side_length_of_hexagon)*scale)
							<< IntPoint((horizontal_x + 0.5*side_length_of_hexagon)*scale, (vertical_y - 0.5*sqrt(3.0)*side_length_of_hexagon)*scale);
						hexagons << hexagon;

						temp_one_layer_hexagon_column_order_.push_back(j + 1);//5.12.2017

						Path bounding_hexagon;//5.16.2017
						bounding_hexagon << IntPoint((horizontal_x + (side_length_of_bounding_hexagon - parallel_line_spacing * 2 / sqrt(3.0)))*scale, (vertical_y)*scale)
							<< IntPoint((horizontal_x + 0.5*(side_length_of_bounding_hexagon - parallel_line_spacing * 2 / sqrt(3.0)))*scale, (vertical_y + 0.5*sqrt(3.0)*(side_length_of_bounding_hexagon - parallel_line_spacing * 2 / sqrt(3.0)))*scale)
							<< IntPoint((horizontal_x - 0.5*(side_length_of_bounding_hexagon - parallel_line_spacing * 2 / sqrt(3.0)))*scale, (vertical_y + 0.5*sqrt(3.0)*(side_length_of_bounding_hexagon - parallel_line_spacing * 2 / sqrt(3.0)))*scale)
							<< IntPoint((horizontal_x - (side_length_of_bounding_hexagon - parallel_line_spacing * 2 / sqrt(3.0)))*scale, (vertical_y)*scale)
							<< IntPoint((horizontal_x - 0.5*(side_length_of_bounding_hexagon - parallel_line_spacing * 2 / sqrt(3.0)))*scale, (vertical_y - 0.5*sqrt(3.0)*(side_length_of_bounding_hexagon - parallel_line_spacing * 2 / sqrt(3.0)))*scale)
							<< IntPoint((horizontal_x + 0.5*(side_length_of_bounding_hexagon - parallel_line_spacing * 2 / sqrt(3.0)))*scale, (vertical_y - 0.5*sqrt(3.0)*(side_length_of_bounding_hexagon - parallel_line_spacing * 2 / sqrt(3.0)))*scale);
						bounding_hexagons << bounding_hexagon;//////////////
					}
				}
			}
		}
	

		hexagons_in_layers_interger_.push_back(hexagons);

		temp_hexagon_column_order_.push_back(temp_one_layer_hexagon_column_order_);//5.12.2017

		interior_hexagons_in_layers_interger_.push_back(bounding_hexagons);//5.16.2017
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
	//transformLayersDataTypeToInteger(slice_of_mesh_);
	//getHexagons(slice_of_mesh_);///////get hexagons in each layer
	//getHexagonsStaggeredBetweenLayers(slice_of_mesh_);//get hexagons starggered between layers 4.22.2017
	if ((layers_integer_.size()) == (hexagons_in_layers_interger_.size()))
	{
		int n_layers = layers_integer_.size();
		for (int i = 0; i < n_layers; i++)
		{
			Paths hexagons = hexagons_in_layers_interger_[i];
			Paths contours_integer = layers_integer_[i];

			std::vector<int> temp_one_layer_hexagon_column_order_=temp_hexagon_column_order_[i];//5.12.2017
			std::vector<int> one_layer_hexagon_column_order_;//5.12.2017
 
			std::vector<Paths> layer_solution_intersection_;
			for (int j = 0; j < hexagons.size(); j++)
			{
				Path hexagon = hexagons[j];
				Paths a_hexagon_contours_intersection_solution_;
				Clipper a_hexagon_contours_intersection_;
				a_hexagon_contours_intersection_.AddPaths(contours_integer, ptSubject, true);
				a_hexagon_contours_intersection_.AddPath(hexagon, ptClip, true);
				a_hexagon_contours_intersection_.Execute(ctIntersection, a_hexagon_contours_intersection_solution_, pftEvenOdd, pftEvenOdd);

				if (a_hexagon_contours_intersection_solution_.size() != 0)
				{
					layer_solution_intersection_.push_back(a_hexagon_contours_intersection_solution_);

					one_layer_hexagon_column_order_.push_back(temp_one_layer_hexagon_column_order_[j]);//5.12.2017
				}
			}
			mesh.mesh_hexagoned_hexagons_int_paths_.push_back(layer_solution_intersection_);
			hexagon_column_order_.push_back(one_layer_hexagon_column_order_);//5.2.2017

			Paths a_layer_solution_intersection_;
			Clipper c_intersection;
			c_intersection.AddPaths(contours_integer, ptSubject, true);
			c_intersection.AddPaths(hexagons, ptClip, true);
			c_intersection.Execute(ctIntersection, a_layer_solution_intersection_, pftEvenOdd, pftEvenOdd);//get subareas
			Paths layer_solution_difference_;
			PolyTree polytree_layer_solution_difference_;
			Clipper c_difference;
			c_difference.AddPaths(contours_integer, ptSubject, true);
			c_difference.AddPaths(a_layer_solution_intersection_, ptClip, true);
			c_difference.Execute(ctDifference, polytree_layer_solution_difference_, pftEvenOdd, pftEvenOdd);///get intervals between hexagonal subareas
			ClosedPathsFromPolyTree(polytree_layer_solution_difference_, layer_solution_difference_);
			mesh.mesh_areas_betweent_hexagons_int_paths_.push_back(layer_solution_difference_);

			////offset subsectors for interval hatches crossing subsectors//5.18.2017
			//Paths a_layer_offsetted_subsectors_;
			//for (int k = 0; k < a_layer_solution_intersection_.size();k++)
			//{
			//	Path p = a_layer_solution_intersection_[k];
			//	Paths solution;
			//	ClipperOffset co;
			//	co.AddPath(p, jtMiter, etClosedPolygon);
			//	co.Execute(solution, (0.0 - parallel_line_spacing)*scale);
			//	if (solution.size()!=0)
			//	{
			//		for (int l = 0; l < solution.size();l++)
			//		{
			//			Path pp = solution[l];
			//			a_layer_offsetted_subsectors_.push_back(pp);
			//		}					
			//	}
			//}
			//Paths layer_solution_difference_with_offsected_subsectors_;
			//Clipper c_difference_with_offsetted_subsectors_;
			//c_difference_with_offsetted_subsectors_.AddPaths(contours_integer, ptSubject, true);
			//c_difference_with_offsetted_subsectors_.AddPaths(a_layer_offsetted_subsectors_, ptClip, true);
			//c_difference_with_offsetted_subsectors_.Execute(ctDifference, layer_solution_difference_with_offsected_subsectors_, pftEvenOdd, pftEvenOdd);///get intervals between hexagonal subareas

			    ////5.19.2017
			/*Paths layer_solution_difference_with_offseted_subsectors_;
			offsetIntervalContoursWithHexagonSubsectors(contours_integer, a_layer_solution_intersection_, layer_solution_difference_with_offseted_subsectors_);
			intervals_with_negative_offseted_subsectors_.push_back(layer_solution_difference_with_offseted_subsectors_);*/

			/////5.19.2017
			Paths layer_solution_difference_with_offseted_intervals_;
			offsetIntervalContoursWithIntervals(contours_integer, layer_solution_difference_, layer_solution_difference_with_offseted_intervals_);
			intervals_with_negative_offseted_subsectors_.push_back(layer_solution_difference_with_offseted_intervals_);
			//////////////////////////////////////////////////////////////////////////
		}
	}
	else
	{
		exit(1);////
	}
}


void GetHexagonalSubarea::getLayerContoursOrientation(TriMesh &mesh, TriMesh::Slicing &slice_of_mesh_)
{
	if (slice_of_mesh_.size()==layers_integer_.size())
	{
		for (int i = 0; i < slice_of_mesh_.size(); i++)
		{
			double min_x = 1.0e8, max_x = 1.0e-9, min_y = 1.0e8, max_y = 1.0e-9;
			TriMesh::Contours layer_contours = slice_of_mesh_[i];
			getMaxAndMinXYofLayer(layer_contours, min_x, max_x, min_y, max_y);

			//////////////////////////////////////////////////////////////////////////
			//adjust the orientation of each layer contour
			Path clip;
			clip << IntPoint((min_x - 10.0)*scale, (min_y - 10.0)*scale) << IntPoint((min_x - 10.0)*scale, (min_y - 20.0)*scale)
				<< IntPoint((min_x - 20.0)*scale, (min_y - 20.0)*scale) << IntPoint((min_x - 20.0)*scale, (min_y - 10.0)*scale);
			Paths contours_integer = layers_integer_[i];
			Clipper C;
			Paths solution;
			C.AddPaths(contours_integer, ptSubject, true);
			C.AddPath(clip, ptClip, true);
			C.Execute(ctDifference, solution, pftEvenOdd, pftEvenOdd);

			//////////////////////////////////////////////////////////////////////////
			//get the oriented slicing data 
			TriMesh::OrientedContours oriented_layer_contours;
			for (int j = 0; j < solution.size();j++)
			{
               std::pair<int, TriMesh::Polylines>  a_oriented_layer_contour;
			   Path p = solution[j];
			   //get the orientation
			   if (Orientation(p))
			   {
				   a_oriented_layer_contour.first = 1;
			   }
			   else
			   {
				   a_oriented_layer_contour.first = 0;
			   }
			   //get the polylines
			   for (int k = 0; k < p.size(); k++)
			   {
				   IntPoint int_pt = p[k];
				   TriMesh::Point mesh_point;
				   mesh_point[0] = (double)(int_pt.X) / scale;
				   mesh_point[1] = (double)(int_pt.Y) / scale;
				   mesh_point[2] = 0.0;
				   a_oriented_layer_contour.second.push_back(mesh_point);
			   }
			   oriented_layer_contours.push_back(a_oriented_layer_contour);
			}
			mesh.mesh_oriented_slicing_.push_back(oriented_layer_contours);
		}
	}
	else exit(1);
}




void GetHexagonalSubarea::getOuterContoursofEachLayer(TriMesh::Slicing &slice_of_mesh_)
{
	for (int i = 0; i < layers_integer_.size();i++)
	{
		Paths contours_integer = layers_integer_[i];
		Paths outer_contours_of_a_alyer_;

		double min_x = 1.0e8, max_x = 1.0e-9, min_y = 1.0e8, max_y = 1.0e-9;
		TriMesh::Contours layer_contours = slice_of_mesh_[i];
		getMaxAndMinXYofLayer(layer_contours, min_x, max_x, min_y, max_y);

		//////////////////////////////////////////////////////////////////////////
		Path clip;
		clip << IntPoint((min_x - 10.0)*scale, (min_y - 10.0)*scale) << IntPoint((min_x - 10.0)*scale, (min_y - 20.0)*scale)
			<< IntPoint((min_x - 20.0)*scale, (min_y - 20.0)*scale) << IntPoint((min_x - 20.0)*scale, (min_y - 10.0)*scale);
		Clipper C;
		PolyTree solution;
		C.AddPaths(contours_integer, ptSubject, true);
		C.AddPath(clip, ptClip, true);
		C.Execute(ctDifference, solution, pftEvenOdd, pftEvenOdd);

		int n = solution.ChildCount();
		for (int j = 0; j < n;j++)
		{
			outer_contours_of_a_alyer_.push_back(solution.Childs[j]->Contour);
		}

		layer_outer_contours_.push_back(outer_contours_of_a_alyer_);

		//////////////////////////////////////////////////////////////////////////
		//layer contours without outer contours

		Paths one_layer_contours, temp_one_layer_contours;
		ClosedPathsFromPolyTree(solution, one_layer_contours);

		//////////////////////////////////////////////////////////////////////////
		///6.22.2017
/*
		for (int  j = 0; j< one_layer_contours.size();j++)
		{
			Path contour = one_layer_contours[j];
			IntPoint spt = contour.front(),
				     ept = contour.back();
			for (int k = 0; k < n; k++)
			{
				Path one_outer_contour_of_a_alyer_ = solution.Childs[k]->Contour;
				IntPoint start_pt = one_outer_contour_of_a_alyer_.front(),
					end_pt = one_outer_contour_of_a_alyer_.back();
				if ((one_outer_contour_of_a_alyer_.size() == contour.size()) &&
					(sqrt(pow((spt.X - start_pt.X), 2) + pow((spt.Y - start_pt.Y), 2)) < 1.0e-9*scale) &&
					(sqrt(pow((ept.X - end_pt.X), 2) + pow((ept.Y - end_pt.Y), 2)) < 1.0e-9*scale))
				{
				}
				else
				{
					temp_one_layer_contours.push_back(contour);
				}
			}
		}
 */

		//////////////////////////////////////////////////////////////////////////
		//7.15.2017
		temp_one_layer_contours = one_layer_contours;
		for (int j = 0; j < one_layer_contours.size(); j++)
		{
			Path contour = one_layer_contours[j];
			IntPoint spt = contour.front(),
				ept = contour.back();
			for (int k = 0; k < n; k++)
			{
				Path one_outer_contour_of_a_alyer_ = solution.Childs[k]->Contour;
				IntPoint start_pt = one_outer_contour_of_a_alyer_.front(),
					end_pt = one_outer_contour_of_a_alyer_.back();
				if ((one_outer_contour_of_a_alyer_.size() == contour.size()) &&
					(sqrt(pow((spt.X - start_pt.X), 2) + pow((spt.Y - start_pt.Y), 2)) < 1.0e-9*scale) &&
					(sqrt(pow((ept.X - end_pt.X), 2) + pow((ept.Y - end_pt.Y), 2)) < 1.0e-9*scale))
				{
					for (auto it = temp_one_layer_contours.begin(); it != temp_one_layer_contours.end();it++)
					{
						Path one_outer_contour_of_temp_layer_contours_ = *it;
						IntPoint temp_start_pt = one_outer_contour_of_temp_layer_contours_.front(),
							temp_end_pt = one_outer_contour_of_temp_layer_contours_.back();
						if ((one_outer_contour_of_temp_layer_contours_.size() == contour.size()) &&
							(sqrt(pow((spt.X - temp_start_pt.X), 2) + pow((spt.Y - temp_start_pt.Y), 2)) < 1.0e-9*scale) &&
							(sqrt(pow((ept.X - temp_end_pt.X), 2) + pow((ept.Y - temp_end_pt.Y), 2)) < 1.0e-9*scale))
						{
							temp_one_layer_contours.erase(it);
							break;
						}
					}
				}
			}
		}


		layer_contours_without_outer_contours.push_back(temp_one_layer_contours);

	}
}


void GetHexagonalSubarea::segmenLayersIntoHexagonalSubareasWithBoundryOffset(TriMesh &mesh, TriMesh::Slicing &slice_of_mesh_)
{
	//transformLayersDataTypeToInteger(slice_of_mesh_);
	//getHexagons(slice_of_mesh_);///////get hexagons in each layer
	//getHexagonsStaggeredBetweenLayers(slice_of_mesh_);//get hexagons starggered between layers 4.22.2017
	//5.1.2017
	//getOuterContoursofEachLayer(slice_of_mesh_);

	//getOffsettedOuterContours(slice_of_mesh_);//5.2.2017
	OffsetForAllLayerContours(slice_of_mesh_);//5,4,2017

	if ((layers_integer_.size()) == (hexagons_in_layers_interger_.size()) &&
		//(layers_integer_.size()) == (layer_outer_contours_.size()))
		(layers_integer_.size()) == (offsetted_layers_integer_.size()))
	{
		int n_layers = layers_integer_.size();
		for (int i = 0; i < n_layers; i++)
		{
			Paths hexagons = hexagons_in_layers_interger_[i];
			//Paths contours_integer = layers_integer_[i];
			Paths offsetted_contours_integer = offsetted_layers_integer_[i];

			std::vector<int> temp_one_layer_hexagon_column_order_ = temp_hexagon_column_order_[i];//5.12.2017
			std::vector<int> one_layer_hexagon_column_order_;//5.12.2017
	/*		////////////////////////////////////
			//offset each layer's outer contours 5.1.2017 
			Paths outer_contours_of_a_alyer_ = layer_outer_contours_[i];
			for (int k = 0; k < contours_integer.size(); k++)
			{
				Path contour = contours_integer[k];
				IntPoint spt = contour.front(),
					ept = contour.back();

				for (int l = 0; l < outer_contours_of_a_alyer_.size(); l++)
				{
					Path one_outer_contour_of_a_alyer_ = outer_contours_of_a_alyer_[l];
					IntPoint start_pt = one_outer_contour_of_a_alyer_.front(),
						end_pt = one_outer_contour_of_a_alyer_.back();

					if ((one_outer_contour_of_a_alyer_.size() == contour.size()) &&
						(sqrt(pow((spt.X - start_pt.X), 2) + pow((spt.Y - start_pt.Y), 2)) < 1.0e-9*scale) &&
						(sqrt(pow((ept.X - end_pt.X), 2) + pow((ept.Y - end_pt.Y), 2)) < 1.0e-9*scale))
					{
						Paths solution;
						ClipperOffset co;
						co.AddPath(contour, jtMiter, etClosedPolygon);
						co.Execute(solution, -2.0*scale);

						Path offsetted_outer_contour_;
						if (solution.size() == 1)
						{
							offsetted_outer_contour_ = solution[0];
						}
						contours_integer[k].swap(offsetted_outer_contour_);
					}
				}
			}
			/////////////////////////////////////
   */
			std::vector<Paths> layer_solution_intersection_;
			for (int j = 0; j < hexagons.size(); j++)
			{
				Path hexagon = hexagons[j];
				Paths a_hexagon_contours_intersection_solution_;
				Clipper a_hexagon_contours_intersection_;
				//a_hexagon_contours_intersection_.AddPaths(contours_integer, ptSubject, true);
				a_hexagon_contours_intersection_.AddPaths(offsetted_contours_integer, ptSubject, true);
				a_hexagon_contours_intersection_.AddPath(hexagon, ptClip, true);
				a_hexagon_contours_intersection_.Execute(ctIntersection, a_hexagon_contours_intersection_solution_, pftEvenOdd, pftEvenOdd);

				if (a_hexagon_contours_intersection_solution_.size() != 0)
				{
					layer_solution_intersection_.push_back(a_hexagon_contours_intersection_solution_);

					one_layer_hexagon_column_order_.push_back(temp_one_layer_hexagon_column_order_[j]);//5.12.2017
				}
			}
			mesh.mesh_hexagoned_hexagons_int_paths_.push_back(layer_solution_intersection_);
			hexagon_column_order_.push_back(one_layer_hexagon_column_order_);//5.2.2017

			Paths a_layer_solution_intersection_;
			Clipper c_intersection;
			//c_intersection.AddPaths(contours_integer, ptSubject, true);
			c_intersection.AddPaths(offsetted_contours_integer, ptSubject, true);
			c_intersection.AddPaths(hexagons, ptClip, true);
			c_intersection.Execute(ctIntersection, a_layer_solution_intersection_, pftEvenOdd, pftEvenOdd);//get subareas
			Paths layer_solution_difference_;
			PolyTree polytree_layer_solution_difference_;
			Clipper c_difference;
			Paths contours_integer = layers_integer_[i];///
			c_difference.AddPaths(contours_integer, ptSubject, true);
			c_difference.AddPaths(a_layer_solution_intersection_, ptClip, true);
			c_difference.Execute(ctDifference, polytree_layer_solution_difference_, pftEvenOdd, pftEvenOdd);///get intervals between hexagonal subareas
			ClosedPathsFromPolyTree(polytree_layer_solution_difference_, layer_solution_difference_);
			mesh.mesh_areas_betweent_hexagons_int_paths_.push_back(layer_solution_difference_);

			////offset subsectors for interval hatches crossing subsectors//5.18.2017
			//Paths a_layer_offsetted_subsectors_;
			//for (int k = 0; k < a_layer_solution_intersection_.size(); k++)
			//{
			//	Path p = a_layer_solution_intersection_[k];
			//	Paths solution;
			//	ClipperOffset co;
			//	co.AddPath(p, jtMiter, etClosedPolygon);
			//	co.Execute(solution, (0.0 - parallel_line_spacing)*scale);
			//	if (solution.size() != 0)
			//	{
			//		for (int l = 0; l < solution.size(); l++)
			//		{
			//			Path pp = solution[l];
			//			a_layer_offsetted_subsectors_.push_back(pp);
			//		}
			//	}
			//}
			//Paths layer_solution_difference_with_offsected_subsectors_;
			//Clipper c_difference_with_offsetted_subsectors_;
			//c_difference_with_offsetted_subsectors_.AddPaths(contours_integer, ptSubject, true);
			//c_difference_with_offsetted_subsectors_.AddPaths(a_layer_offsetted_subsectors_, ptClip, true);
			//c_difference_with_offsetted_subsectors_.Execute(ctDifference, layer_solution_difference_with_offsected_subsectors_, pftEvenOdd, pftEvenOdd);///get intervals between hexagonal subareas

			    ////5.19.2017
			/*Paths layer_solution_difference_with_offseted_subsectors_;
			offsetIntervalContoursWithHexagonSubsectors(contours_integer, a_layer_solution_intersection_, layer_solution_difference_with_offseted_subsectors_);
			intervals_with_negative_offseted_subsectors_.push_back(layer_solution_difference_with_offseted_subsectors_);*/

			     /////5.19.2017
			Paths layer_solution_difference_with_offseted_intervals_;
			offsetIntervalContoursWithIntervals(contours_integer, layer_solution_difference_,layer_solution_difference_with_offseted_intervals_);
			intervals_with_negative_offseted_subsectors_.push_back(layer_solution_difference_with_offseted_intervals_);
			//////////////////////////////////////////////////////////////////////////
		}
	}
	else
	{
		exit(1);////
	}
}


void GetHexagonalSubarea::getOffsettedOuterContours(TriMesh::Slicing &slice_of_mesh_)
{
	getOuterContoursofEachLayer(slice_of_mesh_);

	offsetted_layers_integer_=layers_integer_;

	if (slice_of_mesh_.size() == offsetted_layers_integer_.size())///adjust data of layers integer 5.2.2017
	{
		for (int i = 0; i < slice_of_mesh_.size(); i++)
		{
			double min_x = 1.0e8, max_x = 1.0e-9, min_y = 1.0e8, max_y = 1.0e-9;
			TriMesh::Contours layer_contours = slice_of_mesh_[i];
			getMaxAndMinXYofLayer(layer_contours, min_x, max_x, min_y, max_y);

			//////////////////////////////////////////////////////////////////////////
			//adjust the orientation of each layer contour
			Path clip;
			clip << IntPoint((min_x - 10.0)*scale, (min_y - 10.0)*scale) << IntPoint((min_x - 10.0)*scale, (min_y - 20.0)*scale)
				<< IntPoint((min_x - 20.0)*scale, (min_y - 20.0)*scale) << IntPoint((min_x - 20.0)*scale, (min_y - 10.0)*scale);
			Paths contours_integer = offsetted_layers_integer_[i];
			Clipper C;
			Paths lc;
			C.AddPaths(contours_integer, ptSubject, true);
			C.AddPath(clip, ptClip, true);
			C.Execute(ctDifference, lc, pftEvenOdd, pftEvenOdd);



			////////////////////////////////////
			//offset each layer's outer contours 5.1.2017 
			Paths outer_contours_of_a_alyer_ = layer_outer_contours_[i];
			for (int k = 0; k < lc.size(); k++)
			{
				Path contour = lc[k];
				IntPoint spt = contour.front(),
					ept = contour.back();

				for (int l = 0; l < outer_contours_of_a_alyer_.size(); l++)
				{
					Path one_outer_contour_of_a_alyer_ = outer_contours_of_a_alyer_[l];
					IntPoint start_pt = one_outer_contour_of_a_alyer_.front(),
						end_pt = one_outer_contour_of_a_alyer_.back();

					if ((one_outer_contour_of_a_alyer_.size() == contour.size()) &&
						(sqrt(pow((spt.X - start_pt.X), 2) + pow((spt.Y - start_pt.Y), 2)) < 1.0e-9*scale) &&
						(sqrt(pow((ept.X - end_pt.X), 2) + pow((ept.Y - end_pt.Y), 2)) < 1.0e-9*scale))
					{
						Paths solution;
						ClipperOffset co;
						co.AddPath(contour, jtMiter, etClosedPolygon);
						co.Execute(solution, (0.0-offset)*scale);

						Path offsetted_outer_contour_;/////????? something wrong 
						if (solution.size() == 1)
						{
							offsetted_outer_contour_ = solution[0];
						}
						lc[k].swap(offsetted_outer_contour_);
					}
				}
			}

			offsetted_layers_integer_[i].swap(lc);
			/////////////////////////////////////

		}
	}
	else exit(1);
}


void GetHexagonalSubarea::OffsetForAllLayerContours(TriMesh::Slicing &slice_of_mesh_)
{
	for (int i = 0; i < slice_of_mesh_.size(); i++)
	{
		double min_x = 1.0e8, max_x = 1.0e-9, min_y = 1.0e8, max_y = 1.0e-9;
		TriMesh::Contours layer_contours = slice_of_mesh_[i];
		getMaxAndMinXYofLayer(layer_contours, min_x, max_x, min_y, max_y);

		//////////////////////////////////////////////////////////////////////////
		//adjust the orientation of each layer contour
		Path clip;
		clip << IntPoint((min_x - 10.0)*scale, (min_y - 10.0)*scale) << IntPoint((min_x - 10.0)*scale, (min_y - 20.0)*scale)
			<< IntPoint((min_x - 20.0)*scale, (min_y - 20.0)*scale) << IntPoint((min_x - 20.0)*scale, (min_y - 10.0)*scale);
		Paths contours_integer = layers_integer_[i];
		Clipper C;
		Paths solution;
		C.AddPaths(contours_integer, ptSubject, true);
		C.AddPath(clip, ptClip, true);
		C.Execute(ctDifference, solution, pftEvenOdd, pftEvenOdd);

		////////////////////////////////////
		//offset each layer's  contours 5.4.2017 
		Paths offseted_a_layer_contours_;
		for (int j = 0; j < solution.size();j++)
		{
			Paths offseted_a_layer_contour_;
			Path p = solution[j];

			if (Orientation(p))// offset
			{
				ClipperOffset co;
				co.AddPath(p, jtMiter, etClosedPolygon);
				co.Execute(offseted_a_layer_contour_, (0.0 - offset)*scale);
			} 
			else
			{
				ClipperOffset co;
				co.AddPath(p, jtMiter, etClosedPolygon);
				co.Execute(offseted_a_layer_contour_,  offset*scale);
			}


			for (int k = 0; k < offseted_a_layer_contour_.size();k++)
				//storage offseted layer contour one by one
			{
				Path offsetted_contour_ = offseted_a_layer_contour_[k];
				offseted_a_layer_contours_.push_back(offsetted_contour_);
			}
		}

		offsetted_layers_integer_.push_back(offseted_a_layer_contours_);
	}
}


void GetHexagonalSubarea::offsetIntervalContoursWithHexagonSubsectors(Paths &contours_integer,Paths &a_layer_solution_intersection_, Paths &layer_solution_difference_with_offseted_subsectors_)
{
	Paths a_layer_offsetted_subsectors_;
	for (int k = 0; k < a_layer_solution_intersection_.size(); k++)
	{
		Path p = a_layer_solution_intersection_[k];
		Paths solution;
		ClipperOffset co;
		co.AddPath(p, jtMiter, etClosedPolygon);
		co.Execute(solution, (0.0 - parallel_line_spacing)*scale);
		if (solution.size() != 0)
		{
			for (int l = 0; l < solution.size(); l++)
			{
				Path pp = solution[l];
				a_layer_offsetted_subsectors_.push_back(pp);
			}
		}
	}
	Clipper c_difference_with_offsetted_subsectors_;
	c_difference_with_offsetted_subsectors_.AddPaths(contours_integer, ptSubject, true);
	c_difference_with_offsetted_subsectors_.AddPaths(a_layer_offsetted_subsectors_, ptClip, true);
	c_difference_with_offsetted_subsectors_.Execute(ctDifference, layer_solution_difference_with_offseted_subsectors_, pftEvenOdd, pftEvenOdd);///get intervals between hexagonal subareas
}


void GetHexagonalSubarea::offsetIntervalContoursWithIntervals(Paths &contours_integer, Paths &a_layer_solution_difference_, Paths &layer_solution_difference_with_offsected_intervals_)
{
	Paths a_layer_offsetted_intervals_;
	for (int k = 0; k < a_layer_solution_difference_.size();k++)
	{
		Path p = a_layer_solution_difference_[k];
		Paths offseted_interval_contopur_;

		if (Orientation(p))// offset
		{
			ClipperOffset co;
			co.AddPath(p, jtMiter, etClosedPolygon);
			co.Execute(offseted_interval_contopur_,  parallel_line_spacing*scale);
		}
		else
		{
			ClipperOffset co;
			co.AddPath(p, jtMiter, etClosedPolygon);
			co.Execute(offseted_interval_contopur_, (0.0 - parallel_line_spacing)*scale);
		}

		if (offseted_interval_contopur_.size()!=0)
		{
			Clipper offseted_interval_contour_intersect_with_layer_contours_;
			Paths solution;
			offseted_interval_contour_intersect_with_layer_contours_.AddPaths(contours_integer, ptSubject, true);
			offseted_interval_contour_intersect_with_layer_contours_.AddPaths(offseted_interval_contopur_, ptClip, true);
			offseted_interval_contour_intersect_with_layer_contours_.Execute(ctIntersection, solution, pftEvenOdd, pftEvenOdd);
			if (solution.size()!=0)
			{
				for (int i = 0; i < solution.size();i++)
				{
					Path solution_p = solution[i];
					layer_solution_difference_with_offsected_intervals_.push_back(solution_p);
				}
			}
		}

	}
}


void GetHexagonalSubarea::volumeOffset(Slice &slice_of_date_manager, TriMesh::Slicing &slice_of_mesh_)
{
	

	if (layer_outer_contours_.size()==layer_contours_without_outer_contours.size()&&
		layer_outer_contours_.size()==slice_of_mesh_.size())
	{
		for (int i = 0; i < layer_outer_contours_.size();i++)
		{
			if (i!=layer_outer_contours_.size()-1)
			{
				Paths One_layer_outer_contours_ = layer_outer_contours_[i];
				Paths next_One_layer_outer_contours_ = layer_outer_contours_[i + 1];

				//////////////////////////////////////////////////////////////////////////
				////6.22.2017
				
				Clipper C;
				Paths solution;
				C.AddPaths(One_layer_outer_contours_, ptSubject, true);
				C.AddPaths(next_One_layer_outer_contours_, ptClip, true);
				C.Execute(ctUnion, solution, pftEvenOdd, pftEvenOdd);
				

				//////////////////////////////////////////////////////////////////////////
				///7.15.2017
	/*			Paths solution=One_layer_outer_contours_;
				
				for (int k = 0; k < next_One_layer_outer_contours_.size();k++)
				{
					Path one_contour_of_next_One_layer_outer_contours_ = next_One_layer_outer_contours_[k];
					Clipper C;
					C.AddPaths(solution, ptSubject, true);
					C.AddPath(one_contour_of_next_One_layer_outer_contours_, ptClip, true);
					C.Execute(ctUnion, solution, pftEvenOdd, pftEvenOdd);
				}
				*/
				//////////////////////////////////////////////////////////////////////////
				/////6.22.2017
		       	/*	
		        Paths one_layer_contours_without_outer_contours_ = layer_contours_without_outer_contours[i];
				for (int j = 0; j < solution.size(); j++)
				{
					Path one_volume_offset_outer_layer = solution[j];
					one_layer_contours_without_outer_contours_.push_back(one_volume_offset_outer_layer);
				}

				volume_offset_layers_integer.push_back(one_layer_contours_without_outer_contours_);
				*/
				//////////////////////////////////////////////////////////////////////////


				//////////////////////////////////////////////////////////////////////////
				/////7.14.2017
	            /*
				if ((solution.size()>One_layer_outer_contours_.size())&&
					(solution.size()>next_One_layer_outer_contours_.size()))
				{
					if (One_layer_outer_contours_.size()>=next_One_layer_outer_contours_.size())
					{
						Paths one_layer_contours_without_outer_contours_ = layer_contours_without_outer_contours[i];
						for (int j = 0; j < One_layer_outer_contours_.size(); j++)
						{
							Path one_outer_contour_ = One_layer_outer_contours_[j];
							one_layer_contours_without_outer_contours_.push_back(one_outer_contour_);
						}
						volume_offset_layers_integer.push_back(one_layer_contours_without_outer_contours_);
					} 
					else
					{
						Paths one_layer_contours_without_outer_contours_ = layer_contours_without_outer_contours[i];
						for (int j = 0; j < next_One_layer_outer_contours_.size(); j++)
						{
							Path one_outer_contour_ = next_One_layer_outer_contours_[j];
							one_layer_contours_without_outer_contours_.push_back(one_outer_contour_);
						}
						volume_offset_layers_integer.push_back(one_layer_contours_without_outer_contours_);
					}
				} 
				else
				{
					Paths one_layer_contours_without_outer_contours_ = layer_contours_without_outer_contours[i];
					for (int j = 0; j < solution.size(); j++)
					{
						Path one_volume_offset_outer_layer = solution[j];
						one_layer_contours_without_outer_contours_.push_back(one_volume_offset_outer_layer);
					}

					volume_offset_layers_integer.push_back(one_layer_contours_without_outer_contours_);
				}
			    */

				//////////////////////////////////////////////////////////////////////////
				///7.29.2017
				Paths one_layer_contours_without_outer_contours_ = layer_contours_without_outer_contours[i],
					  next_one_layer_contours_without_outer_contours_ = layer_contours_without_outer_contours[i+1];

				if (one_layer_contours_without_outer_contours_.size()!=0)
				{
					for (int j = 0; j < solution.size(); j++)
					{
						Path one_volume_offset_outer_layer = solution[j];
						one_layer_contours_without_outer_contours_.push_back(one_volume_offset_outer_layer);
					}
					volume_offset_layers_integer.push_back(one_layer_contours_without_outer_contours_);
				} 
				else
				{
					for (int j = 0; j < solution.size(); j++)
					{
						Path one_volume_offset_outer_layer = solution[j];
						next_one_layer_contours_without_outer_contours_.push_back(one_volume_offset_outer_layer);
					}
					volume_offset_layers_integer.push_back(next_one_layer_contours_without_outer_contours_);
				}
	
				//////////////////////////////////////////////////////////////////////////
				//6.22.2017
				//volume_offset_layers_integer.push_back(one_layer_contours_without_outer_contours_);
			} 
			else
			{
				Paths One_layer_outer_contours_ = layer_outer_contours_[i];
				Paths one_layer_contours_without_outer_contours_ = layer_contours_without_outer_contours[i];
				for (int j = 0; j < One_layer_outer_contours_.size(); j++)
				{
					Path one_outer_contour_ = One_layer_outer_contours_[j];
					one_layer_contours_without_outer_contours_.push_back(one_outer_contour_);
				}
				volume_offset_layers_integer.push_back(one_layer_contours_without_outer_contours_);
			}
 
		}

		//////////////////////////////////////////////////////////////////////////
		///release memory
		layer_outer_contours_.swap(std::vector<Paths>());
		layer_contours_without_outer_contours.swap(std::vector<Paths>());//6.22.2017
	} 
	else exit(1);

}


void GetHexagonalSubarea::transformVolumeOffsetLayersDataTypeToDouble(Slice &slice_of_date_manager, std::vector<Paths> &volume_offset_layers_integer, TriMesh::Slicing &slice_of_mesh_)
{
	TriMesh::Slicing temp_volume_offset_slice_of_mesh_;

	//////////////////////////////////////////////////////////////////////////
	//transform volume_offset_layers_integer into mesh slicing data
	for (int i = 0; i < volume_offset_layers_integer.size(); i++)
	{

		double z = (slice_of_date_manager.model_min_z + (i + 1)*(slice_of_date_manager.thickness));

		Paths one_volume_offset_contours_integer_ = volume_offset_layers_integer[i];
		TriMesh::Contours one_volume_offset_contours_;
		for (int j = 0; j < one_volume_offset_contours_integer_.size(); j++)
		{
			Path one_volume_offset_contour_integer_ = one_volume_offset_contours_integer_[j];
			TriMesh::Polylines one_volume_offset_contour_;
			for (int k = 0; k < one_volume_offset_contour_integer_.size(); k++)
			{
				IntPoint intpt = one_volume_offset_contour_integer_[k];
				TriMesh::Point pt;
				pt[0] = (double)intpt.X / scale;
				pt[1] = (double)intpt.Y / scale;
				pt[2] = z;
				one_volume_offset_contour_.push_back(pt);
			}
			one_volume_offset_contours_.push_back(one_volume_offset_contour_);
		}
		temp_volume_offset_slice_of_mesh_.push_back(one_volume_offset_contours_);
	}
	//slice_of_mesh_.swap(temp_volume_offset_slice_of_mesh_);
	slice_of_mesh_ = temp_volume_offset_slice_of_mesh_;
}
