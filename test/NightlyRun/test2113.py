#Test Name: Esa2Dsurface
#Northern hemisphere example for north - south, east - west components of horiz motion
#Same as test2111.m except that AIS is assumed to have located in Northern Hemisphere
from socket import gethostname

import numpy as np

from lovenumbers import *
from model import *
from paterson import *
from roundmesh import *
from solve import *


#mesh ais: {{{
md = model()
md = triangle(md, '../Exp/Ais.exp', 200000)  # max element size
# }}}
#define load: {{{
md.esa.deltathickness = np.zeros((md.mesh.numberofelements, 1))
disc_radius = 500  # km
index = md.mesh.elements
x_element = np.mean(md.mesh.x[index - 1], 1) - 1.0e6
y_element = np.mean(md.mesh.y[index - 1], 1) + 1.0e6
rad_dist = np.sqrt(x_element**2 + y_element**2) / 1000  # radial distance in km
pos = np.where(rad_dist <= disc_radius)[0]
md.esa.deltathickness[pos] = -1  # 1 m water withdrawal
# }}}
#love numbers: {{{
md.solidearth.lovenumbers = lovenumbers('maxdeg', 10000, 'referenceframe', 'CF')
# }}}
#mask:  {{{
#make sure wherever there is an ice load, that the mask is set to ice:
md.mask.ice_levelset = np.ones((md.mesh.numberofvertices, 1))
pos = np.nonzero(md.esa.deltathickness)[0]
md.mask.ice_levelset[md.mesh.elements[pos, :]] = -1

#is ice grounded?
md.mask.ocean_levelset = -np.ones((md.mesh.numberofvertices, 1))
pos = np.where(md.mask.ice_levelset <= 0)[0]
md.mask.ocean_levelset[pos] = 1
# }}}
#geometry:  {{{
di = md.materials.rho_ice / md.materials.rho_water
md.geometry.thickness = np.ones((md.mesh.numberofvertices, 1))
md.geometry.surface = (1 - di) * np.zeros((md.mesh.numberofvertices, 1))
md.geometry.base = md.geometry.surface - md.geometry.thickness
md.geometry.bed = md.geometry.base
# }}}
#materials:  {{{
md.initialization.temperature = 273.25 * np.ones((md.mesh.numberofvertices, 1))
md.materials.rheology_B = paterson(md.initialization.temperature)
md.materials.rheology_n = 3 * np.ones((md.mesh.numberofelements, 1))
# }}}
#additional parameters, miscellaneous: {{{
md.miscellaneous.name = 'test2113'
md.esa.degacc = 0.01
md.esa.hemisphere = 1  # AIS is placed in Northern Hemisphere
# }}}

#solve esa: {{{
md.esa.requested_outputs = ['EsaUmotion', 'EsaNmotion', 'EsaEmotion']
md.cluster = generic('name', gethostname(), 'np', 3)
md.verbose = verbose('111111111')
md = solve(md, 'Esa')
# }}}
#Fields and tolerances to track changes: {{{
field_names = ['EsaUmotion', 'EsaNmotion', 'EsaEmotion']
field_tolerances = [1e-13, 2e-13, 2e-13]
field_values = [md.results.EsaSolution.EsaUmotion,
                md.results.EsaSolution.EsaNmotion,
                md.results.EsaSolution.EsaEmotion]
# }}}
