/*
相机标定
根据投影关系设置GL投影矩阵
设置标记角点在世界坐标系下的坐标
计算图像坐标系下标记角点对应的坐标
根据世界坐标系下的坐标、图像坐标系下标记角点对应的坐标，以及相机内参，使用cv::solvePnP计算GL模型变换矩阵
分别设置GL的投影矩阵及模型视图矩阵
在当前矩阵下绘制立方体
对立方体的各个面设置贴图

*/
#include "GlWndMain.h"
#include "GeneralFunctions.h"
#include <QImage>
#include <math.h>
#include <string>
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
#include <io.h>
#include <QPoint>
#include <QPoint>

using namespace std;


float dARBoxWidth=0.4;


void GlWndMain::loadGLTextures()
{
	glGenTextures(6, texture);//创建6个纹理
	for (int i=0;i<6;i++)
	{
		string strTitle=IntToStr(i);
		string sTexPath="Data\\"+strTitle+".jpg";
		QString qsTexPath = QString::fromStdString(sTexPath);
		QImage mTex,mBuf;
		if (!mBuf.load(qsTexPath))
		{
			cout<<"cannot open image : "<<qsTexPath.toStdString()<<endl;
			QImage dummy(128,128,QImage::Format_ARGB32);
			dummy.fill(Qt::lightGray);
			mBuf = dummy;//如果载入不成功，自动生成颜色图片
		}
		mTex = QGLWidget::convertToGLFormat(mBuf);//QGLWidget提供的专门转换图片的静态函数
		glBindTexture(GL_TEXTURE_2D,texture[i]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D,0,3,mTex.width(),mTex.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,mTex.bits());

	}
	glEnable(GL_TEXTURE_2D);
}
void GlWndMain::DrawARBox()
{
	//如果加载视频成功，就把视频显示在方块上，否则显示图片数据
	if(bARVidioOK)
	{
		GLuint videoTextur;
		glGenTextures(1, &videoTextur);
		videoOfAR>>mFrameForBox ; 
		cvtColor(mFrameForBox, mFrameForBox, CV_BGR2RGB);
		QImage buf2, tex;
		//将Mat类型转换成QImage
		buf2 = QImage((const unsigned char*)mFrameForBox.data, mFrameForBox.cols, mFrameForBox.rows, mFrameForBox.cols * mFrameForBox.channels(), QImage::Format_RGB888);
		tex = QGLWidget::convertToGLFormat(buf2);
		glBindTexture(GL_TEXTURE_2D, videoTextur);//建立一个绑定到目标纹理的纹理
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());

		glBindTexture(GL_TEXTURE_2D, videoTextur);
		glBegin(GL_QUADS);
		// 前面
		glNormal3f(0,0,1);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-dARBoxWidth, -dARBoxWidth, dARBoxWidth); // 纹理和四边形的左下
		glTexCoord2f(1.0f, 0.0f); glVertex3f( dARBoxWidth, -dARBoxWidth, dARBoxWidth); // 纹理和四边形的右下
		glTexCoord2f(1.0f, 1.0f); glVertex3f( dARBoxWidth, dARBoxWidth, dARBoxWidth); // 纹理和四边形的右上
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-dARBoxWidth, dARBoxWidth, dARBoxWidth); // 纹理和四边形的左上
		glEnd();
		glDeleteTextures(1, &videoTextur);

		// 后面
		glBindTexture(GL_TEXTURE_2D,texture[1]);
		glBegin(GL_QUADS);
		glNormal3f(0,0,-1);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-dARBoxWidth, -dARBoxWidth, -dARBoxWidth); // 纹理和四边形的右下
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-dARBoxWidth, dARBoxWidth, -dARBoxWidth); // 纹理和四边形的右上
		glTexCoord2f(0.0f, 1.0f); glVertex3f( dARBoxWidth, dARBoxWidth, -dARBoxWidth); // 纹理和四边形的左上
		glTexCoord2f(0.0f, 0.0f); glVertex3f( dARBoxWidth, -dARBoxWidth, -dARBoxWidth); // 纹理和四边形的左下
		glEnd();

		// 顶面
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glBegin(GL_QUADS);
		glNormal3f(0,1,0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-dARBoxWidth, dARBoxWidth, -dARBoxWidth); // 纹理和四边形的左上
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-dARBoxWidth, dARBoxWidth, dARBoxWidth); // 纹理和四边形的左下
		glTexCoord2f(0.0f, 1.0f); glVertex3f( dARBoxWidth, dARBoxWidth, dARBoxWidth); // 纹理和四边形的右下
		glTexCoord2f(0.0f, 0.0f); glVertex3f( dARBoxWidth, dARBoxWidth, -dARBoxWidth); // 纹理和四边形的右上
		glEnd();

		// 底面
		glBindTexture(GL_TEXTURE_2D, texture[3]);
		glBegin(GL_QUADS);
		glNormal3f(0,-1,0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-dARBoxWidth, -dARBoxWidth, -dARBoxWidth); // 纹理和四边形的右上
		glTexCoord2f(1.0f, 0.0f); glVertex3f( dARBoxWidth, -dARBoxWidth, -dARBoxWidth); // 纹理和四边形的左上
		glTexCoord2f(1.0f, 1.0f); glVertex3f( dARBoxWidth, -dARBoxWidth, dARBoxWidth); // 纹理和四边形的左下
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-dARBoxWidth, -dARBoxWidth, dARBoxWidth); // 纹理和四边形的右下
		glEnd();

		// 右面
		glBindTexture(GL_TEXTURE_2D, texture[4]);
		glBegin(GL_QUADS);
		glNormal3f(1,0,0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( dARBoxWidth, -dARBoxWidth, -dARBoxWidth); // 纹理和四边形的右下
		glTexCoord2f(1.0f, 0.0f); glVertex3f( dARBoxWidth, dARBoxWidth, -dARBoxWidth); // 纹理和四边形的右上
		glTexCoord2f(1.0f, 1.0f); glVertex3f( dARBoxWidth, dARBoxWidth, dARBoxWidth); // 纹理和四边形的左上
		glTexCoord2f(0.0f, 1.0f); glVertex3f( dARBoxWidth, -dARBoxWidth, dARBoxWidth); // 纹理和四边形的左下
		glEnd();

		// 左面
		glBindTexture(GL_TEXTURE_2D, texture[5]);
		glBegin(GL_QUADS);
		glNormal3f(-1,0,0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-dARBoxWidth, -dARBoxWidth, -dARBoxWidth); // 纹理和四边形的左下
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-dARBoxWidth, -dARBoxWidth, dARBoxWidth); // 纹理和四边形的右下
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-dARBoxWidth, dARBoxWidth, dARBoxWidth); // 纹理和四边形的右上
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-dARBoxWidth, dARBoxWidth, -dARBoxWidth); // 纹理和四边形的左上
		glEnd();

	}
	else
	{
		// 前面
		glBindTexture(GL_TEXTURE_2D,texture[0]);
		glBegin(GL_QUADS);
		glNormal3f(0,0,1);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-dARBoxWidth, -dARBoxWidth, dARBoxWidth); // 纹理和四边形的左下
		glTexCoord2f(1.0f, 0.0f); glVertex3f( dARBoxWidth, -dARBoxWidth, dARBoxWidth); // 纹理和四边形的右下
		glTexCoord2f(1.0f, 1.0f); glVertex3f( dARBoxWidth, dARBoxWidth, dARBoxWidth); // 纹理和四边形的右上
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-dARBoxWidth, dARBoxWidth, dARBoxWidth); // 纹理和四边形的左上
		glEnd();

		// 后面
		glBindTexture(GL_TEXTURE_2D,texture[1]);
		glBegin(GL_QUADS);
		glNormal3f(0,0,-1);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-dARBoxWidth, -dARBoxWidth, -dARBoxWidth); // 纹理和四边形的右下
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-dARBoxWidth, dARBoxWidth, -dARBoxWidth); // 纹理和四边形的右上
		glTexCoord2f(0.0f, 1.0f); glVertex3f( dARBoxWidth, dARBoxWidth, -dARBoxWidth); // 纹理和四边形的左上
		glTexCoord2f(0.0f, 0.0f); glVertex3f( dARBoxWidth, -dARBoxWidth, -dARBoxWidth); // 纹理和四边形的左下
		glEnd();

		// 顶面
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glBegin(GL_QUADS);
		glNormal3f(0,1,0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-dARBoxWidth, dARBoxWidth, -dARBoxWidth); // 纹理和四边形的左上
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-dARBoxWidth, dARBoxWidth, dARBoxWidth); // 纹理和四边形的左下
		glTexCoord2f(0.0f, 1.0f); glVertex3f( dARBoxWidth, dARBoxWidth, dARBoxWidth); // 纹理和四边形的右下
		glTexCoord2f(0.0f, 0.0f); glVertex3f( dARBoxWidth, dARBoxWidth, -dARBoxWidth); // 纹理和四边形的右上
		glEnd();

		// 底面
		glBindTexture(GL_TEXTURE_2D, texture[3]);
		glBegin(GL_QUADS);
		glNormal3f(0,-1,0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-dARBoxWidth, -dARBoxWidth, -dARBoxWidth); // 纹理和四边形的右上
		glTexCoord2f(1.0f, 0.0f); glVertex3f( dARBoxWidth, -dARBoxWidth, -dARBoxWidth); // 纹理和四边形的左上
		glTexCoord2f(1.0f, 1.0f); glVertex3f( dARBoxWidth, -dARBoxWidth, dARBoxWidth); // 纹理和四边形的左下
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-dARBoxWidth, -dARBoxWidth, dARBoxWidth); // 纹理和四边形的右下
		glEnd();

		// 右面
		glBindTexture(GL_TEXTURE_2D, texture[4]);
		glBegin(GL_QUADS);
		glNormal3f(1,0,0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( dARBoxWidth, -dARBoxWidth, -dARBoxWidth); // 纹理和四边形的右下
		glTexCoord2f(1.0f, 0.0f); glVertex3f( dARBoxWidth, dARBoxWidth, -dARBoxWidth); // 纹理和四边形的右上
		glTexCoord2f(1.0f, 1.0f); glVertex3f( dARBoxWidth, dARBoxWidth, dARBoxWidth); // 纹理和四边形的左上
		glTexCoord2f(0.0f, 1.0f); glVertex3f( dARBoxWidth, -dARBoxWidth, dARBoxWidth); // 纹理和四边形的左下
		glEnd();

		// 左面
		glBindTexture(GL_TEXTURE_2D, texture[5]);
		glBegin(GL_QUADS);
		glNormal3f(-1,0,0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-dARBoxWidth, -dARBoxWidth, -dARBoxWidth); // 纹理和四边形的左下
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-dARBoxWidth, -dARBoxWidth, dARBoxWidth); // 纹理和四边形的右下
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-dARBoxWidth, dARBoxWidth, dARBoxWidth); // 纹理和四边形的右上
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-dARBoxWidth, dARBoxWidth, -dARBoxWidth); // 纹理和四边形的左上
		glEnd();
	}


}
void GlWndMain::DrawMainBox()
{

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);
	glNormal3f(0,0,1);
	glTexCoord2f(0,0); glVertex3f(-1,		-1,	1);
	glTexCoord2f(1,0); glVertex3f(1,		-1,	1);
	glTexCoord2f(1,1); glVertex3f(1,		1,		1);
	glTexCoord2f(0,1); glVertex3f(-1,		1,		1);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);
	glNormal3f(0,0,-1);
	glTexCoord2f(0,0); glVertex3f(1,		-1,	-1);
	glTexCoord2f(1,0); glVertex3f(-1,		-1,	-1);
	glTexCoord2f(1,1); glVertex3f(-1,		1,		-1);
	glTexCoord2f(0,1); glVertex3f(1,		1,		-1);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glBegin(GL_QUADS);
	glNormal3f(-1,0,0);
	glTexCoord2f(0,0); glVertex3f(-1,		-1,	-1);
	glTexCoord2f(1,0); glVertex3f(-1,		-1,	1);
	glTexCoord2f(1,1); glVertex3f(-1,		1,		1);
	glTexCoord2f(0,1); glVertex3f(-1,		1,		-1);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_QUADS);
	glNormal3f(1,0,0);
	glTexCoord2f(0,0); glVertex3f(1,		-1,	1);
	glTexCoord2f(1,0); glVertex3f(1,		-1,	-1);
	glTexCoord2f(1,1); glVertex3f(1,		1,		-1);
	glTexCoord2f(0,1); glVertex3f(1,		1,		1);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glBegin(GL_QUADS);
	glNormal3f(0,1,0);
	glTexCoord2f(0,0); glVertex3f(-1,		1,		1);
	glTexCoord2f(1,0); glVertex3f(1,		1,		1);
	glTexCoord2f(1,1); glVertex3f(1,		1,		-1);
	glTexCoord2f(0,1); glVertex3f(-1,		1,		-1);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glBegin(GL_QUADS);
	glNormal3f(0,-1,0);
	glTexCoord2f(0,0); glVertex3f(-1,		-1,		-1);
	glTexCoord2f(1,0); glVertex3f(1,		-1,		-1);
	glTexCoord2f(1,1); glVertex3f(1,		-1,		1);
	glTexCoord2f(0,1); glVertex3f(-1,		-1,		1);
	glEnd();

}


