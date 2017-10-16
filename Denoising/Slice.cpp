#include "slice.h"


Slice::Slice() :thickness(10.0), layers_number(0)
{

}


Slice::~Slice()
{
}

/*double Slice::Dot(TriMesh::Normal &a, TriMesh::Point &b)
{
double sum = 0.0;

for (int i=0; i < 3; i++)
{
sum += a[i] * b[i];
}

return sum;
}*/

void Slice::GetMaxMinZ(TriMesh &mesh, double &max_z, double &min_z)
{
	for (TriMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); v_it++)
	{
		TriMesh::Point p = mesh.point(*v_it);
		if (p[2] >= max_z)
		{
			max_z = p[2];
		}
		if (p[2] <= min_z)
		{
			min_z = p[2];
		}
	}
}

double Slice::GetLayersNumber(TriMesh &mesh, double &thickness, double &max_z, double &min_z)//get the number of layers
{
	//return layers_number = (max_z - min_z) / thickness;
	return layers_number = (max_z - min_z-0.00001) / thickness;//10.2.2017
}

bool Slice::IsTheEdgeIntersectWithTheLayer(TriMesh &mesh, TriMesh::EdgeHandle eh, double &height)//judge weather the edge is intersected  the layer or not.
{
	TriMesh::HalfedgeHandle heh = mesh.halfedge_handle(eh, 0);//get one halfedgehandle from edgehandle
	TriMesh::VertexHandle to_vertex_h = mesh.to_vertex_handle(heh);//get to_vertex_handle from halfedgehandle
	TriMesh::VertexHandle from_vertex_h = mesh.from_vertex_handle(heh);//
	TriMesh::Point to_vertex = mesh.point(to_vertex_h);
	TriMesh::Point from_vertex = mesh.point(from_vertex_h);

	if ((to_vertex[2] <= height) && (from_vertex[2] >= height) ||
		(to_vertex[2] >= height) && (from_vertex[2] <= height))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Slice::GetTheIntersectionPoints(TriMesh &mesh, TriMesh::HalfedgeHandle heh, double &height, std::vector<TriMesh::Point> &inter_points)//get the intersection point between the halfedge and the layer
{
	TriMesh::VertexHandle to_vertex_h = mesh.to_vertex_handle(heh);//get to_vertex_handle from halfedgehandle
	TriMesh::VertexHandle from_vertex_h = mesh.from_vertex_handle(heh);//
	TriMesh::Point to_vertex = mesh.point(to_vertex_h);
	TriMesh::Point from_vertex = mesh.point(from_vertex_h);

	double num, denom, t;
	TriMesh::Normal N(0, 0, 1);
	num = height - dot(N, from_vertex);
	TriMesh::Point rq = to_vertex - from_vertex;
	denom = dot(N, rq);

	if (denom == 0.0)
	{
		if (num == 0.0)
		{
			inter_points.resize(2);
			inter_points[0] = from_vertex;
			inter_points[1] = to_vertex;
		}
	}
	else
	{
		t = num / denom;
		if ((t >= 0.0) && (t <= 1.0))
		{
			inter_points.resize(1);
			inter_points[0] = from_vertex + t*rq;
		}

	}

}

void Slice::GetContour(TriMesh &mesh, std::vector<int> &edges_status, TriMesh::HalfedgeHandle first_heh, double &height, TriMesh::Polylines &contour)//get contour from a intersected halfedge.
{
	TriMesh::HalfedgeHandle first_heh_opposite = mesh.opposite_halfedge_handle(first_heh);//
	TriMesh::HalfedgeHandle previous_intersected_heh = first_heh;//indicate the previous intersected halfedge handle

	std::vector<TriMesh::Point> first_inter_points;//first intersected points 
	GetTheIntersectionPoints(mesh, first_heh, height, first_inter_points);

	int n = first_inter_points.size();
	for (int i = 0; i < n; i++)
	{
		contour.push_back(first_inter_points[i]);//get the first intersected points included in the contour
	}

	TriMesh::HalfedgeHandle heh_next = mesh.next_halfedge_handle(first_heh);//get next halfedge handle of the first halfedge handle

	do{
		TriMesh::EdgeHandle next_eh = mesh.edge_handle(heh_next);
		int next_edge_status = edges_status[next_eh.idx()];
		if (next_edge_status == 0)
		{
			if (IsTheEdgeIntersectWithTheLayer(mesh, next_eh, height))
			{
				edges_status[next_eh.idx()] = 1;// set edge status 1(intersect)
				previous_intersected_heh = heh_next;//set the previous halfedge handle
				std::vector<TriMesh::Point> intersected_points;
				GetTheIntersectionPoints(mesh, heh_next, height, intersected_points);
				int total = contour.size();//get the total number of the contour 
				int e_total = intersected_points.size();//get the number of the intersected_points
				for (int i = 0; i < e_total; i++)
				{
					if (total >= 2)
					{
						if (((intersected_points[i] - contour[total - 1]).length() > 1.0e-8)
							&& ((intersected_points[i] - contour[total - 2]).length() > 1.0e-8))
						{
							contour.push_back(intersected_points[i]);
						}
					}
					else if (total == 1)
					{
						if ((intersected_points[i] - contour[total - 1]).length() > 1.0e-8)
						{
							contour.push_back(intersected_points[i]);
						}
					}
				}
			}
			else
			{
				edges_status[next_eh.idx()] = 2;//2(don't intersect)
				heh_next = mesh.next_halfedge_handle(heh_next);
			}
		}

		else if (next_edge_status == 1)
		{
			if (heh_next == first_heh_opposite)
			{
				break;
			}
			else if (heh_next != first_heh_opposite)
			{
				TriMesh::HalfedgeHandle previous_opposite_heh = mesh.opposite_halfedge_handle(previous_intersected_heh);
				heh_next = mesh.next_halfedge_handle(previous_opposite_heh);
			}
		}

		else if (next_edge_status == 2)
		{
			heh_next = mesh.next_halfedge_handle(heh_next);
		}
	} while (true);

}

void Slice::SliceTheModel(TriMesh &mesh)
{
	//double max_z = 1.0e-9;
	//double min_z = 1.0e8;
	GetMaxMinZ(mesh, model_max_z, model_min_z);

	layers_number = GetLayersNumber(mesh, thickness, model_max_z, model_min_z);
	//slicing.resize(layers_number);
	//for (int i = 0; i < (layers_number + 1); i++)//get intersected contours layer by layer
	for (int i = 0; i < (layers_number + 1); i++)//get intersected contours layer by layer//10.2.2017
	{
		double z_height = model_min_z +0.00001+ i*thickness;
		TriMesh::Contours z_contours;//intersected contours of each layer
		std::vector<int> edges_status((int)mesh.n_edges(), 0);//set all edges' status 0(not checked)

		for (TriMesh::EdgeIter e_it = mesh.edges_begin(); e_it != mesh.edges_end(); e_it++)//check every edge
		{
			TriMesh::EdgeHandle eh = mesh.edge_handle(e_it->idx());
			if (edges_status[e_it->idx()] == 0)
			{
				TriMesh::HalfedgeHandle heh_of_eh = mesh.halfedge_handle(eh, 0);
				TriMesh::VertexHandle to_vertex_eh = mesh.to_vertex_handle(heh_of_eh);//get to_vertex_handle from halfedgehandle
				TriMesh::VertexHandle from_vertex_eh = mesh.from_vertex_handle(heh_of_eh);//
				TriMesh::Point to_vertex = mesh.point(to_vertex_eh);
				TriMesh::Point from_vertex = mesh.point(from_vertex_eh);

				if ((to_vertex[2] >= z_height&&from_vertex[2] <= z_height) ||
					(to_vertex[2] <= z_height&&from_vertex[2] >= z_height))//check weather the edge is intersected or not
				{
					//if (edges_status[e_it->idx()] == 0)
					//{
					if (IsTheEdgeIntersectWithTheLayer(mesh, eh, z_height))
					{
						edges_status[e_it->idx()] = 1;// 1(intersect)
						TriMesh::HalfedgeHandle first_heh = mesh.halfedge_handle(eh, 0);//first intersected halfedge

						TriMesh::Polylines contour;
						GetContour(mesh, edges_status, first_heh, z_height, contour);//get one intersected contour through the first intersected halfedge
						z_contours.push_back(contour);//intersected contours of each layer

					}
					else
					{
						edges_status[e_it->idx()] = 2;// 2(don't intersect)
					}
					//}
				}
				else
					edges_status[e_it->idx()] = 2;
			}
		}
		slicing.push_back(z_contours);

	}

	//mesh.set_slicing(slicing);
}


void Slice::getFacetMaxAndMinZ(TriMesh &mesh, TriMesh::FaceIter f_it, double &maximal_z_, double &minimal_z_)
{
	for (TriMesh::FaceVertexIter fv_it = mesh.fv_iter(*f_it); fv_it.is_valid(); fv_it++)
	{
		TriMesh::Point  p= mesh.point(*fv_it);
        if (p[2]>=maximal_z_)
        {
			maximal_z_ = p[2];
        } 
		if (p[2] <= minimal_z_)
        {
			minimal_z_ = p[2];
        }
	}
}


void Slice::sliceTheModelWithThinkingAboutTrianglesBetween2Layers_UnionOneByOne(TriMesh &mesh, std::vector<Paths> &volume_offset_layers_integer,int &scale)
{
	mesh.request_face_normals();
	//mesh.request_vertex_normals();
	mesh.update_normals();
	for (int i = 0; i <mesh.mesh_layers_.size(); i++)
	{
		double layer_z = mesh.mesh_layers_[i],
			   next_layer_z=layer_z+thickness;

		//Clipper triangle_with_the_two_layers_;
		//Paths solution;
		

		/*if (i!=mesh.mesh_layers_.size()-1)
		{*/
			for (TriMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); f_it++)
			{
				TriMesh::Normal facet_normal_ = mesh.normal(f_it);
				TriMesh::Normal z_direction;
				z_direction[0] = 0.0;
				z_direction[1] = 0.0;
				z_direction[2] = 1.0;
				double dot_of_facet_normal_z_ = fabs(facet_normal_ | z_direction);

				Clipper triangle_with_the_two_layers_;
				Paths solution;
				Paths volume_offste_layer_ = volume_offset_layers_integer[i];//layer integer
				//if (dot_of_facet_normal_z_>1.0e-9)
				//judge if the facet is parallel to Z
				//{
					triangle_with_the_two_layers_.AddPaths(volume_offste_layer_, ptSubject, true);
					Path projection_intersection_polygon_;

					double facet_max_z_ = 1.0e-9,
						facet_min_z_ = 1.0e8;
					getFacetMaxAndMinZ(mesh, f_it, facet_max_z_, facet_min_z_);
					if (((layer_z <= facet_max_z_) && (facet_max_z_ <= next_layer_z)) &&
						((layer_z <= facet_min_z_) && (facet_min_z_ <= next_layer_z)))
						// the whole facet is between the 2layers 
					{
						for (TriMesh::FaceVertexIter fv_it = mesh.fv_iter(*f_it); fv_it.is_valid(); fv_it++)
						{
							TriMesh::Point  p = mesh.point(*fv_it);
							projection_intersection_polygon_ << IntPoint(scale*p[0], scale*p[1]);
						}
					}
					else if ((facet_max_z_ <= layer_z && facet_min_z_ < layer_z) ||
						(facet_min_z_ >= next_layer_z && facet_max_z_ > next_layer_z))
						// the whole facet is above the up layer, or the whole facet  is under the lower layer
						// without intersection
					{
						continue;
					}
					else
						// the facet is sort of intersection with the two layers
					{
						getIntersectionIntergerPointOfOneFacetWith2Layers(mesh, f_it, layer_z, next_layer_z, scale, projection_intersection_polygon_);
					}

					triangle_with_the_two_layers_.AddPath(projection_intersection_polygon_, ptClip, true);
					triangle_with_the_two_layers_.Execute(ctUnion, solution, pftEvenOdd, pftEvenOdd);
					volume_offset_layers_integer[i] = solution;
				//}
			}
		//} 
		//else//the top layer
		//{
		//	for (TriMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); f_it++)
		//	{
		//		TriMesh::Normal facet_normal_ = mesh.normal(f_it);
		//		TriMesh::Normal z_direction;
		//		z_direction[0] = 0.0;
		//		z_direction[1] = 0.0;
		//		z_direction[2] = 1.0;
		//		double dot_of_facet_normal_z_ = facet_normal_ | z_direction;

		//		Paths volume_offste_layer_ = volume_offset_layers_integer[i];//layer integer
		//		if (dot_of_facet_normal_z_ != 0.0)
		//			//judge if the facet is parallel to Z
		//		{
		//			triangle_with_the_two_layers_.AddPaths(volume_offste_layer_, ptSubject, true);
		//			Path projection_intersection_polygon_;
		//			double facet_max_z_ = 1.0e-9,
		//				   facet_min_z_ = 1.0e8;
		//			getFacetMaxAndMinZ(mesh, f_it, facet_max_z_, facet_min_z_);

		//			if (facet_max_z_ < layer_z )
		//			//without intersection
		//			{
		//			} 
		//			else if (facet_min_z_>=layer_z)
		//			// the whole facet is above the top layers 
		//			{
		//				for (TriMesh::FaceVertexIter fv_it = mesh.fv_iter(*f_it); fv_it.is_valid(); fv_it++)
		//				{
		//					TriMesh::Point  p = mesh.point(*fv_it);
		//					projection_intersection_polygon_ << IntPoint(scale*p[0], scale*p[1]);
		//				}
		//			}
		//			else
		//			{
		//				getIntersectionIntergerPointOfOneFacetWith2Layers(mesh, f_it, layer_z, next_layer_z, scale, projection_intersection_polygon_);
		//			}

		//			triangle_with_the_two_layers_.AddPath(projection_intersection_polygon_, ptClip, true);
		//			triangle_with_the_two_layers_.Execute(ctUnion, solution, pftEvenOdd, pftEvenOdd);
		//			volume_offset_layers_integer[i] = solution;
		//		}
		//	}
		//}

			Paths temp_volume_offset_one_layer_integer_ ;
			for (auto it_one_contour_volume_offset_one_layer_integer_ = volume_offset_layers_integer[i].begin();
				it_one_contour_volume_offset_one_layer_integer_ != volume_offset_layers_integer[i].end();
				it_one_contour_volume_offset_one_layer_integer_++)
			{
				Path volume_offset_one_layer_integer_ = *it_one_contour_volume_offset_one_layer_integer_;
				/*	if (volume_offset_one_layer_integer_.size()!=3 &&
						volume_offset_one_layer_integer_.size()!=4 &&
						volume_offset_one_layer_integer_.size() != 5 &&
						volume_offset_one_layer_integer_.size() != 6 &&
						volume_offset_one_layer_integer_.size() != 7 &&
						volume_offset_one_layer_integer_.size() != 9 &&
						volume_offset_one_layer_integer_.size() != 12  )*/
				//if (volume_offset_one_layer_integer_.size()>30)
						{
						temp_volume_offset_one_layer_integer_.push_back(volume_offset_one_layer_integer_);
						}
				//if (volume_offset_one_layer_integer_.size() == 3)
				//{
				//	TriMesh::Point p1, p2, p3;
				//	p1[0] = double(volume_offset_one_layer_integer_[0].X / scale);
				//	p1[1] = double(volume_offset_one_layer_integer_[0].Y / scale);
				//	p1[2] = 0.0;
				//	p2[0] = double(volume_offset_one_layer_integer_[1].X / scale);
				//	p2[1] = double(volume_offset_one_layer_integer_[1].Y / scale);
				//	p2[2] = 0.0;
				//	p3[0] = double(volume_offset_one_layer_integer_[2].X / scale);
				//	p3[1] = double(volume_offset_one_layer_integer_[2].Y / scale);
				//	p3[2] = 0.0;
				//	TriMesh::Point edge1 = p2 - p1;
				//	TriMesh::Point edge2 = p2 - p3;
				//	double S = 0.5 * (edge1 % edge2).length();
				//	if (S>1.0e-9)
				//	{
				//		temp_volume_offset_one_layer_integer_.push_back(volume_offset_one_layer_integer_);
				//	}
				//} 
				//else if (volume_offset_one_layer_integer_.size() == 4)
				//{
				//	TriMesh::Point p1, p2, p3,p4;
				//	p1[0] = double(volume_offset_one_layer_integer_[0].X / scale);
				//	p1[1] = double(volume_offset_one_layer_integer_[0].Y / scale);
				//	p1[2] = 0.0;
				//	p2[0] = double(volume_offset_one_layer_integer_[1].X / scale);
				//	p2[1] = double(volume_offset_one_layer_integer_[1].Y / scale);
				//	p2[2] = 0.0;
				//	p3[0] = double(volume_offset_one_layer_integer_[2].X / scale);
				//	p3[1] = double(volume_offset_one_layer_integer_[2].Y / scale);
				//	p3[2] = 0.0;
				//	p4[0] = double(volume_offset_one_layer_integer_[3].X / scale);
				//	p4[1] = double(volume_offset_one_layer_integer_[3].Y / scale);
				//	p4[2] = 0.0;
				//	TriMesh::Point edge1 = p2 - p1;
				//	TriMesh::Point edge2 = p2 - p3;
				//	TriMesh::Point edge3 = p4 - p1;
				//	TriMesh::Point edge4 = p4 - p3;
				//	double S1 = 0.5 * (edge1 % edge2).length();
				//	double S2 = 0.5 * (edge3 % edge4).length();
				//	double S = S1 + S2;
				//	if (S > 1.0e-9)
				//	{
				//		temp_volume_offset_one_layer_integer_.push_back(volume_offset_one_layer_integer_);
				//	}
				//}
				//else if (volume_offset_one_layer_integer_.size() == 6)
				//{
				//	TriMesh::Point p1, p2, p3, p4,p5,p6;
				//	p1[0] = double(volume_offset_one_layer_integer_[0].X / scale);
				//	p1[1] = double(volume_offset_one_layer_integer_[0].Y / scale);
				//	p1[2] = 0.0;
				//	p2[0] = double(volume_offset_one_layer_integer_[1].X / scale);
				//	p2[1] = double(volume_offset_one_layer_integer_[1].Y / scale);
				//	p2[2] = 0.0;
				//	p3[0] = double(volume_offset_one_layer_integer_[2].X / scale);
				//	p3[1] = double(volume_offset_one_layer_integer_[2].Y / scale);
				//	p3[2] = 0.0;
				//	p4[0] = double(volume_offset_one_layer_integer_[3].X / scale);
				//	p4[1] = double(volume_offset_one_layer_integer_[3].Y / scale);
				//	p4[2] = 0.0;
				//	p5[0] = double(volume_offset_one_layer_integer_[4].X / scale);
				//	p5[1] = double(volume_offset_one_layer_integer_[4].Y / scale);
				//	p5[2] = 0.0;
				//	p6[0] = double(volume_offset_one_layer_integer_[5].X / scale);
				//	p6[1] = double(volume_offset_one_layer_integer_[5].Y / scale);
				//	p6[2] = 0.0;
				//	TriMesh::Point edge1 = p2 - p1;
				//	TriMesh::Point edge2 = p2 - p3;
				//	TriMesh::Point edge3 = p4 - p5;
				//	TriMesh::Point edge4 = p4 - p6;
				//	double S1 = 0.5 * (edge1 % edge2).length();
				//	double S2 = 0.5 * (edge3 % edge4).length();
				//	double S = S1 + S2;
				//	if (S > 1.0e-9)
				//	{
				//		temp_volume_offset_one_layer_integer_.push_back(volume_offset_one_layer_integer_);
				//	}
				//}
				//else 
				//{
				//	temp_volume_offset_one_layer_integer_.push_back(volume_offset_one_layer_integer_);
				//}
			}
			volume_offset_layers_integer[i] = temp_volume_offset_one_layer_integer_;
	}
}


