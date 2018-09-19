#include "Shader_Loader.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace Core;

Shader_Loader::Shader_Loader() {}
Shader_Loader::~Shader_Loader() {}

//	Fonction de creation du "programme" contenant les shaders
GLuint Shader_Loader::CreateProgram(char * VertexShaderFilename, char * FragmentShaderFilename)
{
	std::string vertex_shader_code = ReadShader(VertexShaderFilename);
	std::string fragment_shader_code = ReadShader(FragmentShaderFilename);

	//	Crée et compile les shaders
	GLuint vertex_shader = CreateShader(GL_VERTEX_SHADER, vertex_shader_code, (char *)"vertex shader");
	GLuint fragment_shader = CreateShader(GL_FRAGMENT_SHADER, fragment_shader_code, (char *)"fragment shader");

	int link_result = 0;

	//	Crée le handle du programme et le lie aux shaders compilés
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	//	Lie le programme pour créer un exécutable qui sera traité par le GPU
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);

	//	Valide si la liaison a bien fonctionné
	if (link_result == GL_FALSE) {
		int info_log_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> program_log(info_log_length);
		glGetProgramInfoLog(program, info_log_length, NULL, &program_log[0]);
		
		std::cout << "Shader Loader : LINK ERROR" << std::endl << &program_log[0] << std::endl;
		return 0;
	}

	return program;
}

//	Fonction qui lit et retourne le code du fichier shader
std::string Shader_Loader::ReadShader(char * filename)
{
	std::string shaderCode;
	std::ifstream file(filename, std::ios::in);
	if (!file.good()) {
		std::cout << "Can't read file " << filename << std::endl;
		std::terminate();
	}

	file.seekg(0, std::ios::end); //	positioner à la fin du fichier
	//	Redimensionner la chaine de caractères selon la postiion du dernier caractère dans le fichier lu
	shaderCode.resize((unsigned int)file.tellg());

	file.seekg(0, std::ios::beg);
	file.read(&shaderCode[0], shaderCode.size());

	file.close();

	return shaderCode;
}

//	Fonction quit prend le code source d'un shader, le crée et le compile
GLuint Shader_Loader::CreateShader(GLenum shaderType, std::string source, char * shaderName)
{
	int compile_result = 0;

	//	Crée un objet shader vide
	GLuint shader = glCreateShader(shaderType);
	//	Obtient les informations du code source du shader
	const char *shader_code_ptr = source.c_str();
	const int shader_code_size = source.size();

	//	Applique le code source au nouvel objet shader, puis le compile
	glShaderSource(shader, 1, &shader_code_ptr, &shader_code_size);
	glCompileShader(shader);

	//	Valide l'état de la compilation
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_result);

	if (compile_result == GL_FALSE) {
		int info_log_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> shader_log(info_log_length);
		glGetShaderInfoLog(shader, info_log_length, NULL, &shader_log[0]);
		
		std::cout << "ERROR compiling shader: " << shaderName << std::endl;
		return 0;
	}

	return shader;
}
