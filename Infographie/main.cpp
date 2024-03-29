/***********************************
* Pascal Canuel
* 2018-09-24
* TP1 - Infographie
************************************/
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm\glm.hpp"
#include <iostream>
#include <vector> 
#include <cstdlib>
#include <ctime>
#include "Shader_Loader.h"
#include <algorithm>    

#define MENU_REINITIALIZE 5
#define MENU_EXIT 10
#define MENU_RED 20
#define MENU_GREEN 30
#define MENU_BLUE 40
#define MENU_PURPLE 50
#define MENU_RANDOM_COLOR 60
#define MENU_POINT 70
#define MENU_LINE 80
#define MENU_TRIANGLE 90
#define MENU_QUAD 100
#define MENU_CONTINUOUS_LINE 110

struct posVertex {
	float x;
	float y;
	posVertex(float pX, float pY) {
		x = pX;
		y = pY;
	}
};

struct coulVertex {
	float red;
	float green;
	float blue;
	coulVertex() {
		red = 0.0f;
		green = 0.0f;
		blue = 0.0f;
	}
	coulVertex(float pRed, float pGreen, float pBlue) {
		red = pRed;
		green = pGreen;
		blue = pBlue;
	}
};

int _currentForm = MENU_POINT;

coulVertex _currentColor = coulVertex(0.0f, 1.0f, 0.0f);;
bool _isRandomColor;

std::vector<posVertex> _sommets;
std::vector<coulVertex> _couleurs;

bool _mouseHold;
GLuint _program;
GLuint _buffSommets;
GLuint _buffCouleurs;

//	Fonction de rendu
void renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(_program);
	
	glGenBuffers(1, &_buffSommets); //	Cr�ation du buffer
	glEnableVertexAttribArray(0); //	Activation du buffer
	glBindBuffer(GL_ARRAY_BUFFER, _buffSommets); //	Binding du buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(posVertex) * _sommets.size(), _sommets.data(), GL_STREAM_DRAW); //	Insertion des donn�es dans le buffer	
	glVertexAttribPointer(0, 2, GL_FLOAT, FALSE, 0, NULL); //	Explication des donn�es du buffer

	glGenBuffers(1, &_buffCouleurs); //	Cr�ation du buffer
	glEnableVertexAttribArray(1); //	Activation du buffer
	glBindBuffer(GL_ARRAY_BUFFER, _buffCouleurs); //	Binding du buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(coulVertex) * _couleurs.size(), _couleurs.data(), GL_STREAM_DRAW); //	Insertion des donn�es dans le buffer	
	glVertexAttribPointer(1, 3, GL_FLOAT, FALSE, 0, NULL); //	Explication des donn�es du buffer	

	if (_sommets.size() != 0)
	{
		switch (_currentForm)
		{
		case MENU_POINT:
		{
			glPointSize(15.0);
			glDrawArrays(GL_POINTS, 0, _sommets.size());
			break;
		}
		case MENU_LINE:
		{
			glLineWidth(3.0);
			glDrawArrays(GL_LINES, 0, _sommets.size());
			break;
		}
		case MENU_TRIANGLE:
		{
			glDrawArrays(GL_TRIANGLES, 0, _sommets.size());
			break;
		}
		case MENU_QUAD:
		{
			int restant = _sommets.size() % 4;
			int countQuad = (_sommets.size() - restant) / 4;
			for (int i = 0; i < countQuad; i++) {
				glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
			}
			break;
		}
		case MENU_CONTINUOUS_LINE:
		{
			glLineWidth(3.0);
			glDrawArrays(GL_LINE_STRIP, 0, _sommets.size());
			break;
		}
		default:
			break;
		}
	}

	glFlush();

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

#pragma region "Fonctions utilitaires"
GLclampf randomColorValue()
{
	double random = rand() % 100 + 1;
	return (GLclampf)(random / 100);
}
#pragma endregion

#pragma region "Mouse"
void getMouse(int button, int state, int x, int y)
{
	//	TODO cr�er m�thode de conversion
	float Xndc = (float)x * (1 + 1) / glutGet(GLUT_WINDOW_WIDTH) - 1;
	float Yndc = (-1.0 * (y - glutGet(GLUT_WINDOW_HEIGHT)) * (1 + 1)) / glutGet(GLUT_WINDOW_HEIGHT) - 1;

	if (GLUT_LEFT_BUTTON == button && GLUT_DOWN == state) {
		std::cout << "********* CLICK INFO *********" << std::endl;
		std::cout << "X: " << x << " Xndc: " << Xndc << std::endl;
		std::cout << "Y: " << y << " Yndc: " << Yndc << std::endl;

		_mouseHold = true;
		if (_currentForm >= 0) {
			
			_sommets.push_back(posVertex(Xndc, Yndc));

			if (_isRandomColor) {
				_couleurs.push_back(coulVertex(randomColorValue(), randomColorValue(), randomColorValue()));
			} else {
				_couleurs.push_back(_currentColor);
			}
			
		}
		else {
			std::cout << "No form selected" << std::endl << std::endl;
		}
		
	}

	if(GLUT_LEFT_BUTTON == button && GLUT_UP == state) _mouseHold = false;
}

void getMouseMotion(int x, int y)
{
	float Xndc = (float)x * (1 + 1) / glutGet(GLUT_WINDOW_WIDTH) - 1;
	float Yndc = (-1.0 * (y - glutGet(GLUT_WINDOW_HEIGHT)) * (1 + 1)) / glutGet(GLUT_WINDOW_HEIGHT) - 1;
	
	if (_currentForm == MENU_CONTINUOUS_LINE) {
		_sommets.push_back(posVertex(Xndc, Yndc));

		if (_isRandomColor) {
			_couleurs.push_back(coulVertex(randomColorValue(), randomColorValue(), randomColorValue()));
		}
		else {
			_couleurs.push_back(_currentColor);
		}
	}
	//	redessiner la fen�tre
	glutPostRedisplay();
}
#pragma endregion

