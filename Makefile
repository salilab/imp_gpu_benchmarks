ALL: cpp_cpu cpp_cpu_ppair cpp_gpu_ppair

cpp_cpu: cpp_cpu.cpp
	g++ -Wall -O3 cpp_cpu.cpp -o cpp_cpu

cpp_cpu_ppair: cpp_cpu_ppair.cpp
	g++ -Wall -O3 cpp_cpu_ppair.cpp -o cpp_cpu_ppair

cpp_gpu_ppair: cpp_gpu_ppair.cu
	nvcc -O3 cpp_gpu_ppair.cu -o cpp_gpu_ppair