void Slice::getIntersectionIntergerPointOfOneFacetWith2Layers(TriMesh &mesh, TriMesh::FaceIter &f_it, double &layer_z,double &next_layer_z,int &scale, Path &projection_intersection_polygon_)
{ 
	for (TriMesh::FaceHalfedgeIter fhe_it = mesh.fh_iter(*f_it); fhe_it.is_valid(); fhe_it++)
	{
		TriMesh::HalfedgeHandle half_edge_handle = *fhe_it;
		TriMesh::VertexHandle to_vertex_h = mesh.to_vertex_handle(fhe_it);//get to_vertex_handle from halfedgehandle
		TriMesh::VertexHandle from_vertex_h = mesh.from_vertex_handle(fhe_it);//
		TriMesh::Point to_vertex = mesh.point(to_vertex_h);
		TriMesh::Point from_vertex = mesh.point(from_vertex_h);

		if (((to_vertex[2]>=next_layer_z) && (from_vertex[2]>next_layer_z))||
			((to_vertex[2]<layer_z) && (from_vertex[2]<=layer_z)))
			//the whole edge is above the up layer, or the whole facet  is under the lower layer
			//without intersection
		{
		} 
		else if (((layer_z<to_vertex[2]) && (to_vertex[2]<next_layer_z)) &&
			     ((layer_z<from_vertex[2]) && (from_vertex[2]<next_layer_z)))
				 // the whole edge is between the 2layers
		{
			//////////////////////////////////////////////////////////////////////////
			//store edge's intersected points to the intersected polygon
			IntPoint p_from, p_to;
			p_from.X = scale*from_vertex[0];
			p_from.Y = scale*from_vertex[1];
			p_to.X = scale*to_vertex[0];
			p_to.Y = scale*to_vertex[1];

			if (projection_intersection_polygon_.size()==0)
			{
				projection_intersection_polygon_ << IntPoint(p_from.X, p_from.Y)
					<< IntPoint(p_to.X, p_to.Y);
			}
			else 
			{
				IntPoint first_pt = projection_intersection_polygon_.front();
				IntPoint last_pt = projection_intersection_polygon_.back();

				if (sqrt(pow((last_pt.X - p_from.X), 2) + pow((last_pt.Y - p_from.Y), 2)) < 1.0e-9*scale)
				{
					
				}
				else
				{
					projection_intersection_polygon_ << IntPoint(p_from.X, p_from.Y);
				}

				if (sqrt(pow((first_pt.X - p_to.X), 2) + pow((first_pt.Y - p_to.Y), 2)) < 1.0e-9*scale)
				{
					
				}
				else
				{
					projection_intersection_polygon_ << IntPoint(p_to.X, p_to.Y);
				}
			}
		} 
		else
		//  the edge is sort of intersection with the two layers
		{
			std::vector<TriMesh::Point> intersection_points;
			if (isTheEdgeIntersectedwithThelayer(from_vertex[2],to_vertex[2],layer_z)&&
				(!isTheEdgeIntersectedwithThelayer(from_vertex[2], to_vertex[2], next_layer_z)))
			{
				getEdgesIntersectionPointsWithOneLayer(to_vertex,from_vertex, layer_z, next_layer_z, intersection_points);
			}

			else if (isTheEdgeIntersectedwithThelayer(from_vertex[2], to_vertex[2], next_layer_z) &&
				(!isTheEdgeIntersectedwithThelayer(from_vertex[2], to_vertex[2], layer_z)))
			{
				getEdgesIntersectionPointsWithOneLayer(to_vertex, from_vertex, next_layer_z, layer_z, intersection_points);
			}
			else if (isTheEdgeIntersectedwithThelayer(from_vertex[2], to_vertex[2], layer_z) &&
				    isTheEdgeIntersectedwithThelayer(from_vertex[2], to_vertex[2], next_layer_z))
			{
				getEdgesIntersectionPointsWithTwoLayers(to_vertex, from_vertex, layer_z, next_layer_z, intersection_points);
			}

			//////////////////////////////////////////////////////////////////////////
			//store edge's intersected points to the intersected polygon
			IntPoint inter_1_, inter_2_;
			inter_1_.X = scale*intersection_points[0][0];
			inter_1_.Y = scale*intersection_points[0][1];
			inter_2_.X = scale*intersection_points[1][0];
			inter_2_.Y = scale*intersection_points[1][1];
			if (projection_intersection_polygon_.size() == 0)
			{
				projection_intersection_polygon_ << IntPoint(inter_1_.X, inter_1_.Y)
					<< IntPoint(inter_2_.X, inter_2_.Y);
			}
			else
			{
				IntPoint first_pt = projection_intersection_polygon_.front();
				IntPoint last_pt = projection_intersection_polygon_.back();

				if (sqrt(pow((last_pt.X - inter_1_.X), 2) + pow((last_pt.Y - inter_1_.Y), 2)) < 1.0e-9*scale)
				{
					
				}
				else
				{
					projection_intersection_polygon_ << IntPoint(inter_1_.X, inter_1_.Y);
				}

				if (sqrt(pow((first_pt.X - inter_2_.X), 2) + pow((first_pt.Y - inter_2_.Y), 2)) < 1.0e-9*scale)
				{
					
				}
				else
				{
					projection_intersection_polygon_ << IntPoint(inter_2_.X, inter_2_.Y);
				}
			}
		}
	}
}


