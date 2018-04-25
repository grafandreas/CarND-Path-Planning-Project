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
    for(int i = 0; i < pImpl->_x->size();i++) {
        sf::CircleShape circle(5);
        circle.setPointCount(8);
        circle.setFillColor(sf::Color(100, 250, 50));
        circle.setPosition(pImpl->_x->at(i),pImpl->_y->at(i));
        cout << pImpl->_x->at(i) << " " << pImpl->_y->at(i) <<endl;
        pImpl->window.draw(circle);
    }
    pImpl->window.display();

}

void Viz::setWaypoints(vector<double> &x, vector<double> &y) {
    pImpl->_x = &x;
    pImpl->_y = &y;
}

