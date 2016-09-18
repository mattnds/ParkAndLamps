//
// Perspective view of a color cube using LookAt() and Ortho()
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.
//

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include "Angel.h"
#include "simplemodels.h"

// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Zaxis;
GLfloat  Theta1[NumAxes] = { 0.0, 0.0, 0.0 };
GLfloat  Theta2[NumAxes] = { 0.0, 0.0, 0.0 };

float theta_x = 20, theta_y = 20, theta_z = 20;


size_t CUBE_OFFSET;
size_t CYLINDER_OFFSET;

GLuint program;
GLuint vPosition;
GLuint vColor;




// OpenGL initialization
void
init()
{
	program = InitShader("vshader21.glsl", "fshader21.glsl");
	glUseProgram(program);

	//---------------------------------------------------------------------
	colorcube();
	colortube();
	//---------------------------------------------------------------------


	//----set offset variables

	CUBE_OFFSET = 0;
	CYLINDER_OFFSET = sizeof(points_cube) + sizeof(colors);

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);  // removed 'APPLE' suffix for 3.2
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points_cube) + sizeof(colors) + sizeof(points_cylinder), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, CUBE_OFFSET, sizeof(points_cube), points_cube);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points_cube), sizeof(colors), colors);
	glBufferSubData(GL_ARRAY_BUFFER, CYLINDER_OFFSET, sizeof(points_cylinder), points_cylinder);

	//---------------------------------------------------------------------

	// set up vertex arrays
	vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points_cube)));
	//---------------------------------------------------------------------

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
}

//----------------------------------------------------------------------------

void
display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 rot = RotateX(theta_x) * RotateY(theta_y) * RotateZ(theta_z);
	mat4 transform = Translate(0.5, 0.0, 0.0) * rot * Scale(0.25, 1.0, 0.25);

	std::cout << transform;

	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, transform);

	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_OFFSET));

	glUniform1i(glGetUniformLocation(program, "obj_color_on"), true);
	glUniform4fv(glGetUniformLocation(program, "obj_color"), 1, vec4(0.8, 0.0, 0.0, 1.0));

	glDrawArrays(GL_TRIANGLES, 0, NumVerticesCylinder);


	mat4  rot2 = RotateX(theta_x) * RotateY(theta_y) * RotateZ(theta_z);
	transform = Translate(-0.5, 0.0, 0.0) * rot2 * Scale(0.25, 1.0, 0.25);

	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, transform);

	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET));

	glUniform1i(glGetUniformLocation(program, "obj_color_on"), false);

	glDrawArrays(GL_TRIANGLES, 0, NumVerticesCube);

	glutSwapBuffers();
}

//----------------------------------------------------------------------------

void
keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 033: // Escape Key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	case 'x':  Axis = Xaxis;  break;
	case 'y':  Axis = Yaxis;  break;
	case 'z':  Axis = Zaxis;  break;

	}
}

//----------------------------------------------------------------------------
void
mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		switch (button) {
		case GLUT_LEFT_BUTTON:    Axis = Xaxis;  break;
		case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;  break;
		case GLUT_RIGHT_BUTTON:   Axis = Zaxis;  break;
		}
	}
}
//----------------------------------------------------------------------------

void
reshape(int width, int height)
{
	glViewport(0, 0, width, height);
}

//----------------------------------------------------------------------------


void 
idle(void) {
	theta_x = fmod(theta_x - fmod(rand(), 2), 360.0);
	theta_y = fmod(theta_y - fmod(rand(), 2), 360.0);
	theta_z = fmod(theta_z - fmod(rand(), 2), 360.0);

	glutPostRedisplay();
}

int
main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Color Cube");

	glewInit();

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}