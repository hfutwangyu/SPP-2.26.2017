
#include "meshexaminer.h"
#include "glwrapper.h"
#include <QFileDialog>
#include <QString>

MeshExaminer::MeshExaminer()
:draw_points_status_(false), draw_edges_status_(false), draw_faces_status_(true), draw_layers_status_(false), 
draw_hexagons_status_(false),draw_intervals_status_(false),draw_parallel_hatches_status_(false), show_single_layer_number_(-1),
draw_interval_triangles_hatches_status_(false), draw_staircase_(false)
{

}

MeshExaminer::~MeshExaminer()
{

}

void MeshExaminer::resetMesh(const TriMesh &_mesh, bool _need_normalize)
{
	updateMesh(_mesh);

	if (_need_normalize){
		TriMesh::Point max_coord, min_coord;
		if (meshBoundingBox(min_coord, max_coord)){
			setScene((OpenMesh::Vec3f)((min_coord + max_coord)*0.5), 0.5*(max_coord - min_coord).norm());
		}
		else{
			setScene(OpenMesh::Vec3f(0, 0, 0), 1);
		}
	}
}

void MeshExaminer::updateMesh(const TriMesh &_mesh)
{
	mesh_show_ = _mesh;
	mesh_show_.request_face_normals();
	mesh_show_.request_vertex_normals();
	mesh_show_.update_normals();
}

