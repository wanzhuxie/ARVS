#ifndef GlWndMain_H
#define GlWndMain_H

#include "MarkerRecognizer.h"
#include <QtWidgets/QWidget>
#include "ui_GlWndMain.h"
#include <QGLWidget>
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


class GlWndMain : public QGLWidget
{
	Q_OBJECT

public:
	GlWndMain(QWidget *parent = 0);
	~GlWndMain();
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
	Ui::GlWndMainClass ui;

	void initializeGL();//��ʼ��QpenGL���ڲ���
	void paintGL();//����QPenGL����,�и��·����������ͻᱻ����
	void resizeGL(int w, int h);//�����ڴ�С�仯��w��h����״̬�µĿ�͸ߣ���ɺ��Զ�ˢ����Ļ
	void timerEvent(QTimerEvent *);//ʵ�ִ��ڲ����Ķ�ʱ����


	void loadGLTextures();//��������

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
				 
	GLuint texture[6];//6������
	GLuint texturFrame;

	GLfloat sphere_x,sphere_y;

	bool light;
	bool blend;


	GLuint fogFilter;//ѡ���������
	GLuint filter;//�����������
	QPoint lastPos;


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

#endif // GlWndMain_H



