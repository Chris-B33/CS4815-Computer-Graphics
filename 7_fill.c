#include <stdlib.h>
#include <iostream>
#include <GL/glut.h>
#include <vector>
#include <algorithm>

using namespace std;

class Point {
	public:
		GLfloat x;
		GLfloat y;
		GLfloat r;
		GLfloat g;
		GLfloat b;

		Point(GLfloat _x, GLfloat _y,  GLfloat _r,  GLfloat _g,  GLfloat _b) {
			x = _x;
			y = _y;
			r = _r;
			g = _g;
			b = _b;
		}
};

GLfloat M = 0.8;
vector<Point> points = {
	Point(0.0, M, 1.0, 0.0, 0.0),
	Point(-M, 0.0, 0.0, 1.0, 0.0),
	Point(0.0, -2*M, 0.0, 0.0, 1.0),
	Point(2*M, 0.0, 0.0, 0.0, 0.0)
};


void outline(int p1Index, int p2Index) {
	Point p1 = points[p1Index];
	Point p2 = points[p2Index];

	GLfloat dx = p2.x - p1.x;
	GLfloat dy = p2.y - p1.y;

	GLfloat px = p1.x;
	GLfloat py = p1.y;

	GLfloat pointCount = 1000;
	GLfloat i = 0;

	while (i < pointCount) {
		// Change in coords
		px += dx / pointCount;
		py += dy / pointCount;

		// Linear Interpolation for Colour
		GLfloat pr = p1.r * (1 - (i / pointCount)) + p2.r * (i / pointCount);
		GLfloat pg = p1.g * (1 - (i / pointCount)) + p2.g * (i / pointCount);
		GLfloat pb = p1.b * (1 - (i / pointCount)) + p2.b * (i / pointCount);

		// Create new point
		Point newPoint = Point(px, py, pr, pg, pb);
		points.push_back(newPoint);
		i++;
	}
}

bool comparePoints(const Point &a, const Point &b) {
	if (a.y != b.y) { return a.y < b.y; }
	return a.x < b.x;
}

void shapeRow(Point p1, Point p2) {
	GLfloat px = p1.x;
	GLfloat py = p1.y;

	GLfloat pointCount = 1000;
	GLfloat i = 0;
	while (i < pointCount) {
		// Change in coords
		px += (p2.x - p1.x) / pointCount;

		// Linear Interpolation for Colour
		GLfloat pr = p1.r * (1 - (i / pointCount)) + p2.r * (i / pointCount);
		GLfloat pg = p1.g * (1 - (i / pointCount)) + p2.g * (i / pointCount);
		GLfloat pb = p1.b * (1 - (i / pointCount)) + p2.b * (i / pointCount);

		// Create new point
		Point newPoint = Point(px, py, pr, pg, pb);
		points.push_back(newPoint);
		i++;
	}
}

void polygon() {
	// Draw line between each point using Bresenham's line drawing algorithm
	outline(1, 0);
	outline(1, 2);
	outline(0, 3);
	outline(2, 3);

	// Sort points according to height
	sort(points.begin(), points.end(), comparePoints);

	// Interpolate each row and store new points and colors
	int a = points.size() - 1;
	for (int n=0; n < a; n++) {
		if (points[n].x > points[n+1].x) { n++; }
		shapeRow(points[n], points[n+1]);
	}

	// Draw all points
	glBegin(GL_POINTS);
	for(int i=0; i<points.size(); i++) {
		glColor3f(points[i].r, points[i].g, points[i].b);
		glVertex2f(points[i].x, points[i].y);
	}
	glEnd();
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	polygon();

 	glFlush();
	glutSwapBuffers();
}

void myReshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-2.0, 2.0, -2.0 * (GLfloat) h / (GLfloat) w,
	    		 2.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
	else
		glOrtho(-2.0 * (GLfloat) w / (GLfloat) h,
		         2.0 * (GLfloat) w / (GLfloat) h, -2.0, 2.0, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(500, 500);
    glutCreateWindow("Fill");

    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutMainLoop();
}
