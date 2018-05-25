#include <fstream>
#include <math.h>
#include <uWS/uWS.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include "Eigen-3.3/Eigen/Core"
#include "Eigen-3.3/Eigen/QR"
#include "json.hpp"

#include "config.h"
#include "mapservice.h"
#include "vehicle.h"
#include "sensor.h"
#include "common.h"
#include "trajectory.h"
#include "coordinates.h"

#define USE_VIZ 1
#ifdef USE_VIZ
#include "viz.h"
#endif

#define DEBUG 1

using namespace std;

// for convenience
using json = nlohmann::json;

bool check_for_initial_s = true;




// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
string hasData(string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_first_of("}");
  if (found_null != string::npos) {
    return "";
  } else if (b1 != string::npos && b2 != string::npos) {
    return s.substr(b1, b2 - b1 + 2);
  }
  return "";
}


int ClosestWaypoint(double x, double y, const vector<double> &maps_x, const vector<double> &maps_y)
{

	double closestLen = 100000; //large number
	int closestWaypoint = 0;

	for(int i = 0; i < maps_x.size(); i++)
	{
		double map_x = maps_x[i];
		double map_y = maps_y[i];
		double dist = distance(x,y,map_x,map_y);
		if(dist < closestLen)
		{
			closestLen = dist;
			closestWaypoint = i;
		}

	}

	return closestWaypoint;

}

int NextWaypoint(double x, double y, double theta, const vector<double> &maps_x, const vector<double> &maps_y)
{

	int closestWaypoint = ClosestWaypoint(x,y,maps_x,maps_y);

	double map_x = maps_x[closestWaypoint];
	double map_y = maps_y[closestWaypoint];

	double heading = atan2((map_y-y),(map_x-x));

	double angle = fabs(theta-heading);
  angle = min(2*pi() - angle, angle);

  if(angle > pi()/4)
  {
    closestWaypoint++;
  if (closestWaypoint == maps_x.size())
  {
    closestWaypoint = 0;
  }
  }

  return closestWaypoint;
}

// Transform from Cartesian x,y coordinates to Frenet s,d coordinates
vector<double> getFrenet(double x, double y, double theta, const vector<double> &maps_x, const vector<double> &maps_y)
{
	int next_wp = NextWaypoint(x,y, theta, maps_x,maps_y);

	int prev_wp;
	prev_wp = next_wp-1;
	if(next_wp == 0)
	{
		prev_wp  = maps_x.size()-1;
	}

	double n_x = maps_x[next_wp]-maps_x[prev_wp];
	double n_y = maps_y[next_wp]-maps_y[prev_wp];
	double x_x = x - maps_x[prev_wp];
	double x_y = y - maps_y[prev_wp];

	// find the projection of x onto n
	double proj_norm = (x_x*n_x+x_y*n_y)/(n_x*n_x+n_y*n_y);
	double proj_x = proj_norm*n_x;
	double proj_y = proj_norm*n_y;

	double frenet_d = distance(x_x,x_y,proj_x,proj_y);

	//see if d value is positive or negative by comparing it to a center point

	double center_x = 1000-maps_x[prev_wp];
	double center_y = 2000-maps_y[prev_wp];
	double centerToPos = distance(center_x,center_y,x_x,x_y);
	double centerToRef = distance(center_x,center_y,proj_x,proj_y);

	if(centerToPos <= centerToRef)
	{
		frenet_d *= -1;
	}

	// calculate s value
	double frenet_s = 0;
	for(int i = 0; i < prev_wp; i++)
	{
		frenet_s += distance(maps_x[i],maps_y[i],maps_x[i+1],maps_y[i+1]);
	}

	frenet_s += distance(0,0,proj_x,proj_y);

	return {frenet_s,frenet_d};

}

