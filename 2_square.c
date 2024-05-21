#include <GL/glut.h>
#include <stdlib.h>

/* globals */

GLsizei wh = 500, ww = 500; /* initial window size */
GLfloat size = 3.0;   /* half side length of square */


void drawSquare(int x, int y)
{

  y=wh-y;
  glColor3ub( (char) rand()%256, (char) rand()%256, (char) rand()%256);

  glBegin(GL_POLYGON);
  glVertex2f(x+size, y+size);
  glVertex2f(x-size, y+size);
  glVertex2f(x-size, y-size);
  glVertex2f(x+size, y-size);
  glEnd();
  glFlush();
  glutSwapBuffers();
}

void myReshape(GLsizei w, GLsizei h)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity(); 
  glOrtho(0.0, (GLdouble)w, 0.0, (GLdouble)h, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity(); 

  glViewport(0,0,w,h);
  glClearColor (0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
  glutSwapBuffers();

  ww = w;
  wh = h; 
}

void myinit(void)
{

  glViewport(0,0,ww,wh);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity(); 
  glOrtho(0.0, (GLdouble) ww , 0.0, (GLdouble) wh , -1.0, 1.0);

  glClearColor (0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
  
  glutReshapeFunc(myReshape);

}

void mouse(int btn, int state, int x, int y)
{
  if (btn == GLUT_LEFT_BUTTON)
  {
    size = 5.0;
  }
  else if (btn == GLUT_MIDDLE_BUTTON)
  {
    size = 10.0;
  }
  else if (btn == GLUT_RIGHT_BUTTON)
  {
    size = 15.0;
    if (state == GLUT_UP)
      exit(0);
  }
}

void keyboard(unsigned char key, int x, int y)
{
  if (key == 'q' || key == 'Q')
  {
    exit(0);
  }
}

void display(void)
{}

int main(int argc, char** argv)
{

  glutInit(&argc,argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  glutCreateWindow("square");
  myinit();

  glutReshapeFunc (myReshape);
  glutMouseFunc (mouse);
  glutKeyboardFunc(keyboard);
  glutMotionFunc(drawSquare);
  glutDisplayFunc(display);

  glutMainLoop();

}
