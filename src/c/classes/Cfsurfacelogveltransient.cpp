/*!\file Cfsurfacelogveltransient.cpp
 * \brief: Cfsurfacelogveltransient Object
 */

/*Headers:*/
/*{{{*/
#ifdef HAVE_CONFIG_H
#include <config.h>
#else
#error "Cannot compile with HAVE_CONFIG_H symbol! run configure first!"
#endif

#include "./classes.h"
#include "./ExternalResults/ExternalResult.h"
#include "./ExternalResults/Results.h"
#include "../datastructures/datastructures.h"
#include "./Elements/Element.h"
#include "./Elements/Elements.h"
#include "./FemModel.h"
#include "../modules/SurfaceAreax/SurfaceAreax.h"
#include "../classes/Params/Parameters.h"
#include "../classes/gauss/Gauss.h"
#include "./Inputs/DatasetInput.h"
/*}}}*/

/*Cfsurfacelogveltransient constructors, destructors :*/
Cfsurfacelogveltransient::Cfsurfacelogveltransient(){/*{{{*/

	this->definitionenum = -1;
	this->name           = NULL;
	this->datatimes      = NULL;
	this->passedflags    = NULL;
	this->J              = 0.;
}
/*}}}*/
Cfsurfacelogveltransient::Cfsurfacelogveltransient(char* in_name, int in_definitionenum, int in_num_datatimes, IssmDouble* in_datatimes, bool* in_passedflags, IssmDouble in_J){/*{{{*/

	this->definitionenum=in_definitionenum;

	this->name = xNew<char>(strlen(in_name)+1);
	xMemCpy<char>(this->name,in_name,strlen(in_name)+1);

	this->num_datatimes = in_num_datatimes;

	/*Allocate arrays*/
	_assert_(this->num_datatimes>0);
	this->datatimes   = xNew<IssmDouble>(this->num_datatimes);
	this->passedflags = xNew<bool>(this->num_datatimes);
	xMemCpy<IssmDouble>(this->datatimes,in_datatimes,this->num_datatimes);
	xMemCpy<bool>(this->passedflags,in_passedflags,this->num_datatimes);

	#ifdef _ISSM_DEBUG_ 
	for(int i=0;i<this->num_datatimes-1;i++){
		if(this->datatimes[i+1]<=this->datatimes[i]){
			_error_("time series is not in chronological order");
		}
	}
	#endif

	this->J = in_J;
}
/*}}}*/
Cfsurfacelogveltransient::Cfsurfacelogveltransient(char* in_name, int in_definitionenum, int in_num_datatimes, IssmDouble* in_datatimes){/*{{{*/

	this->definitionenum=in_definitionenum;

	this->name = xNew<char>(strlen(in_name)+1);
	xMemCpy<char>(this->name,in_name,strlen(in_name)+1);

	this->num_datatimes = in_num_datatimes;

	/*Allocate arrays*/
	_assert_(this->num_datatimes>0);
	this->datatimes   = xNew<IssmDouble>(this->num_datatimes);
	this->passedflags = xNew<bool>(this->num_datatimes);
	xMemCpy<IssmDouble>(this->datatimes,in_datatimes,this->num_datatimes);

	/*initialize passedtimes to false*/
	for(int i=0;i<this->num_datatimes;i++) this->passedflags[i]= false;
	this->J = 0;
}
/*}}}*/
Cfsurfacelogveltransient::~Cfsurfacelogveltransient(){/*{{{*/
	if(this->name) xDelete(this->name);
	if(this->datatimes) xDelete(this->datatimes);
	if(this->passedflags) xDelete(this->passedflags);
}
/*}}}*/

/*Object virtual function resolutoin: */
Object* Cfsurfacelogveltransient::copy() {/*{{{*/
	Cfsurfacelogveltransient* output = new Cfsurfacelogveltransient(this->name,this->definitionenum, this->num_datatimes, this->datatimes,this->passedflags, this->J);
	return (Object*)output;
}
/*}}}*/
void Cfsurfacelogveltransient::DeepEcho(void){/*{{{*/
	this->Echo();
}
/*}}}*/
void Cfsurfacelogveltransient::Echo(void){/*{{{*/
	_printf_(" Cfsurfacelogveltransient: " << name << " " << this->definitionenum << "\n");
	_error_("not implemented yet");
}
/*}}}*/
int Cfsurfacelogveltransient::Id(void){/*{{{*/
	return -1;
}
/*}}}*/
void Cfsurfacelogveltransient::Marshall(MarshallHandle* marshallhandle){/*{{{*/

	int object_enum=CfsurfacelogveltransientEnum;
	marshallhandle->call(object_enum);

	marshallhandle->call(this->definitionenum);
	marshallhandle->call(this->name);
	marshallhandle->call(this->num_datatimes);
	marshallhandle->call(this->datatimes,this->num_datatimes);
	marshallhandle->call(this->passedflags,this->num_datatimes);
	marshallhandle->call(this->J);
} 
/*}}}*/
int Cfsurfacelogveltransient::ObjectEnum(void){/*{{{*/
	return CfsurfacelogveltransientEnum;
}
/*}}}*/

