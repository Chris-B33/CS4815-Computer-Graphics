#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

class wcPt3D {
   public:
      GLfloat x, y, z;
};

/* Ctrl Points Global variables */
GLint nCtrlPts = 6, nBezCurvePts = 1000;
wcPt3D ctrlPts [6] = {};
bool line_visibility = true;
int selected_point = -1;
int downX = 0, downY = 0;

/*  Set initial size of the display window.  */
GLsizei winWidth = 600, winHeight = 600;

/*  Set size of world-coordinate clipping window.  */
GLfloat xwcMin = -50.0, xwcMax = 50.0;
GLfloat ywcMin = -50.0, ywcMax = 50.0;

void init (void)
{
   /*  Set color of display window to white.  */
   glClearColor (1.0, 1.0, 1.0, 0.0);
}


void updateDisplayExtremes() {
	for (int i=0; i<nCtrlPts; i++) {
		int x = ctrlPts[i].x;
		int y = ctrlPts[i].y;

		if (x < xwcMin) { xwcMin = x - 10; }
		if (x > xwcMax) { xwcMax = x + 10; }
		if (y < ywcMin) { ywcMin = y - 10; }
		if (y > ywcMax) { ywcMax = y + 10; }
	}
}

/*  Compute binomial coefficients C for given value of n.  */
void binomialCoeffs (GLint n, GLint * C)
{
   GLint k, j;

   for (k = 0;  k <= n;  k++) {
      /*  Compute n!/(k!(n - k)!).  */
      C [k] = 1;
      for (j = n;  j >= k + 1;  j--)
        C [k] *= j;
      for (j = n - k;  j >= 2;  j--)
        C [k] /= j;
   }
}

void computeBezPt (GLfloat t, wcPt3D * bezPt, GLint nCtrlPts,
                    wcPt3D * ctrlPts, GLint * C)
{
   GLint k, n = nCtrlPts - 1;
   GLfloat bezBlendFcn;

   bezPt->x = bezPt->y = bezPt->z = 0.0;

   /*  Compute blending functions and blend control points. */
   for (k = 0; k < nCtrlPts; k++) {
      bezBlendFcn = C [k] * pow (t, k) * pow (1 - t, n - k);
      bezPt->x += ctrlPts [k].x * bezBlendFcn;
      bezPt->y += ctrlPts [k].y * bezBlendFcn;
      bezPt->z += ctrlPts [k].z * bezBlendFcn;
   }
}

void bezier (wcPt3D * ctrlPts, GLint nCtrlPts, GLint nBezCurvePts)
{
   wcPt3D bezCurvePt;
   GLfloat t;
   GLint *C;

   /*  Allocate space for binomial coefficients  */
   C = new GLint [nCtrlPts];

   binomialCoeffs (nCtrlPts - 1, C);

   glBegin(GL_LINE_STRIP);

   for (int i = 0;  i <= nBezCurvePts;  i++) {
      t = GLfloat (i) / GLfloat (nBezCurvePts);
      computeBezPt (t, &bezCurvePt, nCtrlPts, ctrlPts, C);
      glVertex2f(bezCurvePt.x, bezCurvePt.y);
   }
   glEnd();
   delete [ ] C;
}

void displayFcn (void)
{
   glClear (GL_COLOR_BUFFER_BIT);   //  Clear display window.

   glPointSize(5);

   if (line_visibility) {
   	glColor3f(1.0, 0.0, 0.0);
   	glBegin(GL_POINTS);
   	for (int i=0; i<nCtrlPts; i++) {
		glVertex2f(ctrlPts[i].x, ctrlPts[i].y);
   	}
   	glEnd();

	glColor3f(0.0, 1.0 ,0.0);
	glBegin(GL_LINE_STRIP);
   	for (int i=0; i<nCtrlPts; i++) {
		glVertex2f(ctrlPts[i].x, ctrlPts[i].y);
   	}
	glEnd();
   }
   glColor3f (0.5, 0.8, 1.0);
   bezier(ctrlPts, nCtrlPts, nBezCurvePts);
   updateDisplayExtremes();
   glutSwapBuffers();
}

void winReshapeFcn (GLint newWidth, GLint newHeight)
{
   /*  Maintain an aspect ratio of 1.0.  */
   glViewport (0, 0, newHeight, newHeight);

   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ( );

   gluOrtho2D (xwcMin, xwcMax, ywcMin, ywcMax);

   glutPostRedisplay();
}

double distance(float x1, float y1, float x2, float y2) {
	return std::sqrt(std::pow((x2 - x1), 2) + std::pow((y2-y1), 2));
}

void mouseFcn(int btn, int state, int x, int y) {
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN && line_visibility) {
		double cwx = ((double)x / winWidth) * (xwcMax - xwcMin) + xwcMin;
		double cwy = ((winHeight - (double)y) / winHeight) * (ywcMax - ywcMin) + ywcMin;
		double scale_factor = (xwcMax - xwcMin) / winWidth;
		double closest_dis = 20 * scale_factor;

		for (int i=0; i< nCtrlPts; i++) {
			double dis = distance(cwx, cwy, ctrlPts[i].x, ctrlPts[i].y);
			if (dis < closest_dis) {
				selected_point = i;
				closest_dis = dis;
			}
		}
	} else {
		selected_point = -1;
	}
}

void motionFcn(int x, int y) {
	if (selected_point != -1) {
		double cwx = ((double)x / winWidth) * (xwcMax - xwcMin) + xwcMin;
		double cwy = ((winHeight - (double)y) / winHeight) * (ywcMax - ywcMin) + ywcMin;

		ctrlPts[selected_point].x = cwx;
		ctrlPts[selected_point].y = cwy;

		updateDisplayExtremes();
	}
	winReshapeFcn(winWidth, winHeight);
}

void keyboardFcn(unsigned char key, int x, int y) {
  switch(key) {
	case 's': line_visibility = !line_visibility; break;
  }
  glutPostRedisplay();
}

int main (int argc, char** argv)
{
   srand((unsigned int)time(NULL));
   for (int i=0; i<nCtrlPts; i++) {
	float x = (rand() % 201) - 100.0f;
	float y = (rand() % 201) - 100.0f;

	ctrlPts[i] = {x, y};
   }

   updateDisplayExtremes();

   glutInit (&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowPosition (50, 50);
   glutInitWindowSize (winWidth, winHeight);
   glutCreateWindow ("Bezier Curve");

   init ( );
   glutDisplayFunc (displayFcn);
   glutReshapeFunc (winReshapeFcn);
   glutMouseFunc(mouseFcn);
   glutMotionFunc(motionFcn);
   glutKeyboardFunc(keyboardFcn);

   glutMainLoop ( );
}
