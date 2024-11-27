#include <cmath>
#include <sstream>
#include <complex>
#include "ComplexPlane.h"
#include <iostream>
using namespace std;

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight) {
	m_pixel_size = { pixelWidth, pixelHeight };
	m_aspectRatio = float(pixelHeight) / pixelWidth;
	m_plane_center = { 0, 0 };
	m_plane_size = { BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };
	m_zoomCount = 0;
	m_state = State::CALCULATING;
	m_vArray.setPrimitiveType(Points);
	m_vArray.resize(pixelWidth * pixelHeight);
}

void ComplexPlane::draw(RenderTarget& target, RenderStates states) const {
	target.draw(m_vArray);
}

void ComplexPlane::updateRender() {
	if (m_state == CALCULATING) {
		for (int j = 0; j < m_pixel_size.x; j++) {
			for (int i = 0; i < m_pixel_size.y; i++) {
				m_vArray[j + i * m_pixel_size.x].position = { (float)j,(float)i };
				
				size_t iterationCount = countIterations(mapPixelToCoords({ j,i }));

				Uint8 r, g, b;
				iterationsToRGB(iterationCount, r, g, b);

				m_vArray[j + i * m_pixel_size.x].color = { r,g,b };
			}
		}
	}
	m_state = DISPLAYING;
}

void ComplexPlane::zoomIn() {
	m_zoomCount++;
	float localX = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	float localY = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
	m_plane_size = { localX, localY };
	m_state = CALCULATING;
}

void ComplexPlane::zoomOut() {
	m_zoomCount--;
	float localX = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	float localY = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
	m_plane_size = { localX, localY };
	m_state = CALCULATING;
}

void ComplexPlane::setCenter(Vector2i mousePixel) {
	m_plane_center = mapPixelToCoords(mousePixel);
	m_state = CALCULATING;
}

void ComplexPlane::setMouseLocation(Vector2i mousePixel) {
	m_mouseLocation = mapPixelToCoords(mousePixel);
}

void ComplexPlane::loadText(Text& text) {
	stringstream ss;

	ss << "Mandelbrot Set\nCenter: (" << m_plane_center.x << ", " << m_plane_center.y << ")\nCursor: (" 
		<< m_mouseLocation.x << ", " << m_mouseLocation.y << ")\n" << "Left-click to Zoom in\nRight-click to Zoom out\n";

	text.setString(ss.str());
}

size_t ComplexPlane::countIterations(Vector2f coord) {
	complex<double> c(coord.x, coord.y);
	complex<double> z = c;
	int i = 0;
	while (abs(z) < 2.0 && i < MAX_ITER) {
		z = z * z + c;
		i++;
	}
	return i;
}

void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b) {
	float division = MAX_ITER / 5;
	if (count >= MAX_ITER) {
		r = 0;
		g = 0;
		b = 0;
	}
	else if (count < division) {
		r = 150;
		g = 255;
		b = 150;
	}
	else if (count < division * 2) {
		r = 150;
		g = 255;
		b = 0;
	}
	else if (count < division * 3) {
		r = 0;
		g = 255;
		b = 0;
	}
	else if (count < division * 4) {
		r = 255;
		g = 150;
		b = 0;
	}
	else {
		r = 255;
		g = 0;
		b = 0;
	}
}

Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel) {
	float xCoord = ((mousePixel.x - 0.0) / (m_pixel_size.x - 0.0)) * m_plane_size.x + (m_plane_center.x - m_plane_size.x / 2.0);
	float yCoord = ((mousePixel.y - m_pixel_size.y) / (0.0 - m_pixel_size.y)) * m_plane_size.y + (m_plane_center.y - m_plane_size.y / 2.0);
	return { xCoord, yCoord };
}