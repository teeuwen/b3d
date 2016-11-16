/*
 *
 * Bastiaan's Game Engine
 * src/main.c
 *
 * Copyright (C) 2016 Bastiaan Teeuwen <bastiaan.teeuwen170@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 *
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <png.h>

#include <AL/alut.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <config.h>

/* XXX Relocate */

#define PI		3.14159265358979323846

#define DTR(d)		((d) * PI / 180)

#define KEY_FORWARD	0b00001
#define KEY_BACKWARD	0b00010
#define KEY_LEFT	0b00100
#define KEY_RIGHT	0b01000
#define KEY_RUN		0b10000

struct coord {
	double x;
	double y;
	double z;
};

/*struct player {
	struct coord	loc;
	struct coord	rot;
};*/

GLFWwindow *window;

short keys;

/* XXX TEMP */
double angt;
double az;

#if 1
struct coord loc;
struct coord rot;
#endif

/* TODO Clean and free */
GLuint texture_load(const char *path)
{
	FILE *file;
	png_structp png;
	png_info *info;

	int i, width, height;
	png_byte *data;
	png_byte **rows;

	GLuint texture;

	file = fopen(path, "r");

	if (!file)
		return 0;

	png = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);

	if (!png)
		return 0;

	info = png_create_info_struct(png);

	if (!info)
		return 0;

	png_init_io(png, file);
	png_read_info(png, info);

	width = png_get_image_width(png, info);
	height = png_get_image_height(png, info);

	data = malloc(png_get_rowbytes(png, info) * height * sizeof(png_byte));

	if (!data)
		return 0;

	rows = malloc(height * sizeof(png_byte *));

	if (!rows)
		return 0;

	for (i = 0; i < height; i++)
		rows[height - 1 - i] = data + i * png_get_rowbytes(png, info);

	png_read_image(png, rows);

	fclose(file);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
			width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	return texture;
}

void draw(void)
{
	double i;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	if (keys & KEY_FORWARD) {
		loc.z += ((keys & KEY_RUN) ? 0.12f : 0.06f) * cos(DTR(rot.y));
		loc.x -= ((keys & KEY_RUN) ? 0.12f : 0.06f) * sin(DTR(rot.y));
	}

	if (keys & KEY_BACKWARD) {
		loc.z -= 0.06f * cos(DTR(rot.y));
		loc.x += 0.06f * sin(DTR(rot.y));
	}

	if (keys & KEY_LEFT) {
		loc.z += 0.06f * sin(DTR(rot.y));
		loc.x += 0.06f * cos(DTR(rot.y));
	}

	if (keys & KEY_RIGHT) {
		loc.z -= 0.06f * sin(DTR(rot.y));
		loc.x -= 0.06f * cos(DTR(rot.y));
	}

	/* TODO Head bouncing */

	glRotatef(rot.x, 1.0f, 0.0f, 0.0f);
	glRotatef(rot.y, 0.0f, 1.0f, 0.0f);
	/* glTranslatef(-x, -y, -z); */
	/* glTranslatef(0, 0, 1.0f); */
	glTranslatef(loc.x, -2.0f, loc.z);
	/* glScalef(-2.0f, -2.0f, -2.0f); */

#if 1
	/* Floor */
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glBegin(GL_LINES);

	glColor3f(1.0f, 1.0f, 1.0f);

	for (i = -50.0f; i < 50.0f; i += 1.0f) {
		glVertex3f(i, 0.0f,  50.0f);
		glVertex3f(i, 0.0f, -50.0f);

		glVertex3f(-50.0f, 0.0f, i);
		glVertex3f( 50.0f, 0.0f, i);
	}

	glEnd();

	glPopMatrix();
#endif

#if 1
	/* Cube */
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	texture_load("src/textures/test.png");

	glTranslatef(0.0f, 0.0f, -14.0f);
	glRotatef(angt, 0.0f, 1.0f, 0.0f);

	if (angt > 360.0f)
		angt = 0.0f;
	angt += 0.2f;

	/* FIXME Use GL_TRIANGLES, GL_QUADS will be deprecated */
	glBegin(GL_QUADS);

	/* Top */
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f,  1.0f,  0.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  1.0f,  0.5f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f,  1.0f, -0.5f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f,  1.0f, -0.5f);

	/* Bottom */
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f,  1.0f, -0.5f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f,  1.0f, -0.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  1.0f,  0.5f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f,  1.0f,  0.5f);

	/* Front */
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f,  0.0f,  0.5f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f,  0.0f,  0.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  1.0f,  0.5f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f,  1.0f,  0.5f);

	/* Back */
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f,  1.0f, -0.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  1.0f, -0.5f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f,  0.0f, -0.5f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f,  0.0f, -0.5f);

	/* Left */
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f,  1.0f,  0.5f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  1.0f, -0.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f,  0.0f, -0.5f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f,  0.0f,  0.5f);

	/* Right */
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f,  0.0f,  0.5f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f,  0.0f, -0.5f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  1.0f, -0.5f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f,  1.0f,  0.5f);

	glEnd();

	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