void MeshExaminer::draw()
{
	if (draw_points_status_)
	{
		/*	glBegin(GL_POINTS);
		for (TriMesh::VertexIter v_it = mesh_show_.vertices_begin();
		v_it != mesh_show_.vertices_end(); v_it++)
		{
		TriMesh::Normal normal = mesh_show_.normal(*v_it);
		TriMesh::Point point = mesh_show_.point(*v_it);
		glColor3f(0.0f, 0.0f, 1.0f);
		glNormal3f(normal[0], normal[1], normal[2]);
		glVertex3f(point[0], point[1], point[2]);
		}
		glEnd();
		*/
		for (auto slicing_iterator = mesh_show_.mesh_slicing_.begin(); slicing_iterator != mesh_show_.mesh_slicing_.end(); slicing_iterator++)
		{
			TriMesh::Contours z_contours = *slicing_iterator;
			for (auto contours_iterator = z_contours.begin(); contours_iterator != z_contours.end(); contours_iterator++)
			{
				TriMesh::Polylines z_polyline = *contours_iterator;
				glPointSize(2.5f);
				glBegin(GL_POINTS);
				for (auto polyline_iterator = z_polyline.begin(); polyline_iterator != z_polyline.end(); polyline_iterator++)
				{
					TriMesh::Point p = *polyline_iterator;

					glColor3f(1.0f, 0.0f, 1.0f);
					glVertex3f(p[0], p[1], p[2]);
				}
				glEnd();
			}
		}
	}

	if (draw_faces_status_)
	{
		glBegin(GL_TRIANGLES);
		for (TriMesh::FaceIter f_it = mesh_show_.faces_begin();
			f_it != mesh_show_.faces_end(); f_it++)
		{
			TriMesh::Normal normal = mesh_show_.normal(*f_it);

			for (TriMesh::FaceHalfedgeIter fh_it = mesh_show_.fh_iter(*f_it);
				fh_it.is_valid(); fh_it++)
			{
				TriMesh::VertexHandle toV = mesh_show_.to_vertex_handle(*fh_it);
				TriMesh::Point point = mesh_show_.point(toV);
				glColor3f(0.7f, 0.7f, 0.0f);
				glNormal3f(normal[0], normal[1], normal[2]);
				glVertex3f(point[0], point[1], point[2]);
			}
		}
		glEnd();
	}

	if (draw_edges_status_)
	{
      for(TriMesh::FaceIter f_it = mesh_show_.faces_begin();
		f_it != mesh_show_.faces_end(); f_it++)
		{
		glBegin(GL_LINE_LOOP);
		for(TriMesh::FaceHalfedgeIter fh_it = mesh_show_.fh_iter(*f_it);
		fh_it.is_valid(); fh_it++)
		{
		 TriMesh::VertexHandle toV = mesh_show_.to_vertex_handle(*fh_it);
		 TriMesh::Normal normal = mesh_show_.normal(toV);
		 TriMesh::Point point = mesh_show_.point(toV);
		 glColor3f(0.3f, 0.1f, 0.1f);
		 glNormal3f(normal[0], normal[1], normal[2]);
		 glVertex3f(point[0], point[1], point[2]);
		}
		glEnd();
		}
	}

	if (draw_layers_status_)
	{
		for (auto slicing_iterator = mesh_show_.mesh_slicing_.begin(); slicing_iterator != mesh_show_.mesh_slicing_.end(); slicing_iterator++)
		{
			TriMesh::Contours z_contours = *slicing_iterator;
			for (auto contours_iterator = z_contours.begin(); contours_iterator != z_contours.end(); contours_iterator++)
			{
				TriMesh::Polylines z_polyline = *contours_iterator;
				glLineWidth(3);
				glBegin(GL_LINE_LOOP);
				//glBegin(GL_POLYGON);
				for (auto polyline_iterator = z_polyline.begin(); polyline_iterator != z_polyline.end(); polyline_iterator++)
				{
					TriMesh::Point p = *polyline_iterator;

					glColor3f(0.3f, 0.3f, 0.3f);
					glVertex3f(p[0], p[1], p[2]);
				}
				glEnd();
				
			}
		}
	}

	if (draw_staircase_)
	{
		
		for (auto slicing_iterator = mesh_show_.mesh_slicing_.begin(); slicing_iterator != mesh_show_.mesh_slicing_.end(); slicing_iterator++)
		{
			TriMesh::Contours z_contours = *slicing_iterator;
			for (auto contours_iterator = z_contours.begin(); contours_iterator != z_contours.end(); contours_iterator++)
			{
				TriMesh::Polylines z_polyline = *contours_iterator;
				//////////////////////////////////////////////////////////////////////////
				//add for staircase display8.32.2017
				glBegin(GL_QUAD_STRIP);
				for (auto polyline_iterator = z_polyline.begin(); polyline_iterator != z_polyline.end(); polyline_iterator++)
				{
					//TriMesh::Point p = *polyline_iterator;
					//TriMesh::Point temp_p = p;
					//temp_p[2] += mesh_show_.mesh_layer_thickness_;
					//TriMesh::Point pp = temp_p;
					////glLineWidth(10);
					//glBegin(GL_LINES);
					//glColor3f(1.0f, 0.3f, 0.3f);
					//glVertex3f(p[0], p[1], p[2]);
					//glVertex3f(pp[0], pp[1], pp[2]);
					//glEnd();

						TriMesh::Point p1 = *polyline_iterator;
						TriMesh::Point temp_p1 = p1;
						temp_p1[2] += mesh_show_.mesh_layer_thickness_;
						TriMesh::Point pp1 = temp_p1;
						glColor3f(1.0f, 0.3f, 0.3f);
						glVertex3f(p1[0], p1[1], p1[2]);
						glVertex3f(pp1[0], pp1[1], pp1[2]);
				}

				auto polyline_iterator1 = z_polyline.begin();
				TriMesh::Point p1 = *polyline_iterator1;
				TriMesh::Point temp_p1 = p1;
				temp_p1[2] += mesh_show_.mesh_layer_thickness_;
				TriMesh::Point pp1 = temp_p1;
				glColor3f(1.0f, 0.3f, 0.3f);
				glVertex3f(p1[0], p1[1], p1[2]);
				glVertex3f(pp1[0], pp1[1], pp1[2]);
				glEnd();
			}
		}
		
	}

	if (draw_hexagons_status_)
	{
		for (auto mesh_segmented_slicing_it = mesh_show_.mesh_segmented_slicing.begin(); 
	        mesh_segmented_slicing_it != mesh_show_.mesh_segmented_slicing.end();
			mesh_segmented_slicing_it++)
		{
			TriMesh::SegmentedLayers segmented_layers = *mesh_segmented_slicing_it;
			for (auto segmented_layers_it = segmented_layers.begin(); segmented_layers_it != segmented_layers.end();
				segmented_layers_it++)
			{
				TriMesh::Subareas layer_subarea = *segmented_layers_it;
				for (auto subarea_contours_it_ = layer_subarea.begin(); subarea_contours_it_ != layer_subarea.end();subarea_contours_it_++)
				{
					TriMesh::Polylines subarea_contour_ = *subarea_contours_it_;
					glLineWidth(3);
					glBegin(GL_LINE_LOOP);
					for (auto layer_subarea_it = subarea_contour_.begin(); layer_subarea_it != subarea_contour_.end(); layer_subarea_it++)
					{
						TriMesh::Point p = *layer_subarea_it;
						glColor3f(0.3f, 0.3f, 0.3f);
						glVertex3f(p[0], p[1], p[2]);
					}
					glEnd();
				}
			}
		}
	}

	if (show_single_layer_number_)
	{
		for (int j = 0 ; j < mesh_show_.mesh_segmented_slicing.size();j++)
			//draw one layer's hexagonal subareas
		{
			if (j==show_single_layer_number_-1)
			{
			 TriMesh::SegmentedLayers segmented_layers = mesh_show_.mesh_segmented_slicing[j];
			 for (auto segmented_layers_it = segmented_layers.begin(); segmented_layers_it != segmented_layers.end();
				 segmented_layers_it++)
			 {
				 TriMesh::Subareas layer_subarea = *segmented_layers_it;
				 for (auto subarea_contours_it_ = layer_subarea.begin(); subarea_contours_it_ != layer_subarea.end(); subarea_contours_it_++)
				 {
					 TriMesh::Polylines subarea_contour_ = *subarea_contours_it_;
					 glLineWidth(3);
					 glBegin(GL_LINE_LOOP);
					 for (auto layer_subarea_it = subarea_contour_.begin(); layer_subarea_it != subarea_contour_.end(); layer_subarea_it++)
					 {
						 TriMesh::Point p = *layer_subarea_it;
						 glColor3f(0.3f, 0.3f, 0.3f);
						 glVertex3f(p[0], p[1], p[2]);
					 }
					 glEnd();
				 }
			 }
			}
		}	

		for (int i = 0; i < mesh_show_.mesh_slicing_.size();i++)
			//draw  one layer's contours
		{
			if (i==show_single_layer_number_-1)
			{
			TriMesh::Contours z_contours = mesh_show_.mesh_slicing_[i];
			for (auto contours_iterator = z_contours.begin(); contours_iterator != z_contours.end(); contours_iterator++)
			for (int j = 0; j < z_contours.size();j++)
			{
				TriMesh::Polylines z_polyline = z_contours[j];
				glLineWidth(3);
				glBegin(GL_LINE_LOOP);
				for (auto polyline_iterator = z_polyline.begin(); polyline_iterator != z_polyline.end(); polyline_iterator++)
				{
					TriMesh::Point p = *polyline_iterator;

					glColor3f(1.0f, 0.0f, 0.0f);
					glVertex3f(p[0], p[1], p[2]);
				}
				glEnd();
			}
			}
		}

		for (int i = 0; i < mesh_show_.mesh_hexagon_hatches_double_.size();i++)
			//draw one layer's hexagonal subareas hatches
		{
			if (i == show_single_layer_number_ - 1)
			{
				TriMesh::HatchesForOneLayer hatches_of_a_layer = mesh_show_.mesh_hexagon_hatches_double_[i];
				//double incerment_line_color = 0.000001;
				for (auto hatches_of_a_layer_it_ = hatches_of_a_layer.begin();
					hatches_of_a_layer_it_ != hatches_of_a_layer.end();
					hatches_of_a_layer_it_++)
				{
					TriMesh::HatchesForOneHexagonSubarea hatches_of_a_subarea = *hatches_of_a_layer_it_;
					//double increment_line_width = 0.4;
					for (auto hatches_of_a_subarea_it_ = hatches_of_a_subarea.begin();
						hatches_of_a_subarea_it_ != hatches_of_a_subarea.end();
						hatches_of_a_subarea_it_++)
					{
						TriMesh::Segment seg = *hatches_of_a_subarea_it_;
						TriMesh::Point start_pt = seg[0];
						TriMesh::Point end_pt = seg[1];
						//glLineWidth(increment_line_width+=increment_line_width);
						glBegin(GL_LINES);
						//glColor3f(incerment_line_color, incerment_line_color, incerment_line_color);
						glColor3f(0.0f, 0.0f, 0.8f);
						glVertex3f(start_pt[0], start_pt[1], start_pt[2]);
						glVertex3f(end_pt[0], end_pt[1], end_pt[2]);
						glEnd();
					}
					//incerment_line_color += incerment_line_color;
				}
			}
		}

		for (int i = 0;i <mesh_show_.mesh_interval_hatches_double_.size();i++)
			//show one layer's interval hatches
		{
			if (i == show_single_layer_number_ - 1)
			{
			TriMesh::HatchesForOneLayersInterval interval_hatches_of_a_layer_ = mesh_show_.mesh_interval_hatches_double_[i];
			for (auto interval_hatches_of_a_layer_it_ = interval_hatches_of_a_layer_.begin();
				interval_hatches_of_a_layer_it_ != interval_hatches_of_a_layer_.end();
				interval_hatches_of_a_layer_it_++)
			 {
				TriMesh::Segment seg = *interval_hatches_of_a_layer_it_;
				TriMesh::Point start_pt = seg[0];
				TriMesh::Point end_pt = seg[1];
				glBegin(GL_LINES);
				glColor3f(0.0f, 0.8f, 0.8f);
				glVertex3f(start_pt[0], start_pt[1], start_pt[2]);
				glVertex3f(end_pt[0], end_pt[1], end_pt[2]);
				glEnd();
			 }
			}
		}
	}

	if (draw_intervals_status_)
	{
		for (auto mesh_between_segmented_slicing_it = mesh_show_.mesh_between_segmented_slicing_.begin();
			mesh_between_segmented_slicing_it != mesh_show_.mesh_between_segmented_slicing_.end();
			mesh_between_segmented_slicing_it++)
		{
			TriMesh::BetweenSegmentedLayers between_segmented_layers_ = *mesh_between_segmented_slicing_it;
			for (auto between_segmented_layers_it_ = between_segmented_layers_.begin();
				between_segmented_layers_it_ != between_segmented_layers_.end();
				between_segmented_layers_it_++)
			{
				TriMesh::BetweenSubareas between_layer_subareas_ = *between_segmented_layers_it_;
				glLineWidth(3);
				glBegin(GL_LINE_LOOP);
				for (auto between_layer_subarea_it = between_layer_subareas_.begin(); between_layer_subarea_it != between_layer_subareas_.end(); between_layer_subarea_it++)
				{
					TriMesh::Point p = *between_layer_subarea_it;
					glColor3f(0.0f, 1.0f, 0.0f);
					glVertex3f(p[0], p[1], p[2]);
				}
				glEnd();
			}
		}	
	}

	if (draw_parallel_hatches_status_)
	{
		for (auto mesh_hexagon_hatches_double_it_ = mesh_show_.mesh_hexagon_hatches_double_.begin();
			mesh_hexagon_hatches_double_it_ != mesh_show_.mesh_hexagon_hatches_double_.end();
			mesh_hexagon_hatches_double_it_++)
		{
			TriMesh::HatchesForOneLayer hatches_of_a_layer = *mesh_hexagon_hatches_double_it_;
			for (auto hatches_of_a_layer_it_ = hatches_of_a_layer.begin();
				hatches_of_a_layer_it_ != hatches_of_a_layer.end();
				hatches_of_a_layer_it_++)
			{
				TriMesh::HatchesForOneHexagonSubarea hatches_of_a_subarea = *hatches_of_a_layer_it_;
				for (auto hatches_of_a_subarea_it_ = hatches_of_a_subarea.begin();
					hatches_of_a_subarea_it_ != hatches_of_a_subarea.end();
					hatches_of_a_subarea_it_++)
				{
					TriMesh::Segment seg = *hatches_of_a_subarea_it_;
					TriMesh::Point start_pt = seg[0];
					TriMesh::Point end_pt = seg[1];
					glBegin(GL_LINES);
					glColor3f(0.0f, 0.0f, 0.8f);
					glVertex3f(start_pt[0], start_pt[1], start_pt[2]);
					glVertex3f(end_pt[0], end_pt[1], end_pt[2]);
					glEnd();
				}
			}
		}
	}

	if (draw_interval_triangles_hatches_status_)
	{
		for (auto mesh_interval_hatches_double_it_ = mesh_show_.mesh_interval_hatches_double_.begin();
			mesh_interval_hatches_double_it_ != mesh_show_.mesh_interval_hatches_double_.end();
			mesh_interval_hatches_double_it_++)
		{
			TriMesh::HatchesForOneLayersInterval interval_hatches_of_a_layer_ = *mesh_interval_hatches_double_it_;
			for (auto interval_hatches_of_a_layer_it_ = interval_hatches_of_a_layer_.begin();
				interval_hatches_of_a_layer_it_ != interval_hatches_of_a_layer_.end();
				interval_hatches_of_a_layer_it_++)
			{
				TriMesh::Segment seg = *interval_hatches_of_a_layer_it_;
				TriMesh::Point start_pt = seg[0];
				TriMesh::Point end_pt = seg[1];
				glBegin(GL_LINES);
				glColor3f(0.0f, 0.8f, 0.8f);
				glVertex3f(start_pt[0], start_pt[1], start_pt[2]);
				glVertex3f(end_pt[0], end_pt[1], end_pt[2]);
				glEnd();
			}
		}
	}
}

bool MeshExaminer::meshBoundingBox(TriMesh::Point &min_coord, TriMesh::Point &max_coord)
{
	if (mesh_show_.n_vertices() == 0){
		return false;
	}

	TriMesh::ConstVertexIter cv_it = mesh_show_.vertices_begin(), cv_end(mesh_show_.vertices_end());
	min_coord = mesh_show_.point(*cv_it);
	max_coord = min_coord;

	for (++cv_it; cv_it != cv_end; ++cv_it){
		min_coord.minimize(mesh_show_.point(*cv_it));
		max_coord.maximize(mesh_show_.point(*cv_it));
	}

	return true;
}
