/*!\file Massconaxpby.h
 * \brief: header file for Massconaxpby object
 */

#ifndef _MASSCON_AXPBY_H_
#define _MASSCON_AXPBY_H_

/*Headers:*/
/*{{{*/
#ifdef HAVE_CONFIG_H
   #include <config.h>
#else
#error "Cannot compile with HAVE_CONFIG_H symbol! run configure first!"
#endif
#include "./Definition.h"
#include "./ExternalResults/ExternalResult.h"
#include "./ExternalResults/Results.h"
#include "./ExternalResults/GenericExternalResult.h"
#include "../datastructures/datastructures.h"
#include "./Elements/Element.h"
#include "./Elements/Elements.h"
#include "./FemModel.h"
#include "../classes/Params/Parameters.h"
#include "../modules/ModelProcessorx/ModelProcessorx.h"
//Must update if OutputDefinitionsResponsex changes signature. This is done because of circular dependency
//(and the fact that Massconaxpby::Response is defined in this header file and not in a cpp file)
//#include "../modules/OutputDefinitionsResponsex/OutputDefinitionsResponsex.h"
int OutputDefinitionsResponsex(IssmDouble* presponse, FemModel* femmodel,const char* output_string);
int OutputDefinitionsResponsex(IssmDouble* presponse, FemModel* femmodel,int output_enum);
/*}}}*/
class Massconaxpby: public Object, public Definition{

	public: 

		int         definitionenum;
		char*       name;
		char*       namex;
		char*       namey;
		IssmDouble  alpha;
		IssmDouble  beta;

