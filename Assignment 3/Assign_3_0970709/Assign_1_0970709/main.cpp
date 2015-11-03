
/* CS 3GC3 Assignment 3
 * Liu Wenbo
 * 0970709
 * This program is coded on Mac computer.
 * Extra Fetures: Winds; Rain/Snow; Floor Hole; Particle Camera; Wire mode for particle system.
 *
 * -------------------------ON MAIN TERRAIN WINDOW---------------------------------
 * Button 'q' Quit, it kills the quite terrain.
 * Button 'l' Enable/Disable lighting
 * 'Space' bar for stop the particle appear
 * 'r' for reset the particles
 * 'w' change the program from fill mode to line mode
 * 'f' turns on/off the friction
 * 'b' for turns on/off the backface culling effects.
 * 'p' particle camera turn on.
 * Button arrow UP rotates the y axis
 * Button arrow DOWN rotates the y axis
 * Button arrow LEFT rotates the x axis
 * Button arrow RIGHTrotates the x axis
 * Button '1' is normal mode
 * Button '2' is rain mode
 * Button '3' is snow
 * Button 'z' starts the winds from left to right
 * Button 'x' stop the winds from left to right
 * Button 'c' starts the winds from front to back
 * Button 'v' stop the winds from front to back
 *  
 */
#include <vector>
#include <iostream>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include "stdafx.h"
#include <windows.h>
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

using namespace std;


int makeRain = 1;
float eye[3] = {15.0f,15.0f,-15.0f};//starting camera point
int axis = 0;
float light_pos[] = {5,5,5,1.0};    //first light source
float light_pos2[] = {-5,5,-5, 1.0};//second light source

const int size = 500;               //particle numbers 


float cam = 0;
int wind = 1000;
float windl = 0;
float windr = 0;
// angle of rotation for the particledirection
float ang=0.0;
int cnt = 0;
int value = 1;
bool winds = false;     //turn on/ off the winds
bool show = true;       //show and pause the particle 
bool friction = true;   //turn on and off the friction
bool backface = true;   //turn on and off the backface_culling
bool lightOn = true;    //turn on and off the lighting
bool particleview = false;

/* drawCube() -- draws a base for the particle system */
void drawCube()
{
	glBegin(GL_QUADS);

    //Front 	
	glColor3f(0.4, 0, 0.7);
    glVertex3f(-15, -1, -15);
    glVertex3f(-15, 0, -15);
    glVertex3f(15, 0, -15); 
    glVertex3f(15, -1, -15);

    
   //left side
  //  glColor3f(0, 1, 1);
    glVertex3f(-15,0,15);
    glVertex3f(-15,-1,15);
    glVertex3f(-15,-1,-15);
    glVertex3f(-15,0,-15);
    
    //back side
    //glColor3f(0, 1, 1);
    glVertex3f(-15, -1, 15);
    glVertex3f(-15, 0, 15);
    glVertex3f(15, 0, 15);    
    glVertex3f(15, -1, 15);
    
    //right side
   // glColor3f(0,1,1);
    glVertex3f(15,0,15);
    glVertex3f(15,-1,15);
    glVertex3f(15,-1,-15);
    glVertex3f(15,0,-15);
    
    //top
    //  glColor3f(0, 1, 1);
    glVertex3f(15,0,-15);
    glVertex3f(15,0,15);
    glVertex3f(-15,0,15);
    glVertex3f(-15,0,-15);
    
    //bottom
    // glColor3f(0, 1, 1);
    glVertex3f(15,-1,-15);
    glVertex3f(15,-1,15);
    glVertex3f(-15,-1,15);
    glVertex3f(-15,-1,-15);
    

    
    glEnd();
}

void whole(){
    glBegin(GL_QUADS);
    
    //Front 	
	glColor3f(0, 0, 0);
    glVertex3f(2, 0, -3);
    glVertex3f(2, -1,-3);
    glVertex3f(4, -1, -3); 
    glVertex3f(4, 0, -3);
    
    
    //left side
    //  glColor3f(0, 1, 1);
    glVertex3f(4, -1, -3); 
    glVertex3f(4, 0, -3);
    glVertex3f(2,-1,-1);
    glVertex3f(2,0,-1);
    
    //back side
    //glColor3f(0, 1, 1);
    glVertex3f(2,-1,-1);
    glVertex3f(2,0,-1);
    glVertex3f(4, 0, -1);    
    glVertex3f(4, -1, -1);
    
    //right side
    // glColor3f(0,1,1);
    glVertex3f(4, 0, -1);    
    glVertex3f(4, -1, -1);
    glVertex3f(4,-1,-3);
    glVertex3f(4,0,-3);
    
    //top
    //  glColor3f(0, 1, 1);
    glVertex3f(2,0,-3);
    glVertex3f(4,0,-3);
    glVertex3f(4,0,-1);
    glVertex3f(2,0,-1);
    
    //bottom
    // glColor3f(0, 1, 1);
    glVertex3f(2,-1,-3);
    glVertex3f(4,-1,-3);
    glVertex3f(4,-1,-1);
    glVertex3f(2,-1,-1);
    
    
    
    glEnd();
    
}
/* drawAxis() -- draws an axis at the origin of the coordinate system
 *   red = +X axis, green = +Y axis, blue = +Z axis
 */
