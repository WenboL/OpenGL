/* CS 3GC3 Finial Project
 * Liu Wenbo 0970709 && Justin Kan 0843763
 * 
 * This program is coded on Mac computer.
 *
 * Program is a game, you using key: w,s,a,d to controll helicopter to avoid the rocks from the forward. *
 */

#include <vector>
#include <iostream>
#include <sstream>

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



#include <ctime>


#include <vector>

#include <ctime>





//Mesh* testPlane = new Mesh();
float planepos[3]={0.0f,0.0f , 0.0f};
float eye[3] = {0.0f,35.0f,70.0f};//starting camera point
float point[3]={0.0,35.0,-70.0};//look at point
int axis = 0;
float light_pos[] = {300.0, 2.0, 10.0, 1.0};//first light source
float light_pos1[] = {000.0, 2.0, -10.0, 1.0};//second light source
float light_pos2[] = {-300, 2, 10, 1.0};//third light source

int size = 200;//window size
float heightmap[300][300];//hight
int iter = size;//iterations
double circleSize = 25-size/100;
double disp = 10-size/100;
int value= 0;//the value for change modes like line mode to fill
int cameraspeed = 2;
bool lightOn = true; //change between light modes
bool smooth = true;
float rockpos = 0;
GLubyte* tex;
int width, height, max1;

bool mFlag = true;
bool gStart = true;
float deltaAngle = 0.0f; //camera angle
float deltaMove = 0;
float camerapos[3]={0.0f,15.0f,5.0f};

float pos = eye[1];
// angle of rotation for the camera direction
float angle=0.0;
int cnt = 0;
int ang = 0;

float speed = 0.02;
int score = 0;

float alpha = 1;
float colorBlue[] = {0.0f, 0.0f, 1.0f, alpha};
float rCount = 0;


struct rock{
	float accel[3];
	float velo[3];
	float pos[3];
	float color[3];
	float scale;
	float bound;
	float count;
    float age;
};



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

std::vector<Point> points;
std::vector<Point> star;
std::vector<rock> rocks;
std::vector<rock> rocks2;


void drawplane(){
    if(cnt % 100 == 0)
	{

		ang++;
		cnt = 0;
	}
    
    glPushMatrix();
    
    glColor3f(0, 0, 1);
    glPushMatrix();
    glTranslatef(0, 2, 6);
    glScalef(10.25, 4, 4);
    glutSolidCube(1);
    glPopMatrix();
    
    /*Plane body*/
    glColor3f(1, 1, 0);
    glPushMatrix();
    glScalef(10, 10, 15);
    glutSolidCube(1);
    glPopMatrix();
    
    /*Plane top Pillar*/
    glColor3f(0.2, 0.4, 0);
    glPushMatrix();
    glTranslatef(0, 5, 0);
    glScalef(2, 4, 2);
    glutSolidCube(1);
    glPopMatrix();
    
    /*Plane back*/
    glColor3f(0.3, 0.4, 0);
    glPushMatrix();
    glTranslatef(0, 4, -13);
    glScalef(2, 2, 13);
    glutSolidCube(1);
    glPopMatrix();
    
    /*Plane top fan*/
    glColor3f(0.3, 0.3, 0.5);
    glPushMatrix();
    glTranslatef(0, 7, 0);
    glRotatef(ang, 0, 1, 0);
    glScalef(2, 1, 30);    
    glutSolidCube(1);
    glPopMatrix();
    
    glColor3f(0.3, 0.3, 0.5);
    glPushMatrix();
    glTranslatef(0, 7, 0);
    glRotatef(ang, 0, 1, 0);
    glScalef(30, 1, 2);    
    glutSolidCube(1);
    glPopMatrix();
    
    /*Plane back fan*/
    glColor3f(0.3, 0.4, 0);
    glPushMatrix();
    glTranslatef(0, 5, -18);
    glScalef(2, 3, 2);    
    glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
    glColor3f(0.2, 1, 0.2);
    glTranslatef(-1, 6, -18);
    glRotatef(ang, -1, 0, 0);
    glScalef(1, 1, 5);    
    glutSolidCube(1);
    glPopMatrix();
    
    /*Plane feet pillars*/
    glColor3f(0.2, 0.2, 0.2);
    glPushMatrix();
    glTranslatef(-3, -5, 5);
    glScalef(2, 4, 2);    
    glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-3, -5, -5);
    glScalef(2, 4, 2);    
    glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(3, -5, 5);
    glScalef(2, 4, 2);    
    glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(3, -5, -5);
    glScalef(2, 4, 2);    
    glutSolidCube(1);
    glPopMatrix();
    
    /*Plane feet */
    glColor3f(0.5, 0, 1);
    glPushMatrix();
    glTranslatef(-3, -7, 0);
    glScalef(2, 1, 20);    
    glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(3, -7, 0);
    glScalef(2, 1, 20);    
    glutSolidCube(1);
    glPopMatrix();
    
    glPopMatrix();
    
}

