/*!\file Cfsurfacelogveltransient.h
 * \brief: header file for Cfsurfacelogveltransient object
 */

#ifndef _CFSURFACELOGVELTRANSIENT_H_
#define _CFSURFACELOGVELTRANSIENT_H_

/*Headers:*/
#include "./Definition.h"
class FemModel;

class Cfsurfacelogveltransient: public Object, public Definition{

	public: 

		int         definitionenum;
		char       *name;
		int         num_datatimes;
		IssmDouble *datatimes;
		bool       *passedflags;
		IssmDouble  J;

		/*Cfsurfacelogveltransient constructors, destructors :*/
		Cfsurfacelogveltransient();
		Cfsurfacelogveltransient(char* in_name, int in_definitionenum, int num_datatimes, IssmDouble* in_datatime);
		Cfsurfacelogveltransient(char* in_name, int in_definitionenum, int num_datatimes, IssmDouble* in_datatime, bool* in_timepassedflag, IssmDouble in_J);
		~Cfsurfacelogveltransient();

		/*Object virtual function resolutoin: */
		Object *copy();
		void    DeepEcho(void);
		void    Echo(void);
		int     Id(void);
		void    Marshall(MarshallHandle  *marshallhandle);
		int     ObjectEnum(void);

		/*Definition virtual function resolutoin: */
		int         DefinitionEnum();
		char       *Name();
		IssmDouble  Response(FemModel *femmodel);
		IssmDouble  Cfsurfacelogveltransient_Calculation(Element  *element, int definitionenum);
};
#endif  /* _CFSURFACELOGVELTRANSIENT_H_ */