void drawAxis()
{
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0,0,0);
	glVertex3f(50,0,0);
	glColor3f(0,1,0);
	glVertex3f(0,0,0);
	glVertex3f(0,50,0);
	glColor3f(0,0,1);
	glVertex3f(0,0,0);
	glVertex3f(0,0,50);
	glEnd();
}

//Build a structure for particles
struct Point {
    float age;
    float dx;
    float dy;
    float speed;
    float dz;
    float px;
    float py;
    float pz;
    double r;
    double g;
    double b;
    bool move;
    
};

vector<Point> points;
vector<Point> rains;
vector<Point> snows;

/*-------------------------------------------------------------- Particles ------------------------------------------------------------*/

/* Create the data for each particle*/
void Create()
{
    
    Point point1;  
    point1.speed=0.1;
    point1.r = (float)rand() / (float)RAND_MAX;                 //r,g,b value for colors, random
    point1.g = (float)rand() / (float)RAND_MAX;
    point1.b = (float)rand() / (float)RAND_MAX;
    point1.px= 0.0;                                             //position x, y, z; the origin point of the particle
    point1.py= 0.00001;
    point1.pz= 0.0;
    point1.dx = 0.05-(float) (rand() %100 )/ wind;              //direction of x,y,z moving
    point1.dy = 0.75-(float)(rand()%300) / 1000;
    point1.dz = 0.05-(float)(rand()%100) /wind;
    point1.age=(float)(rand()%50000);                           //the life time for each particle

    points.push_back(point1);
    
}

/* Draw each partical */
void DrawObjects()
{
    if (show == true) {
        Create();
    } 
    whole();
    float gravity = 0;
    
    for (int i=0;i < points.size();i++){
       if(points[i].age > 0){
           
               gravity = gravity + 0.0009;              //add gravity in to let the particle falls down
           
            //Calculating the movement of each particle, the x,y,z value changes
            points[i].dy = points[i].dy - gravity;
            points[i].py = points[i].py+ (points[i].dy * points[i].speed);
            points[i].px = points[i].px+ (points[i].dx * points[i].speed)-windl;
            points[i].pz = points[i].pz+ (points[i].dz * points[i].speed)-windr; 
            
            glPushMatrix();
           cnt++;
           //increment the rotation angle every 100th display cycle
           if(cnt % 100 == 0)
           {
               ang = ang + 0.2;
               cnt = 0;
           }
            glTranslatef(points[i].px, points[i].py, points[i].pz);
            glRotatef(ang, 0, 1, 0); 
            glColor3f(points[i].r, points[i].g, points[i].b);
            glutSolidCube(0.3);                                 //draw a solid cube particle
           
            
            glPopMatrix();
            points[i].age = points[i].age - 10;                   
        
           
           //define when the particle will bounce up from the ground
            if ((points[i].py <0.00005) && (points[i].px < 15 && points[i].px > -15) && (points[i].pz < 15 && points[i].pz > -15)) {
                if (friction == true) {
                //points[i].dy = -1*points[i].dy;
                points[i].dy = points[i].dy / -2;
                points[i].dx = points[i].dx / 3;
                points[i].dz = points[i].dz / 3;
                    
                } else if (friction == false) {
                    //points[i].speed = -1 * points[i].speed;
                    points[i].dy = points[i].dy*-1;
                }
                if ((points[i].px < 4 && points[i].px > 2) && (points[i].pz < -1 && points[i].pz > -3)) {
                    points[i].dy = points[i].dy *-2;
                }
                
                
            }
       } else if(points[i].age <= 0) {
           points.erase(points.begin()+i);
       }
        gravity = 0;        //reset the gravity for next new particle
    }
}

