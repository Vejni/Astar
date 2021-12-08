gcc main.c -o astar -lm
gcc -Ofast  main.c -o astar_Ofast -lm

# Measuring binary
echo "Measure with recreating binary"
rm "data/spain.bin"
perf stat -o "results/perf_nobinary_haversine_1.txt" ./astar 0 "results/route_nobinary_haversine_1.txt" "results/stats_nobinary_haversine_1.txt" 1 1
echo "."

# Haversine
echo "Running Haversine"
./astar 1 "results/route_haversine_025.txt" "results/stats_haversine_025.txt" 1 0.25
echo "."
./astar 1 "results/route_haversine_05.txt" "results/stats_haversine_05.txt" 1 0.5
echo "."
./astar 1 "results/route_haversine_1.txt" "results/stats_haversine_1.txt" 1 1
echo "."
./astar 1 "results/route_haversine_15.txt" "results/stats_haversine_15.txt" 1 1.5
echo "."
./astar 1 "results/route_haversine_2.txt" "results/stats_haversine_2.txt" 1 2
echo "."

perf stat -o "results/perf_haversine_1.txt" ./astar 0 "results/route_haversine_1.txt" "results/stats_haversine_1.txt" 1 1
echo "."
perf stat -o "results/perf_Ofast_haversine_1.txt" ./astar_Ofast 0 "results/route_haversine_1.txt" "results/stats_haversine_1.txt" 1 1
echo "."

# Spherical Law
echo "Running Spherical"
./astar 1 "results/route_spherical_1.txt" "results/stats_spherical_1.txt" 2 1
echo "."
perf stat -o "results/perf_spherical_1.txt" ./astar 0 "results/route_spherical_1.txt" "results/stats_spherical_1.txt" 2 1
echo "."
perf stat -o "results/perf_Ofast_spherical_1.txt" ./astar_Ofast 0 "results/route_spherical_1.txt" "results/stats_spherical_1.txt" 1 1
echo "."

# Equirectangular
echo "Running Equirectangular"
./astar 1 "results/route_equirectangular_1.txt" "results/stats_equirectangular_1.txt" 3 1
echo "."
perf stat -o "results/perf_equirectangular_1.txt" ./astar 0 "results/route_equirectangular_1.txt" "results/stats_equirectangular_1.txt" 3 1
echo "."
perf stat -o "results/perf_Ofast_equirectangular_1.txt" ./astar_Ofast 0 "results/route_equirectangular_1.txt" "results/stats_equirectangular_1.txt" 1 1
echo "."
