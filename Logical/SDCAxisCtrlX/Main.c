
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _INIT ProgramInit(void)
{
	Axis_X_HW.EncIf1_Typ = ncSDC_ENC16;
	Axis_X_HW.DiDoIf_Typ = ncSDC_DIDO;
	Axis_X_HW.DrvIf_Typ = ncSDC_DRVSERVO16;
	strcpy(Axis_X_HW.EncIf1_Name, "Axis_X_EncIf");
	strcpy(Axis_X_HW.DrvIf_Name, "Axis_X_DrvIf");
	strcpy(Axis_X_HW.DiDoIf_Name, "Axis_X_DiDoIf");

	Axis_X_ModuleOk = 1;
	Axis_X_EncIf.iEncOK = 1;
	Axis_X_DrvIf.iDrvOK = 1;
	Axis_X_DrvIf.iStatusEnable = 1;
	Axis_X_DiDoIf.iDriveReady = 1;
	
	axis_X.counter = axis_X.last_counter = 0;
	axis_X.pwm_period = pwm_period = 0.05;
	axis_X.regulator.dt = 0.002;
	axis_X.regulator.integrator.dt = 0.002;
	axis_X.regulator.k_i = 0.0072;
	axis_X.regulator.k_p = 0.0072;
	axis_X.regulator.max_abs_value = 24.0;
	
	motor_X.dt = 0.01;
	motor_X.ke = 1;
	motor_X.Tm = 0.1;
	motor_X.u = 0;
	motor_X.phi = 0;
	motor_X.integrator1.dt = 0.002 / motor_X.Tm;
	motor_X.integrator2.dt = 0.002;
}

void _CYCLIC ProgramCyclic(void)
{
	Axis_X_EncIf.iLifeCnt++;
	Axis_X_EncIf.iActTime = (INT)AsIOTimeCyclicStart();
	Axis_X_DrvIf.iLifeCnt++;
	Axis_X_DrvIf.oSetPos = Axis_X.monitor.v;
	Axis_X_DiDoIf.iLifeCntDriveEnable++;
	Axis_X_DiDoIf.iLifeCntDriveReady++;
	Axis_X_DiDoIf.iLifeCntNegHwEnd++;
	Axis_X_DiDoIf.iLifeCntPosHwEnd++;
	Axis_X_DiDoIf.iLifeCntReference++;
	FB_Axis(&axis_X);
	if(Axis_X_DrvIf.oSetPos != 0){
		motor_X.u = axis_X.pwm_value / axis_X.pwm_period * MAX_UNITS_PER_SECOND;
		FB_Motor(&motor_X);
		axis_X.counter = motor_X.phi;
		if (axis_X.counter <= -3000)
			axis_X.endswitch_a_reached = 1;
		else
			axis_X.endswitch_a_reached = 0;
		if (axis_X.counter >= 3000)
			axis_X.endswitch_b_reached = 1;
		else
			axis_X.endswitch_b_reached = 0;
	}
}

void _EXIT ProgramExit(void)
{

}


