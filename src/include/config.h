/*
 *
 * Bastiaan's Game Engine
 * src/include/config.h
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

#ifndef _CONFIG_H
#define _CONFIG_H

#include <GLFW/glfw3.h>

/* #define AUDIO */

#define WINDOW_WIDTH	1024
#define WINDOW_HEIGHT	768

#define CUR_SENSITIVITY	16.0f

#define BNDK_QUIT	GLFW_KEY_Q
#define BNDK_FORWARD	GLFW_KEY_W
#define BNDK_BACKWARD	GLFW_KEY_S
#define BNDK_LEFT	GLFW_KEY_A
#define BNDK_RIGHT	GLFW_KEY_D
#define BNDM_RUN	GLFW_MOD_SHIFT

#endif