GlWndMain::GlWndMain(QWidget *parent)
	: QGLWidget(parent)
	,_bRun(false)
	,_bPause(false)
	,_bHyaline(false)
	,_bFog(false)
	,_bOpenAR(true)

{

	bARVidioOK=FALSE;
	videoOfAR=VideoCapture("Data/video.mp4");
	if (videoOfAR.isOpened())
	{
		bARVidioOK=true;
	}
	else
	{
		bARVidioOK=false;
	}

	xrot = yrot = zrot = 0;
	stepRotX=0;
	stepRotY=0;
	stepRotZ=0;

	moveX=moveY=0;
	zoom = -10.0;
	fogFilter = 3;
	light = false;
	fullScreen = false;

	if (fullScreen)
	{
		showFullScreen();
	}
	startTimer(5);
}
GlWndMain::~GlWndMain()
{
}
void GlWndMain::initializeGL()
{

	m_videoFrame=VideoCapture(0);
	//m_videoFrame=VideoCapture(1);

	loadGLTextures();//先载入纹理
	glEnable(GL_TEXTURE_2D);//启用纹理
	glEnable(GL_COLOR_MATERIAL);//可以用颜色来帖纹理
	glShadeModel(GL_SMOOTH);//阴影平滑
	glClearColor(1,1,1,0.5);//设置清除屏幕时所使用的颜色

	glClearDepth(1.0);//设置深度缓存
	glEnable(GL_DEPTH_TEST);//启用深度测试
	glDepthFunc(GL_LEQUAL);//所做深度测试的类型

	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);//投影修正
	glPolygonMode(GL_BACK,GL_FILL);//背景
	glPolygonMode(GL_FRONT,GL_FILL);//前景

	//设置光源
	GLfloat lightAmbient[4] = {0.5,0.5,0.5,1.0};
	GLfloat lightDiffuse[4] = {1.0,1.0,1.0,1.0};
	GLfloat lightPosition[4] = {0.0,0.0,2.0,1.0};
	//雾的设定//三种雾的效果,依次递进
	GLuint fogMode[3] = {GL_EXP,GL_EXP2,GL_LINEAR};
	GLfloat fogColor[4] = {1,1,1,0.3};

	//灯
	glLightfv(GL_LIGHT1,GL_AMBIENT,lightAmbient);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,lightDiffuse);
	glLightfv(GL_LIGHT1,GL_POSITION,lightPosition);
	glEnable(GL_LIGHT1);

	//雾
	glFogi(GL_FOG_MODE,fogMode[0]);
	glFogfv(GL_FOG_COLOR,fogColor);
	glFogf(GL_FOG_DENSITY,0.1);//雾的浓度
	glHint(GL_FOG_HINT,GL_FASTEST);//确定雾的渲染方式，GL_DONT_CARE不关心建议值，GL_NICEST极棒的，每一像素渲染，GL_FASTEST对每一顶点渲染，速度快
	glFogf(GL_FOG_START, 1);//雾离屏幕的距离
	glFogf(GL_FOG_END, 5.0);




	Point3f corners_3d[] = 
	{
		Point3f(-0.5f, -0.5f, 0),
		Point3f(-0.5f,  0.5f, 0),
		Point3f( 0.5f,  0.5f, 0),
		Point3f( 0.5f, -0.5f, 0)
	};

	////通过相机标定确定的参数，立方体与标记有偏差，不确定是不是由于标定数据引起
	//float camera_matrix[] = 
	//{
	//	590.7319		,0						,292.9710,
	//	0					, 619.0881		,202.5625,
	//	0					,0						,1
	//};
	//float dist_coeff[] = {0.1095483732100013, 0.005921985694402154, -0.02522667923131416, -0.0171742783898786, -0.1891767195416431};
	
	////20220626新数据
	//float camera_matrix[] = 
	//{
	//	621.6733		,0						,301.8697,
	//	0					, 596.7352		,223.5491,
	//	0					,0						,1
	//};
	//float dist_coeff[] = {0.2050844086865027, -1.253387945124429, -0.009926487596546369, -0.006799737561947785, 5.45488965637716};


	//外部摄像头参数
	float camera_matrix[] = 
	{
		508.3018		,0						,300.1497,
		0					, 504.5175		, 264.5351,
		0					,0						,1
	};
	float dist_coeff[] = {-0.4172170641396942, -0.1135454666162299, -0.0009781100036345459, -0.006095536879777572, 0.7763703887603729};


	m_corners_3d = vector<Point3f>(corners_3d, corners_3d + 4);//仅仅限制数量为4
	//构造3x3列的mat
	m_camera_matrix = Mat(3, 3, CV_32FC1, camera_matrix).clone();	//Mat构造函数不会拷贝数据，只会将指针指向数据区域，所以对于局部变量内存，需要clone
	//构造1x4列的mat
	m_dist_coeff = Mat(1, 5, CV_32FC1, dist_coeff).clone();

}
void GlWndMain::resizeGL(int w, int h)
{

	if (h==0)//防止高为0
	{
		h=1;
	}
	glViewport(0,0,(GLint)w,(GLint)h);//重置当前的视口
	glMatrixMode(GL_PROJECTION);//选择投影矩阵
	glLoadIdentity();//重置投影矩阵
	gluPerspective(45, (GLfloat)w/(GLfloat)h,0.1,300);//建立透视投影矩阵
	//gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);//选择模型观察矩阵
	glLoadIdentity();//重置模型观察矩阵


}
void GlWndMain::paintGL()
{
	int iStartTime=GetSysTime_number();

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


	if (1)
	{
		int width=this->width();
		int height=this->height();
		QImage buf, mTex;
		m_videoFrame>>mFrame ; 

		buf = QImage((const unsigned char*)mFrame.data, mFrame.cols, mFrame.rows, mFrame.cols * mFrame.channels(), QImage::Format_RGB888);
		mTex = QGLWidget::convertToGLFormat(buf);
		glLoadIdentity();
		//glTranslatef(0, 0, -200);
		glPixelZoom((GLfloat)width/mTex.width(),(GLfloat)height/mTex.height());
		glDrawPixels(mTex.width(),mTex.height(),GL_RGBA,GL_UNSIGNED_BYTE, mTex.bits());
		glClear(GL_DEPTH_BUFFER_BIT);

		if(_bOpenAR && mFrame.data!=NULL)
		{
			if (m_recognizer.update(mFrame, 100 , 10 )>0)
			{
				//intrinsicMatrix2ProjectionMatrix(m_camera_matrix, 640, 480, 0.01f, 100.0f, m_projection_matrix);
				float width=640;float height=480;float near_plane=0.1;float far_plane=100;
				{
					float f_x = m_camera_matrix.at<float>(0,0);
					float f_y = m_camera_matrix.at<float>(1,1);

					float c_x = m_camera_matrix.at<float>(0,2);
					float c_y = m_camera_matrix.at<float>(1,2);
					/*
					w近剪裁面的宽度
					h近剪裁面的高度
					n近剪裁面距离摄像机的距离
					f远剪裁面距离摄像机的距离
					*/
					m_projection_matrix[0] = 2*f_x/width;
					m_projection_matrix[1] = 0.0f;
					m_projection_matrix[2] = 0.0f;
					m_projection_matrix[3] = 0.0f;
					m_projection_matrix[4] = 0.0f;
					m_projection_matrix[5] = 2*f_y/height;
					m_projection_matrix[6] = 0.0f;
					m_projection_matrix[7] = 0.0f;
					m_projection_matrix[8] = 1.0f - 2*c_x/width;
					m_projection_matrix[9] = 2*c_y/height - 1.0f;
					m_projection_matrix[10] = -(far_plane + near_plane)/(far_plane - near_plane);
					m_projection_matrix[11] = -1.0f;
					m_projection_matrix[12] = 0.0f;
					m_projection_matrix[13] = 0.0f;
					m_projection_matrix[14] = -2.0f*far_plane*near_plane/(far_plane - near_plane);
					m_projection_matrix[15] = 0.0f;

				}
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();//重置当前指定的矩阵为单位矩阵
				//glMultMatrixf(m_projection_matrix);
				glLoadMatrixf(m_projection_matrix);

				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				glEnable(GL_DEPTH_TEST);
				glShadeModel(GL_SMOOTH); //some model / light stuff
				vector<Marker>& markers = m_recognizer.getMarkers();
				Mat rotation, translation;
				for (int i = 0; i < markers.size(); ++i)
				{
					//markers[i].estimateTransformToCamera(m_corners_3d, m_camera_matrix, m_dist_coeff, r, t);
					Mat rot_vec;
					bool res = solvePnP(m_corners_3d,		//i世界坐标系下的控制点的坐标
						markers[i].m_corners,							//i图像坐标系下对应的控制点的坐标
						m_camera_matrix,								//i相机内参
						m_dist_coeff,									//i相机畸变
						rot_vec,										//o旋转向量
						translation);											//o平移向量

					Rodrigues(rot_vec, rotation);				//旋转向量变为旋转矩阵
					//cout<<"translation..."<<endl<<translation<<endl;
					//cout<<"rot_vec..."<<endl<<rot_vec<<endl;
					//cout<<"rotation..."<<endl<<rotation<<endl;

					//extrinsicMatrix2ModelViewMatrix(rotation, translation, m_model_view_matrix);
					//绕X轴旋转180度，从OpenCV坐标系变换为OpenGL坐标系
					static double d[] = 
					{
						1, 0, 0,
						0, -1, 0,
						0, 0, -1
					};
					Mat_<double> rx(3, 3, d);
					rotation = rx*rotation;
					translation = rx*translation;


					m_model_view_matrix[0] =		rotation.at<double>(0,0);
					m_model_view_matrix[1] =		rotation.at<double>(1,0);
					m_model_view_matrix[2] =		rotation.at<double>(2,0);
					m_model_view_matrix[3] =		0.0f;

					m_model_view_matrix[4] =		rotation.at<double>(0,1);
					m_model_view_matrix[5] =		rotation.at<double>(1,1);
					m_model_view_matrix[6] =		rotation.at<double>(2,1);
					m_model_view_matrix[7] =		0.0f;

					m_model_view_matrix[8] =		rotation.at<double>(0,2);
					m_model_view_matrix[9] =		rotation.at<double>(1,2);
					m_model_view_matrix[10] =		rotation.at<double>(2,2);
					m_model_view_matrix[11] =		0.0f;

					m_model_view_matrix[12] =		translation.at<double>(0, 0)+stepRotX;
					m_model_view_matrix[13] =		translation.at<double>(1, 0)+stepRotY;
					m_model_view_matrix[14] =		translation.at<double>(2, 0)+stepRotZ;
					m_model_view_matrix[15] =		1.0f;

					
					glLoadMatrixf(m_model_view_matrix);////把当前矩阵GL_MODELVIEW的16个值设置为指定的值

					DrawARBox();
					
					cout<<"更新视图耗时ms："<<GetSysTime_number()-iStartTime<<endl;


					string strPicExportFolder=m_recognizer.GetExportPicFolder();
					if (strPicExportFolder!=""&&_access(strPicExportFolder.c_str() , 0)==0)
					{
						QPixmap::grabWindow(this->winId()).save((strPicExportFolder+"\\result.png").c_str(),"png");
					}
				}
			}
		}
	}


	if (!_bRun){	return;}

	if (!_bPause)//Pause
	{
		xrot += stepRotX;
		yrot += stepRotY;
		zrot += stepRotZ;
	}

	glLoadIdentity();
	glTranslatef(moveX, moveY, zoom);
	glRotatef(xrot,1,0,0);
	glRotatef(yrot,0,1,0);
	glRotatef(zrot,0,0,1);

	if (_bFog)
	{
		glEnable(GL_FOG);
	}
	else
	{
		glDisable(GL_FOG);
	}

	if (_bHyaline)
	{
		glColor4f(1,1,0.8,0.8);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND );
		glDepthMask(FALSE);
	}
	else
	{
		glColor4f(1,1,1,1);
	}

	DrawMainBox();

	if (_bHyaline)
	{
		glDepthMask(TRUE);
		glDisable(GL_BLEND);
	}


}

