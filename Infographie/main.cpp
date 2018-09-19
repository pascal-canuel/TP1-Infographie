#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Shader_Loader.h"

#define MENU_EXIT 10
#define MENU_RED 20
#define MENU_GREEN 30
#define MENU_BLUE 40
#define MENU_PURPLE 50
#define MENU_RANDOM_COLOR 60
#define MENU_POINT 70
#define MENU_LINE 80
#define MENU_TRIANGLE 90
#define MENU_RECTANGLE 100
#define MENU_CONTINUOUS_LINE 110

float posXndc;
float posYndc;
int currentForm;
GLuint program;

//	Fonction de rendu
void renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program);

	if (currentForm == GL_POINTS)
	{

	}

	glFlush();
}

#pragma region "Mouse"
void getMouse(int button, int state, int x, int y)
{
	float Xndc = (float)x * (1 + 1) / glutGet(GLUT_WINDOW_WIDTH) - 1;
	float Yndc = (-1.0 * (y - glutGet(GLUT_WINDOW_HEIGHT)) * (1 + 1)) / glutGet(GLUT_WINDOW_HEIGHT) - 1;

	if (GLUT_LEFT_BUTTON == button && GLUT_DOWN == state) {
		std::cout << "********* CLICK INFO *********" << std::endl;
		std::cout << "X: " << x << " Xndc: " << Xndc << std::endl;
		std::cout << "Y: " << y << " Yndc: " << Yndc << std::endl << std::endl;
		posXndc = Xndc;
		posYndc = Yndc;
	}
}
#pragma endregion

#pragma region "Fonctions utilitaires"
GLclampf randomColorValue()
{
	double random = rand() % 100 + 1;
	return (GLclampf)(random / 100);
}
#pragma endregion

#pragma region  "Menu & Sous-menu"
void traitementMenu(int valeur)
{
	if (valeur == MENU_EXIT)
	{
		glutLeaveMainLoop();
	}
}

void traitementSousMenuForme(int valeur)
{
	switch (valeur)
	{
	case MENU_POINT:
		currentForm = GL_POINTS;
		break;
	case MENU_LINE:
		break;
	case MENU_TRIANGLE:
		break;
	case MENU_RECTANGLE:
		break;
	case MENU_CONTINUOUS_LINE:
		break;
	default:
		break;
	}
}

void traitementSousMenuCouleurForme(int valeur)
{
	switch (valeur)
	{
	case MENU_RED:
		break;
	case MENU_GREEN:
		break;
	case MENU_BLUE:
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
	glutAddMenuEntry("Rectangle", MENU_RECTANGLE);
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
	glutAddMenuEntry("Exit", MENU_EXIT);

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
	//	Fonction de rappel de fermeture?

	Core::Shader_Loader shaderLoader;

	program = shaderLoader.CreateProgram((char *)"Vertex_Shader.glsl", (char *)"Fragment_Shader.glsl");

	creerMenu();

	//	d�marre la boucle de traitement des �v�nements 
	glutMainLoop();

	return 0;
}

