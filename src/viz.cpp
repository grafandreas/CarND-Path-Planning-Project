#include "viz.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "config.h"
#include "sensor.h"
#include <string>
#include "common.h"

using namespace std;

double const car_view_scale=2.0;

class Viz::impl {
public:
    sf::RenderWindow window;
    vector<double> * _x;
    vector<double> * _y;
    vector<Vehicle> * vs;
    // TODO: Make Numlanes flexible
    vector<double> lane_speed;

    double car_x, car_y;
    double wp_x, wp_y;
    double yaw, speed;

    sf::Font font;

    impl() : window(sf::VideoMode(Config::getInstance()->w(), Config::getInstance()->h()), "SFML works!") {
        font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
        vs = new vector<Vehicle>; // Empty dummy to make sure that list works.
        for(int i = 0; i< Config::getInstance()->numLanes();i++)
            lane_speed.push_back(0.0);


    }

    void drawCircle(int radius, sf::Color  col, double x, double y );
    void drawCircle(int radius, sf::Color  col, XY coord );
    void mapCoord2CarCenter(double x, double y, XY&o);
    void drawCarCenterCircle(int radius, sf::Color  col, double x, double y );

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

void Viz::impl::drawCircle(int radius, sf::Color col, XY coord ) {
    drawCircle(radius,col,coord.first, coord.second);
}



void Viz::impl::mapCoord2CarCenter(double x, double y, XY & o) {
//    cout << x << " / " << y << endl;
//     cout << car_x << " - " << car_y << endl;
    double r_x = x-car_x;
    double r_y = y-car_y;

    o.first = (Config::getInstance()->w()/2 + r_x*car_view_scale);
    o.second = (Config::getInstance()->h()/2 - r_y*car_view_scale);
//    cout << *o_x << " - " << *o_y << endl;
}

void Viz::impl::drawCarCenterCircle(int radius, sf::Color  col, double x, double y ) {
    XY o_c;
    mapCoord2CarCenter(x,y,o_c);
    cout << "x " << o_c.first << " " << o_c.second << endl;
    sf::CircleShape circle(radius);
    circle.setPointCount(8);
    circle.setFillColor(col);
    circle.setPosition(o_c.first,o_c.second);
    window.draw(circle);
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



//    cout << "--- " << pImpl->vs->size() << endl;

    for( auto const &  v : *pImpl->vs) {
        cout << "---" << endl;
        pImpl->drawCircle(3,sf::Color::Blue,v.x,v.y);
        pImpl->drawCarCenterCircle(6,sf::Color::Blue,v.x,v.y);
    }
    pImpl->drawCircle(6,sf::Color::Black, pImpl->car_x,pImpl->car_y);



    XY o_c;
    pImpl->mapCoord2CarCenter(pImpl->car_x,pImpl->car_y, o_c);
     sf::CircleShape circle(8);
     circle.setPointCount(8);
     circle.setFillColor(sf::Color::Black);
     circle.setPosition(o_c.first,o_c.second);
     pImpl->window.draw(circle);



     sf::RectangleShape line(sf::Vector2f(pImpl->speed*10, 5));
     line.setFillColor(sf::Color::Black);
     line.setPosition(o_c.first,o_c.second);

     line.setRotation(-pImpl->yaw);
     pImpl->window.draw(line);

     pImpl->mapCoord2CarCenter(pImpl->wp_x,pImpl->wp_y,o_c);
     pImpl->drawCircle(6,sf::Color::Red, o_c.first,o_c.second);




     for(int i = 0; i< Config::getInstance()->numLanes();i++) {
        sf::Text t;

        t.setString(std::to_string(pImpl->lane_speed.at(i)));
        t.setColor(sf::Color::Black);
        t.setPosition(40,40+20*i);
        t.setFont(pImpl->font);
        t.setCharacterSize(20);
        pImpl->window.draw(t);
     }


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

void Viz::setVehicles(vector<Vehicle> &vehicles) {
    pImpl->vs = &vehicles;
}

