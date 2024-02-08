System setup: 2000 XYZ particles; all-against-all harmonic distance restraints;
1000 optimization steps


With IMP.core.HarmonicDistancePairScore and IMP.container.ListPairContainer
(imp_hdps.py)
Release build: 16.78s
  (with std::get to read ParticleIndexPair, ~11.5s)
  (with Vector3D uninitialized check moved to internal, 10.0s)
  (with Vector3D compatible check moved to compile time, 9.87s)
Fast build: 9.81s

Pure C++ implementation (cpp_cpu) using x,y,z,pi arrays (double loop): 6.17s

Pure C++ implementation (cpp_cpu_ppair) using x,y,z,ppair arrays (single loop): 6.44s

With custom optimizer that scores using raw sphere data plus pi array
(imp_hdps_with_score.py), fast build: 6.03s

With regular optimizer but custom scoring function that scores using raw
sphere data plus pi array
(imp_hdps_custom_scoring.py), fast build: 6.03s

With regular optimizer and scoring function but custom restraint that scores
using raw sphere data plus pi array
(imp_hdps_custom_restraint.py), fast build: 6.02s

With custom pair score that scores using raw sphere data plus ppair array
(imp_hdps_custom_pair_score.py), fast build: 6.39s
(double loop, ignoring ppairs: 5.63s)
(using decorators: 7.88s)
(using decorators, release mode: 7.79s)
(using decorators just to get particle indexes, fast build: 6.38s)
(using decorators and get_coordinates(), fast build: 6.55s)
(using decorators and Vector3d::operator-, fast build: 7.69s)
(using operator-= and our own copy, fast build: 8.10s)


TODO
- Try pure C++ implementation using GPU
- Replace harmonic restraints with crosslink restraints
- Replace raw sphere data with XYZ decorator
