#ifndef IMPCUSTOM_MOVE_X_OPTIMIZER_H
#define IMPCUSTOM_MOVE_X_OPTIMIZER_H

#include <IMP/custom/custom_config.h>

#include <IMP/Optimizer.h>
#include <IMP/ScoringFunction.h>

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
  virtual Restraints create_restraints() const;

  IMP_OBJECT_METHODS(CustomScoringFunction);

 private:
  ParticleIndexes pis_;
};

IMPCUSTOM_END_NAMESPACE

#endif /* IMPCUSTOM_MOVE_X_OPTIMIZER_H */