void initstar(){
    Point stars;
	stars.age = rand()%1000;
    stars.px=rand()%300-150;
    stars.py=rand()%300-150;;
    stars.pz=rand()%300-150;;
    star.push_back(stars);
    
}

GLubyte* LoadPPM(char* file, int* width, int* height, int* max1)
{
	GLubyte* img;
	FILE *fd;
	int n, m;
	int  k, nm;
	char c;
	int i;
	char b[100];
	float s;
	int red, green, blue;
	
	fd = fopen(file, "r");
	fscanf(fd,"%[^\n] ",b);
	if(b[0]!='P'|| b[1] != '3')
	{
		printf("%s is not a PPM file!\n",file); 
		exit(0);
	}
	printf("%s is a PPM file\n", file);
	fscanf(fd, "%c",&c);
	while(c == '#') 
	{
		fscanf(fd, "%[^\n] ", b);
		printf("%s\n",b);
		fscanf(fd, "%c",&c);
	}
	ungetc(c,fd); 
	fscanf(fd, "%d %d %d", &n, &m, &k);
	
	printf("%d rows  %d columns  max1 value= %d\n",n,m,k);
	
	nm = n*m;
	
	img = (GLubyte*)malloc(3*sizeof(GLuint)*nm);
	
	
	s=255.0/k;
	
	
	for(i=0;i<nm;i++) 
	{
		fscanf(fd,"%d %d %d",&red, &green, &blue );
		img[3*nm-3*i-3]=red*s;
		img[3*nm-3*i-2]=green*s;
		img[3*nm-3*i-1]=blue*s;
	}
	
	*width = n;
	*height = m;
	*max1 = k;
	
	return img;
}

/* function: init()
 This function is to initailize the texture
 Source: Tutorial*/
void init()
{
	
	tex = LoadPPM("copter.ppm", &width, &height, &max1);
	
	glClearColor(0, 0, 0, 0);
	glColor3f(1, 1, 1);
	
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-2, 2, -2, 2, -2, 2);
	//glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	gluPerspective (75,1,2,5000);
	
	glEnable(GL_TEXTURE_2D);
	//	glGenTextures(1, &tex);
	//	glBindTexture(GL_TEXTURE_2D, &tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glDisable(GL_TEXTURE_2D);
	
	//glBindTexture(GL_TEXTURE_2D, tex);
	
	//	printf("%d\n", glGetError());
}

/* function: menu()
 This is the main menu display. It uses GL_QUADS to create the plane, and then it will load the texture in*/
