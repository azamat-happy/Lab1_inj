#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glm/glm.hpp"
#include "Pipeline.h"
#include "Pipeline.h"
// Задаём параметры окна
#define WINDOW_WIDTH 1366
#define WINDOW_HEIGHT 768
#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)
// Хранит указатель на буфер вершин
GLuint VBO;
//Мы используем этот указатель для доступа к всемирной матрице,
//представленной в виде uniform - переменной внутри шейдера.
//Всемирная она потому, что всё что мы делаем с объектом, это 
//изменение его позиции в место, которое мы указываем относительно 
//координатной системы внутри нашего виртуального 'мира'.

GLuint gWorldLocation;
static const char* pVS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
layout (location = 0) in vec3 Position;                                             \n\
                                                                                    \n\
uniform mat4 gWorld;                                                                \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    gl_Position = gWorld * vec4(Position, 1.0);                                     \n\
}";

static const char* pFS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
out vec4 FragColor;                                                                 \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    FragColor = vec4(1.0, 1.0, 0.0, 1.0);                                           \n\
}";

static void RenderSceneCB() {
	//очищение окна (используя цвет, заданный выше)
	glClearColor(0.1f, 0.9f, 0.4f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//угол
	static float Scale = 0.0f;
	Scale += 0.001f;
	glm::mat4 World1;

	
	//Преобразования
	// Создаём pipeline для трансформаций
	Pipeline p;

	p.Scale(cos(Scale * 0.5), sinf(Scale * 0.5), 0.0f);
	p.WorldPos(sinf(Scale) / 2, cosf(Scale) / 2, 0.0f);
	p.Rotate(1.0f, 1.0f, 1.0f);

	p.PerspectiveProj(100.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 200.0f);
	// Загружаем данные в uniform - переменные шейдера(адрес переменной, количество матриц,
	// передаётся ли матрица по строкам, указатель на первый элемент матрицы)
	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)p.getTransformation());
	//Наш третий параметр в glUniformMatrix4fv() - это GL_TRUE, потому что мы поставляем матрицу упорядоченную по строкам.
	//Четвертый параметр - это просто указатель на первый элемент матрицы.
	//glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &Worllll[0][0]);

	// Разрешение использования каждого атрибута вершины (аттрибут вершины)
	glEnableVertexAttribArray(0);
	// Обратно привязываем буфер, приготавливая для отрисовки
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Этот вызов говорит конвейеру как воспринимать данные внутри буфера(индекс атрибута, количество аттрибутов,
	// тип данных каждого компонента, нормализировать ли данные перед использованием, шаг - число байтов
	// между двумя экземплярами атрибута, смещение первого компонента первого универсального атрибута вершины)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	////вызвали функцию для отрисовки (режим рисования, индекс первого элемента в буфере, количество элементов)
	glDrawArrays(GL_TRIANGLES, 0, 3);
	// Отключение каждого атрибута вершины
	glDisableVertexAttribArray(0);
	// Меняем фоновый буфер и буфер кадра местами
	glutSwapBuffers();
}
static void CreateVertexBuffer()
{
	glm::vec3 Vertices[3];
	Vertices[0] = glm::vec3(-1.0f, -1.0f, 0.0f);
	Vertices[1] = glm::vec3(1.0f, -1.0f, 0.0f);
	Vertices[2] = glm::vec3(0.0f, 1.0f, 0.0f);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}
static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		exit(0);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	//Функция glShaderSource принимает тип шейдера как параметр
	//Второй параметр - это размерность обоих массивов (в нашем случае это 1).
	glShaderSource(ShaderObj, 1, p, Lengths);
	glCompileShader(ShaderObj);

	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		exit(1);
	}

	//мы присоединяем скомпилированный объект шейдера к объекту программы
	glAttachShader(ShaderProgram, ShaderObj);
}

static void CompileShaders()
{
	//Мы начинаем процесс разработки шейдеров через создание 
	/*программного объекта.
	Позже мы слинкуем все шейдеры в этот объект.*/
	GLuint ShaderProgram = glCreateProgram();

	if (ShaderProgram == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	AddShader(ShaderProgram, pVS, GL_VERTEX_SHADER);
	AddShader(ShaderProgram, pFS, GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//После компиляции всех шейдеров и подсоединения их к программе мы наконец можем линковать их.
	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	glUseProgram(ShaderProgram);

	gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld");
	assert(gWorldLocation != 0xFFFFFFFF);
}
int main(int argc, char** argv)
{
	// Инициализация glut
	glutInit(&argc, argv);
	//двойная буферизация и буфер цвета
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	//задание размера окна (ширина, высота)
	glutInitWindowSize(1024, 768);
	//задание положения позиции окна (x и y относительно левого верхнего угла)
	glutInitWindowPosition(100, 100);
	//создание окна приложения с названием "Tutorial 01"
	glutCreateWindow("Tutorial 04");

	glutDisplayFunc(RenderSceneCB);
	//Здесь мы указываем функцию рендера в качестве ленивой.
	glutIdleFunc(RenderSceneCB);
	// Инициализируем glew
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	CreateVertexBuffer();

	CompileShaders();

	glutMainLoop();

	return 0;
}
