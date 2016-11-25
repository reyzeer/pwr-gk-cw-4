/// Laboratorium: Grafika Komputerowa
/// Prowadzący: Dr inż. Marek Woda
/// Zadanie 3
/// Zbigniew Czarnecki 209909

#include "common.h"

void Egg::prepareMatrix()
{
	
	matrix = new float ** [n];
	for (int i = 0; i < n; i++) {
		matrix[i] = new float * [n];
		for (int j = 0; j < n; j++) {
			matrix[i][j] = new float[3];
		}
	}

}

float Egg::x(float u, float v)
{
	return (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * cos(pi*v);
}

float Egg::y(float u, float v)
{
	return 160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2);
}

float Egg::z(float u, float v)
{
	return (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * sin(pi*v);
}

void Egg::transform2Egg()
{

	float distance = 1.0f / (float) (n - 1);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			float u = distance * i;
			float v = distance * j;
			matrix[i][j][0] = x(u, v) - posX;
			matrix[i][j][1] = y(u, v) - posY;
			matrix[i][j][2] = z(u, v) - posZ;
		}
	}

}

void Egg::randColors()
{

	colors = new float **[n];
	for (int i = 0; i < n; i++) {
		colors[i] = new float *[n];
		for (int j = 0; j < n; j++) {

			colors[i][j] = new float[3];

			colors[i][j][0] = RDraw::randIntensity();
			colors[i][j][1] = RDraw::randIntensity();
			colors[i][j][2] = RDraw::randIntensity();

		}
	}

}

void Egg::fourColors()
{

	colors = new float **[n];

	for (int i = 0; i < n; i++) {
		colors[i] = new float *[n];
		for (int j = 0; j < n; j++) {

			colors[i][j] = new float[3];

			float in = 1 / ((float)n - 1) * i;

			if (i < n / 2) {
				if (j < n / 2) {
					//white
					colors[i][j][0] = in;
					colors[i][j][1] = in;
					colors[i][j][2] = in;
				}
				else {
					//red
					colors[i][j][0] = in;
					colors[i][j][1] = 0.0f;
					colors[i][j][2] = 0.0f;
				}
			}
			else {
				if (j < n / 2) {
					//green
					colors[i][j][0] = 0.0f;
					colors[i][j][1] = in;
					colors[i][j][2] = 0.0f;
				}
				else {
					//blue
					colors[i][j][0] = 0.0f;
					colors[i][j][1] = 0.0f;
					colors[i][j][2] = in;
				}
			}

		}
	}

}

void Egg::deleteSeam()
{

	for (int i = 0; i < n; i++) {
		colors[i][0][0] = colors[n - 1 - i][n - 1][0];
		colors[i][0][1] = colors[n - 1 - i][n - 1][1];
		colors[i][0][2] = colors[n - 1 - i][n - 1][2];
	}

}

void Egg::drawPoints()
{

	glBegin(GL_POINTS);

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				glVertex3fv(matrix[i][j]);
			}
		}

	glEnd();

}

void Egg::drawLines()
{

	glBegin(GL_LINES);

	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - 1; j++) {

			glVertex3fv(matrix[i][j]);
			glVertex3fv(matrix[i + 1][j]);

			glVertex3fv(matrix[i + 1][j]);
			glVertex3fv(matrix[i][j + 1]);

			glVertex3fv(matrix[i][j]);
			glVertex3fv(matrix[i][j + 1]);

		}
	}

	glEnd();

}

void Egg::insertVertexWithColor(int i, int j)
{
	glColor3fv(colors[i][j]);
	glVertex3fv(matrix[i][j]);
}

void Egg::drawTriangles()
{

	glBegin(GL_TRIANGLES);

	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - 1; j++) {

			//Triangle
			insertVertexWithColor(i, j);
			insertVertexWithColor(i, j + 1);
			insertVertexWithColor(i + 1, j);

			//Triangle
			insertVertexWithColor(i, j + 1);
			insertVertexWithColor(i + 1, j);
			insertVertexWithColor(i + 1, j + 1);

		}
	}

	glEnd();

}

void Egg::destruct()
{

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			delete[] matrix[i][j];
		}
		delete[] matrix[i];
	}
	delete[] matrix;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			delete[] colors[i][j];
		}
		delete[] colors[i];
	}
	delete[] colors;

	build = false;

}

void Egg::draw()
{

	if (!build) {

		prepareMatrix();
		transform2Egg();

		randColors();
		deleteSeam();

		build = true;

	}

	switch (renderType) {
		case 0:
			drawPoints();
			break;
		case 1:
			drawLines();
			break;
		case 2:
			drawTriangles();
			break;
		case 3:
			fourColors();
			break;
		default:
			drawPoints();
			break;
	}

}