		/*Massconaxpby constructors, destructors :*/
		Massconaxpby(){/*{{{*/

			this->definitionenum = -1;
			this->name = NULL;
			this->namex = NULL;
			this->namey = NULL;
			this->alpha=UNDEF;
			this->beta=UNDEF;

		}
		/*}}}*/
		Massconaxpby(char* in_name,int in_definitionenum, char* in_namex, char* in_namey, IssmDouble in_alpha,IssmDouble in_beta){ /*{{{*/

			this->definitionenum = in_definitionenum;
			this->name   = xNew<char>(strlen(in_name)+1);
			xMemCpy<char>(this->name,in_name,strlen(in_name)+1);

			this->namex   = xNew<char>(strlen(in_namex)+1);
			xMemCpy<char>(this->namex,in_namex,strlen(in_namex)+1);

			this->namey   = xNew<char>(strlen(in_namey)+1);
			xMemCpy<char>(this->namey,in_namey,strlen(in_namey)+1);

			this->alpha=in_alpha;
			this->beta=in_beta;

		}
		/*}}}*/
		~Massconaxpby(){/*{{{*/
			if(this->name)xDelete(this->name); 
			if(this->namex)xDelete(this->namex); 
			if(this->namey)xDelete(this->namey); 
		}
		/*}}}*/
		/*Object virtual function resolutoin: */
		Object* copy() {/*{{{*/
			Massconaxpby* mf = new Massconaxpby(this->name,this->definitionenum,this->namex,this->namey, this->alpha, this->beta);
			return (Object*) mf;
		}
		/*}}}*/
		void DeepEcho(void){/*{{{*/
			this->Echo();
		}
		/*}}}*/
		void Echo(void){/*{{{*/
			_printf_(" Massconaxpby: " << this->name << " " << this->definitionenum << "\n");
			_printf_("    namex: " << this->namex << "\n");
			_printf_("    namey: " << this->namey << "\n");
			_printf_("    alpha: " << this->alpha << "\n");
			_printf_("    beta: " << this->beta << "\n");
		}
		/*}}}*/
		int Id(void){/*{{{*/
			return -1;
		}
		/*}}}*/
		void Marshall(MarshallHandle* marshallhandle){/*{{{*/
			/*ok, marshall operations: */
			int object_enum=MassconaxpbyEnum;
			marshallhandle->call(object_enum);

			marshallhandle->call(this->definitionenum);
			marshallhandle->call(this->name);
			marshallhandle->call(this->namex);
			marshallhandle->call(this->namey);
			marshallhandle->call(this->alpha);
			marshallhandle->call(this->beta);
			//_error_("not implemented yet!"); 
		} 
		/*}}}*/
		int ObjectEnum(void){/*{{{*/
			return MassconaxpbyEnum;
		}
		/*}}}*/
		/*Definition virtual function resolutoin: */
		int DefinitionEnum(){/*{{{*/

			return this->definitionenum;
		}
		/*}}}*/
		char* Name(){/*{{{*/

			char* name2=xNew<char>(strlen(this->name)+1);
			xMemCpy(name2,this->name,strlen(this->name)+1);

			return name2;
		}
		/*}}}*/
		IssmDouble Response(FemModel* femmodel){/*{{{*/
			//int ierr;
			//IssmDouble xresponse,yresponse;

			///*Get response from both masscons: */
			//ierr=OutputDefinitionsResponsex(&xresponse,femmodel,this->namex);
			//if(ierr) _error_("not found");
			//ierr=OutputDefinitionsResponsex(&yresponse,femmodel,this->namey);
			//if(ierr) _error_("not found");

			//return this->alpha*xresponse+this->beta*yresponse;

			IssmDouble time,starttime,finaltime,dt,yts;
			int step;
			int ierr;
			//femmodel->parameters->FindParam(&starttime,TimesteppingStartTimeEnum);
			//femmodel->parameters->FindParam(&finaltime,TimesteppingFinalTimeEnum);
			//femmodel->parameters->FindParam(&time,TimeEnum);
			femmodel->parameters->FindParam(&step,StepEnum);
			//femmodel->parameters->FindParam(&yts,ConstantsYtsEnum);

			IssmDouble xresponse,yresponse,axpbyresponse;
			ExternalResult* xresult;
			ExternalResult* yresult;
			ExternalResult* result;
			GenericExternalResult<IssmDouble>* gxresult;
			GenericExternalResult<IssmDouble>* gyresult;

			xresult = femmodel->results->FindResult(this->namex,step);
			yresult = femmodel->results->FindResult(this->namey,step);
			if (xresult) {
				gxresult= xDynamicCast<GenericExternalResult<IssmDouble>*>(xresult);
				xresponse = xresult->GetValue();
			}
			else {
				ierr = OutputDefinitionsResponsex(&xresponse, femmodel, this->namex);
				if(ierr) _error_("Could not find response "<<this->namex);
				femmodel->results->AddResult(new GenericExternalResult<IssmDouble>(femmodel->results->Size()+1,this->namex,xresponse,step,time));
			}
			if (yresult) {
				gyresult = xDynamicCast<GenericExternalResult<IssmDouble>*>(yresult);
				yresponse = yresult->GetValue();
			}
			else {
				ierr = OutputDefinitionsResponsex(&yresponse, femmodel, this->namey);
				if(ierr) _error_("Could not find response "<<this->namey);
				femmodel->results->AddResult(new GenericExternalResult<IssmDouble>(femmodel->results->Size()+1,this->namey,yresponse,step,time));
			}
			return this->alpha*xresponse+this->beta*yresponse;

			///*Get response from both masscons: */
			//if (time>=finaltime) {
			//	/*parameters: */
			//	bool       iscontrol,isautodiff;
			//	int        timestepping;
			//	char     **requested_outputs = NULL;
			//	IssmDouble* element_accum_values = xNewZeroInit<IssmDouble>(3);

			//	/*intermediary: */
			//	/*first, figure out if there was a check point, if so, do a reset of the FemModel* femmodel structure. */
			//	/*then recover parameters common to all solutions*/
			//	femmodel->parameters->FindParam(&time,TimeEnum);
			//	femmodel->parameters->FindParam(&timestepping,TimesteppingTypeEnum);

			//	/*initialize:  */
			//	femmodel->parameters->SetParam(starttime,TimeEnum);
			//	femmodel->parameters->SetParam(0,StepEnum);
			//	step = 0;
			//	femmodel->parameters->FindParam(&time,TimeEnum);
			//	while(time < finaltime){ //make sure we run up to finaltime.
			//		/*Time Increment*/
			//		switch(timestepping){
			//			case AdaptiveTimesteppingEnum:
			//				femmodel->TimeAdaptx(&dt);
			//				if(time+dt>finaltime) dt=finaltime-time;
			//				femmodel->parameters->SetParam(dt,TimesteppingTimeStepEnum);
			//				break;
			//			case FixedTimesteppingEnum:
			//				femmodel->parameters->FindParam(&dt,TimesteppingTimeStepEnum);
			//				break;
			//			default:
			//				_error_("Time stepping \""<<EnumToStringx(timestepping)<<"\" not supported yet");
			//		}
			//		step += 1;
			//		time += dt;
			//		femmodel->parameters->SetParam(time,TimeEnum);
			//		femmodel->parameters->SetParam(step,StepEnum);

			//		IssmDouble xresponse_value, yresponse_value;

			//		/*Get response from both masscons: */
			//		xresult = femmodel->results->FindResult(this->namex,step);
			//		yresult = femmodel->results->FindResult(this->namey,step);
			//		if (xresult) {
			//			xresponse = xresult->GetValue();
			//		} else {
			//			_printf0_("   Defaulting: Massconaxpby.h:201 name: " << this->name << " xresponse: " << xresponse << " xresponse->name: " << this->namex << "\n");
			//		}
			//		if (yresult) {
			//			yresponse = yresult->GetValue();
			//		} else {
			//			_printf0_("   Defaulting: Massconaxpby.h:206 name: " << this->name << " yresponse: " << yresponse << " yresponse->name: " << this->namey << "\n");
			//		}
			//		axpbyresponse = this->alpha*xresponse+this->beta*yresponse;
			//		if (time < finaltime){ //make sure we run up to finaltime.
			//			result = femmodel->results->FindResult(this->name,step);
			//			result->SetValue(axpbyresponse);
			//		} else {
			//			return axpbyresponse;
			//		}
			//	}
			//}
			//return this->alpha*xresponse+this->beta*yresponse;
		} /*}}}*/
};

#endif  /* _MASSCON_H_ */
