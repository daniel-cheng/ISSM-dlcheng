%MISFIT class definition
%
%   Usage:
%      cfsurfacelogveltransient=cfsurfacelogveltransient();
%      cfsurfacelogveltransient=cfsurfacelogveltransient('name','SurfaceAltimetry',...
%                    'definitionstring','Outputdefinition1',... 
%							'model_string','Surface',...
%                    'observations',[md.geometry.surface;0],...
%                    'weights',ones(md.mesh.numberofvertices+1,1));
%
%

classdef cfsurfacelogveltransient
	properties (SetAccess=public)
		%cfsurfacelogveltransient
		name                = '';
		definitionstring    = ''; %string that identifies this output definition uniquely, from 'Outputdefinition[1-100]'
		vxobs					 = NaN; %observed field that we compare the model against
		vxobs_string		 = ''; %string for observed field.
		vyobs			       = NaN; %observed field that we compare the model against
		vyobs_string		 = ''; %string for observed field.
		weights            = NaN; %weight coefficients for every vertex
		weights_string     = ''; %string to identify this particular set of weights
	end
	
	methods
		function self = extrude(self,md) % {{{
			if ~isnan(self.weights)
				self.weights=project3d(md,'vector',self.weights,'type','node');
			end
			if ~isnan(self.vxobs)
				self.vxobs=project3d(md,'vector',self.vxobs,'type','node');
			end
			if ~isnan(self.vyobs)
				self.vyobs=project3d(md,'vector',self.vyobs,'type','node');
			end
		end % }}}
		function self = cfsurfacelogveltransient(varargin) % {{{
			if nargin==0,
				self=setdefaultparameters(self);
			else
				%use provided options to change fields
				options=pairoptions(varargin{:});

				%get name
				self.name                = getfieldvalue(options,'name','');
				self.definitionstring    = getfieldvalue(options,'definitionstring');
				self.vxobs				 = getfieldvalue(options,'vxobs',NaN);
				self.vyobs				 = getfieldvalue(options,'vyobs',NaN);
				self.vxobs_string		 = getfieldvalue(options,'vxobs_string');			
				self.vyobs_string		 = getfieldvalue(options,'vyobs_string');
				self.weights			 = getfieldvalue(options,'weights',NaN);
				self.weights_string 	 = getfieldvalue(options,'weights_string','');
			end
		end % }}}
		function self = setdefaultparameters(self) % {{{
		end % }}}
		function md = checkconsistency(self,md,solution,analyses) % {{{

			if ~ischar(self.name),
				error('cfsurfacelogveltransient error message: ''name'' field should be a string!');
			end
			OutputdefinitionStringArray={};
			for i=1:100
				OutputdefinitionStringArray{i}=strcat('Outputdefinition',num2str(i));
			end
			md = checkfield(md,'fieldname','self.definitionstring','field',self.definitionstring,'values',OutputdefinitionStringArray);
			md = checkfield(md,'fieldname','self.vxobs','field',self.vxobs,'size',[md.mesh.numberofvertices+1 NaN],'NaN',1,'Inf',1,'timeseries',1);
			md = checkfield(md,'fieldname','self.vyobs','field',self.vxobs,'size',[md.mesh.numberofvertices+1 NaN],'NaN',1,'Inf',1,'timeseries',1);
			md = checkfield(md,'fieldname','self.weights','field',self.weights,'size',[md.mesh.numberofvertices+1 NaN],'NaN',1,'Inf',1);

		end % }}}
		function md = disp(self) % {{{
		
			disp(sprintf('   cfsurfacelogveltransient:\n'));

			fielddisplay(self,'name','identifier for this cfsurfacelogveltransient response');
			fielddisplay(self,'definitionstring','string that identifies this output definition uniquely, from ''Outputdefinition[1-10]''');
			fielddisplay(self,'vxobs','observed field that we compare the model against');
			fielddisplay(self,'vxobs_string','observation string');
			fielddisplay(self,'vyobs','observed field that we compare the model against');
			fielddisplay(self,'vyobs_string','observation string');
			fielddisplay(self,'weights','weights (at vertices) to apply to the cfsurfacelogveltransient');
			fielddisplay(self,'weights_string','string for weights for identification purposes');

		end % }}}
		function md = marshall(self,prefix,md,fid) % {{{

		WriteData(fid,prefix,'data',self.name,'name','md.cfsurfacelogveltransient.name','format','String');
		WriteData(fid,prefix,'data',self.definitionstring,'name','md.cfsurfacelogveltransient.definitionstring','format','String');
		WriteData(fid,prefix,'data',self.vxobs,'name','md.cfsurfacelogveltransient.vxobs','format','DoubleMat','mattype',1,'timeserieslength',md.mesh.numberofvertices+1,'yts',md.constants.yts,'scale',1./md.constants.yts);
		WriteData(fid,prefix,'data',self.vxobs_string,'name','md.cfsurfacelogveltransient.vxobs_string','format','String');
		WriteData(fid,prefix,'data',self.vyobs,'name','md.cfsurfacelogveltransient.vyobs','format','DoubleMat','mattype',1,'timeserieslength',md.mesh.numberofvertices+1,'yts',md.constants.yts,'scale',1./md.constants.yts);
		WriteData(fid,prefix,'data',self.vyobs_string,'name','md.cfsurfacelogveltransient.vyobs_string','format','String');
		WriteData(fid,prefix,'data',self.weights,'name','md.cfsurfacelogveltransient.weights','format','DoubleMat','mattype',1,'timeserieslength',md.mesh.numberofvertices+1,'yts',md.constants.yts);
		WriteData(fid,prefix,'data',self.weights_string,'name','md.cfsurfacelogveltransient.weights_string','format','String');
		end % }}}
	end
end