bool Slice::isTheEdgeIntersectedwithThelayer( double &frome_z, double &to_z, double &height)
{
	if ((to_z <= height) && (frome_z >= height) ||
		(to_z >= height) && (frome_z <= height))
	{
		return true;
	}
	else
	{
		return false;
	}
}


void Slice::getEdgesIntersectionPointsWithOneLayer(TriMesh::Point &to_vertex, TriMesh::Point &from_vertex, double &height_1_, double &height_2_, std::vector<TriMesh::Point> &intersection_points)
{
	double num, denom, t;
	TriMesh::Normal N(0, 0, 1);
	num = height_1_ - dot(N, from_vertex);
	TriMesh::Point rq = to_vertex - from_vertex;
	denom = dot(N, rq);

	intersection_points.resize(2);
	if (denom == 0.0)
		//the edge is in the layer
	{
		if (num == 0.0)
		{
			intersection_points[0] = from_vertex;
			intersection_points[1] = to_vertex;
		}
	}
	else
		//the edge is intersected with the layer
	{
		TriMesh::Point inter_point;
		t = num / denom;
		if ((t >= 0.0) && (t <= 1.0))
		{
			inter_point = from_vertex + t*rq;
		}
		
		if (height_1_<height_2_)
		{
			if ((height_1_ <= from_vertex[2]) && (from_vertex[2] <= height_2_))
			{
				intersection_points[0] = from_vertex;
				intersection_points[1] = inter_point;
			} 
			else
			{
				intersection_points[0] = inter_point;
				intersection_points[1] = to_vertex;
			}
		} 
		else
		{
			if ((height_2_<from_vertex[2]) && (from_vertex[2]<height_1_))
			{
				intersection_points[0] = from_vertex;
				intersection_points[1] = inter_point;
			}
			else
			{
				intersection_points[0] = inter_point;
				intersection_points[1] = to_vertex;
			}
		}
	}
}


