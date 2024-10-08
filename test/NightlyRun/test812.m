%Test Name: SquareShelfLevelsetCalvingMOLHO2dLevermann
md=triangle(model(),'../Exp/Square.exp',50000.);
md=setmask(md,'all','');
md=parameterize(md,'../Par/SquareShelf.par');
md=setflowequation(md,'MOLHO','all');
md.cluster=generic('name',oshostname(),'np',3);

%Do not kill ice bergs as all is floating
md.levelset.kill_icebergs=0;

x = md.mesh.x;
xmin = min(x);
xmax = max(x);
Lx = (xmax-xmin);
alpha = 2./3.;
md.mask.ice_levelset = ((x - alpha*Lx)>0) - ((x - alpha*Lx)<0);

md.timestepping.time_step=10;
md.timestepping.final_time=30;

%Transient
md.transient.isstressbalance=1;
md.transient.ismasstransport=1;
md.transient.issmb=1;
md.transient.isthermal=0;
md.transient.isgroundingline=0;
md.transient.ismovingfront=1;

md.calving=calvinglevermann();
md.calving.coeff=4.89e13*ones(md.mesh.numberofvertices,1);
md.frontalforcings.meltingrate=zeros(md.mesh.numberofvertices,1);
md.levelset.spclevelset=NaN(md.mesh.numberofvertices,1);
md.levelset.migration_max = 1e8;

md.transient.requested_outputs={'default','StrainRateparallel','StrainRateperpendicular','Calvingratex','Calvingratey','CalvingCalvingrate'};

md=SetMOLHOBC(md);
md=solve(md,'Transient');

%Fields and tolerances to track changes
field_names     ={'Vx1','Vy1','Vel1','Pressure1','Thickness1','Surface1','MaskIceLevelset1','StrainRateparallel1','StrainRateperpendicular1','CalvingCalvingrate1'...
		'Vx2','Vy2','Vel2','Pressure2','Thickness2','Surface2','MaskIceLevelset2','StrainRateparallel2','StrainRateperpendicular2','CalvingCalvingrate2'...
		'Vx3','Vy3','Vel3','Pressure3','Thickness3','Surface3','MaskIceLevelset3','StrainRateparallel3','StrainRateperpendicular3','CalvingCalvingrate3'};
field_tolerances={1e-11,1e-11,1e-11,1e-11,1e-11,1e-11,1e-11,1e-11,1e-11,1e-11,...
		2e-11,2e-11,2e-11,1e-11,1e-11,1e-11,1e-11,1e-11,1e-11,1e-11,...
		2e-11,2e-11,2e-11,1e-11,1e-11,1e-11,1e-11,5e-11,5e-11,5e-11};
field_values={...
	md.results.TransientSolution(1).Vx,...
	md.results.TransientSolution(1).Vy,...
	md.results.TransientSolution(1).Vel,...
	md.results.TransientSolution(1).Pressure,...
	md.results.TransientSolution(1).Thickness,...
	md.results.TransientSolution(1).Surface,...
	md.results.TransientSolution(1).MaskIceLevelset,...
	md.results.TransientSolution(1).StrainRateparallel,...
	md.results.TransientSolution(1).StrainRateperpendicular,...
	md.results.TransientSolution(1).CalvingCalvingrate,...
	md.results.TransientSolution(2).Vx,...
	md.results.TransientSolution(2).Vy,...
	md.results.TransientSolution(2).Vel,...
	md.results.TransientSolution(2).Pressure,...
	md.results.TransientSolution(2).Thickness,...
	md.results.TransientSolution(2).Surface,...
	md.results.TransientSolution(2).MaskIceLevelset,...
	md.results.TransientSolution(2).StrainRateparallel,...
	md.results.TransientSolution(2).StrainRateperpendicular,...
	md.results.TransientSolution(2).CalvingCalvingrate,...
	md.results.TransientSolution(3).Vx,...
	md.results.TransientSolution(3).Vy,...
	md.results.TransientSolution(3).Vel,...
	md.results.TransientSolution(3).Pressure,...
	md.results.TransientSolution(3).Thickness,...
	md.results.TransientSolution(3).Surface,...
	md.results.TransientSolution(3).MaskIceLevelset,...
	md.results.TransientSolution(3).StrainRateparallel,...
	md.results.TransientSolution(3).StrainRateperpendicular,...
	md.results.TransientSolution(3).CalvingCalvingrate,...
	};
