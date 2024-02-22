import IMP.core
import IMP.atom
import IMP.custom
import time

m = IMP.Model()
n_particles = 2000

ps = [IMP.Particle(m) for i in range(n_particles)]
for i, p in enumerate(ps):
    xyz = IMP.core.XYZ.setup_particle(p)
    xyz.set_coordinates(IMP.algebra.Vector3D(0.01 * i, 0., 0.))
    xyz.set_coordinates_are_optimized(True)
    IMP.atom.Mass.setup_particle(p, 1.0)

rsrs = []
pairscore = IMP.core.HarmonicDistancePairScore(2.0, 0.1)
lpc = IMP.container.ListPairContainer(m)
for i in range(n_particles - 1):
    lpc.add((ps[i], ps[i + 1]))

sf = IMP.core.RestraintsScoringFunction([IMP.container.PairsRestraint(
                                                         pairscore, lpc)])
print("Initial score:", sf.evaluate(False))
opt = IMP.custom.MoveXOptimizer(m, ps)
opt.set_scoring_function(sf)
timestart = time.time()
opt.optimize(1000000)
timeend = time.time()

print("Optimization took %.2f seconds" % (timeend-timestart))
print("Final score:", sf.evaluate(False))
