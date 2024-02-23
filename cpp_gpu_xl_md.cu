#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

static const unsigned n_particles = 2000;

__global__
void evaluate(double *x, double *y, double *z, unsigned *ppairs,
              double *scores, unsigned n_ppairs) {
  static const double mean = 2.0;
  static const double force = 0.1;
  unsigned start = blockIdx.x * blockDim.x + threadIdx.x;
  unsigned stride = blockDim.x * gridDim.x;
  for (unsigned i = start; i < n_ppairs; i += stride) {
    double dx = x[ppairs[i*2]] - x[ppairs[i*2+1]];
    double dy = y[ppairs[i*2]] - y[ppairs[i*2+1]];
    double dz = z[ppairs[i*2]] - z[ppairs[i*2+1]];
    double r = sqrt((dx*dx) + (dy*dy) + (dz*dz));
    scores[i] = 0.5 * force * (r - mean) * (r - mean);
  }
}

__global__
void integrate(double *x, unsigned n_particles) {
  unsigned start = blockIdx.x * blockDim.x + threadIdx.x;
  unsigned stride = blockDim.x * gridDim.x;
  for (unsigned p = start; p < n_particles; p += stride) {
    if (p % 2 == 0) {
      x[p] += 0.01;
    } else {
      x[p] -= 0.01;
    }
  }
}

double optimize(unsigned max_steps, double *x, double *y, double *z,
                unsigned *ppairs, double *scores, unsigned n_ppairs) {
  double score = 0.;
  int block_size = 256;
  int num_blocks_eval = (n_ppairs + block_size - 1) / block_size;
  int num_blocks_opt = (n_particles + block_size - 1) / block_size;
  for (unsigned step = 0; step < max_steps; ++step) {
    /*score = */evaluate<<<num_blocks_eval, block_size>>>(x, y, z, ppairs, scores, n_ppairs);
    // cudaDeviceSynchronize(); ?
    integrate<<<num_blocks_opt, block_size>>>(x, n_particles);
    // cudaDeviceSynchronize(); ?
  }
  cudaDeviceSynchronize();
  return score;
}

int main() {
  double *x, *y, *z, *scores;
  unsigned *ppairs;
  cudaMallocManaged(&x, n_particles * sizeof(double));
  cudaMallocManaged(&y, n_particles * sizeof(double));
  cudaMallocManaged(&z, n_particles * sizeof(double));
  unsigned n_ppairs = n_particles - 1;
  cudaMallocManaged(&ppairs, n_ppairs * 2 * sizeof(unsigned));
  cudaMallocManaged(&scores, n_ppairs * sizeof(double));

  for (unsigned i = 0; i < n_particles; ++i) {
    x[i] = 0.01 * i;
    y[i] = 0.0;
    z[i] = 0.0;
  }

  unsigned k = 0;
  for (unsigned i = 0; i < n_particles - 1; ++i) {
    ppairs[k++] = i;
    ppairs[k++] = i + 1;
  }

  int block_size = 256;
  int num_blocks = (n_ppairs + block_size - 1) / block_size;

  evaluate<<<num_blocks, block_size>>>(x, y, z, ppairs, scores, n_ppairs);
  cudaDeviceSynchronize();
  double score = 0.;
  for (unsigned i = 0; i < n_ppairs; ++i) {
    score += scores[i];
  }
  std::cerr << "Initial score " << score << std::endl;

  auto timestart = std::chrono::steady_clock::now();
  optimize(1000000, x, y, z, ppairs, scores, n_ppairs);
  auto timeend = std::chrono::steady_clock::now();
  auto span = std::chrono::duration_cast<
                      std::chrono::duration<double> >(timeend - timestart);

  std::cerr << "Optimization time: " << span.count() << std::endl;

    cudaDeviceSynchronize();
  evaluate<<<num_blocks, block_size>>>(x, y, z, ppairs, scores, n_ppairs);
  cudaDeviceSynchronize();
  score = 0.;
  for (unsigned i = 0; i < n_ppairs; ++i) {
    score += scores[i];
  }
  std::cerr << "Final score " << score << std::endl;

  return 0;
}
