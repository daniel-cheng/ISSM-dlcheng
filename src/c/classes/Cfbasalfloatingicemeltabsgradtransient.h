/*!\file Cfbasalfloatingicemeltabsgradtransient.h
 * \brief: header file for Cfbasalfloatingicemeltabsgradtransient object
 */

#ifndef _CFBASALFLOATINGICEMELTABSGRADTRANSIENT_H_
#define _CFBASALFLOATINGICEMELTABSGRADTRANSIENT_H_

/*Headers:*/
#include "./Definition.h"
class FemModel;

class Cfbasalfloatingicemeltabsgradtransient: public Object, public Definition{

	public: 

		int         definitionenum;
		char       *name;
		int         num_datatimes;
		IssmDouble *datatimes;
		bool       *passedflags;
		IssmDouble  J;

		/*Cfbasalfloatingicemeltabsgradtransient constructors, destructors :*/
		Cfbasalfloatingicemeltabsgradtransient();
		Cfbasalfloatingicemeltabsgradtransient(char* in_name, int in_definitionenum, int num_datatimes, IssmDouble* in_datatime);
		Cfbasalfloatingicemeltabsgradtransient(char* in_name, int in_definitionenum, int num_datatimes, IssmDouble* in_datatime, bool* in_timepassedflag, IssmDouble in_J);
		~Cfbasalfloatingicemeltabsgradtransient();

		/*Object virtual function resolutoin: */
		Object* copy();
		void DeepEcho(void);
		void Echo(void);
		int Id(void);
		void Marshall(MarshallHandle* marshallhandle);
		int ObjectEnum(void);

		/*Definition virtual function resolutoin: */
		int DefinitionEnum();
		char* Name();
		IssmDouble Response(FemModel* femmodel);
		IssmDouble Cfbasalfloatingicemeltabsgradtransient_Calculation(Element* element);
};
#endif  /* _CFBASALFLOATINGICEMELTABSGRADTRANSIENT_H_ */
