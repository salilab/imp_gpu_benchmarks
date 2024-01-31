#include <IMP/custom/MoveXOptimizer.h>
#include <IMP/log.h>
#include <IMP/Model.h>
#include <IMP/core/XYZ.h>
#include <IMP/algebra/Vector3D.h>

IMPCUSTOM_BEGIN_NAMESPACE

MoveXOptimizer::MoveXOptimizer(Model *m, ParticleIndexes pis)
    : Optimizer(m), pis_(pis) {
}

ModelObjectsTemp MoveXOptimizer::do_get_inputs() const {
  ModelObjectsTemp objs = get_optimizer_state_inputs();
  objs += get_particles(get_model(), pis_);
  return objs;
}

Float MoveXOptimizer::do_optimize(unsigned int max_steps) {
  IMP_OBJECT_LOG;
  IMP_USAGE_CHECK(get_model(),
                  "Must set the model on the optimizer before optimizing");

  Model *m = get_model();
  double score = 0.;
  for (unsigned step = 0; step < max_steps; ++step) {
    score = get_scoring_function()->evaluate(false);
    update_states();
    for (unsigned p = 0; p < pis_.size(); ++p) {
      IMP::core::XYZ xyz(m, pis_[p]);
      IMP::algebra::Vector3D v = xyz.get_coordinates();
      if (p % 2 == 0) {
        v[0] += 0.01;
      } else {
        v[0] -= 0.01;
      }
      xyz.set_coordinates(v);
    }
  }
  return score;
}

MoveXAndScoreOptimizer::MoveXAndScoreOptimizer(Model *m, ParticleIndexes pis)
    : Optimizer(m), pis_(pis) {
}

ModelObjectsTemp MoveXAndScoreOptimizer::do_get_inputs() const {
  ModelObjectsTemp objs = get_optimizer_state_inputs();
  objs += get_particles(get_model(), pis_);
  return objs;
}

namespace {
double our_evaluate(algebra::Sphere3D *spheres, const ParticleIndexes &pis) {
  static const double mean = 2.0;
  static const double force = 0.1;
  double score = 0.;
  unsigned n_particles = pis.size();
  for (unsigned i = 0; i < n_particles; ++i) {
    unsigned pi = pis[i].get_index();
    algebra::Sphere3D *si = spheres + pi;
    for (unsigned j = i + 1; j < n_particles; ++j) {
      unsigned pj = pis[j].get_index();
      algebra::Sphere3D *sj = spheres + pj;

      double dx = (*sj)[0] - (*si)[0];
      double dy = (*sj)[1] - (*si)[1];
      double dz = (*sj)[2] - (*si)[2];
      double r = sqrt((dx*dx) + (dy*dy) + (dz*dz));
      score += 0.5 * force * (r - mean) * (r - mean);
    }
  }
  return score;
}

double our_evaluate_pairs(algebra::Sphere3D *spheres,
                          const ParticleIndexPairs &ppis,
                          unsigned lower_bound, unsigned upper_bound) {
  static const double mean = 2.0;
  static const double force = 0.1;
  double score = 0.;
  for (unsigned i = lower_bound; i < upper_bound; ++i) {
    unsigned pi = ppis[i][0].get_index();
    unsigned pj = ppis[i][1].get_index();
    algebra::Sphere3D *si = spheres + pi;
    algebra::Sphere3D *sj = spheres + pj;
    double dx = (*sj)[0] - (*si)[0];
    double dy = (*sj)[1] - (*si)[1];
    double dz = (*sj)[2] - (*si)[2];
    double r = sqrt((dx*dx) + (dy*dy) + (dz*dz));
    score += 0.5 * force * (r - mean) * (r - mean);
  }
  return score;
}
} // namespace

Float MoveXAndScoreOptimizer::do_optimize(unsigned int max_steps) {
  IMP_OBJECT_LOG;
  IMP_USAGE_CHECK(get_model(),
                  "Must set the model on the optimizer before optimizing");

  Model *m = get_model();
  algebra::Sphere3D *spheres = m->access_spheres_data();
  double score = 0.;
  for (unsigned step = 0; step < max_steps; ++step) {
    score = our_evaluate(spheres, pis_);
    for (unsigned p = 0; p < pis_.size(); ++p) {
      if (p % 2 == 0) {
        spheres[pis_[p].get_index()][0] += 0.01;
      } else {
        spheres[pis_[p].get_index()][0] -= 0.01;
      }
    }
  }
  return score;
}

CustomScoringFunction::CustomScoringFunction(Model *m, ParticleIndexes pis)
        : ScoringFunction(m, "CustomScoringFunction"), pis_(pis) {}

ModelObjectsTemp CustomScoringFunction::do_get_inputs() const {
  return get_particles(get_model(), pis_);
}

void CustomScoringFunction::do_add_score_and_derivatives(ScoreAccumulator sa,
                                            const ScoreStatesTemp &ss) {
  IMP::Model *m = get_model();
  algebra::Sphere3D *spheres = m->access_spheres_data();
  sa.add_score(our_evaluate(spheres, pis_));
}

Restraints CustomScoringFunction::create_restraints() const {
  Restraints x;
  return x;
}

CustomRestraint::CustomRestraint(Model *m, ParticleIndexes pis)
        : Restraint(m, "CustomRestraint"), pis_(pis) {}

ModelObjectsTemp CustomRestraint::do_get_inputs() const {
  return get_particles(get_model(), pis_);
}

void CustomRestraint::do_add_score_and_derivatives(ScoreAccumulator sa) const {
  IMP::Model *m = get_model();
  algebra::Sphere3D *spheres = m->access_spheres_data();
  sa.add_score(our_evaluate(spheres, pis_));
}

double CustomPairScore::evaluate_index(Model *m, const ParticleIndexPair& vt,
                        DerivativeAccumulator *da) const {
  std::cerr << "No implementation" << std::endl;
  return 0.;
}

double CustomPairScore::evaluate_indexes(Model *m, const ParticleIndexPairs &o,
                          DerivativeAccumulator *da,
                          unsigned int lower_bound,
                          unsigned int upper_bound) const {
  algebra::Sphere3D *spheres = m->access_spheres_data();
  return our_evaluate_pairs(spheres, o, lower_bound, upper_bound);
}

double CustomPairScore::evaluate_indexes_scores(
                        Model *m, const ParticleIndexPairs &o,
                        DerivativeAccumulator *da,
                        unsigned int lower_bound,
                        unsigned int upper_bound,
                        std::vector<double> &score) const {
  std::cerr << "No implementation" << std::endl;
  return 0.;
}

double CustomPairScore::evaluate_indexes_delta(
                        Model *m, const ParticleIndexPairs &o,
                        DerivativeAccumulator *da,
                        const std::vector<unsigned> &indexes,
                        std::vector<double> &score) const {
  std::cerr << "No implementation" << std::endl;
  return 0.;
}

double CustomPairScore::evaluate_if_good_indexes(Model *m,
                        const ParticleIndexPairs &o,
                        DerivativeAccumulator *da, double max,
                        unsigned int lower_bound,
                        unsigned int upper_bound) const {
  std::cerr << "No implementation" << std::endl;
  return 0.;
}

ModelObjectsTemp CustomPairScore::do_get_inputs(
    Model *m, const ParticleIndexes &pis) const {
  return get_particles(m, pis);
}

IMPCUSTOM_END_NAMESPACE
