#ifndef TOOLTESTMAIN_H
#define TOOLTESTMAIN_H

#include "MarkerRecognizer.h"
#include <QtWidgets/QWidget>
#include "ui_GlWndMain.h"
#include <QGLWidget>
#include <QTimer>
#include <QTimer>  
#include "gl/glu.h"
#include "gl/glut.h"
#include <QWidget>
#include <qgl.h>
#include <QKeyevent>
#include <QtOpenGL>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
#include <QtOpenGL>


class ToolTestMain : public QGLWidget
{
	Q_OBJECT

public:
	ToolTestMain(QWidget *parent = 0);
	~ToolTestMain();
	void DrawARBox();
	void DrawMainBox();
	void Run();
	void Stop();
	void Pause();
	void Continue();
	void OpenHyaline();
	void CloseHyaline();
	void OpenFog();
	void CloseFog();
	void OpenVideo();
	void CloseVideo();
	void SetRotateStepX(GLfloat fStep);
	void SetRotateStepY(GLfloat fStep);
	void SetRotateStepZ(GLfloat fStep);



private:
	Ui::ToolTestMainClass ui;

//	void initializeGL();
//	void resizeGL(int w, int h);
//	void paintGL();
//	int R;  
//	float x,y,z;  
//	bool wired;  
//	float ang;  
//
//private:  
//	QTimer timer;  
	void initializeGL();//��ʼ��QpenGL���ڲ���
	void paintGL();//����QPenGL����,�и��·����������ͻᱻ����
	void resizeGL(int w, int h);//�����ڴ�С�仯��w��h����״̬�µĿ�͸ߣ���ɺ��Զ�ˢ����Ļ
	void timerEvent(QTimerEvent *);//ʵ�ִ��ڲ����Ķ�ʱ����


	void loadGLTextures();//��������
	void buildLists();//�����б�
	void drawList();//���б�
	void drawFlag();//������
	void drawSphere();//������
	int drawGLobject();

	//void keyPressEvent(QKeyEvent *e);//��갴���¼�������
	void mousePressEvent(QMouseEvent *e);//��굥���¼�
	void mouseMoveEvent(QMouseEvent *e);//����ƶ��¼�
	void mouseReleaseEvent(QMouseEvent *e);//����ͷ��¼�
	void wheelEvent(QWheelEvent *e);//�������¼�
	void intrinsicMatrix2ProjectionMatrix(cv::Mat& camera_matrix, float width, float height, float near_plane, float far_plane, float* projection_matrix);
	void extrinsicMatrix2ModelViewMatrix(cv::Mat& rotation, cv::Mat& translation, float* model_view_matrix);

	bool fullScreen;
	GLfloat xrot,yrot,zrot;
	GLfloat stepRotX,stepRotY,stepRotZ;
	GLfloat moveX, moveY, zoom;
				 
	GLfloat hold;//�����������⻬�ĸ�����
	GLuint texture[6];//6������
	GLuint texturFrame;
	GLuint box,top,tri;//�����ʾ�б��ָ��
	GLuint xLoop,yLoop;//��ʾ����������λ�õı���

	GLfloat sphere_x,sphere_y;

	bool light;
	bool blend;

	float points[45][45][3];//����������������أ�x,y,z������
	int wiggle_count;//ָ�����˵��ٶ�

	GLuint fogFilter;//ѡ���������
	GLuint filter;//�����������
	QPoint lastPos;
	GLUquadric *qobj;//���η��̶���

	int part1;//Բ�̵���ʼ�Ƕ�
	int part2;//Բ�̵Ľ����Ƕ�
	int p1;//����
	int p2;//����
	GLuint obj;//Ҫ���Ķ���

	bool sp;//�ո���Ƿ���
	bool _bRun;//��ʼ����
	bool _bPause;//��ͣ����
	bool _bHyaline;//�Ƿ���͸��
	bool _bFog;//�Ƿ���͸��
	bool _bOpenAR;//�Ƿ�������ͷ

	VideoCapture m_video;
	VideoCapture m_videoFrame;

	//AR������Ƶ
	VideoCapture videoOfAR;
	BOOL bARVidioOK;



	Mat mFrame, mFrameForBox;

	vector<cv::Point3f> m_corners_3d;

	cv::Mat m_camera_matrix;
	cv::Mat m_dist_coeff;
	float m_projection_matrix[16];
	float m_model_view_matrix[16];

	cv::Mat m_img_gray;
	cv::Mat m_img_color;
	cv::Mat mFrame_gray;
	MarkerRecognizer m_recognizer;

	vector<Point3f> g_bgVertices;

	vector<QPointF> m_vecHandPos;
	qreal m_HandDia;
};

#endif // TOOLTESTMAIN_H



