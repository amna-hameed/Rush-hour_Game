//============================================================================
// Name        : Amna.cpp
// Author      : FAST CS Department
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game of Rush Hour...
//============================================================================

#ifndef RushHour_CPP_
#define RushHour_CPP_

#include <iostream>
#include<string>
#include<cmath> // for basic math functions such as cos, sin, sqrt

#include <fstream>
#include <string>
//#include "Physics.h"

#include <ctime>

using namespace std;
//class for creating obstacles
class Obstacle
{
    public:
    int x;
    int y;
    int width;
    int height;
    Obstacle(int x1, int y1, int width, int height)
    {
        this->x = x1;
        this->y = y1;
        this->width = width;
        this->height = height;
    }
};
#include <iomanip>
#include "util.h"
//class for placing random people and destinations
class People
{
private:
    /* data */
public:
    int x;
    int y;
    int width;
    int height;
    float *color;

    int destX;
    int destY;
    int destWidth;
    int destHeight;

    bool isOnboard;
    bool reached;
    People(/* args */)
    {
        int randomY = 50 + rand() % (( 800 + 1 ) - 50);
        int randomX = 50 + rand() % (( 800 + 1 ) - 50);
        int color = 0 + rand() % (( 100 + 1 ) - 0);
        this->x = randomX;
        this->y = randomY;
        this->width = 10;
        this->height = 10;
        this->color = colors[color];

        this->destX = 50 + rand() % (( 800 + 1 ) - 50);
        this->destY = 50 + rand() % (( 800 + 1 ) - 50);
        this->destWidth = 30;
        this->destHeight = 30;

        this->isOnboard = false;
    }

};
//class for moving the car
class Car
{
    public:
    int x;
    int y;
    int width;
    int height;
    float *color;
    bool flag;
    bool hasPassenger;
    Car()
    {
        flag = true;
    }
    Car(int x1, int y1, float *color)
    {
        this->x = x1;
        this->y = y1;        
        this->color = color;
        width = 30;
        height = 20;
        flag = false;
    }
    void moveLeft(int jump)
    {
        this->x -= jump;
    }
    void moveRight(int jump)
    {
        this->x += jump;
    }
    void moveDown(int jump)
    {
        this->y -= jump;
    }
    void moveUp(int jump)
    {
        this->y += jump;
    }
};
//#include "Car.h"
#include <list>
//#include "Obstacle.h"
#include <stdio.h>
//class made for collision detection
class Physics
{
    public:
    static bool isCollisionWithObstacles(Car myCar, std::list<Obstacle> obstacles)
    {
        bool detected = false;
        for(std::list<Obstacle>::iterator i = obstacles.begin() ; i != obstacles.end() ; ++i)
        {
            if(colissionDetected(myCar.x, myCar.y, myCar.width, myCar.height,
                                    i->x, i->y, i->width, i->height))
                                    {
                                        detected = true;
                                        break;
                                    }
        }
        return detected;
    }
    static bool isCollisionWithCars(Car myCar, std::list<Car> cars)
    {
        bool detected = false;
        for(std::list<Car>::iterator i = cars.begin() ; i != cars.end() ; ++i)
        {
            if(colissionDetected(myCar.x, myCar.y, myCar.width, myCar.height,
                                    i->x, i->y, i->width, i->height))
                                    {
                                        detected = true;
                                        break;
                                    }
        }
        return detected;
    }
    static bool isCollisionWithPeople(Car &myCar, std::list<People> &peoples)
    {
        bool detected = false;
        for(std::list<People>::iterator i = peoples.begin() ; i != peoples.end() ; ++i)
        {
            if(!i->reached  && colissionDetected(myCar.x, myCar.y, myCar.width, myCar.height,
                                    i->x, i->y, i->width, i->height))
                                    {
                                        i->isOnboard = true;
                                        detected = true;
                                        i->x = myCar.x + 10;
                                        i->y = myCar.y + 10;
                                        myCar.hasPassenger = true;
                                        break;
                                    }
        }
        return detected;
    }
    static bool isPassengerReached(Car &myCar, std::list<People> &peoples, int &score)
    {
        bool detected = false;
        for(std::list<People>::iterator i = peoples.begin() ; i != peoples.end() ; ++i)
        {
            if(colissionDetected(i->x, i->y, i->width, i->height,
                                    i->destX, i->destY, i->destWidth, i->destHeight))
                                    {
                                        i->isOnboard = false;
                                        i->reached = true;
                                        i->x = 1000;
                                        i->y = 1000;
                                        detected = true;
                                        myCar.hasPassenger = false; 
                                        break;
                                    }
        }
        return detected;
    }
    static bool colissionDetected(int x1, int y1, int width1, int height1,
                            int x2, int y2, int width2, int height2)
    {
        // collision x-axis?
        bool collisionX = x1 + width1 > x2 &&
            x2 + width2> x1;
        // collision y-axis?
        bool collisionY = y1 + height1 > y2 &&
            y2 + height2 > y1;
        // collision only if on both axes
        return collisionX && collisionY;
    }
    
};
static int speed = 5;
std::list<Obstacle> obstacles;
std::list<Car> cars;
std::list<People> peoples;
string msg="";
Car myCar;
int score=0;
bool collisionStatus = false;
// seed the random numbers generator by current time (see the documentation of srand for further help)...