#endif
}

void key_handle(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	(void) scancode;

	if (mods & BNDM_RUN)
		keys |= KEY_RUN;
	else
		keys &= ~KEY_RUN;

	if (action == GLFW_PRESS) {
		switch (key) {
		case BNDK_QUIT:
			glfwSetWindowShouldClose(window, 1);
			break;
		case BNDK_FORWARD:
			keys |= KEY_FORWARD;
			break;
		case BNDK_BACKWARD:
			keys |= KEY_BACKWARD;
			break;
		case BNDK_LEFT:
			keys |= KEY_LEFT;
			break;
		case BNDK_RIGHT:
			keys |= KEY_RIGHT;
			break;
		}
	} else if (action == GLFW_RELEASE) {
		switch (key) {
		case BNDK_FORWARD:
			keys &= ~KEY_FORWARD;
			break;
		case BNDK_BACKWARD:
			keys &= ~KEY_BACKWARD;
			break;
		case BNDK_LEFT:
			keys &= ~KEY_LEFT;
			break;
		case BNDK_RIGHT:
			keys &= ~KEY_RIGHT;
			break;
		}
	}
}

void cur_handle(GLFWwindow *window, double xpos, double ypos)
{
	rot.x += (ypos - (WINDOW_HEIGHT / 2)) / CUR_SENSITIVITY;
	rot.y += (xpos - (WINDOW_WIDTH / 2)) / CUR_SENSITIVITY;

	if (rot.x < -90.0f)
		rot.x = -90.0f;
	else if (rot.x > 90.0f)
		rot.x = 90.0f;

	if (rot.y < -180.0f)
		rot.y += 360.0f;
	else if (rot.y > 180.0f)
		rot.y -= 360.0f;

	glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
}

int main()
{
	double time;
	int frame_cnt = 0;

	int res = 0;

	if (!glfwInit())
		goto err;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "B3D",
			NULL, NULL);

	if (!window)
		goto err;

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_handle);
	glfwSetCursorPosCallback(window, cur_handle);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(FIELD_OF_VIEW, (double) WINDOW_WIDTH / WINDOW_HEIGHT,
			0.01f, RENDER_DISTANCE);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	/* TODO Lighting */

	time = glfwGetTime();

#ifdef AUDIO
	if (!alutInit(0, 0))
		goto err;
#endif

	printf("\e[?25l\n\n\n\n\n\n\n\n\n");

	while (!glfwWindowShouldClose(window)) {
		printf("\033[A\033[A\033[A\033[A\033[A\033[A\033[A\033[A\033[A");

		if (glfwGetTime() - time >= 1.0) {
			printf("\033[2K\r%f FPS\n",
					frame_cnt / (glfwGetTime() - time));

			frame_cnt = 0;
			time += 1.0;
		} else {
			printf("\n");
		}

		printf("\033[2K\rLocation:\n");
		printf("\033[2K\rX: %7.2f\n", loc.x);
		printf("\033[2K\rY: %7.2f\n", loc.y);
		printf("\033[2K\rZ: %7.2f\n", loc.z);

		printf("\033[2K\rRotation:\n");
		printf("\033[2K\rX: %7.2f\n", rot.x);
		printf("\033[2K\rY: %7.2f\n", rot.y);
		printf("\033[2K\rZ: %7.2f\n", rot.z);

		draw();

		frame_cnt++;

		glfwPollEvents();

		glfwSwapBuffers(window);
	}

	printf("\e[?25h");

	goto ret;

err:
	res = 1;

	fprintf(stderr, "An internal OpenGL error has occured!\n");

ret:
#ifdef AUDIO
	alutExit();
#endif

	glfwTerminate();

	return res;
}
