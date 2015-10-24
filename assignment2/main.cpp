#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include <cmath>
#include"Vector.h"
#include "Camera.h"
#include "SpringMesh.h"
#include "gauss.h"


#define automaticSpeed 50
#define VERTEXNUMBER 6
#define STRUTNUMBER 9
#define FACENUMBER 4

#define ERRORTHRESH 0.01

#define Cd 0.5
#define Cl 0.5

#define Ksita 0.5
#define Dsita 0.5

#define SITA0 PI

double hStep=0.1;

int WIDTH = 1000;
int HEIGHT = 800;

Camera *camera;

bool  resetSign = false;
bool showGrid = true;

SpringMesh springMesh;


Vector3d vertexStates[VERTEXNUMBER*2];
Vector3d vertexStatesNew[VERTEXNUMBER*2];
Vector3d vertexStatesA[VERTEXNUMBER*2];

Vector3d wind(0,0,0);

// draws a simple grid
void makeGrid()
{
    //Setting  material
    GLfloat ball_mat_ambient[]  = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat ball_mat_diffuse[]  = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat ball_mat_specular[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat ball_mat_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat ball_mat_shininess  = 10.0f;
    
    glMaterialfv(GL_FRONT, GL_AMBIENT,   ball_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   ball_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  ball_mat_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION,  ball_mat_emission);
    glMaterialf (GL_FRONT, GL_SHININESS, ball_mat_shininess);
    
    glLineWidth(1.0);
    
    for (float i=-12; i<12; i++) {
        for (float j=-12; j<12; j++) {
            glBegin(GL_LINES);
            glVertex3f(i, 0, j);
            glVertex3f(i, 0, j+1);
            glEnd();
            glBegin(GL_LINES);
            glVertex3f(i, 0, j);
            glVertex3f(i+1, 0, j);
            glEnd();
            
            if (j == 11){
                glBegin(GL_LINES);
                glVertex3f(i, 0, j+1);
                glVertex3f(i+1, 0, j+1);
                glEnd();
            }
            if (i == 11){
                glBegin(GL_LINES);
                glVertex3f(i+1, 0, j);
                glVertex3f(i+1, 0, j+1);
                glEnd();
            }
        }
    }
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glVertex3f(-12, 0, 0);
    glVertex3f(12, 0, 0);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(0, 0, -12);
    glVertex3f(0, 0, 12);
    glEnd();
    glLineWidth(1.0);
}

void mouseEventHandler(int button, int state, int x, int y) {
    // let the camera handle some specific mouse events (similar to maya)
    camera->HandleMouseEvent(button, state, x, y);
    glutPostRedisplay();
}

void motionEventHandler(int x, int y) {
    // let the camera handle some mouse motions if the camera is to be moved
    camera->HandleMouseMotion(x, y);
    glutPostRedisplay();
}



double calAngle(Vector3d& a, Vector3d& b)
{
    return acos(a.normalize() * b.normalize())*180/PI;
}



void myDisplay(void)
{
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    // draw the camera created in perspective
    camera->PerspectiveDisplay(WIDTH, HEIGHT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0,0,0);
    glPushMatrix();
    if (showGrid)
        makeGrid();
    glPopMatrix();
     
    //Setting ball material
    GLfloat ball_mat_ambient[]  = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat ball_mat_diffuse[]  = {0.6f, 0.3f, 0.1f, 1.0f};
    GLfloat ball_mat_specular[] = {0.6f, 0.3f, 0.1f, 1.0f};
    GLfloat ball_mat_emission[] = {0.6f, 0.3f, 0.1f, 1.0f};
    GLfloat ball_mat_shininess  = 10.0f;
    
    glMaterialfv(GL_FRONT, GL_AMBIENT,   ball_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   ball_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  ball_mat_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION,  ball_mat_emission);
    glMaterialf (GL_FRONT, GL_SHININESS, ball_mat_shininess);
    

    
    
    int PIN[4][3] = {{0,4,3}, {0,1,4}, {1,5,4}, {1,2,5}};
    for (int i=0; i<4; i++) {
        
        float a= 0.6/4*i+0.1;
        float b = 0.5/4*(4-i) + 0.1;
        float c = 0.4/4*(4-i) + 0.1;
        
        
        GLfloat ball_mat_ambient[]  = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat ball_mat_diffuse[]  = {a, b, c, 1.0f};
        GLfloat ball_mat_specular[] = {a, b, c, 1.0f};
        GLfloat ball_mat_emission[] = {a, b, c, 1.0f};
        GLfloat ball_mat_shininess  = 8.0f;
        
        glMaterialfv(GL_FRONT, GL_AMBIENT,   ball_mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,   ball_mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR,  ball_mat_specular);
        glMaterialfv(GL_FRONT, GL_EMISSION,  ball_mat_emission);
        glMaterialf (GL_FRONT, GL_SHININESS, ball_mat_shininess);
        
        
        
        
        glBegin(GL_TRIANGLES);
        glVertex3f(springMesh.points[PIN[i][0]].xposition.x, springMesh.points[PIN[i][0]].xposition.y, springMesh.points[PIN[i][0]].xposition.z);
        glVertex3f(springMesh.points[PIN[i][1]].xposition.x, springMesh.points[PIN[i][1]].xposition.y, springMesh.points[PIN[i][1]].xposition.z);
        glVertex3f(springMesh.points[PIN[i][2]].xposition.x, springMesh.points[PIN[i][2]].xposition.y, springMesh.points[PIN[i][2]].xposition.z);
        glEnd();
    }
    
    
    

    
    glFlush();
    glutSwapBuffers();
}



//*******************  calculate states *******************//

void accelerationUpdate(Vector3d* states, Vector3d* statesNew, Vector3d* statesA)
{
    
    //calculate springForce and dampForce
    for (int k=0; k<STRUTNUMBER; k++) {
        int i = springMesh.struts[k].vertexIndices[0];
        int j = springMesh.struts[k].vertexIndices[1];
        
        double Kij = springMesh.struts[k].k;
        double Dij = springMesh.struts[k].d;
        double L0ij = springMesh.struts[k].l;
        Vector3d Xij = springMesh.points[j].xposition - springMesh.points[i].xposition;
        Vector3d XijN = Xij.normalize();
        double Lij = Xij.norm();
        
        //set the threshhold
        
        Vector3d Fis(0,0,0);
        Vector3d Fjs(0,0,0);
        Vector3d Fid(0,0,0);
        Vector3d Fjd(0,0,0);
        if (abs(Lij -L0ij)>ERRORTHRESH) {
            Fis = Kij*(Lij - L0ij)* XijN;
            Fjs = -1*Fis;
        }
        if ((springMesh.points[j].velocity - springMesh.points[i].velocity).norm()>ERRORTHRESH) {
            Fid = Dij*((springMesh.points[j].velocity - springMesh.points[i].velocity)*XijN)*XijN;
            Fjd = -1*Fid;
        }
       
        
        springMesh.points[i].force = springMesh.points[i].force + Fis + Fid;
        springMesh.points[j].force = springMesh.points[j].force + Fjs + Fjd;
    }
    
    /*
    //calculate AirResistance and wind
    for (int faceI=0; faceI<FACENUMBER; faceI++) {
        double angle0 = springMesh.faces[faceI].vertexAngles[0];
        double angle1 = springMesh.faces[faceI].vertexAngles[1];
        double angle2 = springMesh.faces[faceI].vertexAngles[2];
        
        int pointA = springMesh.faces[faceI].pointIndices[0];
        int pointB = springMesh.faces[faceI].pointIndices[1];
        int pointC = springMesh.faces[faceI].pointIndices[2];
        Vector3d V = angle0/180.0 * springMesh.points[pointA].velocity +
                        angle1/180.0 * springMesh.points[pointB].velocity +
                       angle2/180.0 * springMesh.points[pointC].velocity;
        
        if (faceI == 2) {
            //std::cout<<angle2;
            //std::cout<<V<<"\n";
        }
        
        
        Vector3d edge0 = springMesh.points[pointB].xposition - springMesh.points[pointA].xposition;
        Vector3d edge1 = springMesh.points[pointC].xposition - springMesh.points[pointA].xposition;
       
        Vector3d aN = (edge0%edge1).normalize();
        Vector3d Vr = V-wind;
        Vector3d VrN = Vr.normalize();
        double Area = 0.5*(edge0%edge1).norm();
        double Ae =Area * (aN * VrN);
        
        Vector3d Fd = -1 * Cd * Ae * Vr;
        Vector3d Fl = -1 * Cl * Ae * (Vr % ((aN%Vr).normalize()));
        
        //std::cout<<Fl<<"\n";
        
        springMesh.points[pointA].force = springMesh.points[pointA].force + angle0/180.0*(Fd+Fl);
        springMesh.points[pointB].force = springMesh.points[pointB].force + angle1/180.0*(Fd+Fl);
        springMesh.points[pointC].force = springMesh.points[pointC].force + angle2/180.0*(Fd+Fl);
        
    }
    */
    
    //calculate Torque
    for (int k=0; k<STRUTNUMBER; k++) {
        if (springMesh.struts[k].faceIndices[0]!=-1 && springMesh.struts[k].faceIndices[1] != -1) {
            int faceA = springMesh.struts[k].faceIndices[0];
            int faceB = springMesh.struts[k].faceIndices[1];
            int vertexC = 0;
            int vertexD = 0;
            int vertexA = springMesh.struts[k].vertexIndices[0];
            int vertexB = springMesh.struts[k].vertexIndices[1];
            
            
            for (int q=0; q<3; q++) {
                vertexC += springMesh.faces[faceA].pointIndices[q];
            }
            vertexC = vertexC - vertexA - vertexB;
            
            for (int q=0; q<3; q++) {
                vertexD += springMesh.faces[faceB].pointIndices[q];
            }
            vertexD = vertexD - vertexA - vertexB;
            
            Vector3d Xab = springMesh.points[vertexB].xposition - springMesh.points[vertexA].xposition;
            Vector3d Xac = springMesh.points[vertexC].xposition - springMesh.points[vertexA].xposition;
            Vector3d Xad = springMesh.points[vertexD].xposition - springMesh.points[vertexD].xposition;
            
            Vector3d hN = Xab.normalize();
            
            double Dac = Xac * hN;
            double Dad = Xad * hN;
            
            Vector3d Rl = Xac - Dac*hN;
            Vector3d Rr = Xad - Dad*hN;
            
            Vector3d Nl = (Xac % Xab).normalize();
            Vector3d Nr = (Xad % Xab).normalize();
            
            
            
            
            double SITA = atan2((Nl%Nr)*hN, Nl*Nr);
            double SITAl = springMesh.points[vertexC].velocity * Nl / Rl.norm();
            double SITAr = springMesh.points[vertexD].velocity * Nr / Rr.norm();
            
            Vector3d T = (Ksita*(SITA-SITA0) - Dsita*(SITAl-SITAr))*hN;
            
            Vector3d Fd = (T*hN)/(Rr.norm()) * Nr;
            Vector3d Fc = (T*hN)/(Rl.norm()) * Nl;
            
            Vector3d Fb = -1 * (Dac*Fc + Dad*Fd) / (Xab.norm()) ;
            
            Vector3d Fa = -1 * (Fc+Fb+Fd);
            
            
            springMesh.points[vertexA].velocity = springMesh.points[vertexA].velocity + Fa;
            springMesh.points[vertexB].velocity = springMesh.points[vertexB].velocity + Fb;
            springMesh.points[vertexC].velocity = springMesh.points[vertexC].velocity + Fc;
            springMesh.points[vertexD].velocity = springMesh.points[vertexD].velocity + Fd;
            
            //std::cout<<"\n";
            
        }
    }
    
    
    for (int k=0; k<VERTEXNUMBER; k++) {

        statesA[k+VERTEXNUMBER] = springMesh.points[k].force*(1.0/springMesh.points[k].mass);
    }
}

//system Dynamical Function
void sysDynaFunc(Vector3d* states, Vector3d* statesNew, Vector3d* statesA)
{
    accelerationUpdate(states, statesNew, statesA);
    for (int i =0; i<VERTEXNUMBER; i++) {
        statesA[i] = states[i+VERTEXNUMBER];
    }
}

//numerical integration function
void statesNumInt(Vector3d* states, Vector3d* statesNew, Vector3d* statesA)
{
    for (int i = 0; i<VERTEXNUMBER*2; i++) {
        statesNew[i] = states[i] + statesA[i]*hStep;
    }
}


void update()
{
    for (int i=0; i<VERTEXNUMBER; i++) {
        vertexStates[i] = springMesh.points[i].xposition;
        vertexStates[i+VERTEXNUMBER] = springMesh.points[i].velocity;
    }
    
    sysDynaFunc(vertexStates, vertexStatesNew, vertexStatesA);
    statesNumInt(vertexStates, vertexStatesNew, vertexStatesA);
    for (int i=0; i<VERTEXNUMBER; i++) {
        springMesh.points[i].xposition = vertexStatesNew[i];
        if (i==0 || i==3) {
            springMesh.points[i].velocity = Vector3d(0,0,0);
        }
        else
        {
            //limit
            if (vertexStatesNew[i+VERTEXNUMBER].norm() > 3) {
                vertexStatesNew[i+VERTEXNUMBER] = Vector3d(0,0,0);
            }
            springMesh.points[i].velocity = vertexStatesNew[i+VERTEXNUMBER];
        }
    }
    
    
    
    //update angle
    for (int faceI=0; faceI<FACENUMBER; faceI++) {
        
        int VertexIndex[3][2];
        int faceIndex = 0;
        for (int k=0; k<3; k++) {
            faceIndex = springMesh.faces[faceI].strutIndices[k];
            VertexIndex[k][0] = springMesh.struts[faceIndex].vertexIndices[0];
            VertexIndex[k][1] = springMesh.struts[faceIndex].vertexIndices[1];
        }
        
        Vector3d edge[3];
        for (int k=0; k<3; k++) {
            edge[k] = springMesh.points[VertexIndex[k][1]].xposition -
            springMesh.points[VertexIndex[k][0]].xposition;
            
        }
        
        for (int k=0; k<3; k++) {
            if (VertexIndex[k][0] == VertexIndex[(k+1)%3][0]  ||
                VertexIndex[k][1] == VertexIndex[(k+1)%3][1]) {
                springMesh.faces[faceI].vertexAngles[k] = calAngle(edge[k],edge[(k+1)%3]);
            }
            else
            {
                springMesh.faces[faceI].vertexAngles[k] = 180 - calAngle(edge[k],edge[(k+1)%3]);
            }
            
            //std::cout<<springMesh.faces[faceI].vertexAngles[k]<<"\n";
        }
        //std::cout<<springMesh.points[0].force<<"\n";
    }
    
    
}

void leadWing()
{
    if (springMesh.points[5].xposition.z>0.3) {
        springMesh.points[5].velocity.z = -0.3;
    }
    if (springMesh.points[5].xposition.z<-0.3) {
        springMesh.points[5].velocity.z = +0.3;
    }
    
    if (springMesh.points[2].xposition.z>0.3) {
        springMesh.points[2].velocity.z = -0.3;
    }
    if (springMesh.points[2].xposition.z<-0.3) {
        springMesh.points[2].velocity.z = +0.3;
    }
}

//*******************************************************//


void timeProc(int id)
{
    if (id == 1) {
        leadWing();
        update();
        
        glutPostRedisplay();
        if (resetSign == false) {
            glutTimerFunc(automaticSpeed, timeProc, 1);
        }
    }
    
}

void handleKey(unsigned char key, int x, int y){
    switch(key){
        case 'a':
        case 'A':
            
            resetSign = false;
            glutTimerFunc(automaticSpeed, timeProc, 1);
            break;
        case 's':
        case 'S':
            resetSign = true;
            break;
            
        case 'g':
        case 'G':
            showGrid = !showGrid;
            break;
            
        case 'q':       // q - quit
        case 'Q':
        case 27:        // esc - quit
            exit(0);
            
        default:        // not a valid key -- just ignore it
            return;
    }
}


void init() {
    springMesh.show();
    //LoadParameters(ParamFilename);
    // set up camera
    // parameters are eye point, aim point, up vector
    camera = new Camera(Vector3d(0, 0, 1), Vector3d(0, 0, 0), Vector3d(0, 1, 0));
    
    // black background for window
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    //glDepthRange(0, 1);
    
    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    
    
    //Setting lights
    
    GLfloat light0_position[] = {0.0f, 0.0f, 30.0f, 1.0f}; //spot light
    GLfloat light1_position[] = {0.0f, 30.0f, 0.0f, 1.0f};
    GLfloat light0_ambient[]  = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat light0_diffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light0_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    
    
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    
    
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glLightfv(GL_LIGHT1, GL_AMBIENT,  light0_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  light0_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light0_specular);
    
    
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
    
    for (int i=0; i<VERTEXNUMBER; i++) {
        vertexStates[i] = springMesh.points[i].xposition;
        vertexStates[i+VERTEXNUMBER] = springMesh.points[i].velocity;
        if (i==2 || i==5 ) {
            springMesh.points[i].velocity.z = 0.5;
        }
        
    }
    
}


int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Particle");
    
    init();
    
    glutDisplayFunc(myDisplay);
    glutMouseFunc(mouseEventHandler);
    glutMotionFunc(motionEventHandler);
    glutKeyboardFunc(handleKey);
    
    glutMainLoop();
    
    
    return 0;
}