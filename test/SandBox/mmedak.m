%Test Name: EarthSlr

%mesh earth:
md=model;
md.mesh=gmshplanet('radius',6.371012*10^3,'resolution',1000.); %500 km resolution mesh
md.cluster.np=16;

%parameterize slr solution:
%slr loading:  {{{
nt=100;
md.solidearth.surfaceload.icethicknesschange=zeros(md.mesh.numberofelements+1,nt);
md.solidearth.surfaceload.icethicknesschange(end,:)=2000.5:1:2099.5;
md.solidearth.sealevel=zeros(md.mesh.numberofvertices,1);
md.dsl.global_average_thermosteric_sea_level_change=[0;0];
md.dsl.sea_surface_height_change_above_geoid=zeros(md.mesh.numberofvertices+1,1);
md.dsl.sea_water_pressure_change_at_sea_floor=zeros(md.mesh.numberofvertices+1,1);

%antarctica
late=sum(md.mesh.lat(md.mesh.elements),2)/3;
longe=sum(md.mesh.long(md.mesh.elements),2)/3;

posant=find(late <-80);
md.solidearth.surfaceload.icethicknesschange(posant,:)=-50;

%greenland
posgre=find(late > 70 &  late < 80 & longe>-60 & longe<-30);
md.solidearth.surfaceload.icethicknesschange(posgre,:)=-100;

%alaska : 
posala=find(late > 62 &  late < 68 & longe>-152 & longe<-147);
md.solidearth.surfaceload.icethicknesschange(posala,:)=-150;

%himalaya : 
poshim=find(late > 25 &  late < 32 & longe>81 & longe<86);
md.solidearth.surfaceload.icethicknesschange(poshim,:)=-150;


%elastic loading from love numbers:
md.solidearth.lovenumbers=lovenumbers('maxdeg',100);

%}}}
%mask:  {{{
mask=gmtmask(md.mesh.lat,md.mesh.long);
icemask=ones(md.mesh.numberofvertices,1);
pos=find(mask==0);  icemask(pos)=-1;
pos=find(sum(mask(md.mesh.elements),2)<3);   icemask(md.mesh.elements(pos,:))=-1;
md.mask.ice_levelset=icemask;
md.mask.ocean_levelset=-icemask;

%make sure that the elements that have loads are fully grounded:
pos=find(sum(md.solidearth.surfaceload.icethicknesschange(1:end-1,:),2));
md.mask.ocean_levelset(md.mesh.elements(pos,:))=1;

%make sure wherever there is an ice load, that the mask is set to ice:
md.mask.ice_levelset(md.mesh.elements(pos,:))=-1;
% }}}

md.solidearth.settings.ocean_area_scaling=0;

%Geometry for the bed, arbitrary: 
md.geometry.bed=-ones(md.mesh.numberofvertices,1);

%Materials: 
md.materials=materials('hydro');

%Miscellaneous
md.miscellaneous.name='test2002';

%Solution parameters
%Solution parameters
md.solidearth.settings.reltol=NaN;
md.solidearth.settings.abstol=1e-3;
md.solidearth.settings.computesealevelchange=1;

% max number of iteration reverted back to 10 (i.e., the original default value)
md.solidearth.settings.maxiter=10;

%initialize GIA: 
md.gia=giamme();
md.gia.Ngia=ones(md.mesh.numberofvertices,1);
md.gia.Ugia=ones(md.mesh.numberofvertices,1);
md.gia.modelid=1;