/* Function sets canvas size (drawing area) in pixels...
 *  that is what dimensions (x and y) your game will have
 *  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
 * */
void SetCanvasSize(int width, int height) {
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
}


int xI = 400, yI = 400;

void drawCar() {
	DrawSquare(xI, yI, 20, colors[WHITE]);
	glutPostRedisplay();
}


bool flag = true;
void moveCar() {
	
	for(std::list<Car>::iterator i = cars.begin() ; i != cars.end() ; ++i)
	{
		if (i->x > 10 && i->flag) {
			i->x -= speed;
			
			i->moveLeft(speed);
			if(i->x < 100)
				
				i->flag = false;

		}
		else if (i->x < 850 && !i->flag) {
			i->x += speed;
			i->moveRight(speed);
			if (i->x > 750)
				i->flag = true;
		}
	}
}
void Gamestarter()/**/{
	// set the background color using function glClearColor.
	// to change the background play with the red, green and blue values below.
	// Note that r, g and b values must be in the range [0,1] where 0 means dim rid and 1 means pure red and so on.

	glClearColor(0/*Red Component*/, 0,	//148.0/255/*Green Component*/,
			0/*Blue Component*/, 0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
	glClear (GL_COLOR_BUFFER_BIT); //Update the colors
	
	
DrawString( 200, 800, "0******STARTER MENU*******"+std::to_string(score), colors[WHITE]);
	
	ofstream MyFile("highscore.txt");

  // Write to the file
  MyFile << "User Name:Amna\nScore:100\n\nUser Name:Sarosh\nScore:80";

  // Close the file
  MyFile.close();
	

		
	
	
	glutPostRedisplay();
	glutSwapBuffers(); // do not modify this line..

}


/*
 * Main Canvas drawing function.
 * */

void GameDisplay()/**/{
	// set the background color using function glClearColor.
	// to change the background play with the red, green and blue values below.
	// Note that r, g and b values must be in the range [0,1] where 0 means dim rid and 1 means pure red and so on.

	glClearColor(.8/*Red Component*/, .8,	//148.0/255/*Green Component*/,
			.8/*Blue Component*/, 0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
	glClear (GL_COLOR_BUFFER_BIT); //Update the colors
	
	DrawLine( 500 , 0 ,  500 , 840 , 3 , colors[BLACK] );		
	DrawLine( 440 , 0 ,  440 , 840 , 3 , colors[BLACK] );
	DrawLine( 380 , 0 ,  380 , 840 , 3 , colors[BLACK] );
	DrawLine( 320 , 0 ,  320 , 840 , 3 , colors[BLACK] );
	DrawLine( 260 , 0 ,  260 , 840 , 3 , colors[BLACK] );
	DrawLine( 200 , 0 ,  200 , 840 , 3 , colors[BLACK] );
	DrawLine( 140 , 0 ,  140 , 840 , 3 , colors[BLACK] );		
	DrawLine( 80 , 0 ,  80 , 840 , 3 , colors[BLACK] );
	DrawLine( 20 , 0 ,  20 , 840 , 3 , colors[BLACK] );
	DrawLine( -160 , 0 ,  -160 , 840 , 3 , colors[BLACK] );
	DrawLine( 560 , 0 ,  560 , 840 , 3 , colors[BLACK] );
	DrawLine( 620 , 0 ,  620 , 840 , 3 , colors[BLACK] );
	DrawLine( 680 , 0 ,  680 , 840 , 3 , colors[BLACK] );
	DrawLine( 740 , 0 ,  740 , 840 , 3 , colors[BLACK] );
	DrawLine( 800 , 0 ,  800 , 840 , 3 , colors[BLACK] );
	DrawLine( 860 , 0 ,  860 , 840 , 3 , colors[BLACK] );
	DrawLine( 920 , 0 ,  920 , 840 , 3 , colors[BLACK] );
	DrawLine( 980 , 0 ,  980 , 840 , 3 , colors[BLACK] );

	//Display Score
	DrawString( 100, 800, "Score="+std::to_string(score), colors[BLACK]);
	DrawString( 500, 800, msg, colors[BLACK]);
	

	//rendering obstacles through Rects.	
	for(list<Obstacle>::iterator i = obstacles.begin() ; i != obstacles.end() ; ++i)
	{
		DrawRectangle( i->x, i->y,  i->width, i->height, colors[BLACK] );
	}

	//rendering cars
	for(list<Car>::iterator ca = cars.begin() ; ca != cars.end() ; ++ca)
	{
		DrawRectangle(ca->x, ca->y, ca->width, ca->height, ca->color);
	}
	DrawRectangle(myCar.x, myCar.y, myCar.width, myCar.height, myCar.color);
	
	//rendering people
	for(list<People>::iterator ca = peoples.begin() ; ca != peoples.end() ; ++ca)
	{
		if(!ca->reached)
		{/*
			//if onbaord he sits in car
			if(ca->isOnboard)
			{
				DrawCircle(myCar.x+5, myCar.y+5, ca->width/2, ca->color);
			}
			else//if not he is drawn where he already is
			*/
			{		
				DrawCircle(ca->x, ca->y, ca->width/2, ca->color);
			}
			DrawTriangle(ca->destX, ca->destY, ca->destX+ca->destWidth, ca->destY,
							ca->destX+(ca->destWidth/2), ca->destY+ca->destHeight,ca->color);
		}
	}
	
	glutPostRedisplay();
	glutSwapBuffers(); // do not modify this line..

}



/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
 * is pressed from the keyboard
 *
 * You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
 *
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 *
 * */

void NonPrintableKeys(int key, int x, int y) {
	if (key
			== GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/) {
		// what to do when left key is pressed...
		myCar.moveLeft(speed);

	} else if (key
			== GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) {
		myCar.moveRight(speed);

	} else if (key
			== GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) {
		myCar.moveUp(speed);

	}

	else if (key
			== GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/) {
		myCar.moveDown(speed);

	}

	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
	 * this function*/

	glutPostRedisplay();

}

/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
 * is pressed from the keyboard
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 * */
void PrintableKeys(unsigned char key, int x, int y) {
	if (key == 27/* Escape key ASCII*/) {
		exit(1); // exit the program when escape key is pressed.
	}

	if (key == 'b' || key == 'B') //Key for placing the bomb
			{
		//do something if b is pressed
		cout << "b pressed" << endl;

	}
	glutPostRedisplay();
}



/*
 * This function is called after every 1000.0/FPS milliseconds
 * (FPS is defined on in the beginning).
 * You can use this function to animate objects and control the
 * speed of different moving objects by varying the constant FPS.
 *
 * */
void Timer(int m) {

	// implement your functionality here
	moveCar();
	bool thisCollision = false;
	bool carColl = Physics::isCollisionWithCars(myCar, cars);
	bool obstColl = Physics::isCollisionWithObstacles(myCar, obstacles);
	bool peopleColl = Physics::isCollisionWithPeople(myCar, peoples);
	bool destinationColl = Physics::isPassengerReached(myCar, peoples, score);
	if(obstColl || carColl || peopleColl || destinationColl)
	{
		thisCollision = true;
		if(collisionStatus != thisCollision)
		{
			if(destinationColl)
			{
				msg = "reached with destination";
				score += 10;
			}
			else if(obstColl)
			{
				msg = "collission with obstacle";
				score -= 10;
			}
			else if(carColl)
			{
				msg = "collission with Cars";
				score -= 10;
			}

			else if(peopleColl)
			{
				msg = "Picked up Person";
				score += 10;
			}
			
			collisionStatus = thisCollision;
		}
	}
	else
	{
		msg = "";
		collisionStatus = false;

	}
	// once again we tell the library to call our Timer function after next 1000/FPS
	glutTimerFunc(100, Timer, 0);
}

/*This function is called (automatically) whenever your mouse moves witin inside the game window
 *
 * You will have to add the necessary code here for finding the direction of shooting
 *
 * This function has two arguments: x & y that tells the coordinate of current position of move mouse
 *
 * */
void MousePressedAndMoved(int x, int y) {
	cout << x << " " << y << endl;
	glutPostRedisplay();
}
void MouseMoved(int x, int y) {
	//cout << x << " " << y << endl;
	glutPostRedisplay();
}

/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window
 *
 * You will have to add the necessary code here for shooting, etc.
 *
 * This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),
 * x & y that tells the coordinate of current position of move mouse
 *
 * */
void MouseClicked(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON) // dealing only with left button
			{
		cout << GLUT_DOWN << " " << GLUT_UP << endl;

	} else if (button == GLUT_RIGHT_BUTTON) // dealing with right button
			{
			cout<<"Right Button Pressed"<<endl;

	}
	glutPostRedisplay();
}

