#ifndef IMPCUSTOM_MOVE_X_OPTIMIZER_H
#define IMPCUSTOM_MOVE_X_OPTIMIZER_H

#include <IMP/custom/custom_config.h>

#include <IMP/Optimizer.h>
#include <IMP/ScoringFunction.h>
#include <IMP/Restraint.h>
#include <IMP/PairScore.h>

IMPCUSTOM_BEGIN_NAMESPACE

class IMPCUSTOMEXPORT MoveXOptimizer : public Optimizer {
 public:
  MoveXOptimizer(Model *m, ParticleIndexes pis);

  virtual Float do_optimize(unsigned int max_steps) override;

  virtual ModelObjectsTemp do_get_inputs() const override;

  IMP_OBJECT_METHODS(MoveXOptimizer);

 private:
  ParticleIndexes pis_;

};

class IMPCUSTOMEXPORT MoveXAndScoreOptimizer : public Optimizer {
 public:
  MoveXAndScoreOptimizer(Model *m, ParticleIndexes pis);

  virtual Float do_optimize(unsigned int max_steps) override;

  virtual ModelObjectsTemp do_get_inputs() const override;

  IMP_OBJECT_METHODS(MoveXAndScoreOptimizer);

 private:
  ParticleIndexes pis_;

};

class IMPCUSTOMEXPORT CustomScoringFunction : public ScoringFunction {
 public:
  CustomScoringFunction(Model *m, ParticleIndexes pis);

  virtual ModelObjectsTemp do_get_inputs() const override;
  virtual void do_add_score_and_derivatives(ScoreAccumulator sa,
                                            const ScoreStatesTemp &ss) override;
  virtual Restraints create_restraints() const override;

  IMP_OBJECT_METHODS(CustomScoringFunction);

 private:
  ParticleIndexes pis_;
};

class IMPCUSTOMEXPORT CustomRestraint : public Restraint {
 public:
  CustomRestraint(Model *m, ParticleIndexes pis);

  ModelObjectsTemp do_get_inputs() const override;
  void do_add_score_and_derivatives(ScoreAccumulator sa) const override;
  IMP_OBJECT_METHODS(CustomRestraint);

 private:
  ParticleIndexes pis_;
};

class IMPCUSTOMEXPORT CustomPairScore : public PairScore {
 public:
  virtual ModelObjectsTemp do_get_inputs(
      Model *m, const ParticleIndexes &pis) const override;
  double evaluate_index(Model *m, const ParticleIndexPair& vt,
                        DerivativeAccumulator *da) const override;
  double evaluate_indexes(Model *m, const ParticleIndexPairs &o,
                          DerivativeAccumulator *da,
                          unsigned int lower_bound,
                          unsigned int upper_bound) const override;
  double evaluate_indexes_scores(
                        Model *m, const ParticleIndexPairs &o,
                        DerivativeAccumulator *da,
                        unsigned int lower_bound,
                        unsigned int upper_bound,
                        std::vector<double> &score) const override;
  double evaluate_indexes_delta(
                        Model *m, const ParticleIndexPairs &o,
                        DerivativeAccumulator *da,
                        const std::vector<unsigned> &indexes,
                        std::vector<double> &score) const override;
  double evaluate_if_good_indexes(Model *m,
                        const ParticleIndexPairs &o,
                        DerivativeAccumulator *da, double max,
                        unsigned int lower_bound,
                        unsigned int upper_bound) const override;
  IMP_OBJECT_METHODS(CustomPairScore);
};

IMPCUSTOM_END_NAMESPACE

#endif /* IMPCUSTOM_MOVE_X_OPTIMIZER_H */