%Uncertainty Quantification%{{{
md.qmu.variables=struct();;

ns=size(md.gia.Ngia,2);
ids=(1:(ns+1))';
p=rand(ns,1);
probs=[p(1:ns); 0];  probs=probs/sum(probs);
md.qmu.variables.giamodelid=histogram_bin_uncertain('GiaModelid',ns+1,ids,probs);

%partitioning
npart=2;
partition=-ones(md.mesh.numberofelements,1);
partition(posg)=0;
partition(posa)=1;

%variables: 
md.qmu.variables.deltathickness=normal_uncertain('descriptor','scaled_SealevelriseDeltathickness',...
	'mean',ones(2,nt),...
	'stddev',[.1*ones(1,nt);.2*ones(1,nt)],...
	'partition',partition,'nsteps',nt);

md.qmu.correlation_matrix=zeros(npart*nt,npart*nt);
for i=1:npart,
	for j=1:nt,
		indi=(i-1)*nt+j;
		for k=1:npart,
			for l=1:nt,
				indj=(k-1)*nt+l;
				if i~=k,
					md.qmu.correlation_matrix(indi,indj)=0;
				else
					%same partition:
					if j==l, 
						md.qmu.correlation_matrix(indi,indj)=1;
					else
						md.qmu.correlation_matrix(indi,indj)=.2;
					end
				end
			end
		end
	end
end
md.qmu.correlation_matrix=[];



%responses 
md.qmu.responses.sealevel1=response_function('descriptor','Outputdefinition1');
md.qmu.responses.sealevel2=response_function('descriptor','Outputdefinition2');
md.qmu.responses.sealevel3=response_function('descriptor','Outputdefinition3');
md.qmu.responses.sealevel4=response_function('descriptor','Outputdefinition4');
md.qmu.responses.sealevel5=response_function('descriptor','Outputdefinition5');
md.qmu.responses.sealevel6=response_function('descriptor','Outputdefinition6');
md.qmu.responses.sealevel7=response_function('descriptor','Outputdefinition7');
md.qmu.responses.sealevel8=response_function('descriptor','Outputdefinition8');
md.qmu.responses.sealevel8=response_function('descriptor','Outputdefinition9');
md.qmu.responses.sealevel10=response_function('descriptor','Outputdefinition10');

%output definitions: 
for i=1:10,
	if i==1,
		md.outputdefinition.definitions={nodalvalue('name','SNode','definitionstring','Outputdefinition1', ...
			'model_string','Sealevel','node',i)}; 
	else
		md.outputdefinition.definitions{end+1}=nodalvalue('name','SNode','definitionstring',['Outputdefinition' num2str(i)], ...
			'model_string','Sealevel','node',i); 
end
end
%algorithm: 
md.qmu.method     =dakota_method('nond_samp');
md.qmu.method(end)=dmeth_params_set(md.qmu.method(end),...
	'seed',1234,...
	'samples',3,...
	'sample_type','lhs');

%parameters
md.qmu.params.direct=true;
md.qmu.params.interval_type='forward';
md.qmu.params.analysis_driver='matlab';
md.qmu.params.evaluation_scheduling='master';
md.qmu.params.processors_per_evaluation=1;
md.qmu.params.tabular_graphics_data=true;
md.qmu.output=1;
%}}}

%transient: 
md.timestepping.start_time=2000;
md.timestepping.interp_forcings=0;
md.timestepping.final_time=2002;
md.transient.issmb=0;
md.transient.ismasstransport=0;
md.transient.isstressbalance=0;
md.transient.isthermal=0;
md.transient.isslr=1;
md.transient.isgia=1;
md.slr.requested_outputs= {'default',...
		'SealevelriseDeltathickness','Sealevel','SealevelRSLRate','SealevelriseCumDeltathickness',...
		'SealevelNEsaRate', 'SealevelUEsaRate', 'NGiaRate', 'UGiaRate',...
		'SealevelEustaticMask','SealevelEustaticOceanMask'};

%hack: 
md.geometry.thickness=ones(md.mesh.numberofvertices,1);
md.geometry.base=-ones(md.mesh.numberofvertices,1);
md.geometry.surface=zeros(md.mesh.numberofvertices,1);

%eustatic + rigid + elastic + rotation run:
md.verbose=verbose('11111111111');
%md.verbose.qmu=1;
md.solidearth.settings.rigid=1; md.solidearth.settings.elastic=1;md.solidearth.settings.rotation=1;
md.qmu.isdakota=1;
md=solve(md,'slr');