/* Extra feature : wire mode !*/
void draw(){
    if (value == 0) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//polygon mode
        DrawObjects();
        
    }
    else if (value == 1) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//matrix mode
        DrawObjects();        
    }

}
/*--------------------------------------------------------------Rain parts------------------------------------------------------------*/


/*Set the data for the drain drops and snow drops*/
void creatrain(){
    Point rain;  
    rain.speed=0.1;
    rain.r = 0;                                                         //r,g,b value for colors, random
    rain.g = 0;
    rain.b = 1;
    rain.px=  (float) (rand() %30)-15;                                             //position x, y, z; the origin point of the particle
    rain.py= 30.00001;
    rain.pz=  (float) (rand() %30 )-15;
    rain.dx = 0;              //direction of x,y,z moving
    rain.dy = -(float)(rand()%300) / 1000;
    rain.dz = 0;
    rain.age=(float)(rand()%50000);                           //the life time for each rain drops
    
    rains.push_back(rain);
}

void Drawrain()
{
        creatrain();
    float gravity = 0;
    
    for (int i=0;i < rains.size();i++){
        if(rains[i].age > 0){
            
            gravity = gravity + 0.0009;              //add gravity in to let the particle falls down
            
            //Calculating the movement of each particle, the x,y,z value changes
            rains[i].dy = rains[i].dy - gravity;
            rains[i].py = rains[i].py+ (rains[i].dy * rains[i].speed);
            rains[i].px = rains[i].px+ (rains[i].dx * rains[i].speed)- windl;
            rains[i].pz = rains[i].pz+ (rains[i].dz * rains[i].speed)-windr; 
            
            glPushMatrix();
            cnt++;
            //increment the rotation angle every 100th display cycle
            if(cnt % 100 == 0)
            {
                ang = ang + 0.2;
                cnt = 0;
            }
            glTranslatef(rains[i].px, rains[i].py, rains[i].pz);
            glRotatef(ang, 0, 1, 0); 
            glColor3f(rains[i].r, rains[i].g, rains[i].b);
            glutSolidCube(0.2);                                 //draw a solid cube particle
            
            
            glPopMatrix();
            rains[i].age = rains[i].age - 10;                   
            
            
            //define when the particle will bounce up from the ground
            if ((rains[i].py <=0.00005) && (rains[i].px < 15 && rains[i].px > -15) && (rains[i].pz < 15 && rains[i].pz > -15)) {
                rains[i].dy = 0;
            }
        }
        gravity = 0;        //reset the gravity for next new particle
    }
}


/*----------------------------------------------------------Snow ----------------------------------------------------*/
void creatsnow(){
    Point snow;  
    snow.speed=0.1;
    snow.r = 0.9;                                                         //r,g,b value for colors, random
    snow.g = 0.9;
    snow.b = 0.9;
    snow.px=  (float) (rand() %30)-15;                                             //position x, y, z; the origin point of the particle
    snow.py= 30.00001;
    snow.pz=  (float) (rand() %30 )-15;
    snow.dx = 0.05-(float) (rand() %100 )/ 1000;            //direction of x,y,z moving
    snow.dy = -(float)(rand()%300) / 1000;
    snow.dz = 0.05-(float) (rand() %100 )/ 1000;
    snow.age=(float)(rand()%50000);                           //the life time for each rain drops
    
    snows.push_back(snow);
}

void Drawsnow()
{
    creatsnow();
    float gravity = 1;
    
    for (int i=0;i < snows.size();i++){
        if(snows[i].age > 0){
            
            gravity = gravity + 0.00005;              //add gravity in to let the particle falls down
            
            //Calculating the movement of each particle, the x,y,z value changes
            snows[i].dy = snows[i].dy - gravity;
            snows[i].py = snows[i].py+ (snows[i].dy * snows[i].speed);
            snows[i].px = snows[i].px+ (snows[i].dx * snows[i].speed)-windl;
            snows[i].pz = snows[i].pz+ (snows[i].dz * snows[i].speed)-windr; 
            
            
            glPushMatrix();
            cnt++;
            //increment the rotation angle every 100th display cycle
            if(cnt % 100 == 0)
            {
                ang = ang + 0.2;
                cnt = 0;
            }
            if (cnt%10000 ==0) {                        //let the snow swing in the air
                snows[i].dx -= 0.04;
                snows[i].dz -= 0.04;
            }
            if (cnt%20000 ==0) {
                snows[i].dx += 0.04;
                snows[i].dz += 0.04;
            }
            glTranslatef(snows[i].px, snows[i].py, snows[i].pz);
            glRotatef(ang, 0, 1, 0); 
            glColor3f(snows[i].r, snows[i].g, snows[i].b);
            glutSolidCube(0.3);                                 //draw a solid cube particle
            
            
            glPopMatrix();
            snows[i].age = snows[i].age - 10;                   
            
            
            //define when the particle will bounce up from the ground
            if ((snows[i].py <=0.00005) && (snows[i].px < 15 && snows[i].px > -15) && (snows[i].pz < 15 && snows[i].pz > -15)) {
                snows[i].dy = 0;
                snows[i].dx = 0;
                snows[i].dz=0;
            }
        }
        gravity = 0;        //reset the gravity for next new particle
    }
}

