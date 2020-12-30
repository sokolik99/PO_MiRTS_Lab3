
#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif
	#include "MotorCtrl.h"
#ifdef __cplusplus
	};
#endif
/* TODO: Add your comment here */
void FB_Motor(struct FB_Motor* inst)
{
	/*TODO: Add your code here*/
	REAL a = inst->u / inst->ke - inst->w;
	
	inst->integrator1.in = a;
	FB_Integrator(&inst->integrator1);
	inst->w = inst->integrator1.out;
	
	inst->integrator2.in = inst->w;
	FB_Integrator(&inst->integrator2);
	inst->phi = inst->integrator2.out;
}