void menu()
{
	if(mFlag)
	{
		init();
		glBegin(GL_QUADS);
		
		glTexCoord2f(0, 0);
		glVertex3f(0, 0, 0);
		
		glTexCoord2f(1, 0);
		glVertex3f(16, 0, 0);
		
		glTexCoord2f(1,1);
		glVertex3f(16, 16, 0);
		
		glTexCoord2f(0, 1);
		glVertex3f(0, 16, 0);
		
		
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
}


void drawstar(){
    initstar();
    for (int i = 0; i < star.size(); i++) {
		if(star[i].age > 0){
        glPushMatrix();
       // glRotatef(ang/900, 1, 1, 1);
        glTranslatef(star[i].px,star[i].py,star[i].pz);
        glRotatef(ang, 0, 1, 0); 
        glColor3f(1, 1, 1 );
        glutSolidCube(0.2);
        glPopMatrix();
		star[i].age -= 1.5;
		}
  }
}
// --------------------------------------------------- //
void initexplor(float x, float y    ){
    
	Point point1;  
    point1.px= x;                                             //position x, y, z; the origin point of the particle
    point1.py=y;
    point1.pz= 0;
    point1.dx = .5-(float)rand()/(float)RAND_MAX;              //direction of x,y,z moving
    point1.dy = .5-(float)rand()/(float)RAND_MAX;
    point1.dz = .5-(float)rand()/(float)RAND_MAX;
    point1.age=10000;                           //the life time for each particle
    
    points.push_back(point1);
}
void explor(){
     
    for (int i=0;i < points.size();i++){

        if(points[i].age > 0){
            
                              
            //Calculating the movement of each particle, the x,y,z value changes
            points[i].py = points[i].py+ points[i].dy;
            points[i].px = points[i].px+ points[i].dx ;
            points[i].pz = points[i].pz+ points[i].dz; 
            
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
            glColor3f(1, 1, 0);
            glutSolidCube(3);                                 //draw a solid cube particle
            
            
            glPopMatrix();
            points[i].age = points[i].age - 100;                   
//            if (points[i].age == 0) {
//                exit(0);
//            }
            
        }
    }

}

void initRock()
{
	rock specRock;
	
	specRock.accel[0] = 1;
	specRock.accel[1] = 1;
	specRock.accel[2] = 1;
	
	specRock.velo[0] = 1.2;//0.05-(float)(rand()%100)/100.0;
	specRock.velo[1] = 1;
	specRock.velo[2] = 1;
    
    specRock.age = 500000;
	
   // rockpos=(float) (rand() %10 );
    
    if (rockpos > 0 && rockpos<= 1) {
        specRock.pos[0] = -25;
        specRock.pos[1] = 0;
        specRock.pos[2] = -80;


    } else if (rockpos >1 && rockpos<= 2) {
        specRock.pos[0] = -0;
        specRock.pos[1] = 0;
        specRock.pos[2] = -80;
    } else if (rockpos > 2 && rockpos<= 3) {
        specRock.pos[0] = 25;
        specRock.pos[1] = 0;
        specRock.pos[2] = -80;
    }  else if (rockpos > 3 && rockpos<= 4) {
        specRock.pos[0] = -25;
        specRock.pos[1] = 25;
        specRock.pos[2] = -80;
    }  else if (rockpos > 4 && rockpos<= 5) {
        specRock.pos[0] = 0;
        specRock.pos[1] = 25;
        specRock.pos[2] = -80;
    }  else if (rockpos > 5 && rockpos<= 6) {
        specRock.pos[0] = 25;
        specRock.pos[1] = 25;
        specRock.pos[2] = -80;
    }  else if (rockpos > 6 && rockpos<= 7) {
        specRock.pos[0] = -25;
        specRock.pos[1] = -25;
        specRock.pos[2] = -80;
    }  else if (rockpos > 7 && rockpos<= 8) {
        specRock.pos[0] = 0;
        specRock.pos[1] = -25;
        specRock.pos[2] = -80;
    }  else if (rockpos > 8 && rockpos<= 9) {
        specRock.pos[0] = 25;
        specRock.pos[1] = -25;
        specRock.pos[2] = -80;
    }  
	
	specRock.color[0] = 0.8;
	specRock.color[1] = 0;
	specRock.color[2] = 0.8;
	
	specRock.scale = 1;
	
	specRock.bound = 12;
	
	specRock.count = 0;
	
	rocks.push_back(specRock);
}

//Draw the rocks out
void drawRocks()
{

    glPushMatrix();
    glTranslated(planepos[0], planepos[1], planepos[2]);                                 // change the position of terrain to make it shows infront of camera
    glRotatef(180, 0, 1, 0);
    drawplane();                                               
    glPopMatrix();
    
    
    
	for (int i = 0; i < 9; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'A');
        initRock();
        if(rocks[i].age > 0){
            
            
            
            rocks[i].age -= 1.5;
            rockpos=(float) (rand() %10 );
		glPushMatrix();
        rocks[i].pos[2] += speed;
        glColor3f(rocks[i].color[0], rocks[i].color[1], rocks[i].color[2]);
		glMaterialfv(GL_FRONT, GL_AMBIENT, colorBlue);
		glTranslatef(rocks[i].pos[0], rocks[i].pos[1], rocks[i].pos[2]);
		//rocks[i].pos[0] += rocks[i].velo[0];
        //glutSolidCube(10);
            glutSolidSphere(10, 15, 15);
		glPopMatrix();
                            
                

            
            
            
        }
    }
}   

