#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>

// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// 

//std::vector<double> p = {0.1,0.004,3.0}; // initial parameters

//std::vector<double> p = {0.2,0.0,0.00}; // iteration 1 -> diverges (Kp only)
//std::vector<double> p = {1.2,0.0,0.00}; // iteration 2 -> diverges (Kp only)
//std::vector<double> p = {0.5,0.0,0.00}; // iteration 3 -> diverges (Kp only)
//std::vector<double> p = {0.250,0.0,0.00}; // iteration 4 -> slightly diverges (Kp only)
//std::vector<double> p = {0.225,0.0,0.00}; // iteration 5 -> quite stable oscillations (Kp only)
//std::vector<double> p = {0.225,0.0,1.00}; // iteration 6 -> able to complete the track, some oscillations on the bridge part (Kp, Kd only)
//std::vector<double> p = {0.225,0.0,2.00}; // iteration 7 -> able to complete the track, less oscillations on the bridge part  (Kp, Kd only) -> candidate 1
//std::vector<double> p = {0.225,0.0,3.00}; // iteration 8 -> able to complete the track, more oscillations during curves (Kp, Kd only)
//std::vector<double> p = {0.225,0.0,2.50}; // iteration 9 -> able to complete the track, still more oscillations during curves (Kp, Kd only)
//std::vector<double> p = {0.225,0.0,2.25}; // iteration 10 -> able to complete the track, still more oscillations during curves (Kp, Kd only) -> candidate 2
//std::vector<double> p = {0.225,1.0,2.25}; // iteration 11 -> goes off-track almost immediately (Kp, Kd, Ki)
//std::vector<double> p = {0.225,0.1,2.25}; // iteration 12 -> goes off-track almost immediately (Kp, Kd, Ki)
//std::vector<double> p = {0.225,0.001,2.25}; // iteration 13 -> able to complete the track, some minor oscillations during curves (Kp, Kd only) -> candidate 3
//std::vector<double> p = {0.275,0.001,2.55}; // iteration 12 -> fine tuning, average CTE printed out during execution -> candidate 4
//std::vector<double> p = {0.250,0.001,2.75}; // iteration 13 -> fine tuning, average CTE printed out during execution -> candidate 5
//std::vector<double> p = {0.235,0.001,2.75}; // iteration 14 -> fine tuning, average CTE printed out during execution -> candidate 6
std::vector<double> p = {0.235,0.005,2.75}; // iteration 15 -> steering during curves is less abrupt -> candidate 7 -> selected
//std::vector<double> p = {0.235,0.008,2.75}; // iteration 16 -> minor oscillations
//std::vector<double> p = {0.235,0.003,2.75}; // iteration 17 -> some oscillations, especially when leaving the bridge

double max_steering_angle = 1.0;
double cumulative_cte = 0.0;
double avg_cte = 0.0;
int steps = 0;


// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}


int main()
{
  uWS::Hub h;

  PID pid;

  // TODO: Initialize the pid variable.
  pid.Init(p[0], p[1], p[2]);

  
  h.onMessage([&pid](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      
      if (s != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());
          double speed = std::stod(j[1]["speed"].get<std::string>());
          double angle = std::stod(j[1]["steering_angle"].get<std::string>());
          double throttle = std::stod(j[1]["throttle"].get<std::string>());
          double steer_value;
          /*
          * TODO: Calculate steering value here, remember the steering value is
          * [-1, 1].
          * NOTE: Feel free to play around with the throttle and speed. Maybe use
          * another PID controller to control the speed!
          */

          steps += 1;
          cumulative_cte += cte;
          avg_cte = cte / steps;

          pid.UpdateError(cte);
          steer_value = pid.TotalError();

          if (steer_value > max_steering_angle) {
            steer_value = max_steering_angle;
          } else if (steer_value < -max_steering_angle) {
            steer_value = -max_steering_angle;
          }

          // DEBUG
          std::cout << "CTE: " << cte << " Steering Value: " << steer_value << " Avg CTE: " << avg_cte << std::endl;

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = 0.3;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);

        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
