/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Game.cpp																			  *
*	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "Cube.h"
#include "Mat3.h"

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
}

void Game::Go()
{
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	const float dt = 1.0f / 60.0f;

	if (wnd.kbd.KeyIsPressed('Q'))
	{
		theta_y += clamp_angle(dt * dTheta);
	}
	if (wnd.kbd.KeyIsPressed('E'))
	{
		theta_y -= clamp_angle(dt * dTheta);
	}
	if (wnd.kbd.KeyIsPressed('W'))
	{
		theta_x += clamp_angle(dt * dTheta);
	}
	if (wnd.kbd.KeyIsPressed('S'))
	{
		theta_x -= clamp_angle(dt * dTheta);
	}
	if (wnd.kbd.KeyIsPressed('A'))
	{
		theta_z += clamp_angle(dt * dTheta);
	}
	if (wnd.kbd.KeyIsPressed('D'))
	{
		theta_z -= clamp_angle(dt * dTheta);
	}
	if (wnd.kbd.KeyIsPressed('R'))
	{
		offset_z += 2.0f * dt;
		if (offset_z < 2.0f)
			offset_z = 2.0f;
	}
	if (wnd.kbd.KeyIsPressed('F'))
	{
		offset_z -= 2.0f * dt;
		if (offset_z < 2.0f)
			offset_z = 2.0f;
	}
}

void Game::ComposeFrame()
{
	Cube cube{ 1.0f };

	IndexedLineList lines = cube.GetLines();
	IndexedTriangleList triangles = cube.GetTriangles();

	const Mat3 rot =
		Mat3::RotationX(theta_x)*
		Mat3::RotationY(theta_y)*
		Mat3::RotationZ(theta_z);

	for (auto &v : lines.vertices)
	{
		v *= rot;
		v += {0.0f, 0.0f, offset_z};
		pst.Transform(v);
	}

	for (auto &v : triangles.verticies)
	{
		v *= rot;
		v += {0.0f, 0.0f, offset_z};
	}

	//for (auto i = lines.indeces.cbegin(), end = lines.indeces.cend(); i != end; std::advance(i, 2))
	//{
	//	gfx.DrawLine(lines.vertices[*i], lines.vertices[*std::next(i)], Colors::White);
	//}

	for (size_t v = 0; v < triangles.indicies.size() / 3; v++)
	{
		const Vec3 &v0 = triangles.verticies[triangles.indicies[v * 3]];
		const Vec3 &v1 = triangles.verticies[triangles.indicies[v * 3 + 1]];
		const Vec3 &v2 = triangles.verticies[triangles.indicies[v * 3 + 2]];
		
		const Vec3 &v01 = (v1 - v0);
		const Vec3 &v02 = (v2 - v0);

		// if triangle has cross product vector in the same direction as camera, the dot prod of these two vecs are negative, so cull this triangle
		triangles.cullFlags[v] = v01 % v02 * v0 >= 0.0f;
	}

	// transform after backface culling
	for (auto &v : triangles.verticies)
	{
		pst.Transform(v);
	}

	Color c;
	std::vector<Color> colors
	{
	Color(255u, 255u, 255u),
	Color(10u, 54u, 210u),
	Color(0x80u, 0x80u, 0x80u),
	Color(0xD3u, 0xD3u, 0xD3u),
	Color(255u, 0u, 0u),
	Color(0u, 255u, 0u),
	Color(0u, 0u, 255u),
	Color(255u, 255u, 0u),
	Color(0u, 255u, 255u),
	Color(255u, 0u, 255u),
	Color(6u, 25u, 125u),
	Color(23u, 155u, 55u)
	};
	int counter = 0;
	for (auto i = triangles.indicies.cbegin(), end = triangles.indicies.cend(); i != end; std::advance(i, 3))
	{
		if (!triangles.cullFlags[counter])
			gfx.DrawTriangle(triangles.verticies[*i], triangles.verticies[*std::next(i)], triangles.verticies[*std::next(i, 2)], colors[counter]);
		counter++;
	}
}