//Move the rocks and check if the plane hit it.
void rockMotion()
{
	for (int i = 0; i < 9; i++) {
		//rocks[i].pos[0] += 1;//rocks[i].velo[0];
		if (rocks[i].pos[2] > 20) {
			rocks.erase(rocks.begin() + i);
            score = score +1;
            //std::cout << score;
            
		}
        if (rocks[i].pos[0]< planepos[0]+5 && rocks[i].pos[0]>planepos[0]-5 && rocks[i].pos[1]<planepos[1]+5 && rocks[i].pos[1]>planepos[1]-5 && rocks[i].pos[2]<planepos[2]+8 && rocks[i].pos[2]>planepos[2]-8) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
            std::cout <<"Game over! Your Score:  ";
            rocks.clear();
            
            //drawStrokeText(game, 0, 40);
            
            std::cout << score/9;
            
            glPushMatrix();
            glTranslatef(0, 0, 0);
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'G');
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'A');
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'M');
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'E');
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,' ');
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'O');
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'V');
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'E');
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'R');
            glPopMatrix();
            
            
            //initexplor(planepos[0],planepos[1]);
            //explor();
            exit(0 );

        }
        
	}
}

//void  convertInt(int number)
//
//{
//    
//    stringstream ss;//create a stringstream
//    
//    ss << number;//add number to the stream
//    
//    return ss.str();//return a string with the contents of the stream
//    
//}

void motion(int x)
{
	rockMotion();
    
	glutPostRedisplay();
	glutTimerFunc(50, motion, 0);
}

void drawStrokeText(char* text, int x, int y)
{
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	gluOrtho2D(0, 600, 0, 600);  
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
    
	glTranslatef(x, y, 0);
	glScalef(0.25, 0.25, 0.25);
	
	char* c;  //character iterator (ptr) 
	for(c = text; *c != '\0'; c++) //stop when we hit null character 
	{
        //		printf("loop");
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *c); //print one char 
	}
}

void computePos(float deltaMove) {
    
	eye[0] += deltaMove * point[0] * 0.2f;
    //eye[1] += deltaMove * point[1]*0.2f;
	eye[2]+= deltaMove * point[2] * 0.2f;
}

/* Compute the direction*/
void computeDir(float deltaAngle) {
    
	angle += deltaAngle * 0.5;
	point[0] = sin(angle);
    //point[1] = -sin(angle)+cos(angle);
	point[2] = -cos(angle);
}


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