/*Definition virtual function resolutoin: */
int Cfsurfacelogveltransient::DefinitionEnum(){/*{{{*/
	return this->definitionenum;
}
/*}}}*/
char* Cfsurfacelogveltransient::Name(){/*{{{*/
	char* name2=xNew<char>(strlen(this->name)+1);
	xMemCpy(name2,this->name,strlen(this->name)+1);

	return name2;
}
/*}}}*/
IssmDouble Cfsurfacelogveltransient::Response(FemModel* femmodel){/*{{{*/

	/*recover model time parameters: */
	IssmDouble time;
	femmodel->parameters->FindParam(&time,TimeEnum);

	/*Find closest datatime that is less than time*/
	int pos=-1;
	for(int i=0;i<this->num_datatimes;i++){
		if(this->datatimes[i]<=time){
			pos = i;
		}
		else{
			break;
		}
	}

	/*if pos=-1, time is earlier than the first data observation in this dataset*/
	if(pos==-1){
		_assert_(this->J==0.);
		return 0.;
	}

	/*Check that we have not yet calculated this cost function*/
	if(this->passedflags[pos]){
		return this->J;
	}

	/*Calculate cost function for this time slice*/
	IssmDouble J_part=0.;
	for(Object* & object : femmodel->elements->objects){
		Element* element=xDynamicCast<Element*>(object);
		J_part+=this->Cfsurfacelogveltransient_Calculation(element,definitionenum);
	}

	/*Sum across partition*/
	IssmDouble J_sum;
	ISSM_MPI_Allreduce((void*)&J_part,(void*)&J_sum,1,ISSM_MPI_DOUBLE,ISSM_MPI_SUM,IssmComm::GetComm());
	ISSM_MPI_Bcast(&J_sum,1,ISSM_MPI_DOUBLE,0,IssmComm::GetComm());

	/*Record this cost function so that we do not recalculate it later*/
	this->passedflags[pos]= true;
	this->J += J_sum;

	/*Return full cost function this far*/
	return this->J;
}/*}}}*/
IssmDouble Cfsurfacelogveltransient::Cfsurfacelogveltransient_Calculation(Element* element, int definitionenum){/*{{{*/

	int        domaintype,numcomponents;
	IssmDouble Jelem=0.;
	IssmDouble epsvel=2.220446049250313e-16;
	IssmDouble meanvel=3.170979198376458e-05; /*1000 m/yr*/
	IssmDouble velocity_mag,obs_velocity_mag;
	IssmDouble misfit,Jdet;
	IssmDouble vx,vy,vxobs,vyobs,weight;
	IssmDouble* xyz_list = NULL;

	/*Get basal element*/
	if(!element->IsOnSurface()) return 0.;

	/*If on water, return 0: */
	if(!element->IsIceInElement()) return 0.;

	/*Get problem dimension*/
	element->FindParam(&domaintype,DomainTypeEnum);
	switch(domaintype){
		case Domain2DverticalEnum:   numcomponents   = 1; break;
		case Domain3DEnum:           numcomponents   = 2; break;
		case Domain2DhorizontalEnum: numcomponents   = 2; break;
		default: _error_("not supported yet");
	}

	/*Spawn surface element*/
	Element* topelement = element->SpawnTopElement();

	/* Get node coordinates*/
	topelement->GetVerticesCoordinates(&xyz_list);

	/*Get model values*/
	Input *vx_input = topelement->GetInput(VxEnum); _assert_(vx_input);
	Input *vy_input = NULL;
	if(numcomponents==2){
		vy_input = topelement->GetInput(VyEnum); _assert_(vy_input);
	}

	/*Retrieve all inputs we will be needing: */
	DatasetInput *datasetinput = topelement->GetDatasetInput(definitionenum); _assert_(datasetinput);

	/* Start  looping on the number of gaussian points: */
	Gauss* gauss=topelement->NewGauss(2);
	while(gauss->next()){

		/* Get Jacobian determinant: */
		topelement->JacobianDeterminant(&Jdet,xyz_list,gauss);

		/*Get all parameters at gaussian point*/
		datasetinput->GetInputValue(&weight,gauss,WeightsSurfaceObservationEnum);
		vx_input->GetInputValue(&vx,gauss);
		datasetinput->GetInputValue(&vxobs,gauss,VxObsEnum);
		if(numcomponents==2){
			vy_input->GetInputValue(&vy,gauss);
			datasetinput->GetInputValue(&vyobs,gauss,VyObsEnum);
		}

		/*Compute SurfaceLogVelMisfit:
		 *        *                 [        vel + eps     ] 2
		 *               * J = 4 \bar{v}^2 | log ( -----------  ) |
		 *                          [       vel   + eps    ]
		 *                                      obs
		 */
		if(numcomponents==1){
			velocity_mag    =fabs(vx)+epsvel;
			obs_velocity_mag=fabs(vxobs)+epsvel;
		}
		else{
			velocity_mag    =sqrt(vx*vx+vy*vy)+epsvel;
			obs_velocity_mag=sqrt(vxobs*vxobs+vyobs*vyobs)+epsvel;
		}

		misfit=4*pow(meanvel,2)*pow(log(velocity_mag/obs_velocity_mag),2);

		/*Add to cost function*/
		Jelem+=misfit*weight*Jdet*gauss->weight;

	}

	/*clean up and Return: */
	if(topelement->IsSpawnedElement()){topelement->DeleteMaterials(); delete topelement;};
	xDelete<IssmDouble>(xyz_list);
	delete gauss;
	return Jelem;
}/*}}}*/
