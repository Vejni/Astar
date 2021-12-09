#include "math.h"
#define PI 3.14159265358979323846
#define R 6371008.8

float haversine_dist(node node1, node node2){
    // Geodesic distance between two nodes with the haversine equation (high precission)
    double phi1 = node1.lat * PI/180.0;
    double phi2 = node2.lat * PI/180.0;
    double phi_dif =  phi1 - phi2;
    double lambda_dif = (node1.lon - node2.lon) * PI/180.0;
    double a = 2 * asin(sqrt(pow(sin(phi_dif * 0.5), 2) + cos(phi2) * cos(phi1) * pow(sin(lambda_dif * 0.5), 2)));
    return R * a;
}

float spherical_law(node node1, node node2){
  double phi1 = node1.lat * PI/180;
  double phi2 = node2.lat * PI/180;
  double lambda_dif = (node1.lon - node2.lon) * PI/180;
  double d = acos(sin(phi1) * sin(phi2) + cos(phi1) * cos(phi2) * cos(lambda_dif));
  return R * d;
}

float equirectangular(node node1, node node2){
  double phi1 = node1.lat * PI/180;
  double phi2 = node2.lat * PI/180;
  double delta_lambda = (node2.lon - node1.lon) * PI/180;
  double x = delta_lambda * (cos(phi1 + phi2) * 0.5);
  double y = phi2 - phi1;
  double d = sqrt(x*x + y*y);
  return R * d;
}

float heuristic(node current_node, node goal, int heuristic_func, double heuristic_param){
  double heu_dist;
  switch (heuristic_func){
    case 3:
      heu_dist = equirectangular(current_node, goal);
    case 2:
      heu_dist = spherical_law(current_node, goal);
    default:
      heu_dist = haversine_dist(current_node, goal);
  }
  if(heuristic_param != 1.0){
    heu_dist = pow(heu_dist, heuristic_param);
  }

  return heu_dist;

}