void drawcube(){
    
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
   



void glutsolidsphere()

{
    
    glBegin(GL_SPHERE_MAP);
    
    
    
}



void drawsphere()

{
    
    glutSolidSphere(3, 48, 16);
    
}



void display()
{
    
    /*---------------------------------------Lighting ------------------------------------------------*/       
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
        50.0 
    }; 
    
    //  glEnable(GL_LIGHTING); 
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);                   //First light point
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, redPlastic.ambient);  
    glLightfv(GL_LIGHT0, GL_DIFFUSE, redPlastic.diffuse); 
    glLightfv(GL_LIGHT0, GL_SPECULAR, redPlastic.specular); 
    
    
    
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_POSITION, light_pos1);                  //Second Light point
    
    glLightfv(GL_LIGHT1, GL_AMBIENT, redPlastic.ambient);  
    glLightfv(GL_LIGHT1, GL_DIFFUSE, redPlastic.diffuse); 
    glLightfv(GL_LIGHT1, GL_SPECULAR, redPlastic.specular); 
    
    
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
    glLightfv(GL_LIGHT2, GL_POSITION, light_pos2);                      //Third Light point
    
    glLightfv(GL_LIGHT2, GL_AMBIENT, redPlastic2.ambient);  
    glLightfv(GL_LIGHT2, GL_DIFFUSE, redPlastic2.diffuse); 
    glLightfv(GL_LIGHT2, GL_SPECULAR, redPlastic2.specular); 

    
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  redPlastic2.ambient);  
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  redPlastic2.diffuse);  
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  redPlastic2.specular);  
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,  redPlastic2.shininess);  
    
	//clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
    
    
    if (deltaMove)
		computePos(deltaMove);
	if (deltaAngle)
		computeDir(deltaAngle);
    
    //gluLookAt(0, 30, 70, 0, 0, 0, 0, 1, 0);
	gluLookAt(eye[0],eye[1],eye[2],eye[0]+point[0],eye[1],eye[2]+point[2],0,1,0);
    camerapos [0] = eye[0];
    camerapos[2] = eye[2];
	//optionally draw the axis
    //glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, "SPACE Helicopter PRO");
	//drawAxis();
    drawstar();
    	

	drawRocks();
    
    glPushMatrix();
    
    /*---------------------------------------Solar system-----------------------------------------*/

    
    //Venus
    
    glRotatef(ang/200, 0, 1, 1);
    
    glTranslatef(100, 0, 0);
    
    glColor3f(1, 0, 0);
    
    glScalef(2, 2, 2);
    
    drawsphere();
    
    
    
    glPopMatrix();
    
    glPushMatrix();
    
    //Earth
    
    glRotatef(ang/250, 0, 1, 1);
    
    glTranslatef(150, 0, 0);
    
    glColor3f(0, 0, 1);
    
    glScalef(3, 3, 3);
    
    drawsphere();
    
    //moon
    
    glRotatef(ang/80, 0, 1, 1);
    
    glTranslatef(75, 0, 0);
    
    glColor3f(1, .5, .5);
    
    glScalef(1, 1, 1);
    
    drawsphere();
    
    
    
    glPopMatrix();
    
    glPushMatrix();
    
    
    
    //Mars
    
    glRotatef(ang/500, 0, 1, 0);
    
    glTranslatef(200, 0, 0);
    
    glColor3f(1, 0, 0);
    
    glScalef(5, 5, 5);
    
    drawsphere();
    
    
    
    glRotatef(ang/800, 0, 1, 1);
    
    glTranslatef(-500, 0, 0);
    
    glColor3f(.5, .5, .5);
    
    glScalef(3, 3, 3);
    
    drawsphere();
    
    
  
    
    glPopMatrix();
    
    glRotatef(ang/800, 0, 1, 1);
    
    glTranslatef(500, 0, 0);
    
    glColor3f(.5, .5, .5);
    
    glScalef(3, 3, 3);
    
    drawsphere();
    

    
    glRotatef(ang/550, 0, 1, 1);
    
    glTranslatef(-200, 0, 0);
    
    //glRotatef(ang, 0, 1, 0);
    
    glScalef(5, 5, 5);
    
    
    glPopMatrix();
    
    
  
    
    glPopMatrix();
    
    glPopMatrix();
    

    
    /*---------------------------------------Draw Light Source-----------------------------------------*/
    glPushMatrix();
    glTranslated(light_pos2[0], light_pos2[1], light_pos2[2]);
    glColor3f(1, 1, 0);
    
    glPopMatrix();
	
	glutSwapBuffers();
    
    
	glutPostRedisplay();
    
    
    
    
}
void kbd(unsigned char key, int x, int y)
{
    if(key == 'q' || key == 'Q')
	{
		exit(0);                                                //quite the program
	}
	//if the "q" key is pressed, quit the program
    
    if(key == 'z')
	{
        // pos -= 1;
        eye[1] -= 2.0;
        //point[1]-=cameraspeed;
        
	}
    if(key == 'x')
	{
        // pos += 1;
        eye[1] +=2.0;
        //point[1]+=cameraspeed;
	}
    
   
    if(key =='l' || key =='L'){
        if (lightOn == true) {
            
            glEnable(GL_LIGHTING); 
            lightOn = false;
        }
        else if(lightOn == false) {
            glDisable(GL_LIGHTING);
            lightOn = true;
        }
    }
    if(key =='1' || key =='1'){
        glPushMatrix();
        glTranslatef(0, 0, 0);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'G');
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'A');
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'M');
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'E');
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,' ');
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'O');
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'V');
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'E');
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'R');
        glPopMatrix();

    }
    if(key =='2' || key =='2'){
        drawstar();
        drawRocks();
        
    }
    
    
    if (key == 'w' || key == 'W') {
        planepos[1] = planepos[1]+25;
    }
    if (key == 's' || key == 'S') {
        planepos[1] = planepos[1]-25;
    }
    if (key == 'a' || key == 'A') {
        planepos[0] = planepos[0]-25;
       
        
    }
    if (key == 'd' || key == 'D') {
        planepos[0] = planepos[0]+25;
     
        }
    }



