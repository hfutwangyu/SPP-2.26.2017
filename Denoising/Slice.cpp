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
	return layers_number = (max_z - min_z) / thickness;
}

bool Slice::IsIntersect(TriMesh &mesh, TriMesh::EdgeHandle eh, double &height)//judge weather the edge is intersected  the layer or not.
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
			if (IsIntersect(mesh, next_eh, height))
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
	double max_z = 1.0e-9;
	double min_z = 1.0e8;
	GetMaxMinZ(mesh, max_z, min_z);

	layers_number = GetLayersNumber(mesh, thickness, max_z, min_z);
	//slicing.resize(layers_number);
	for (int i = 1; i < (layers_number + 1); i++)//get intersected contours layer by layer
	{
		double z_height = min_z + i*thickness;
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
					if (IsIntersect(mesh, eh, z_height))
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



