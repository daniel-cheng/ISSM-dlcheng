#Test Name: SquareShelfConstrainedBedSlop3d
from model import *
from socket import gethostname
from triangle import *
from setmask import *
from parameterize import *
from setflowequation import *
from solve import *


md = triangle(model(), '../Exp/Square.exp', 150000)
md = setmask(md, 'all', '')
md = parameterize(md, '../Par/SquareShelfConstrained.py')
md.extrude(5, 1.)
md = setflowequation(md, 'SSA', 'all')
md.cluster = generic('name', gethostname(), 'np', 3)
md = solve(md, 'BedSlope')

#Fields and tolerances to track changes
field_names = ['BedSlopeX', 'BedSlopeY']
field_tolerances = [1e-13, 1e-13]
field_values = [md.results.BedSlopeSolution.BedSlopeX,
                md.results.BedSlopeSolution.BedSlopeY]
