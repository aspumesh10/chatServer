#ifndef __SERVERINFO_H_INCLUDED__
#define __SERVERINFO_H_INCLUDED__

enum /*class*/ clientState {

	idle=0,
    inProgress,
	readable,
    writable,
    inRegister,
    getName

};
#endif
