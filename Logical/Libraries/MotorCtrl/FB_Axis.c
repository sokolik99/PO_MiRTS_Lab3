
#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif
	#include "MotorCtrl.h"
#ifdef __cplusplus
	};
#endif
#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void FB_Axis(struct FB_Axis* inst)
{
	inst->counter_buffer += inst->counter - inst->last_counter;
	inst->last_counter = inst->counter;
	Axis_X_EncIf.iActPos = inst->counter;
	if((Axis_X_EncIf.iLifeCnt) % COUNTER_BUFFER_CYCLES == 0){
		inst->speed = (inst->counter_buffer) / (0.002 * COUNTER_BUFFER_CYCLES); // cycle = 2 ms
		inst->counter_buffer = 0;
	}
	Axis_X_DiDoIf.iNegHwEnd = inst->endswitch_a_reached;
	Axis_X_DiDoIf.iPosHwEnd = inst->endswitch_b_reached;
	inst->desired_speed = Axis_X_DrvIf.oSetPos * UNITS_PER_ROTATION / 32767;
	inst->regulator.e = inst->desired_speed - inst->speed;
	FB_Regulator(&(inst->regulator));
	inst->pwm_value = (inst->regulator.u) / (inst->regulator.max_abs_value) * (inst->pwm_period);
}

