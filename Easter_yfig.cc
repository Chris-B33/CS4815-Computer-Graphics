/* --------------------------------------------------------------------------------- */
/* |                             IMPORTS & VARIABLES                               | */
/* --------------------------------------------------------------------------------- */

#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <vector>

#if !defined(GLUT_WHEEL_UP)
#define GLUT_WHEEL_UP 3
#define GLUT_WHEEL_DOWN 4
#endif

#define QUIT -1

class wcPt2d {
	public:
		GLfloat x, y;
};

GLsizei winWidth = 600, winHeight = 600;
GLfloat xwcMin = -50.0, xwcMax = 50.0;
GLfloat ywcMin = -50.0, ywcMax = 50.0;

int downX, downY;
bool middle_pressed = false;
int shapeMode = 1;
bool firstPress = true;
int firstX, firstY;

float zoom = 1;
int thickness = 3;
float red = 1.0, green = 0.0, blue = 0.0;



/* --------------------------------------------------------------------------------- */
/* |                               INIT FUNCTION                                   | */
/* --------------------------------------------------------------------------------- */

void init(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(red, green, blue);
  glPointSize(thickness);
}



/* --------------------------------------------------------------------------------- */
/* |                               SHAPE FUNCTIONS                                 | */
/* --------------------------------------------------------------------------------- */

float sqr(float val) { return val * val; }

void line(int x1, int y1, int x2, int y2) {
  glBegin(GL_LINES);
  glVertex2i(x1, y1);
  glVertex2i(x2, y2);
  glEnd();
}

void rect(int x1, int y1, int x2, int y2) {
  glBegin(GL_LINE_STRIP);
  glVertex2i(x1, y1);
  glVertex2i(x1, y2);
  glVertex2i(x2, y2);
  glVertex2i(x2, y1);
  glVertex2i(x1, y1);
  glEnd();
}

void circle(int x1, int y1, int x2, int y2) {
  float radius = sqrt(sqr(x2 - x1) + sqr(y2 - y1));
  int points = 36 * radius;
  float theta = 360.0 / points;

  glBegin(GL_POINTS);
  while (points > 0) {
    glVertex2f(x1 + radius * cos(theta * points), y1 + radius * sin(theta * points));
    points--;
  }
  glEnd();
}

class Shape {
	public:
		int type;
		int x1;
		int y1;
		int x2;
		int y2;
		int thick;
		double red;
		double green;
		double blue;

		Shape(int _type, int _x1, int _y1, int _x2, int _y2, int _thickness, double _red, double _green, double _blue) {
			type =_type;
			x1 = _x1;
			y1 = _y1;
			x2 = _x2;
			y2 = _y2;
			thick = _thickness;
			red = _red;
			green = _green;
			blue = _blue;
		}

		void draw() {
			glColor3f(red, green, blue);
			glPointSize(thick);
			glLineWidth(thick);
			if (type == 1) {
				line(x1, y1, x2, y2);
			}
			if (type == 2) {
				rect(x1, y1, x2, y2);
			}
			if (type == 3) {
				circle(x1, y1, x2, y2);
			}
		}
};

std::vector<Shape> elements;



/* --------------------------------------------------------------------------------- */
/* |                             DISPLAY FUNCTIONS                                 | */
/* --------------------------------------------------------------------------------- */

void displayFcn(void)
{
  glClear(GL_COLOR_BUFFER_BIT);

  for (Shape s : elements) {
	s.draw();
  }

  glutSwapBuffers();
}

void winReshapeFcn(GLint newWidth, GLint newHeight)
{
  glViewport(0, 0, newHeight, newHeight);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(
	(xwcMin - (xwcMax + xwcMin) / 2)  * zoom + (xwcMax + xwcMin) / 2,
	(xwcMax - (xwcMax + xwcMin) / 2)  * zoom + (xwcMax + xwcMin) / 2,
	(ywcMin - (ywcMax + ywcMin) / 2)  * zoom + (ywcMax + ywcMin) / 2,
	(ywcMax - (ywcMax + ywcMin) / 2)  * zoom + (ywcMax + ywcMin) / 2
  );

  glutPostRedisplay();
}



