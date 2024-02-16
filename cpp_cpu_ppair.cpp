#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

static const unsigned n_particles = 2000;

double evaluate(std::vector<double> &x, std::vector<double> &y,
                std::vector<double> &z,
                std::vector<unsigned> &ppairs) {
  static const double mean = 2.0;
  static const double force = 0.1;
  double score = 0.;
  for (unsigned i = 0; i < ppairs.size(); i += 2) {
    double dx = x[ppairs[i]] - x[ppairs[i+1]];
    double dy = y[ppairs[i]] - y[ppairs[i+1]];
    double dz = z[ppairs[i]] - z[ppairs[i+1]];
    double r = sqrt((dx*dx) + (dy*dy) + (dz*dz));
    score += 0.5 * force * (r - mean) * (r - mean);
  }
  return score;
}

double optimize(unsigned max_steps, std::vector<double> &x,
                std::vector<double> &y, std::vector<double> &z,
                std::vector<unsigned> &ppairs) {
  double score = 0.;
  for (unsigned step = 0; step < max_steps; ++step) {
    score = evaluate(x, y, z, ppairs);
    for (unsigned p = 0; p < n_particles; ++p) {
      if (p % 2 == 0) {
        x[p] += 0.01;
      } else {
        x[p] -= 0.01;
      }
    }
  }
  return score;
}

int main() {
  std::vector<double> x(n_particles);
  std::vector<double> y(n_particles);
  std::vector<double> z(n_particles);
  std::vector<unsigned> ppairs(n_particles * (n_particles - 1));

  for (unsigned i = 0; i < n_particles; ++i) {
    x[i] = 0.01 * i;
    y[i] = 0.0;
    z[i] = 0.0;
  }

  unsigned ppi = 0;
  for (unsigned i = 0; i < n_particles; ++i) {
    for (unsigned j = i + 1; j < n_particles; ++j) {
      ppairs[ppi++] = i;
      ppairs[ppi++] = j;
    }
  }

  std::cerr << "Initial score " << evaluate(x, y, z, ppairs) << std::endl;

  auto timestart = std::chrono::steady_clock::now();
  optimize(1000, x, y, z, ppairs);
  auto timeend = std::chrono::steady_clock::now();
  auto span = std::chrono::duration_cast<
                      std::chrono::duration<double> >(timeend - timestart);

  std::cerr << "Optimization time: " << span.count() << std::endl;

  std::cerr << "Final score " << evaluate(x, y, z, ppairs) << std::endl;

  return 0;
}