/*making list of obstacles*/
void initObstacles()
{
	obstacles = {};
	obstacles.emplace_back(50,700,100,40);
	obstacles.emplace_back(250,700,550,40);

	//L shaped lines
	obstacles.emplace_back(180,350,40,300);
	obstacles.emplace_back(100,350,100,40);

	obstacles.emplace_back(700,100,100,40);
	obstacles.emplace_back(100,100,500,40);

	//L shaped lines
	obstacles.emplace_back(625,210,40,250);
	obstacles.emplace_back(560,420,100,40);
}
/*making list of People*/
void initPeople()
{
	peoples = {};
	peoples.emplace_back();
	peoples.emplace_back();
}
/*making list of random cars and user car*/
void initCars()
{
	int randomY = 50 + rand() % (( 800 + 1 ) - 50);
	cars = {};
	cars.emplace_back(200,randomY, colors[GREEN]);
	randomY = 50 + rand() % (( 800 + 1 ) - 50);
	cars.emplace_back(400,randomY, colors[GREEN]);
	randomY = 50 + rand() % (( 800 + 1 ) - 50);
	cars.emplace_back(600,randomY, colors[GREEN]);

	myCar = Car(0,800, colors[GOLD]);
}
/*
 * our gateway main function
 * */
int main(int argc, char*argv[]) {

	int width = 800, height = 850,choice,a,b; // i have set my window size to be 800 x 600

	InitRandomizer(); // seed the random number generator...
	initObstacles();
	initCars();
	initPeople();
	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("Rush Hour by Amna"); // set the title of our game window
	SetCanvasSize(width, height); // set the number of pixels...

	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	//glutDisplayFunc(display); // tell library which function to call for drawing Canvas.

	glutDisplayFunc(Gamestarter);

	glutDisplayFunc(GameDisplay); // tell library which function to call for drawing Canvas.

	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
	// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(1000.0, Timer, 0);

	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse
	glutMotionFunc(MousePressedAndMoved); // Mouse

	// now handle the control to library and it will call our registered functions when
	// it deems necessary...
	glutMainLoop();
	return 1;
}
#endif /* RushHour_CPP_ */
