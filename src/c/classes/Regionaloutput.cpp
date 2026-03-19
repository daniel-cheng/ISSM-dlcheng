/*!\file Regionaloutput.cpp
 * \brief: implementation for the Regionaloutput object
 */

/*Include files: {{{*/
#ifdef HAVE_CONFIG_H
   #include <config.h>
#else
#error "Cannot compile with HAVE_CONFIG_H symbol! run configure first!"
#endif

/*Headers:*/
#include <execinfo.h>   // for backtrace()
#include <unistd.h>     // for STDERR_FILENO
#include <iostream>
#include <cstdlib>

#include "./classes.h"
#include "./Definition.h"
#include "./Elements/Element.h"
#include "./Elements/Elements.h"
#include "./FemModel.h"
#include "../classes/Params/Parameters.h"

/*}}}*/

Regionaloutput::Regionaloutput(){/*{{{*/

	this->definitionenum = -1;
	this->outputname = NULL;
	this->name = NULL;
	this->mask = NULL;
	this->M = 0.;

}
/*}}}*/
Regionaloutput::Regionaloutput(char* in_name, int in_definitionenum, char* in_outputname, IssmDouble* maskin, int Min){ /*{{{*/

	this->definitionenum=in_definitionenum;
	this->outputname = xNew<char>(strlen(in_outputname)+1);
	xMemCpy<char>(this->outputname,in_outputname,strlen(in_outputname)+1);
	this->name = xNew<char>(strlen(in_name)+1);
	xMemCpy<char>(this->name,in_name,strlen(in_name)+1);

	this->mask   = xNew<IssmDouble>(Min);
	xMemCpy<IssmDouble>(this->mask, maskin, Min);

	this->M=Min;

}
/*}}}*/
Regionaloutput::~Regionaloutput(){/*{{{*/
	//this->Echo();
	//void* addrlist[64 + 1];

    //// Retrieve current stack addresses
    //int addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));

    //if (addrlen == 0) {
    //    std::cerr << "  <empty, possibly corrupt stack>\n";
    //    return;
    //}

    //// Print out all the frames to stderr
    //char** symbollist = backtrace_symbols(addrlist, addrlen);

    //for (int i = 0; i < addrlen; i++) {
    //    std::cerr << symbollist[i] << std::endl;
    //}

    //free(symbollist);
	if(this->name) {
		xDelete(this->name);
	}
	if(this->outputname){
		xDelete(this->outputname);
	}
	if(this->mask) {
		//_printf_("    mask with size " + std::to_string(this->M) + " deleting...\n");
		xDelete(this->mask);
		//this->M = 0;
		//_printf_("    mask deleted!\n");
	}
}
/*}}}*/

/*Object virtual function resolutoin: */
Object* Regionaloutput::copy() {/*{{{*/
	Regionaloutput* mf = new Regionaloutput(this->name,this->definitionenum,this->outputname,this->mask,this->M);
	return (Object*) mf;
}
/*}}}*/
void Regionaloutput::DeepEcho(void){/*{{{*/
	this->Echo();
}
/*}}}*/
void Regionaloutput::Echo(void){/*{{{*/
	_printf_(" Regionaloutput: " << this->name << " " << this->definitionenum << "\n");
	_printf_("    outputname enum: " << this->outputname << "Enum\n");
	_printf_("    mask: " << this->mask << "\n");
	_printf_("    M: " << this->M << "\n");
}
/*}}}*/
int Regionaloutput::Id(void){/*{{{*/
	return -1;
}
/*}}}*/
void Regionaloutput::Marshall(MarshallHandle* marshallhandle){/*{{{*/
	//_error_("not implemented yet!");
	int object_enum=RegionaloutputEnum;
	marshallhandle->call(object_enum);

	marshallhandle->call(this->definitionenum);
	marshallhandle->call(this->outputname);
	marshallhandle->call(this->name);
	marshallhandle->call(this->mask);
	marshallhandle->call(this->M);
}
/*}}}*/
int Regionaloutput::ObjectEnum(void){/*{{{*/
	return RegionaloutputEnum;
}
/*}}}*/

/*Definition virtual function resolutoin: */
int Regionaloutput::DefinitionEnum(){/*{{{*/

	return this->definitionenum;
}
/*}}}*/
char* Regionaloutput::Name(){/*{{{*/

	char* name2=xNew<char>(strlen(this->name)+1);
	xMemCpy(name2,this->name,strlen(this->name)+1);

	return name2;
}
/*}}}*/
IssmDouble Regionaloutput::Response(FemModel* femmodel){/*{{{*/

	IssmDouble val_t=0.;
	IssmDouble all_val_t=0.;
	int outputenum = StringToEnumx(this->outputname);

	for(Object* & object : femmodel->elements->objects){
		Element* element=xDynamicCast<Element*>(object);
		switch(outputenum){
			case GroundedAreaEnum:
				val_t+=element->GroundedArea(this->mask,false);
				break;
			case GroundedAreaScaledEnum:
				val_t+=element->GroundedArea(this->mask,true);
				break;
			case FloatingAreaEnum:
				val_t+=element->FloatingArea(this->mask,false);
				break;
			case FloatingAreaScaledEnum:
				val_t+=element->FloatingArea(this->mask,true);
				break;
			case IceMassEnum:
				val_t+=element->IceMass(this->mask,false);
				break;
			case IceMassScaledEnum:
				val_t+=element->IceMass(this->mask,true);
				break;
			case IceVolumeEnum:
				val_t+=element->IceVolume(this->mask,false);
				break;
			case IceVolumeScaledEnum:
				val_t+=element->IceVolume(this->mask,true);
				break;
			case IceVolumeAboveFloatationEnum:
				val_t+=element->IceVolumeAboveFloatation(this->mask,false);
				break;
			case IceVolumeAboveFloatationScaledEnum:
				val_t+=element->IceVolumeAboveFloatation(this->mask,true);
				break;
			case TotalFloatingBmbEnum:
				val_t+=element->TotalFloatingBmb(this->mask,false);
				break;
			case TotalFloatingBmbScaledEnum:
				val_t+=element->TotalFloatingBmb(this->mask,true);
				break;
			case TotalGroundedBmbEnum:
				val_t+=element->TotalGroundedBmb(this->mask,false);
				break;
			case TotalGroundedBmbScaledEnum:
				val_t+=element->TotalGroundedBmb(this->mask,true);
				break;
			case TotalSmbEnum:
				val_t+=element->TotalSmb(this->mask,false);
				break;
			case TotalSmbScaledEnum:
				val_t+=element->TotalSmb(this->mask,true);
				break;
			case TotalSmbMeltEnum:
				val_t+=element->TotalSmbMelt(this->mask,true);
				break;
			case TotalSmbRefreezeEnum:
				val_t+=element->TotalSmbRefreeze(this->mask,true);
				break;
			default:
				_error_("Regional output type " << this->outputname << " not supported yet!");
		}
	}

	ISSM_MPI_Allreduce ( (void*)&val_t,(void*)&all_val_t,1,ISSM_MPI_DOUBLE,ISSM_MPI_SUM,IssmComm::GetComm());
	val_t=all_val_t;

	return val_t;
}
/*}}}*/
