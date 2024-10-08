#Test Name: SquareShelfConstrainedTherTranAdolc
from model import *
from socket import gethostname
from triangle import *
from setmask import *
from parameterize import *
from setflowequation import *
from solve import *
from issmgslsolver import issmgslsolver


md = triangle(model(), '../Exp/Square.exp', 180000.)
md = setmask(md, 'all', '')
md = parameterize(md, '../Par/SquareShelfConstrained.py')
md.extrude(3, 1.)
md = setflowequation(md, 'SSA', 'all')
md.cluster = generic('name', gethostname(), 'np', 1)
md.transient.isstressbalance = False
md.transient.ismasstransport = False
md.transient.issmb = True
md.transient.isthermal = True
md.transient.isgroundingline = False
md.autodiff.isautodiff = True
md.toolkits.DefaultAnalysis = issmgslsolver()
md = solve(md, 'Transient')

#Fields and tolerances to track changes
field_names = ['Temperature', 'BasalforcingsGroundediceMeltingRate']
field_tolerances = [1e-13, 1e-13]
field_values = [md.results.TransientSolution[0].Temperature,
                md.results.TransientSolution[0].BasalforcingsGroundediceMeltingRate]
