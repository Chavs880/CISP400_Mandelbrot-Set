#include <iostream>
#include "ComplexPlane.h"
using namespace sf;

int main()
{
    RenderWindow window(VideoMode(VideoMode::getDesktopMode().width/2, VideoMode::getDesktopMode().height/2), "Mandelbrot");
    ComplexPlane cPlane(VideoMode::getDesktopMode().width/2, VideoMode::getDesktopMode().height/2);
    Font font;
    font.loadFromFile("Arial.ttf");
    Text text;
    text.setFont(font);
    text.setCharacterSize(30);
    text.setFillColor(Color::Red);

    while (window.isOpen()) {
        Event event;

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Button::Right) {
                    cPlane.zoomOut();
                    cPlane.setCenter(Mouse::getPosition(window));
                }
                if (event.mouseButton.button == Mouse::Button::Left) {
                    cPlane.zoomIn();
                    cPlane.setCenter(Mouse::getPosition(window));
                }
            }

            if (event.type == Event::MouseMoved) {
                cPlane.setMouseLocation(Mouse::getPosition(window));
            }

            if (event.type == Keyboard::isKeyPressed(Keyboard::Escape)) {
                window.close();
            }
        }

        cPlane.updateRender();
        cPlane.loadText(text);

        window.clear();
        window.draw(cPlane);
        window.draw(text);
        window.display();
    }
}