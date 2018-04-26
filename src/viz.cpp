#include "viz.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "config.h"


using namespace std;

double const car_view_scale=0.5;

class Viz::impl {
public:
    sf::RenderWindow window;
    vector<double> * _x;
    vector<double> * _y;

    double car_x, car_y;
    double wp_x, wp_y;
    double yaw, speed;

    impl() : window(sf::VideoMode(Config::getInstance()->w(), Config::getInstance()->h()), "SFML works!") {

    }

    void drawCircle(int radius, sf::Color  col, double x, double y );
    void mapCoord2CarCenter(double x, double y, double * o_x, double * o_y);

};


void Viz::impl::drawCircle(int radius, sf::Color col, double x, double y ) {

    double t_x = Config::getInstance()->VizTranslateX();
    double t_y = Config::getInstance()->VizTranslateY();
    double sc = Config::getInstance()->VizScale();
    double h = (double) Config::getInstance()->h();

    sf::CircleShape circle(radius);
    circle.setPointCount(8);
    circle.setFillColor(col);
    circle.setPosition((x+t_x)*sc-radius/2,h-(y+t_y)*sc-radius/2);
    window.draw(circle);
}

void Viz::impl::mapCoord2CarCenter(double x, double y, double * o_x, double * o_y) {
    cout << x << " / " << y << endl;
     cout << car_x << " - " << car_y << endl;
    double r_x = x-car_x;
    double r_y = y-car_y;

    *o_x = (Config::getInstance()->w()/2 + r_x*car_view_scale);
    *o_y = (Config::getInstance()->h()/2 - r_y*car_view_scale);
    cout << *o_x << " - " << *o_y << endl;
}



Viz::Viz()  : pImpl(new impl)
{



      pImpl->window.clear(sf::Color::White);
      pImpl->window.display();

//    while (window.isOpen())
//       {
//           // check all the window's events that were triggered since the last iteration of the loop
//           sf::Event event;
//           while (window.pollEvent(event))
//           {
//               // "close requested" event: we close the window
//               if (event.type == sf::Event::Closed)
//                   window.close();
//           }

//           // clear the window with black color


//           // draw everything here...
//           // window.draw(...);

//           // end the current frame

//       }


}

void Viz::visualize() {
    pImpl->window.clear(sf::Color::White);
    double t_x = Config::getInstance()->VizTranslateX();
    double t_y = Config::getInstance()->VizTranslateY();
    double sc = Config::getInstance()->VizScale();
    double h = (double) Config::getInstance()->h();
    for(int i = 0; i < pImpl->_x->size();i++) {
        pImpl->drawCircle(5,sf::Color::Green, pImpl->_x->at(i),pImpl->_y->at(i));
    }
    pImpl->drawCircle(6,sf::Color::Black, pImpl->car_x,pImpl->car_y);



    double x1, y1;
    pImpl->mapCoord2CarCenter(pImpl->car_x,pImpl->car_y, &x1,&y1);
     sf::CircleShape circle(8);
     circle.setPointCount(8);
     circle.setFillColor(sf::Color::Black);
     circle.setPosition(x1,y1);
     pImpl->window.draw(circle);



     sf::RectangleShape line(sf::Vector2f(pImpl->speed*10, 5));
     line.setFillColor(sf::Color::Black);
     line.setPosition(x1,y1);

     line.setRotation(-pImpl->yaw);
     pImpl->window.draw(line);

     pImpl->mapCoord2CarCenter(pImpl->wp_x,pImpl->wp_y,&x1,&y1);
     pImpl->drawCircle(6,sf::Color::Red, x1, y1);



    pImpl->window.display();

}

void Viz::setWaypoints(vector<double> &x, vector<double> &y) {
    pImpl->_x = &x;
    pImpl->_y = &y;
}

void Viz::setCarPos(double x, double y, double yaw, double speed) {
    pImpl->car_x = x;
    pImpl->car_y = y;
    pImpl->yaw = yaw;
    pImpl->speed = speed;
}

void Viz::setWaypoint(double x, double y) {
    pImpl->wp_x = x;
    pImpl->wp_y = y;
}

