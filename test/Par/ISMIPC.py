import numpy as np
from SetIceSheetBC import SetIceSheetBC

#Ok, start defining model parameters here

print("      creating thickness")
md.geometry.surface = 2000. - md.mesh.x * np.tan(0.1 * np.pi / 180.)  #to have z > 0
md.geometry.base = md.geometry.surface - 1000.
md.geometry.thickness = md.geometry.surface - md.geometry.base

print("      creating drag")
#md.friction.coefficient = sqrt(md.constants.yts. * (1000. + 1000. * sin(md.mesh.x * 2. * pi / max(md.mesh.x / 2.)). * sin(md.mesh.y * 2. * pi / max(md.mesh.x / 2.))). / (md.constants.g * (md.materials.rho_ice * md.geometry.thickness + md.materials.rho_water * md.geometry.base)))
md.friction.coefficient = np.sqrt(md.constants.yts * (1000. + 1000. * np.sin(md.mesh.x * 2. * np.pi / np.max(md.mesh.x)) * np.sin(md.mesh.y * 2. * np.pi / np.max(md.mesh.x))))
md.friction.coefficient[np.nonzero(md.mask.ocean_levelset < 0.)[0]] = 0.
md.friction.p = np.ones((md.mesh.numberofelements))
md.friction.q = np.zeros((md.mesh.numberofelements))

print("      creating flow law parameter")
md.materials.rheology_B = 6.8067 * 10**7 * np.ones((md.mesh.numberofvertices))
md.materials.rheology_n = 3. * np.ones((md.mesh.numberofelements))

print("      boundary conditions for stressbalance model:")
#Create node on boundary first (because we can not use mesh)
md = SetIceSheetBC(md)