/*-----------------------------------------------Display, KeybFun and Main---------------------------------------------*/


/* display() - the OpenGL display function, this draws the screen
 *  it displays a particle system
 */
void display()
{
    
    //Set up lighting data
    typedef struct materialStruct { 
        float ambient[4]; 
        float diffuse[4]; 
        float specular[4]; 
        float emission[4];
        float m_amb[4];
        float m_diff[4]; 
        float m_spec[4] ;
        float shininess; 
    } materialStruct; 
    
    materialStruct redPlastic = { 
        {0.0, 0.0, 0.3, 1.0}, 
        {0.0, 0.4, 0.1, 1.0}, 
        {0.6, 0.6, 0.8, 1.0}, 
        {0.0, 0.0, 0.0, 0.0}, 
        {0.03, 0.22, 0.03, 1.0},
        {0.18, 0.77, 0.1, 1.0},
        {0.10, 0.41, 0.29, 1.0},
        25.0 
    }; 
    
    
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);                   //First light point
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, redPlastic.ambient);  
    glLightfv(GL_LIGHT0, GL_DIFFUSE, redPlastic.diffuse); 
    glLightfv(GL_LIGHT0, GL_SPECULAR, redPlastic.specular); 
    
    typedef struct materialStruct2 { 
        float ambient[4]; 
        float diffuse[4]; 
        float specular[4]; 
        float emission[4];
        float m_amb[4];
        float m_diff[4]; 
        float m_spec[4] ;
        float shininess; 
    } materialStruct2; 
    
    materialStruct2 redPlastic2 = { 
        {0.0, 0.0, 0.3, 1.0}, 
        {0.6, 0.1, 0.1, 1.0}, 
        {0.6, 0.6, 0.8, 1.0}, 
        {0.0, 0.0, 0.0, 0.0}, 
        {0.03, 0.02, 0.33, 1.0},
        {0.78, 0.17, 0.21, 1.0},
        {0.30, 0.11, 0.19, 1.0},
        50.0 
    }; 
    
    
    glEnable(GL_LIGHT2);
    glLightfv(GL_LIGHT2, GL_POSITION, light_pos2);                      //second Light point
    
    glLightfv(GL_LIGHT2, GL_AMBIENT, redPlastic2.ambient);  
    glLightfv(GL_LIGHT2, GL_DIFFUSE, redPlastic2.diffuse); 
    glLightfv(GL_LIGHT2, GL_SPECULAR, redPlastic2.specular); 
    

	//clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();  
    if (particleview == false) {
        gluLookAt(eye[0], eye[1], eye[2],0,0,0, 0, 1, 0 );
    }else if(particleview ==true){
//        for (int i=0;i < points.size();i++){

        gluLookAt(points[0].px-1.5, points[0].py+1.5, points[0].pz-1.5, points[0].px, points[0].py, points[0].pz, 0, 1, 0);
   // }
    }
    
    
    glEnable(GL_CULL_FACE_MODE);
    glPushMatrix(); 
    glRotatef(cam,0,1, 0);
    //glPushMatrix();
    drawAxis(); 
	drawCube();                         //draw the base
   // whole();
    //glPopMatrix();
    if (makeRain ==1) {
        
        DrawObjects();
    }
    else if (makeRain == 2){
        Drawrain();
    }
    else if (makeRain == 3){
    Drawsnow();
    }
    glPopMatrix();
	//swap buffers - rendering is done to the back buffer, bring it forward to display
	glutSwapBuffers();
    
	//force a redisplay, to keep the animation running
	glutPostRedisplay();
}

/* kbd -- the GLUT keyboard function 
 *  key -- the key pressed
 *  x and y - mouse x and y coordinates at the time the function is called
 */
