#ifndef GLVIEWER_H
#define GLVIEWER_H

#include <QtOpenGL>
#include "meshexaminer.h"

#include <QColorDialog>
#include <QInputDialog>

class GLViewer : public QGLWidget
{
    Q_OBJECT

public:
    GLViewer(QWidget *parent = 0);
    ~GLViewer();

    void updateMesh(const TriMesh &_mesh);
    void resetMesh(const TriMesh &_mesh, bool _needNormalize = false);

    MeshExaminer* getExaminer(){
        return examiner_;
    }

public slots:
    void setDrawPointsStatus(bool _val){
        examiner_->setDrawPointsStatus(_val);
        this->updateGL();
    }

    void setDrawFacesStatus(bool _val){
        examiner_->setDrawFacesStatus(_val);
        this->updateGL();
    }

    void setDrawEdgesStatus(bool _val){
        examiner_->setDrawEdgesStatus(_val);
        this->updateGL();
    }

	void setDrawStaircase(bool _val){//add for layers 3-8-2017
		examiner_->setDrawStairacase(_val);
		this->updateGL();
	}

	void setDrawLayersStatus(bool _val){//add for layers 3-8-2017
		examiner_->setDrawLayersStatus(_val);
		this->updateGL();
	}

	void setDrawHexagonsStatus(bool _val){//add for hexagonal subareas 3-9-2017
		examiner_->setDrawHexagonsStatus(_val);
		this->updateGL();
	}
	void setDrawIntervalsStatus(bool _val){//add for intervals 3.14.2017
		examiner_->setDrawIntervalsStatus(_val);
		this->updateGL();
	}

	void setDrawParallelHatchessStatus(bool _val){//add for parallel hatches of hexagonal subareas 3.15.2017
		examiner_->setDrawParallelHatchessStatus(_val);
		this->updateGL();
	}

	void setDrawIntervalTrianglesHatchessStatus(bool _val){//add for triangles hatches of interval 3.21.2017
		examiner_->setDrawIntervalTrianglesHatchessStatus(_val);
		this->updateGL();
	}

    void setBackgroundColor(){
        QColor color = QColorDialog::getColor(Qt::black, this, tr("Set Background Color!"));
        if(!color.isValid()) return;
        this->qglClearColor(color);
        this->updateGL();
    }
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

    void initializeGL();
    void resizeGL(int _w, int _h);
    void paintGL();

//private:
//    MeshExaminer *examiner_;
public:
	MeshExaminer *examiner_;///change 3.13.2017
};

#endif // GLVIEWER_H
