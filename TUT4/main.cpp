//snowman

#include <GLUT/glut.h>  // GLUT, include glu.h and gl.h
#include <stdlib.h>
#include<iostream>
#include <cmath>

using namespace std;

// Global variables

float camx = 0;
float camy = 1.5;
float camz = -50;
float lookx = 0;
float looky = 1.5;
float lookz = 0;


float angle = -M_PI/2;
float angle2 = 0;

/* Initialize OpenGL Graphics */
void initGL()
{
    // Set "clearing" or background color
    glClearColor(0, 0, 0, 1); // White and opaque
    
    glEnable(GL_DEPTH_TEST); // turns on hidden surface removal so that objects behind other objects do not get displayed
}

void drawSnowMan()
{
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    
    // Draw Body
    glTranslatef(0.0, -0.25, 0.0);
    glutSolidSphere(0.75, 20, 20);
    
    // Draw Head
    glTranslatef(0.0, 0.95, 0.0);
    glutSolidSphere(0.25, 20, 20);
    
    // Draw Eyes
    glPushMatrix();
    glColor3f(0.0, 0.0, 1.0);
    glTranslatef(0.1, 0.1, -0.2);
    glutSolidSphere(0.05, 10, 10);
    glTranslatef(-0.2, 0.0, 0.0);
    glutSolidSphere(0.05, 10, 10);
    glPopMatrix();
    
    // Draw Nose
    glPushMatrix();
    glTranslatef(0.0, -0.04, -0.25);
    glRotatef(180, 0.0, 1.0, 0.0);
    glColor3ub(255, 182, 108);
    glutSolidCone(0.06, 0.3, 10, 2);
    glPopMatrix();
    
    glPopMatrix();
}


void render()
{
    
    // GL_DEPTH_BUFFER_BIT - resets the depth test values for hidden surface removal
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Reset transformations
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    

    
    gluLookAt(camx, camy, camz, // camera position
              lookx, looky, lookz, // look at position, line of sight
              0, 1, 0); // up direction (vector) for camera tilt


    
    // Draw ground
    glColor3ub(0,64,0); // green
    glBegin(GL_QUADS);
    glVertex3f(-100.0f, -2.0f, -100.0f);
    glVertex3f(-100.0f, -2.0f, 100.0f);
    glVertex3f(100.0f, -2.0f, 100.0f);
    glVertex3f(100.0f, -2.0f, -100.0f);
    glEnd();
    
    // Draw 36 SnowMen
    for (int i = -3; i < 3; i++)
        for (int j = -3; j < 3; j++)
        {
            glPushMatrix();
            glTranslated(i * 10.0, 0, j * 10.0);
            drawSnowMan();
            glPopMatrix();
        }
    
    
    glFlush();   // ******** DO NOT FORGET THIS **********
    
}


void update()
{
    
}


void display()
{
    
    update();
    render();
}

/* Handler for window re-size event. Called back when the window first appears and
 whenever the window is re-sized with its new width and height */

void reshape(int w, int h)
{
    
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if(h == 0)	h = 1;
    
    GLfloat ratio = 1.0f * (GLfloat)w / (GLfloat)h;
    
    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Set the clipping volume
    gluPerspective(45, ratio, 0.1, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    
    
    
    
}

void lookaround(){
    
    angle2 = angle;
    float lim = angle2 + 2*M_PI;
    
    do{
        angle2 = angle2 + M_PI/200;
        
        lookx = 50*cos(-angle2);
        lookz = 50*sin(-angle2)-50;
        render();
    }while(angle2 < lim);
    
    angle2 = angle;
    
}

void key (unsigned char key, int xx, int yy){
    
    switch (key) {
        
        case 'r':
            camx = 0;
            camz = -50;
            lookx = 0;
            lookz = 0,0;
            angle = -M_PI/2;
            break;
            
        case 'q':
            looky = looky + 1;
            break;
        
        case 'a':
            looky = looky - 1;
            break;
        
        case 'l':
            lookaround();
            break;
        
        case 27:
            exit(1);
            break;
            
        default:
            break;
    }
    
    glutPostRedisplay();

    
}

void arrowKey(int key, int xx, int y){
    
    
    switch (key) {
        case GLUT_KEY_RIGHT :
            
            angle = angle - M_PI/20;
            break;
            
        case GLUT_KEY_LEFT :

            angle = angle + M_PI/20;
            break;
            
        case GLUT_KEY_DOWN :
            
            camz = camz - sin(-angle);
            camx = camx - cos(-angle);
            break;
            
        case GLUT_KEY_UP :
    
            camz = camz + sin(-angle);
            camx = camx + cos(-angle);
            break;
    }
    
    cout << "lookx " << lookx << "   lookz "<<lookz<<"  angle " <<angle<<endl;
    
    lookx = cos(-angle) + camx;
    lookz = sin(-angle) + camz;

    glutPostRedisplay();
}





/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv)
{
    glutInit(&argc, argv);          // Initialize GLUT
    
    glutInitWindowSize(640, 480);   // Set the window's initial width & height - non-square
    glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
    glutCreateWindow("Model Transform");  // Create window with the given title
    
    glutDisplayFunc(display);       // Register callback handler for window re-paint event
    glutReshapeFunc(reshape);
    glutKeyboardFunc(key);
    glutSpecialFunc(arrowKey);

    initGL();                       // Our own OpenGL initialization
    
    glutMainLoop();                 // Enter the infinite event-processing loop
    return 0;
}