#include "viz.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "config.h"


using namespace std;

class Viz::impl {
public:
    sf::RenderWindow window;
    vector<double> * _x;
    vector<double> * _y;

    double car_x, car_y;

    impl() : window(sf::VideoMode(Config::getInstance()->w(), Config::getInstance()->h()), "SFML works!") {

    }

};




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
        sf::CircleShape circle(5);
        circle.setPointCount(8);
        circle.setFillColor(sf::Color(100, 250, 50));
        circle.setPosition((pImpl->_x->at(i)+t_x)*sc,h-(pImpl->_y->at(i)+t_y)*sc);
//        cout << pImpl->_x->at(i) << " " << pImpl->_y->at(i) <<endl;
        pImpl->window.draw(circle);
    }

    sf::CircleShape circle(6);
    circle.setPointCount(8);
    circle.setFillColor(sf::Color(0, 0, 0));
    circle.setPosition((pImpl->car_x+t_x)*sc,h-(pImpl->car_y+t_y)*sc);
    cout << pImpl->car_x << " x " << pImpl->car_y << endl;
    pImpl->window.draw(circle);

    pImpl->window.display();

}

void Viz::setWaypoints(vector<double> &x, vector<double> &y) {
    pImpl->_x = &x;
    pImpl->_y = &y;
}

void Viz::setCarPos(double x, double y) {
    pImpl->car_x = x;
    pImpl->car_y = y;
}