//重绘
void GlWndMain::timerEvent(QTimerEvent *)
{
	updateGL();
}
//鼠标单击事件
void GlWndMain::mousePressEvent(QMouseEvent *e)
{
	setCursor(Qt::OpenHandCursor);
	lastPos = e->pos();
}
void GlWndMain::mouseReleaseEvent(QMouseEvent *e)
{
	setCursor(Qt::ArrowCursor);
	lastPos = e->pos();
}
void GlWndMain::mouseMoveEvent(QMouseEvent *e)
{
	GLfloat dx = GLfloat(e->x()-lastPos.x())/width();
	GLfloat dy = GLfloat(e->y()-lastPos.y())/height();
	lastPos = e->pos();
	if (e->buttons()&Qt::LeftButton)
	{
		xrot -= dy*50;
		yrot += dx*50;
		updateGL();
	}
	else if (e->buttons()&Qt::RightButton)
	{
		xrot -= dy*200;
		yrot += dx*200;
		updateGL();
	}
	else if (e->buttons()&Qt::MiddleButton)
	{
		moveX += dx*5;
		moveY -= dy*5;
		updateGL();
	}
}
//滚轮事件
void GlWndMain::wheelEvent(QWheelEvent *e)
{
	GLfloat zValue = e->delta();
	zoom += zValue*0.005;
	if (zoom>1.0)
	{
		zoom = 1.0;
	}
	updateGL();
}

