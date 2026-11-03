#include "xgpio.h"

XGpio_Config XGpio_ConfigTable[] __attribute__ ((section (".drvcfg_sec"))) = {

	{
		"xlnx,axi-gpio-2.0", /* compatible */
		0x41200000, /* reg */
		0x1, /* xlnx,interrupt-present */
		0x1, /* xlnx,is-dual */
		0x401e, /* interrupts */
		0xf8f01000, /* interrupt-parent */
		0x4 /* xlnx,gpio-width */
	},
	 {
		 NULL
	}
};