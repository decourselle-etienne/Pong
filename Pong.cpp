// Pong.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <sstream>
#include <math.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include "GL/freeglut.h"
#pragma comment(lib, "OpenGL32.lib")

//taille de la fenêtre et fréquence de rafraichissement (60fps)
int width = 1000;
int height = 400;
int interval = 1000 / 60;

//keycodes
#define VK_Z 0x5A
#define VK_S 0x53

//score des joueurs
int score_left = 0;
int score_right = 0;

//taille des raquettes
int racket_width = 10;
int racket_height = 80;
int racket_speed = 3;

//position de la raquette gauche
float racket_left_x = 10.0f;
float racket_left_y = 50.0f;

//position de la raquette droite
float racket_right_x = width - racket_width - 10;
float racket_right_y = 50;

//balle
float ball_pos_x = width / 2;
float ball_pos_y = height / 2;
float ball_dir_x = -1.0f;
float ball_dir_y = 0.0f;
int ball_size = 8;
int ball_speed = 6s;

//Conversion des INT en STRING
std::string int2str(int x)
{
	std::stringstream ss;
	ss << x;
	return ss.str();
}


void drawText(float x, float y, std::string text)
{
	//configuration du score
	glRasterPos2f(x, y);
	glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char*) text.c_str());
}


void keyboard ()
{
	//déplacement raquette gauche
	if(GetAsyncKeyState(VK_Z)) racket_left_y += racket_speed;
	if(GetAsyncKeyState(VK_S)) racket_left_y -= racket_speed;

	//déplacement raquette gauche
	if(GetAsyncKeyState(VK_UP)) racket_right_y += racket_speed;
	if(GetAsyncKeyState(VK_DOWN)) racket_right_y -= racket_speed;
}


void drawRect(float x, float y, float width, float height)
{
	//création des raquettes
	glBegin(GL_QUADS);
		glVertex2f(x, y);
		glVertex2f(x+width, y);
		glVertex2f(x+width, y+height);
		glVertex2f(x, y+height);
	glEnd();
}

void updateBall() 
{
	//Déplacement de la balle
	ball_pos_x += ball_dir_x * ball_speed;
	ball_pos_y += ball_dir_y * ball_speed;

	//Balle touchée par la raquette de gauche >> inverse la direction de la balle
	if(ball_pos_x < racket_left_x + racket_width && 
		ball_pos_x > racket_left_x &&
		ball_pos_y < racket_left_y + racket_height &&
		ball_pos_y > racket_left_y) {
			float t = ((ball_pos_y - racket_left_y) / racket_height) -0.5f;
			ball_dir_x = fabs(ball_dir_x);
			ball_dir_y = t;
	}

	//Balle touchée par la raquette de droite >> inverse la direction de la balle
	if(ball_pos_x > racket_right_x &&
		ball_pos_x < racket_right_x + racket_width && 
		ball_pos_y < racket_right_y + racket_height &&
		ball_pos_y > racket_right_y) {
			float t = ((ball_pos_y - racket_right_y) / racket_height) -0.5f;
			ball_dir_x = -fabs(ball_dir_x);
			ball_dir_y = t;
	}

	//Balle touche le mur de gauche
	if(ball_pos_x > width) {
		++score_left;
		ball_pos_x = width/2;
		ball_pos_y = height/2;
		ball_dir_x = -fabs(ball_dir_x);
		ball_dir_y = 0;
	}

	//Balle touche le mur de droite
	if(ball_pos_x < 0) {
		++score_right;
		ball_pos_x = width/2;
		ball_pos_y = height/2;
		ball_dir_x = fabs(ball_dir_x);
		ball_dir_y = 0;
	}

	//Balle touche le mur du haut
	if(ball_pos_y > height) {
			ball_dir_y = -fabs(ball_dir_y);
	}

	//Balle touche le mur du bas
	if(ball_pos_y < 0) {
		ball_dir_y = fabs(ball_dir_y);
	}
}


void update(int value)
{
	//gestion du clavier
	keyboard();

	//Mise à jour automatique de la balle
	updateBall();

	//Call update() en millisecondes d'intervalle
	glutTimerFunc(interval, update, 0);
	
	//Redessiner le cadre
	glutPostRedisplay();
}


void draw() 
{
	//clear (doit être fait au début)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//Dessiner les raquettes
	drawRect(racket_left_x, racket_left_y, racket_width, racket_height);
	drawRect(racket_right_x, racket_right_y, racket_width, racket_height);

	//Dessiner la balle
	drawRect(ball_pos_x - ball_size / 2, ball_pos_y - ball_size / 2, ball_size, ball_size);

	//Dessiner le score
	drawText(width/2-10, height-15, int2str(score_left)+" : "+int2str(score_right));

	//échange les variables tampon (doit être fait à la fin)
	glutSwapBuffers();
}


void enable2D(int width, int height)
{
	//configuration de l'espace 2D
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, width, 0.0f, height, 0.0f, 1.0f);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();	
}





//Début du programme
	int _tmain(int argc, char** argv)
	{
		//initialisation opengl (via glut)
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
		glutInitWindowSize(width,height);
		glutCreateWindow("Pong");

		//Fonction Call Back
		glutDisplayFunc(draw);
		glutTimerFunc(interval, update, 0);

		//mise en place de l'espace en 2D et de l'utilisation de la couleur blanche
		enable2D(width, height);
		glColor3f(1.0f, 1.0f, 1.0f);

		//démarrage du jeu
		glutMainLoop();
		return 0;
	}