#pragma region  "Menu & Sous-menu"
void traitementMenu(int valeur)
{
	if (valeur == MENU_EXIT)
	{
		glDeleteBuffers(1, &_buffSommets);
		glDeleteBuffers(1, &_buffCouleurs);
		glDeleteProgram(_program);
		glutLeaveMainLoop();
	}
	else if (valeur == MENU_REINITIALIZE) {
		_sommets.clear();
		_couleurs.clear();
		glClearColor(1, 1, 1, 0); // fond d'�cran blanc
		//	redessiner la fen�tre
		glutPostRedisplay();
	}
}

void traitementSousMenuForme(int valeur)
{
	_sommets.clear();
	_couleurs.clear();
	switch (valeur)
	{
	case MENU_POINT:
		_currentForm = MENU_POINT;
		break;
	case MENU_LINE:
		_currentForm = MENU_LINE;
		break;
	case MENU_TRIANGLE:
		_currentForm = MENU_TRIANGLE;
		break;
	case MENU_QUAD:
		_currentForm = MENU_QUAD;
		break;
	case MENU_CONTINUOUS_LINE:
		_currentForm = MENU_CONTINUOUS_LINE;
		break;
	default:
		break;
	}
}

void traitementSousMenuCouleurForme(int valeur)
{
	_isRandomColor = false;
	switch (valeur)
	{
	case MENU_RED:
		_currentColor = coulVertex(1.0f, 0.0f, 0.0f);
		break;
	case MENU_GREEN:
		_currentColor = coulVertex(0.0f, 1.0f, 0.0f);
		break;
	case MENU_BLUE:
		_currentColor = coulVertex(0.0f, 0.0f, 1.0f);
		break;
	case MENU_PURPLE:		
		_currentColor = coulVertex(0.5f, 0.0f, 1.0f);
		break;
	case MENU_RANDOM_COLOR:
		_isRandomColor = true;
		break;
	default:
		break;
	}
}

void traitementSousMenuCouleurFond(int valeur)
{
	switch (valeur)
	{
	case MENU_RED:
		glClearColor(1, 0, 0, 0);
		break;
	case MENU_GREEN:
		glClearColor(0, 1, 0, 0);
		break;
	case MENU_BLUE:
		glClearColor(0, 0, 1, 0);
		break;
	case MENU_PURPLE:
		glClearColor(0.5, 0, 1, 0);
		break;
	case MENU_RANDOM_COLOR:
		glClearColor(randomColorValue(), randomColorValue(), randomColorValue(), 0);
		break;
	default:
		break;
	}

	//	redessiner la fen�tre
	glutPostRedisplay();
}

void creerMenu()
{
	int menuPrincipal;
	int sousMenuForme, sousMenuCouleurForme, sousMenuCouleurFond;

	sousMenuForme = glutCreateMenu(traitementSousMenuForme);
	glutAddMenuEntry("Point", MENU_POINT);
	glutAddMenuEntry("Ligne", MENU_LINE);
	glutAddMenuEntry("Triangle", MENU_TRIANGLE);
	glutAddMenuEntry("Quad", MENU_QUAD);
	glutAddMenuEntry("Ligne continue", MENU_CONTINUOUS_LINE);

	sousMenuCouleurForme = glutCreateMenu(traitementSousMenuCouleurForme);
	glutAddMenuEntry("Bleu", MENU_BLUE);
	glutAddMenuEntry("Vert", MENU_GREEN);
	glutAddMenuEntry("Rouge", MENU_RED);
	glutAddMenuEntry("Mauve", MENU_PURPLE);
	glutAddMenuEntry("Al�atoire", MENU_RANDOM_COLOR);

	sousMenuCouleurFond = glutCreateMenu(traitementSousMenuCouleurFond);
	glutAddMenuEntry("Bleu", MENU_BLUE);
	glutAddMenuEntry("Vert", MENU_GREEN);
	glutAddMenuEntry("Rouge", MENU_RED);
	glutAddMenuEntry("Mauve", MENU_PURPLE);
	glutAddMenuEntry("Al�atoire", MENU_RANDOM_COLOR);

	menuPrincipal = glutCreateMenu(traitementMenu);
	glutAddSubMenu("Forme", sousMenuForme);
	glutAddSubMenu("Couleur forme", sousMenuCouleurForme);
	glutAddSubMenu("Couleur fond", sousMenuCouleurFond);
	glutAddMenuEntry("Quitter", MENU_EXIT);
	glutAddMenuEntry("R�initialiser", MENU_REINITIALIZE);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

}
#pragma endregion

int main(int argc, char **argv)
{
	//	initialiser la valeur de base utilis�e pour la fonction rand()
	srand((int)time(0));

	//	�crire ici le code d�initialisation
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Travail Pratique 1 - Infographie");
	glClearColor(1, 1, 1, 0); // fond d'�cran blanc � l'ouverture

	glewInit();
	
	glutDisplayFunc(renderScene);
	glutMouseFunc(getMouse);
	glutMotionFunc(getMouseMotion);
	//	Fonction de rappel de fermeture?

	Core::Shader_Loader shaderLoader;

	_program = shaderLoader.CreateProgram((char *)"Vertex_Shader.glsl", (char *)"Fragment_Shader.glsl");

	creerMenu();

	//	d�marre la boucle de traitement des �v�nements 
	glutMainLoop();

	return 0;

}