/* --------------------------------------------------------------------------------- */
/* |                            MOUSE AND KEYBOARD                                 | */
/* --------------------------------------------------------------------------------- */

void mouse(int btn, int state, int x, int y) {
  downX = x; downY = y;
  double cwx = ((double)x / winWidth) * (xwcMax-xwcMin) + xwcMin;
  double cwy =  ((winHeight-(double)y) / winHeight) * (ywcMax-ywcMin) + ywcMin;
  if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
      if (firstPress == true) {
         firstX = cwx;
	 firstY = cwy;
	 firstPress = false;
      }
      else {
	 elements.push_back(Shape(shapeMode, firstX, firstY, cwx, cwy, thickness, red, green, blue));
	 firstPress = true;
      }
  }

  middle_pressed = ((btn == GLUT_MIDDLE_BUTTON) && (state == GLUT_DOWN));

  if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
      if (btn == GLUT_WHEEL_DOWN) {
	 zoom *= 1.1;
      } else if (btn == GLUT_WHEEL_UP) {
	 zoom /= 1.1;
      }
  }

  winReshapeFcn(winWidth, winHeight);
}

void motion(int x, int y) {
  if (middle_pressed) {
     xwcMin += (float)(downX - x) / 8, xwcMax += (float)(downX - x) / 8;
     ywcMin -= (float)(downY - y) / 8, ywcMax -= (float)(downY - y) / 8;
  }
  downX = x; downY = y;
  winReshapeFcn(winWidth, winHeight);
}

void keyboard(unsigned char key, int x, int y) {
  switch (key) {
	case '1': shapeMode = 1; break;
	case '2': shapeMode = 2; break;
	case '3': shapeMode = 3; break;
	case 'c': init(); break;
  };
  winReshapeFcn(winWidth, winHeight);
}


/* --------------------------------------------------------------------------------- */
/* |                               MENU FUNCTIONS                                  | */
/* --------------------------------------------------------------------------------- */

void processColourEvents(int option) {
  switch(option) {
	case 1: red = (red == 1.0) ? 0.0 : 1.0; break;
	case 2: green = (green == 1.0) ? 0.0 : 1.0; break;
	case 3: blue = (blue == 1.0) ? 0.0 : 1.0; break;
  }
}

void processThicknessEvents(int option) {
  switch(option) {
	case 1: thickness = 3; break;
	case 2: thickness = 5; break;
	case 3: thickness = 7; break;
  }
}

void processMainEvents(int option) {
  switch(option) {
	case QUIT: exit(0); break;
  }
}

void createGlutMenus() {
  int mainMenu, thicknessMenu, colourMenu;

  colourMenu = glutCreateMenu(processColourEvents);
  glutAddMenuEntry("Red", 1);
  glutAddMenuEntry("Green", 2);
  glutAddMenuEntry("Blue", 3);

  thicknessMenu = glutCreateMenu(processThicknessEvents);
  glutAddMenuEntry("Thin", 1);
  glutAddMenuEntry("Regular", 2);
  glutAddMenuEntry("Thick", 3);

  mainMenu = glutCreateMenu(processMainEvents);
  glutAddSubMenu("Thickness", thicknessMenu);
  glutAddSubMenu("Toggle Colours", colourMenu);
  glutAddMenuEntry("Quit", QUIT);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}


/* --------------------------------------------------------------------------------- */
/* |                                MAIN FUNCTION                                  | */
/* --------------------------------------------------------------------------------- */

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowPosition(50, 50);
  glutInitWindowSize(winWidth, winHeight);
  glutCreateWindow("YFig");

  init();
  glutDisplayFunc(displayFcn);
  glutReshapeFunc(winReshapeFcn);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutKeyboardFunc(keyboard);
  createGlutMenus();

  elements.push_back(Shape(1, -1000000, 0, 1000000, 0, 1, 0.5, 0.8, 1.0));
  elements.push_back(Shape(1, 0, -1000000, 0, 1000000, 1, 0.5, 0.8, 1.0));

  glutMainLoop();
}
