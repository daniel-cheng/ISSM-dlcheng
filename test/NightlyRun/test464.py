#Test Name: SquareSheetShelfAmrBamgIceFront
from model import *
from socket import gethostname
from triangle import *
from setmask import *
from parameterize import *
from setflowequation import *
from solve import *

md = triangle(model(), '../Exp/Square.exp', 150000.)
md = setmask(md, '../Exp/SquareShelf.exp', '')
md = parameterize(md, '../Par/SquareSheetShelf.py')
md = setflowequation(md, 'SSA', 'all')
md.cluster = generic('name', gethostname(), 'np', 3)
md.transient.isstressbalance = 1
md.transient.ismasstransport = 1
md.transient.issmb = 0
md.transient.isthermal = 0
md.transient.isgroundingline = 0
#amr bamg settings, just ice front
md.amr.hmin = 10000
md.amr.hmax = 100000
md.amr.fieldname = 'None'
md.amr.keepmetric = 0
md.amr.gradation = 1.2
md.amr.groundingline_resolution = 12000
md.amr.groundingline_distance = 0
md.amr.icefront_resolution = 12000
md.amr.icefront_distance = 100000
md.amr.thicknesserror_resolution = 1000
md.amr.thicknesserror_threshold = 0
md.amr.deviatoricerror_resolution = 1000
md.amr.deviatoricerror_threshold = 0
md.transient.amr_frequency = 1
md.timestepping.start_time = 0
md.timestepping.final_time = 3
md.timestepping.time_step = 1
md = solve(md, 'Transient')

#Fields and tolerances to track changes
field_names = ['Vx', 'Vy', 'Vel', 'Pressure']
field_tolerances = [1e-13, 1e-13, 1e-13, 1e-13]
field_values = [md.results.TransientSolution[2].Vx,
                md.results.TransientSolution[2].Vy,
                md.results.TransientSolution[2].Vel,
                md.results.TransientSolution[2].Pressure]