void Slice::getEdgesIntersectionPointsWithTwoLayers(TriMesh::Point &to_vertex, TriMesh::Point &from_vertex, double &height_1_, double &height_2_, std::vector<TriMesh::Point> &intersection_points)
{
	intersection_points.resize(2);
	TriMesh::Point intersection_point_1_, intersection_point_2_;
	getEdgesIntersectionPointWithOneLayer(to_vertex, from_vertex, height_1_, intersection_point_1_);
	getEdgesIntersectionPointWithOneLayer(to_vertex, from_vertex, height_2_, intersection_point_2_);

	TriMesh::Normal n1(intersection_point_1_[0] - from_vertex[0], intersection_point_1_[1] - from_vertex[1], intersection_point_1_[2] - from_vertex[2]),
		            n2(intersection_point_2_[0] - from_vertex[0], intersection_point_2_[1] - from_vertex[1], intersection_point_2_[2] - from_vertex[2]);

	if (n1.length()<n2.length())
		//judge which intersected point is nearer to the from point.
	{
		intersection_points[0] = intersection_point_1_;
		intersection_points[1] = intersection_point_2_;
	} 
	else
	{
		intersection_points[1] = intersection_point_2_;
		intersection_points[0] = intersection_point_1_;		
	}
	
}


void Slice::getEdgesIntersectionPointWithOneLayer(TriMesh::Point &to_vertex, TriMesh::Point &from_vertex, double &height, TriMesh::Point &intersection_point)
{
	double num, denom, t;
	TriMesh::Normal N(0, 0, 1);
	num = height - dot(N, from_vertex);
	TriMesh::Point rq = to_vertex - from_vertex;
	denom = dot(N, rq);

	t = num / denom;
	if ((t >= 0.0) && (t <= 1.0))
	{
		intersection_point = from_vertex + t*rq;
	}

}