// Transform from Frenet s,d coordinates to Cartesian x,y
vector<double> getXY(double s, double d, const vector<double> &maps_s, const vector<double> &maps_x, const vector<double> &maps_y)
{
	int prev_wp = -1;

	while(s > maps_s[prev_wp+1] && (prev_wp < (int)(maps_s.size()-1) ))
	{
		prev_wp++;
	}

	int wp2 = (prev_wp+1)%maps_x.size();

	double heading = atan2((maps_y[wp2]-maps_y[prev_wp]),(maps_x[wp2]-maps_x[prev_wp]));
	// the x,y,s along the segment
	double seg_s = (s-maps_s[prev_wp]);

	double seg_x = maps_x[prev_wp]+seg_s*cos(heading);
	double seg_y = maps_y[prev_wp]+seg_s*sin(heading);

	double perp_heading = heading-pi()/2;

	double x = seg_x + d*cos(perp_heading);
	double y = seg_y + d*sin(perp_heading);

	return {x,y};

}

XY getXYasXY(double s, double d, const vector<double> &maps_s, const vector<double> &maps_x, const vector<double> &maps_y) {
    auto r = getXY(s,d,maps_s,maps_x,maps_y);
    return XY(r.at(0),r.at(1));
}

int main() {
  uWS::Hub h;

  // Load up map values for waypoint's x,y,s and d normalized normal vectors
  vector<double> map_waypoints_x;
  vector<double> map_waypoints_y;
  vector<double> map_waypoints_s;
  vector<double> map_waypoints_dx;
  vector<double> map_waypoints_dy;

  // Waypoint map to read from
  string map_file_ = "../data/highway_map.csv";
  // The max s value before wrapping around the track back to 0
  double max_s = 6945.554;

  ifstream in_map_(map_file_.c_str(), ifstream::in);

  string line;
  while (getline(in_map_, line)) {
  	istringstream iss(line);
  	double x;
  	double y;
  	float s;
  	float d_x;
  	float d_y;
  	iss >> x;
  	iss >> y;
  	iss >> s;
  	iss >> d_x;
  	iss >> d_y;
  	map_waypoints_x.push_back(x);
  	map_waypoints_y.push_back(y);
  	map_waypoints_s.push_back(s);
  	map_waypoints_dx.push_back(d_x);
  	map_waypoints_dy.push_back(d_y);
  }

  Config * cfg = Config::getInstance();
  MapService * mapS = new MapService(map_waypoints_x, map_waypoints_y);

#ifdef USE_VIZ
  Viz *viz = new Viz;
  viz->setWaypoints(map_waypoints_x,map_waypoints_y);
  viz->visualize();
#endif


  h.onMessage([&map_waypoints_x,&map_waypoints_y,&map_waypoints_s,&map_waypoints_dx,&map_waypoints_dy, viz,mapS](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length,
                     uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    //auto sdata = string(data).substr(0, length);
    //cout << sdata << endl;
    if (length && length > 2 && data[0] == '4' && data[1] == '2') {

      auto s = hasData(data);

      if (s != "") {
        auto j = json::parse(s);
        
        string event = j[0].get<string>();
        
        if (event == "telemetry") {

            // Initialize ego vehicle from json;
            Vehicle ego(j,true);

            // This is for debugging, we can set an initial s position on purpose,
            // useful for the left turn or the overflow at end of track
            if(check_for_initial_s) {
                check_for_initial_s = false;

                if(Config::getInstance()->initialS()) {
                    cout << "Setting initial S for debugging";
                    auto initial = getXYasXY(*Config::getInstance()->initialS(),6.0,map_waypoints_s,map_waypoints_x,map_waypoints_y);
                    ego.s = *Config::getInstance()->initialS();

                 }
            }



          	// Previous path data given to the Planner
          	auto previous_path_x = j[1]["previous_path_x"];
          	auto previous_path_y = j[1]["previous_path_y"];

          	// Previous path's end s and d values 
          	double end_path_s = j[1]["end_path_s"];
          	double end_path_d = j[1]["end_path_d"];

#if 0
            auto errfile = "/mnt/d/udacity/self-driving-car-sim-build/simulation-error.txt";
            std::ifstream fin(errfile);

            if(fin) {
                std::cout << "Errorfile exists" << std::endl;

                cout << "+ " << ego.x << " " << ego.y << " " << ego.speed << endl;
                cout << distance(ego.x,ego.y,previous_path_x.at(0),previous_path_y.at(0)) << endl;
                cout << distance(ego.x,ego.y,previous_path_x.at(0),previous_path_y.at(0)) / TICK << endl;

                dumpDetail("p- ", previous_path_x, previous_path_y, 20);

                exit(-1);
            }

#endif

          	// Sensor Fusion Data, a list of all other cars on the same side of the road.
          	auto sensor_fusion = j[1]["sensor_fusion"];

            vector<Vehicle> * vehicles = new vector<Vehicle>;
//            cout << "! " << endl;
//            cout << sensor_fusion <<endl;

            for( auto& vehic : sensor_fusion) {
//                cout << "!!" << endl;
//                cout << vehic <<endl;
                vehicles->push_back(Vehicle(vehic,false));
            }

          	json msgJson;

          	vector<double> next_x_vals;
          	vector<double> next_y_vals;

            Sensor front_sensor(SensorType::FRONT, *mapS);

            cout << "!! " << ego.s << " " << ego.d << " " << ego.yaw << endl;
            auto lane_speeds = front_sensor.laneSpeeds(*vehicles,ego.s);
#ifdef USE_VIZ
            viz->setCarPos(ego.x,ego.y,ego.yaw,ego.speed);
            viz->setVehicles(*vehicles);
            viz->setPrevPath(previous_path_x,previous_path_y);

            viz->visualize();
#endif
          	// TODO: define a path made up of (x,y) points that the car will visit sequentially every .02 seconds
            const double plan_ahead_dist = Config::getInstance()->planAhead();
            auto g_speed = lane_speeds.at(1);
            const double resolution = 0.02;
            auto secs_to_be_there =  plan_ahead_dist / g_speed;
            int segments = (int)(secs_to_be_there/resolution+0.5);

            double inc = plan_ahead_dist / segments;
            cout << "P: " << g_speed << " " << secs_to_be_there << " " << segments << " " << inc << endl;
            vector<Sd> sd_list;
//            for(int i = 1; i<segments;i++) {
//                sd_list.push_back(Sd(car_s+inc*i,6.0));
//            }

            bool lane_change_flag = false;

            // If we still have enough distance to the end path, we are not replanning.
            // TODO: Adjust to make sure that we replan in critical situations.
            //
            // NOTE: This will stop execution
            //
            if(previous_path_x.size() > 4 && fabs(ego.s - end_path_s) > Config::getInstance()->trajectoryTrajectoryMin() ) {
                cout << "Not replanning" << endl;

                for(auto v : previous_path_x)
                    next_x_vals.push_back((v));
                for(auto v : previous_path_y)
                    next_y_vals.push_back((v));


                msgJson["next_x"] = next_x_vals;
                msgJson["next_y"] = next_y_vals;

                auto msg = "42[\"control\","+ msgJson.dump()+"]";

                //this_thread::sleep_for(chrono::milliseconds(1000));
                ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);

                return; //<----------------------------- IMPORTANT TO NOTE TH;
            }

            double replanFrom = ego.s; //!< replanFrom specifies the point that our new trajector starts at.
            if(previous_path_x.size() >= Config::getInstance()->trajectoryReuseNPoints()  ) {
                // When replanning, we start the trajectory from the 3 point in the
                // previous path. We need to know how long that is.
                replanFrom = ego.s+calc_length(previous_path_x,previous_path_y,Config::getInstance()->trajectoryReuseNPoints()  -1);
            }
            cout << "Replanning from " << ego.s << " " << replanFrom << "(" << (replanFrom-ego.s) << ")" << endl;
            auto fastestLane = front_sensor.bestLaneFrom(*vehicles,ego.s,ego.lane);

            // If we have a lange change, calculate if we would have a
            // collision with one of the other cars.
            //
            if(fastestLane != ego.lane) {
                std::vector<Vehicle> dest_lane_vehic ;
                std::copy_if(vehicles->begin(),vehicles->end(),back_inserter(dest_lane_vehic),
                             [fastestLane](const Vehicle &it) {return it.lane == fastestLane;});

                std::vector<double> coll_times;
                std::transform(dest_lane_vehic.begin(),dest_lane_vehic.end(),back_inserter(coll_times),
                               [ego](const Vehicle &it) {return it.collision_time(ego);});

                for(auto t : coll_times) {
                    cout << "Coll " << t << endl;
                }
            }

            if(fastestLane == ego.lane)
            {
                // The number of points that we caclulate depends on
                // the distance we want to look ahead and the
                // distance of the waypoints. It does not make sense
                // to use more fine grained waypoints, since the getXY function is limited
                //
                auto count = Config::getInstance()->trajectoryTrajectoryLength()/Config::getInstance()->trajectoryWaypointDist();
                for(int i = 1; i<count;i++) {
                    // We are pushing the points on the track that we want to hit!
                    sd_list.push_back(Sd(replanFrom+Config::getInstance()->trajectoryWaypointDist()*i,lane2d(ego.lane)));
                }
            } else
            {
                double cur_d = 0.0;
                double dest_d = 0.0;

                cur_d = lane2d(ego.lane);
                dest_d = lane2d(fastestLane);

//                sd_list.push_back(Sd(replanFrom+0,cur_d));
                sd_list.push_back(Sd(replanFrom+30.0,cur_d));
                sd_list.push_back(Sd(replanFrom+60,(cur_d+dest_d)/2 ));
                sd_list.push_back(Sd(replanFrom+90,dest_d ));
                sd_list.push_back(Sd(replanFrom+120,dest_d ));
            }
            vector<XY> traj_points;

            // If we have a previous path when replanning, we are reusing a number of
            // path points to get a smooth transition.
            //
            if(previous_path_x.size() >= Config::getInstance()->trajectoryReuseNPoints()   ) {
                cout << "Pushing previous Paths" << endl;
                for(int i = 0; i<Config::getInstance()->trajectoryReuseNPoints();i++)
                traj_points.push_back(XY(previous_path_x.at(i),previous_path_y.at(i)));
            } else {
                traj_points.push_back(ego.predictPosByM(0.22));
                traj_points.push_back(ego.predictPosByM(0.44));
            }
            for(auto const& it : sd_list) {
                auto t = getXYasXY(it.first,it.second,map_waypoints_s,map_waypoints_x,map_waypoints_y);
                traj_points.push_back(t);
            }


            cout << "T: " << sd_list.size() << " " << traj_points.size() <<endl;
            std::vector<XY> traj_points_car_coord_list;

            world2car(XY(ego.x,ego.y), traj_points,traj_points_car_coord_list, ego.yaw_rad);

            cout << ego.x << ego.y << endl;
#if 0
            dump("sd- ",sd_list, 8);
            dump("t- ", traj_points, 8);
            dump("tl- ", traj_points_car_coord_list, 8);
#endif
            double startX = 0.0; // We are in car coordinates!

            if(previous_path_x.size() > 0) {
                   startX = traj_points_car_coord_list.at(Config::getInstance()->trajectoryReuseNPoints()-1).first;
            }

            Trajectory traj(traj_points_car_coord_list,210);
            std::vector<XY> sd_list_next;
            traj.fillLists(sd_list_next,ego.speed,front_sensor.laneSpeed(*vehicles,ego.lane,ego.s), 0.0);

            std::vector<XY> sd_list_next_world;
            car2world(XY(ego.x,ego.y),sd_list_next, sd_list_next_world,ego.yaw_rad);

            split(sd_list_next_world, next_x_vals,next_y_vals);
#if 0
            cout << "+ " << ego.x << " " << ego.y << " " << next_x_vals.at(0) << " " << next_y_vals.at(0) << endl;
            cout << distance(ego.x,ego.y,next_x_vals.at(0),next_y_vals.at(0)) << endl;
            cout << distance(ego.x,ego.y,next_x_vals.at(0),next_y_vals.at(0)) / TICK << endl;

            dump("p- ", previous_path_x,previous_path_y,8);
            cout << "speed: " << ego.speed << endl;
            dumpDetail("n- ",next_x_vals, next_y_vals, 50);
#endif



          	msgJson["next_x"] = next_x_vals;
          	msgJson["next_y"] = next_y_vals;

          	auto msg = "42[\"control\","+ msgJson.dump()+"]";

          	//this_thread::sleep_for(chrono::milliseconds(1000));
          	ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
          
        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the
  // program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data,
                     size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1) {
      res->end(s.data(), s.length());
    } else {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code,
                         char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port)) {
    std::cout << "Listening to port " << port << std::endl;
  } else {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
