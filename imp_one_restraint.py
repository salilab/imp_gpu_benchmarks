import IMP.core
import IMP.atom
import time

m = IMP.Model()
n_particles = 1000

ps = [IMP.Particle(m) for i in range(n_particles)]
for p in ps:
    IMP.core.XYZ.setup_particle(p).set_coordinates_are_optimized(True)
    IMP.atom.Mass.setup_particle(p, 1.0)

rsrs = []
uf = IMP.core.Harmonic(2.0, 0.1)
pairscore = IMP.core.DistancePairScore(uf)
lpc = IMP.container.ListPairContainer(m)
for i in range(n_particles):
    for j in range(i+1, n_particles):
        lpc.add((ps[i], ps[j]))

sf = IMP.core.RestraintsScoringFunction([IMP.container.PairsRestraint(
                                                         pairscore, lpc)])
md = IMP.atom.MolecularDynamics(m)
md.set_scoring_function(sf)
md.set_temperature(300.)
timestart = time.time()
md.optimize(1000)
timeend = time.time()

print("MD took %.2f seconds" % (timeend-timestart))