void GlWndMain::intrinsicMatrix2ProjectionMatrix(cv::Mat& camera_matrix, float width, float height, float near_plane, float far_plane, float* projection_matrix)
{
	float f_x = camera_matrix.at<float>(0,0);
	float f_y = camera_matrix.at<float>(1,1);

	float c_x = camera_matrix.at<float>(0,2);
	float c_y = camera_matrix.at<float>(1,2);

	projection_matrix[0] = 2*f_x/width;
	projection_matrix[1] = 0.0f;
	projection_matrix[2] = 0.0f;
	projection_matrix[3] = 0.0f;

	projection_matrix[4] = 0.0f;
	projection_matrix[5] = 2*f_y/height;
	projection_matrix[6] = 0.0f;
	projection_matrix[7] = 0.0f;

	projection_matrix[8] = 1.0f - 2*c_x/width;
	projection_matrix[9] = 2*c_y/height - 1.0f;
	projection_matrix[10] = -(far_plane + near_plane)/(far_plane - near_plane);
	projection_matrix[11] = -1.0f;

	projection_matrix[12] = 0.0f;
	projection_matrix[13] = 0.0f;
	projection_matrix[14] = -2.0f*far_plane*near_plane/(far_plane - near_plane);
	projection_matrix[15] = 0.0f;
}

