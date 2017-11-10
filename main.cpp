#include <cstdio>
#include <cstdlib>
#include <vector>
#include <windows.h>
#include <GL\glut.h>
#include <cstring>
#include "button.h"
#include "Texture.h"
#include "obj.h"
//�����ȫ������ϵ��˼�����������⣬��ô����Ҫע�����˷���˳��ʹ����е�˳�����෴�ġ����þֲ�����ϵ��˼���Ļ������еĲ�����������ڵ�ǰ���ϱ仯������ϵ����ˣ�����˷�����Ȼ���������ٴ����г��ֵ�˳��һ����
//glPushMatrix()��glPopMatrix()����������һ�α任�Ա��α任��Ӱ�죬ʹ�ñ��α任���ǻ�����������ϵ��ԭ��Ϊ�ο�����еġ���ˣ�ʵ��̫��ϵ��ֻ��Ҫ��̫���Ȼ�����Ĭ������������ϵԭ�㣩��Ȼ�󱣴浱ǰ��ͼ���������ǣ��ָ���ͼ������˼·������ȫ������ϵ��˼�롣
using std::vector;
using std::pair;

#define MAX_PATH_NAME 128
Obj teapot;
button* hint;

void init() {
	//������obj
	char localPath[MAX_PATH_NAME];
	GetCurrentDirectoryA(MAX_PATH_NAME, localPath);
	strcat(localPath, "\\");
	strcat(localPath, "teapot.obj");
	teapot.init(localPath);

	//����hint����
	hint = new button(-teapot.size, -teapot.size, teapot.size, teapot.size * 35 / 113);
	hint->setTex(1);
	GetCurrentDirectoryA(MAX_PATH_NAME, localPath);
	strcat(localPath, "\\");
	strcat(localPath, "set.bmp");
	hint->setTexImage(localPath);

	//����������
	GetCurrentDirectoryA(MAX_PATH_NAME, localPath);
	strcat(localPath, "\\");
	strcat(localPath, "tex1.bmp");
	BuildTexture(localPath, teapot.tex[0]);
	GetCurrentDirectoryA(MAX_PATH_NAME, localPath);
	strcat(localPath, "\\");
	strcat(localPath, "tex2.bmp");
	BuildTexture(localPath, teapot.tex[1]);
	GetCurrentDirectoryA(MAX_PATH_NAME, localPath);
	strcat(localPath, "\\");
	strcat(localPath, "tex3.bmp");
	BuildTexture(localPath, teapot.tex[2]);
}

GLfloat windowLeft, windowDown;
void reshape(int w, int h)
{//�Զ������´��ڵĴ�С�����أ�
	glViewport(0, 0, w, h);//�ڴ�������ʾ��������ʾ�������½�������ʾ����Ŀ����߶�
	glMatrixMode(GL_PROJECTION);//������Ҫ����ͶӰ���ò���
	glLoadIdentity();//���ñ任����

					 //ʹ������ͶӰ�۲����壬�۲췽ʽ��ƽ���޼��ռ�
	if (w <= h) {//����ʹ���ۿ�߱���Σ�ʼ�չ۲촰����ô���һ������
		glOrtho(-teapot.size, teapot.size, -teapot.size*(GLfloat)h / (GLfloat)w, teapot.size*(GLfloat)h / (GLfloat)w, 0, teapot.size*2);
		windowLeft = -teapot.size;
		windowDown = -teapot.size*(GLfloat)h / (GLfloat)w;
		//����۲�ռ�ģ����ҡ��¡��ϡ�ǰ����
	}
	else {
		glOrtho(-teapot.size*(GLfloat)w / (GLfloat)h, teapot.size*(GLfloat)w / (GLfloat)h, -teapot.size, teapot.size, 0, teapot.size*2);
		windowLeft = -teapot.size*(GLfloat)w / (GLfloat)h;
		windowDown = -teapot.size;
	}

	//Ҳ����ʹ��͸��ͶӰͶӰ�۲���
	//gluPerspective(90, w / h, 0.1, 6);//�����������Ҫ����
	//����۲�ռ�Ĵ�ֱ�Ƕȣ���߱ȣ�ǰ���棬�����

	glMatrixMode(GL_MODELVIEW);//�����������Ӿ����ò���
	glLoadIdentity();//���ñ任����
					 //ps.��������GL_TEXTUREʱ���Խ����������
}

void setLight() {//��ӹ���
	GLfloat lmodel_ambient[] = { 1,1,1,1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	GLfloat light_position[] = { 1,1,1,0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	GLfloat light[] = { 1,1,1,1 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, light);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, light);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, light);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0);
}