void Slice::sliceTheModelWithThinkingAboutTrianglesBetween2Layers_AllShapesUnionFirst(TriMesh &mesh, std::vector<Paths> &volume_offset_layers_integer, int &scale)
{
	mesh.request_face_normals();
	mesh.update_normals();
	for (int i = 0; i <mesh.mesh_layers_.size(); i++)
	{
		double layer_z = mesh.mesh_layers_[i],
			next_layer_z = layer_z + thickness;

		Paths volume_offste_layer_ = volume_offset_layers_integer[i];//layer integer
		Paths all_intersected_shapes_between_the_two_layers_;
		for (TriMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); f_it++)
		{
			//TriMesh::Normal facet_normal_ = mesh.normal(f_it);
			//TriMesh::Normal z_direction;
			//z_direction[0] = 0.0;
			//z_direction[1] = 0.0;
			//z_direction[2] = 1.0;
			//double dot_of_facet_normal_z_ = fabs(facet_normal_ | z_direction);			
			//if (dot_of_facet_normal_z_>1.0e-9)
			//	//judge if the facet is parallel to Z
			//{
				Path projection_intersection_polygon_;

				double facet_max_z_ = 1.0e-9,
					facet_min_z_ = 1.0e8;
				getFacetMaxAndMinZ(mesh, f_it, facet_max_z_, facet_min_z_);
				if (((layer_z <= facet_max_z_) && (facet_max_z_ <= next_layer_z)) &&
					((layer_z <= facet_min_z_) && (facet_min_z_ <= next_layer_z)))
					// the whole facet is between the 2layers 
				{
					for (TriMesh::FaceVertexIter fv_it = mesh.fv_iter(*f_it); fv_it.is_valid(); fv_it++)
					{
						TriMesh::Point  p = mesh.point(*fv_it);
						projection_intersection_polygon_ << IntPoint(scale*p[0], scale*p[1]);
					}
				}
				else if ((facet_max_z_ <= layer_z && facet_min_z_ < layer_z) ||
					(facet_min_z_ >= next_layer_z && facet_max_z_ > next_layer_z))
					// the whole facet is above the up layer, or the whole facet  is under the lower layer
					// without intersection
				{
					continue;
				}
				else
					// the facet is sort of intersection with the two layers
				{
					getIntersectionIntergerPointOfOneFacetWith2Layers(mesh, f_it, layer_z, next_layer_z, scale, projection_intersection_polygon_);
				}
				all_intersected_shapes_between_the_two_layers_.push_back(projection_intersection_polygon_);
			//}
		}
		Clipper triangle_with_the_two_layers_;
	    Paths solution;

		triangle_with_the_two_layers_.AddPaths(volume_offste_layer_, ptSubject, true);
		triangle_with_the_two_layers_.AddPaths(all_intersected_shapes_between_the_two_layers_, ptClip, true);
		triangle_with_the_two_layers_.Execute(ctUnion, solution, pftEvenOdd, pftEvenOdd);
		volume_offset_layers_integer[i] = solution;

		//Paths temp_volume_offset_one_layer_integer_;
		//for (auto it_one_contour_volume_offset_one_layer_integer_ = volume_offset_layers_integer[i].begin();
		//	it_one_contour_volume_offset_one_layer_integer_ != volume_offset_layers_integer[i].end();
		//	it_one_contour_volume_offset_one_layer_integer_++)
		//{
		//	Path volume_offset_one_layer_integer_ = *it_one_contour_volume_offset_one_layer_integer_;
		//	if (volume_offset_one_layer_integer_.size() != 3 &&
		//		volume_offset_one_layer_integer_.size() != 4 &&
		//		volume_offset_one_layer_integer_.size() != 5 &&
		//		volume_offset_one_layer_integer_.size() != 6 &&
		//		volume_offset_one_layer_integer_.size() != 7 &&
		//		volume_offset_one_layer_integer_.size() != 9 &&
		//		volume_offset_one_layer_integer_.size() != 12)
		//		//if (volume_offset_one_layer_integer_.size()>30)
		//	{
		//		temp_volume_offset_one_layer_integer_.push_back(volume_offset_one_layer_integer_);
		//	}
		//}
		//volume_offset_layers_integer[i] = temp_volume_offset_one_layer_integer_;
	}
}