/*---------------------------------------Advanced Camera-----------------------------------------*/


void pressKey(int key, int xx, int yy) {
    
	switch (key) {
		case GLUT_KEY_LEFT : deltaAngle = -0.01f;  break;
		case GLUT_KEY_RIGHT : deltaAngle = 0.01f; break;
		case GLUT_KEY_UP : deltaMove = 0.8f; break;
		case GLUT_KEY_DOWN : deltaMove = -0.8f; break;
	}
}

void releaseKey(int key, int x, int y) {
    
	switch (key) {
		case GLUT_KEY_LEFT :
		case GLUT_KEY_RIGHT : deltaAngle = 0.0f;break;
		case GLUT_KEY_UP : 
		case GLUT_KEY_DOWN : deltaMove = 0;break;
	}
}

int main(int argc, char** argv)
{
    
    std::cout <<"'W'moves up          'S' moves down \n";
    std::cout <<"'a' move left,        'd' moves right \n";
    std::cout <<"If you hit the rocks, the program will shut down";
    std::cout <<"Press L to turn on the light\n";
    std::cout <<"Have fun .\n";
	glutInit(&argc, argv);
    
    /*---------------------------------Main Window---------------------------------------*/
	glutInitWindowSize(600, 600);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Space Helicoptor PRO");
    
    
	glEnable(GL_DEPTH_TEST);
    
    speed += 0.1;
	//setup the initial view
	// change to projection matrix mode, set the extents of our viewing volume
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-2.5, 2.5, -2.5, 2.5, -2.5, 2.5);
	gluPerspective (75,1,2,5000);
    
	//set clear colour to white
	//glClearColor(1, 1, 1, 0);
    
	//register glut callbacks for keyboard and display function
	glutKeyboardFunc(kbd);
    //  glutSpecialFunc(special);
	glutSpecialFunc(pressKey);
    
	// here are the new entries
	glutIgnoreKeyRepeat(1);
	glutSpecialUpFunc(releaseKey);
    	glutTimerFunc(25, motion, 0);
    
	glutDisplayFunc(display);
    

    
    
    
    
	glutMainLoop();
    
	return 0;
}
