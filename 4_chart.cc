
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

class scrPt {
public:
    GLint x, y;
};

int curFcn = 1;
float z = 1.0;

const GLdouble twoPi = 6.283185;

void circleMidpoint (scrPt , GLint ); // fn. defined in circle.cc

GLsizei winWidth = 400, winHeight = 300;    // Initial display window size.
GLint xRaster = 25, yRaster = 150; // Initial raster position

GLubyte label[36] = { 'J', 'a', 'n',   'F', 'e', 'b',   'M', 'a', 'r',
		      'A', 'p', 'r',   'M', 'a', 'y',   'J', 'u', 'n',
		      'J', 'u', 'l',   'A', 'u', 'g',   'S', 'e', 'p',
		      'O', 'c', 't',   'N', 'o', 'v',   'D', 'e', 'c' };

GLint dataValue[12] = { 420, 342, 324, 310, 262, 185,
			190, 196, 217, 240, 213, 438 };



void init (void)
{
  glClearColor (1.0, 1.0, 1.0, 1.0);

  glMatrixMode (GL_PROJECTION);
  gluOrtho2D (0.0, (GLdouble)winWidth, 0.0, (GLdouble)winHeight);
}

void barChart (void)
{
  GLint month, k;

  glClear (GL_COLOR_BUFFER_BIT);       //  Clear display window.

  glColor3f (1.0, 0.0, 0.0);           //  Set bar color to red.
  for (k = 0; k < 12; k++)
    glRecti (20 + k*50, 165, 40 + k*50, dataValue [k]);

  glColor3f (0.0, 0.0, 0.0);          //  Set text color to black.
  xRaster = 0;                       //  Display chart labels.
  for (month = 0; month < 12; month++) {
    glRasterPos2i (xRaster, yRaster);
    for (k = 3*month; k < 3*month + 3; k++)
      glutBitmapCharacter (GLUT_BITMAP_HELVETICA_12, label [k]);
    xRaster += 50;
  }
  glFlush ( );
}

void lineChart (void)
{
  GLint x = 30;

  xRaster = 25;	yRaster = 150;

  glClear(GL_COLOR_BUFFER_BIT);	// clear display window

  glColor3f(0.0, 0.0, 1.0);	// set line colour to blue

  glBegin(GL_LINE_STRIP);
    for (int k = 0; k < 12; k++)
      glVertex2i(x + k*50, dataValue[k]);
  glEnd();

  glColor3f(1.0, 0.0, 0.0);	// Set marker colour to red
  for (int k = 0; k < 12; k++)
  {
    glRasterPos2i(xRaster + k*50, dataValue[k]-4);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '*');
  }

  glColor3f(0.0, 0.0, 0.0);	// black
  xRaster = 20;
  for (int m = 0; m < 12; m++)
  {
    glRasterPos2i(xRaster, yRaster);
    for (int k = 3*m; k < 3*m+3; k++)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, label[k]);
    xRaster += 50;
  }
  glFlush();
}

void pieChart (void)
{
    scrPt circCtr, piePt;
    GLint radius = winWidth / 4;               // Circle radius.

    GLdouble sliceAngle, previousSliceAngle = 0.0;

    GLint k, nSlices = 12;                     // Number of Slices. 
    GLfloat dataValues[12] = {10.0, 7.0, 13.0, 5.0, 13.0, 14.0,
                               3.0, 16, 5.0, 3.0, 17.0, 8.0};
    GLfloat dataSum = 0.0;

    circCtr.x = winWidth / 2;                  // Circle center position.
    circCtr.y = winHeight / 2;
    circleMidpoint (circCtr, radius);  // Call midpoint circle-plot routine.

    for (k = 0; k < nSlices; k++)
        dataSum += dataValues[k];

    for (k = 0; k < nSlices; k++) {
        sliceAngle = twoPi * dataValues[k] / dataSum + previousSliceAngle;
        piePt.x = (GLint) (circCtr.x + radius * cos (sliceAngle));
        piePt.y = (GLint) (circCtr.y + radius * sin (sliceAngle));
        glBegin (GL_LINES);
            glVertex2i (circCtr.x, circCtr.y);
            glVertex2i (piePt.x, piePt.y);
        glEnd ( );
        previousSliceAngle = sliceAngle;
    }
}

void displayFcn (void)
{
    glClear (GL_COLOR_BUFFER_BIT);   //  Clear display window.

    glColor3f (0.0, 0.0, 1.0);       //  Set circle color to blue.

    switch(curFcn) {
    case 0: lineChart(); break;
    case 1: barChart(); break;
    case 2: pieChart(); break;
    }
    glFlush ( );
}


void zoom(double factor) {
	z = factor;
  	glScalef(z, z, 1.0f);
	displayFcn();
}

void winReshapeFcn (int newWidth, int newHeight)
{
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ( );
    gluOrtho2D (0.0, GLdouble (newWidth), 0.0, GLdouble (newHeight));

    glClear (GL_COLOR_BUFFER_BIT);

    /*  Reset display-window size parameters.  */
    winWidth = newWidth;
    winHeight = newHeight;
    glViewport(0, 0, winWidth, winHeight);
    glutPostRedisplay();
}

void processChartEvents(int option) {
	switch(option) {
		case 0: curFcn = 0; break;
  		case 1: curFcn = 1; break;
  		case 2: curFcn = 2; break;
	}
	displayFcn();
}


void processZoomEvents(int option) {
	switch(option) {
		case 0: zoom(4.0); break;
  		case 1: zoom(2.0); break;
  		case 2: zoom(0.5); break;
		case 3: zoom(0.25); break;
	}
	displayFcn();
}

void processMainEvents(int button) {
  switch(button) {
  case 0: exit(0); break;
  }
  displayFcn();
}

void createGLUTMenus() {
  int mainMenu, chartMenu, zoomMenu;

  chartMenu = glutCreateMenu(processChartEvents);
  glutAddMenuEntry("Line", 0);
  glutAddMenuEntry("Pie", 2);
  glutAddMenuEntry("Bar", 1);

  zoomMenu = glutCreateMenu(processZoomEvents);
  glutAddMenuEntry("4.0x", 0);
  glutAddMenuEntry("2.0x", 1);
  glutAddMenuEntry("0.5x", 2);
  glutAddMenuEntry("0.25x", 3);


  mainMenu = glutCreateMenu(processMainEvents);
  glutAddSubMenu("Charts", chartMenu);
  glutAddSubMenu("Zoom", zoomMenu);
  glutAddMenuEntry("Quit", 0);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void keyboardFcn(unsigned char key, int x, int y) {
  switch(key) {
  	case 'l': curFcn = 0; break;
  	case 'b': curFcn = 1; break;
 	case 'p': curFcn = 2; break;
  	case 'q': exit(0); break;
	case 'z': zoom(2.0); break;
	case 'Z': zoom(0.5); break;
  }
  displayFcn();
}

int main (int argc, char** argv)
{
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition (100, 100);
    glutInitWindowSize (winWidth, winHeight);
    glutCreateWindow ("Charts");

    init ( );
    glutDisplayFunc (displayFcn);
    glutReshapeFunc (winReshapeFcn);
    glutKeyboardFunc(keyboardFcn);

    createGLUTMenus();

    glutMainLoop ( );
}
