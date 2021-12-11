gcc main.c -o astar -lm
gcc -O3 main.c -o astar_O3 -lm
gcc -Ofast  main.c -o astar_Ofast -lm

# Measuring binary
echo "Measure with recreating binary"
rm "data/spain.bin"
perf stat -o "results/perf_nobinary_haversine_1.txt" ./astar 0 "results/route_nobinary_haversine_1.txt" "results/stats_nobinary_haversine_1.txt" 1 1
echo "."

# Haversine
echo "Running Haversine"
./astar 1 "results/route_haversine_0.txt" "results/stats_haversine_0.txt" 1 0
echo "."
./astar 1 "results/route_haversine_025.txt" "results/stats_haversine_025.txt" 1 0.25
echo "."
./astar 1 "results/route_haversine_05.txt" "results/stats_haversine_05.txt" 1 0.5
echo "."
./astar 1 "results/route_haversine_1.txt" "results/stats_haversine_1.txt" 1 1
echo "."
./astar 1 "results/route_haversine_15.txt" "results/stats_haversine_15.txt" 1 1.5
echo "."
./astar 1 "results/route_haversine_125.txt" "results/stats_haversine_125.txt" 1 1.25
echo "."
./astar 1 "results/route_haversine_2.txt" "results/stats_haversine_2.txt" 1 2
echo "."
./astar_O3 1 "results/route_O3_haversine_1.txt" "results/stats_O3_haversine_1.txt" 1 1
echo "."
./astar_Ofast 1 "results/route_Ofast_haversine_1.txt" "results/stats_Ofast_haversine_1.txt" 1 1
echo "."

perf stat -o "results/perf_haversine_0.txt" ./astar 0 "results/route_haversine_0.txt" "results/stats_haversine_0.txt" 1 0
echo "."
perf stat -o "results/perf_haversine_025.txt" ./astar 0 "results/route_haversine_025.txt" "results/stats_haversine_025.txt" 1 0.25
echo "."
perf stat -o "results/perf_haversine_05.txt" ./astar 0 "results/route_haversine_05.txt" "results/stats_haversine_05.txt" 1 0.5
echo "."
perf stat -o "results/perf_haversine_1.txt" ./astar 0 "results/route_haversine_1.txt" "results/stats_haversine_1.txt" 1 1
echo "."
perf stat -o "results/perf_haversine_15.txt" ./astar 0 "results/route_haversine_15.txt" "results/stats_haversine_15.txt" 1 1.5
echo "."
perf stat -o "results/perf_haversine_125.txt" ./astar 0 "results/route_haversine_125.txt" "results/stats_haversine_125.txt" 1 1.25
echo "."
perf stat -o "results/perf_haversine_2.txt" ./astar 0 "results/route_haversine_2.txt" "results/stats_haversine_2.txt" 1 2
echo "."
./astar_O3 1 "results/route_O3_haversine_1.txt" "results/stats_O3_haversine_1.txt" 1 1
echo "."
./astar_Ofast 1 "results/route_Ofast_haversine_1.txt" "results/stats_Ofast_haversine_1.txt" 1 1
echo "."

perf stat -o "results/perf_haversine_1.txt" ./astar 0 "results/route_haversine_1.txt" "results/stats_haversine_1.txt" 1 1
echo "."
perf stat -o "results/perf_O3_haversine_1.txt" ./astar_O3 0 "results/route_haversine_1.txt" "results/stats_haversine_1.txt" 1 1
echo "."
perf stat -o "results/perf_Ofast_haversine_1.txt" ./astar_Ofast 0 "results/route_haversine_1.txt" "results/stats_haversine_1.txt" 1 1
echo "."

# Spherical Law
echo "Running Spherical"
./astar 1 "results/route_spherical_1.txt" "results/stats_spherical_1.txt" 2 1
echo "."
./astar_Ofast 1 "results/route_Ofast_spherical_1.txt" "results/stats_Ofast_spherical_1.txt" 2 1
echo "."
./astar_O3 1 "results/route_O3_spherical_1.txt" "results/stats_O3_spherical_1.txt" 2 1
echo "."
perf stat -o "results/perf_spherical_1.txt" ./astar 0 "results/route_spherical_1.txt" "results/stats_spherical_1.txt" 2 1
echo "."
perf stat -o "results/perf_Ofast_spherical_1.txt" ./astar_Ofast 0 "results/route_spherical_1.txt" "results/stats_spherical_1.txt" 2 1
echo "."
perf stat -o "results/perf_O3_spherical_1.txt" ./astar_O3 0 "results/route_spherical_1.txt" "results/stats_spherical_1.txt" 2 1
echo "."

# Equirectangular
echo "Running Equirectangular"
./astar 1 "results/route_equirectangular_1.txt" "results/stats_equirectangular_1.txt" 3 1
echo "."
./astar_Ofast 1 "results/route_Ofast_equirectangular_1.txt" "results/stats_Ofast_equirectangular_1.txt" 3 1
echo "."
./astar_O3 1 "results/route_O3_equirectangular_1.txt" "results/stats_O3_equirectangular_1.txt" 3 1
echo "."
perf stat -o "results/perf_equirectangular_1.txt" ./astar 0 "results/route_equirectangular_1.txt" "results/stats_equirectangular_1.txt" 3 1
echo "."
perf stat -o "results/perf_Ofast_equirectangular_1.txt" ./astar_Ofast 0 "results/route_equirectangular_1.txt" "results/stats_equirectangular_1.txt" 3 1
echo "."
perf stat -o "results/perf_O3_equirectangular_1.txt" ./astar_O3 0 "results/route_equirectangular_1.txt" "results/stats_equirectangular_1.txt" 3 1
echo "."