void GlWndMain::extrinsicMatrix2ModelViewMatrix(cv::Mat& rotation, cv::Mat& translation, float* model_view_matrix)
{
	//绕X轴旋转180度，从OpenCV坐标系变换为OpenGL坐标系
	static double d[] = 
	{
		1, 0, 0,
		0, -1, 0,
		0, 0, -1
	};
	Mat_<double> rx(3, 3, d);

	rotation = rx*rotation;
	translation = rx*translation;

	model_view_matrix[0] = rotation.at<double>(0,0);
	model_view_matrix[1] = rotation.at<double>(1,0);
	model_view_matrix[2] = rotation.at<double>(2,0);
	model_view_matrix[3] = 0.0f;

	model_view_matrix[4] = rotation.at<double>(0,1);
	model_view_matrix[5] = rotation.at<double>(1,1);
	model_view_matrix[6] = rotation.at<double>(2,1);
	model_view_matrix[7] = 0.0f;

	model_view_matrix[8] = rotation.at<double>(0,2);
	model_view_matrix[9] = rotation.at<double>(1,2);
	model_view_matrix[10] = rotation.at<double>(2,2);
	model_view_matrix[11] = 0.0f;

	model_view_matrix[12] = translation.at<double>(0, 0);
	model_view_matrix[13] = translation.at<double>(1, 0);
	model_view_matrix[14] = translation.at<double>(2, 0);
	model_view_matrix[15] = 1.0f;
}


void GlWndMain::Run()
{
	_bRun=true;
}
void GlWndMain::Stop()
{
	_bRun=false;
}
void GlWndMain::Pause()
{
	_bPause=true;
}
void GlWndMain::Continue()
{
	_bPause=false;
}
void GlWndMain::OpenHyaline()
{
	_bHyaline=true;
}
void GlWndMain::CloseHyaline()
{
	_bHyaline=false;
}
void GlWndMain::OpenFog()
{
	_bFog=true;
}
void GlWndMain::CloseFog()
{
	_bFog=false;
}
void GlWndMain::SetRotateStepX(GLfloat fStep)
{
	stepRotX=fStep;
}
void GlWndMain::SetRotateStepY(GLfloat fStep)
{
	stepRotY=fStep;
}
void GlWndMain::SetRotateStepZ(GLfloat fStep)
{
	stepRotZ=fStep;
}
void GlWndMain::OpenVideo()
{
	_bOpenAR=true;
}
void GlWndMain::CloseVideo()
{
	_bOpenAR=false;
}

