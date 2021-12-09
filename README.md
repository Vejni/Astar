# Astar

This repo contains code to solve the routing problem assignment in the Optimization course of the UAB master's in Modelling for Science and Engineering. The repo structure is as follows:

- data: Contains the map files and intermediate binary files used by the algorithm.
- docs: Contains the problem statement, and report about the project.
- lib: Contains the source code.
- notes: Contains other files used during development.
- results: Contains performance statistics and the routes.
- plot: Script for plotting and outputs.

Main executable: astar or get_results.sh to do a batch of problems.

Arguments (in order):
  - Save Route and Performance (1/0)
  - Route file name
  - Stats file name
  - Heuristic function (1: Haversine, 2: Spherical, 3: Equirectangular)
  - Heuristic parameter

Example: ./astar 1 "results/route_haversine_1.txt" "results/stats_haversine_1.txt" 1 1

Note: The repo does not contain the csv file because of its size. It can be found here https://mat.uab.cat/~alseda/MasterOpt/spain.csv.zip.

For plotting we used:
http://dwtkns.com/pointplotter/
