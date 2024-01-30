System setup: 2000 XYZ particles; all-against-all harmonic distance restraints;
1000 optimization steps


With IMP.core.HarmonicDistancePairScore and IMP.container.ListPairContainer
(imp_hdps.py)
Release build: 16.78s
Fast build: 9.81s

Pure C++ implementation (cpp_cpu) using x,y,z,pi arrays: 6.17s

With custom optimizer that scores using raw sphere data plus pi array
(imp_hdps_with_score.py), fast build: 5.69s



TODO
- Try pure C++ implementation using GPU
- Replace harmonic restraints with crosslink restraints
- Replace custom optimizer with custom ScoringFunction, custom Restraint
- Replace raw sphere data with XYZ decorator