void kbd(unsigned char key, int x, int y)
{
	//if the "q" key is pressed, quit the program
	if(key == 'q' || key == 'Q')
	{
		exit(0);
	}
    if(key == 'f' || key == 'F')        //turn on/off the friction
	{
        if (friction == true) {
            friction = false;
        }else if ( friction == false)
        {
            friction = true;
            }
        
	}
    if(key == 'r' || key== 'R')         //reset the program
	{
        points.clear();
        DrawObjects();
        
        
	}  
    if(key == ' ' || key== ' ')         //pause the program
	{
        if (show == true) {
            show= false;
        }else if (show==false){
            show=true;
        }
	}
    if (key == 'w' || key == 'W') {       //change to wire mode   
        
        if (value == 0) {
            draw();
            value = 1;
        }
        else if (value == 1) {
            draw();
            value = 0;
        }
    }
    if (key == 'b' || key == 'B') {        // turn on the backface cull
        if(backface == false){
        glEnable(GL_CULL_FACE);
            backface = true;
        }else if (backface == true){
        glDisable(GL_CULL_FACE);
            backface = false;
        }
    }
    if(key =='l' || key =='L'){             //enable lighting 
        if (lightOn == true) {
            
            glEnable(GL_LIGHTING); 
            lightOn = false;
        }
        else if(lightOn == false) {
            glDisable(GL_LIGHTING);
            lightOn = true;
        }
    }
    if (key == 'p'|| key == 'P') {
        points.clear();
        DrawObjects();
        if (particleview == false) {
            particleview = true;
        } else if (particleview ==true)
            particleview = false;
    }
    if (key == '2' || key =='2') {                  //turn on/off the rain/ snow/ normal mode
        rains.clear();
        makeRain=2;
        
    }
    if (key == '1' || key =='1') {
        points.clear();
        makeRain=1;}
    if (key == '3' || key =='3') {
        snows.clear();
        makeRain=3;}
    if (key == 'x' || key =='X') {                                  //add wind effects in
        windr = 0;
    }
    if (key == 'z' || key =='Z') {
            windr = 0.02;
    }
    if (key == 'c' || key =='C') {
        windl = 0.02;
    }
    if (key == 'v' || key =='V') {
        windl = 0;
    }
}

/* Rotation of the camera */
void pressKey(int key, int xx, int yy) {
    
	switch (key) {
        case GLUT_KEY_LEFT : cam-=1.5; break;
		case GLUT_KEY_RIGHT : cam +=1.5;break;
		case GLUT_KEY_UP : eye[1]++; break;
		case GLUT_KEY_DOWN : eye[1] --; break;
//		case GLUT_KEY_LEFT : eye[0]++; eye[2] ++; break;
//		case GLUT_KEY_RIGHT : eye[0]--; eye[2] -- ;break;
//		case GLUT_KEY_UP : eye[1]++; break;
//		case GLUT_KEY_DOWN : eye[1] --; break;
	}
}

int main(int argc, char** argv)
{
    
    std::cout <<" USER MANUAL.\n" ;
    std::cout <<"'1' Normal particle mode,         '2' rain mode \n";
    std::cout <<"'SPACE' pause the program,         '3' snow mode \n";
    std::cout <<"'w' Change to wire modes, 'r' resets particle system \n";
    std::cout <<"'q' Quit Program,         'l' Enable/Disable Lighting \n";
    std::cout <<"'f' turns on/off friction \n";
    std::cout <<"'UP'move up camera          'DOWN' moves down camera \n";
    std::cout <<"'LEFT' rotate left,        'RIGHT' rotate right \n";
    std::cout <<"'z' winds from left, 'x' turn off the winds from left\n";
    std::cout <<"'c' winds from front, 'v' turn off the winds from front\n";
    std::cout <<"'p' Turns on particle camera         'b' turn on the culling effects \n";
	//glut initialization stuff:
	// set the window size, display mode, and create the window
	glutInit(&argc, argv);
	glutInitWindowSize(600, 600);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("particle System");
    
	//enable Z buffer test, otherwise things appear in the order they're drawn
	glEnable(GL_DEPTH_TEST);
    
	//setup the initial view
	// change to projection matrix mode, set the extents of our viewing volume
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    //glutIdleFunc(idle);
	//glOrtho(-50, 50, -10, 30, -100, 100);
    glutSpecialFunc(pressKey);
    gluPerspective (55,1,2,1000);

	//set clear colour to white
	//glClearColor(1, 1, 1, 0);
    
	glMatrixMode(GL_MODELVIEW);
	//look down from a 45 deg. angle
	//glRotatef(45, 1, 10, 0);
    
    
	//register glut callbacks for keyboard and display function
	glutKeyboardFunc(kbd);
	glutDisplayFunc(display);
    
	//start the program!
	glutMainLoop();
    
	return 0;
}