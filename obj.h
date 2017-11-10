#include <vector>
#include <Windows.h>
#include <GL\glut.h>
using std::vector;

class Point {
public:
	float x, y, z;
	Point(float _x, float _y, float _z):x(_x),y(_y),z(_z) {}
};

class Vertex {
public:
	int vi, vti, vni;
	Vertex(float _vi, float _vti, float _vni) :vi(_vi), vti(_vti), vni(_vni) {}
};

class Obj {
public:
	vector<Point> v,vn,vt;
	vector<vector<Vertex>> f;
	float size = 0;
	GLuint tex[3];
	Obj(){}
	void init(char* filename) {
		char line[100];
		char mark[5];
		char *pch;

		FILE* fp = fopen(filename, "r");
		v.push_back(Point(0, 0, 0));
		vt.push_back(Point(0, 0, 0));
		vn.push_back(Point(0, 0, 0));

		if (!fp) {
			MessageBoxA(HWND_DESKTOP, "Í¼Æ¬µ¼ÈëÊ§°Ü!\n(TextureLoad Failed!)", "Error", MB_OK | MB_ICONEXCLAMATION);
			exit(1);
		}

		while (fgets(line, 100, fp)) {
			switch (line[0]) {
			case 'v':
				float x, y, z;
				sscanf(line, "%s %f %f %f", mark, &x, &y, &z);
				switch (line[1]) {
				case ' ':
					v.push_back(Point(x, y, z));
					if (abs(x) > size)
						size = abs(x);
					if (abs(y) > size)
						size = abs(y);
					if (abs(z) > size)
						size = abs(z);
					break;
				case 't':
					vt.push_back(Point(x, y, z));
					break;
				case 'n':
					vn.push_back(Point(x, y, z));
					break;
				}
				break;
			case 'f':
				pch = strtok(line, " ");
				pch = strtok(NULL, " ");
				vector<Vertex> now_f;
				while (pch != NULL && pch[0] != '\n')
				{
					int v, vt, vn;
					sscanf(pch, "%d/%d/%d", &v, &vt, &vn);
					now_f.push_back(Vertex(v, vt, vn));
					pch = strtok(NULL, " ");
				}
				f.push_back(now_f);
				break;
			}
		}
	}

	void draw() {
		glPushMatrix();
		for (int i = 0; i < f.size(); i++) {
			glBegin(GL_POLYGON);
			for (int j = 0; j < f[i].size(); j++) {
				glTexCoord3f(vt[f[i][j].vti].x, vt[f[i][j].vti].y, vt[f[i][j].vti].z);
				glNormal3f(vn[f[i][j].vni].x, vn[f[i][j].vni].y, vn[f[i][j].vni].z);
				glVertex3f(v[f[i][j].vi].x, v[f[i][j].vi].y, v[f[i][j].vi].z);
			}
			glEnd();
		}
		glPopMatrix();
	}
};