void showHint() {//��ʾ��ʾ
	glPushMatrix();
	//glTranslatef(teapot.size, 0, 0);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	hint->show(windowLeft, windowDown);
	glPopMatrix();
}

enum Model { TEAPOT_TEX1, TEAPOT_TEX2, TEAPOTTEX3, TEAPOT_SLOID, TEAPOT_LINE, TEAPOT_POINT }model;
vector<pair<int, GLfloat>> rotateLog;
const GLfloat axis[3][3] = { { 1.0,0.0,0.0 },{ 0.0,1.0,0.0 },{ 0.0,0.0,1.0 } };
GLfloat bigger = 1.0;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�����ɫ����ά���棿
	glMatrixMode(GL_MODELVIEW);//�����������Ӿ����ò���
	glLoadIdentity();//���ñ任����
	gluLookAt(teapot.size, 0, 0, 0, 0, 0, 0, 0, 1);//�������xyz���۲���xyz������Ϸ����xyz
	setLight();//���ù���
	glPushMatrix();
	for (int i = rotateLog.size() - 1; i >= 0; i--)
		glRotatef(rotateLog[i].second, axis[rotateLog[i].first][0], axis[rotateLog[i].first][1], axis[rotateLog[i].first][2]);
	//������ת
	glTranslatef(0, 0, -teapot.size*0.5);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(bigger, bigger, bigger);//���÷Ŵ�

	switch (model) {//��ʾģ��
	case TEAPOT_SLOID://��������ɫģʽ
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(0.2, 0.2, 0.8);
		teapot.draw();
	case TEAPOT_LINE://���ñ���ɫģʽ
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3f(0.5, 0.5, 0.5);
		teapot.draw();
	case TEAPOT_POINT://���õ���ɫģʽ
		glColor3f(1, 1, 1);
		glPointSize(10);
		glBegin(GL_POINTS);
		for (int i = 0; i < teapot.v.size(); i++)
			glVertex3f(teapot.v[i].x, teapot.v[i].y, teapot.v[i].z);
		glEnd();
		break;
	default:
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, teapot.tex[model]);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		teapot.draw();
		glDisable(GL_TEXTURE_2D);
		break;
	}
	glPopMatrix();
	showHint();//��ʾ��ʾ
	glutSwapBuffers();//�û����棨ˢ�»��棬��Ϊʹ����˫���棩
}

void logRotate(int axis, int count) {//��¼��ת���
	if (rotateLog.size() == 0)
		rotateLog.push_back(pair<int, GLfloat>(axis, count));
	else if (rotateLog.back().first == axis) {
		rotateLog.back().second += count;
		if ((int)rotateLog.back().second % 360 == 0)
			rotateLog.pop_back();
	}
	else rotateLog.push_back(pair<int, GLfloat>(axis, count));
}

void keyboard(unsigned char key, int _x, int _y) {
	switch (key)
	{//��Ӧ�л�ģ���¼�
	case 'j':
		model = TEAPOT_SLOID;
		break;
	case 'k':
		model = TEAPOT_LINE;
		break;
	case 'l':
		model = TEAPOT_POINT;
		break;
	case 'u':
		model = TEAPOT_TEX1;
		break;
	case 'i':
		model = TEAPOT_TEX2;
		break;
	case 'o':
		model = TEAPOTTEX3;
		break;
	}
	switch (key)
	{//��Ӧ��ת�¼�
	case 'q':
		logRotate(0, 1);
		break;
	case 'e':
		logRotate(0, -1);
		break;
	case 'w':
		logRotate(1, -1);
		break;
	case 's':
		logRotate(1, 1);
		break;
	case 'a':
		logRotate(2, -1);
		break;
	case 'd':
		logRotate(2, 1);
		break;
	}
	switch (key)
	{
	case 'r':
		bigger *= 1.01;
		break;
	case 'f':
		bigger /= 1.01;
		if (bigger == 0)
			bigger = 1;
		break;
	}
	glutPostRedisplay();//ˢ����ͼ
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);//��ʼ��GLUT�⣻
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//˫����ģʽ��RGBģʽ����άģʽ��
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1200, 1000);//���ô��ڴ�С
	glutCreateWindow("colorcube");//���ô�������
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//˫���դ��
	glEnable(GL_DEPTH_TEST);//������άģʽ��
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glLineWidth(3);//ʹ��3ά��դ��
	init();
	glutReshapeFunc(reshape);//���õ����ڴ�С�����ı�ʱ���õĺ���
	glutDisplayFunc(display);//����ˢ�º���
	glutKeyboardFunc(keyboard);//���ü�����Ӧ�¼�
	glutMainLoop();//�����Զ�ˢ��ѭ��
	return 0;
}

