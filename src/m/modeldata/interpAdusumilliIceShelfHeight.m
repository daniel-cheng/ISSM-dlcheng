function [output, output_err] = interpAdusumilliIceShelfHeight(X,Y)
%INTERPADUSUMILLIICESHELFMELT - imports basal height rates from (Adusumilli et al., 2020).
%   About the data: "Average basal height rates for Antarctic ice shelves for the 2010–2018 period at 
%   high spatial resolution, estimated using CryoSat-2 data. This data file was last updated on 2020-06-11."
%
%   Citation: Adusumilli, Susheel; Fricker, Helen A.; Medley, Brooke C.; Padman, Laurie; Siegfried, Matthew R. (2020). 
%   Data from: Interannual variations in heightwater input to the Southern Ocean from Antarctic ice shelves. 
%   UC San Diego Library Digital Collections. https://doi.org/10.6075/J04Q7SHT
%
%   Usage:
%      output = interpAdusumilliIceShelfHeight(X,Y)

% define path and filename for this machine
switch (oshostname()),
	case {'totten'}
		filename ='/totten_1/ModelData/Antarctica/Adusumilli2020IceShelfMelt/ANT_iceshelf_height_changes_RA_1994_2018_v0.h5';
	case {'larsen'}
		filename ='/export/proj483/b/ModelData/Adusumilli2020IceShelfMelt/ANT_iceshelf_height_changes_RA_1994_2018_v0.h5';
	otherwise
		error('hostname not supported yet');
end

disp(['   -- Adusumilli Ice Shelf Height: loading height data']);
% read in coordinates:
%	coordinates are in Polar Stereographic projection 'PS-71'
xdata = double(h5read(filename,'/x'));
ydata = double(h5read(filename,'/y'));
xdata=xdata(:,1);
ydata=ydata(1,:)';

% read in data:
% 'Height rate (1994–2018), in meters of ice equivalent per year, positive is heighting, relative to 1994'
% 'For ice shelf areas where CryoSat-2 data were not available, w_b_interp provides the 
%  mean height rate measured at the same ice draft as the grid cell elsewhere on the ice shelf. 
%  Ice draft was estimated using BedMachine data.'
time = double(h5read(filename,'/time'));
data = double(h5read(filename,'/h_alt'));
%data = permute(data,[2 1 3]);
data_err = double(h5read(filename,'/uncert_alt'));
%data_err = permute(data_err,[2 1 3]);

disp(['   -- Adusumilli Ice Shelf Height: interpolating height data']);
timesteps = size(time,1);
output=zeros(size(X,1),timesteps);
output_err=zeros(size(X,1),timesteps);

for i=1:size(data,3),
	output(:,i) = InterpFromGrid(xdata,ydata,data(:,:,i)',X(:),Y(:));
	output_err(:,i) = InterpFromGrid(xdata,ydata,data_err(:,:,i)',X(:),Y(:));
end
output(end+1,:) = time;
output_err(end+1,:) = time;

