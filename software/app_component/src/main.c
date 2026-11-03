#include <stdio.h>
#include <xgpio_l.h>
#include <xstatus.h>

#include "xparameters.h"
#include "xgpio.h"
#include "sleep.h"
#include "xinterrupt_wrap.h"
#include "xtmrctr.h"

#define BTN_CH 1
#define LED_CH 2

#define TMR_CNT_0 0

#define BTN_INT XGPIO_IR_CH1_MASK

#define BTN_INC 0b0001
#define BTN_DEC 0b0010

volatile unsigned int data_val;
volatile unsigned int data_interval;
volatile unsigned int timer_interrupt_count;

void ISR_AxiGpioHandler(void *CallbackRef) {
    XGpio * AxiGpioInstPtr = (XGpio*) CallbackRef;
    unsigned int btn_val;

    XGpio_InterruptDisable(AxiGpioInstPtr, BTN_INT);

    btn_val = XGpio_DiscreteRead(AxiGpioInstPtr, BTN_CH);

    if (btn_val & BTN_INC) {
        if (data_interval <5) data_interval++;
    }
    else if (btn_val & BTN_DEC) {
        if (data_interval > 1) data_interval--;
    }

    printf("Interval:%d \n", data_interval);

    XGpio_InterruptClear(AxiGpioInstPtr, BTN_INT);
    XGpio_InterruptEnable(AxiGpioInstPtr, BTN_INT);


}

void ISR_AxiTimerHandler(void *CallbackRef, unsigned char TmrCtrNumber){
    XTmrCtr* AxiTimerPtr = (XTmrCtr*) CallbackRef;

    if(XTmrCtr_IsExpired(AxiTimerPtr, TMR_CNT_0)){
        if(timer_interrupt_count >= data_interval-1){
            timer_interrupt_count = 0;
            data_val = data_val + 1;
        }
        else {
            timer_interrupt_count++;
        }
    }

    printf("Timer cnt:%d \n", data_val);

}

int main() {
    
    XGpio AxiGpioInst;
    XGpio_Config *AxiGpioConfig;
    XTmrCtr AxiTimerInst;

    int status;

    AxiGpioConfig = XGpio_LookupConfig(XPAR_AXI_GPIO_0_BASEADDR);

    status = XGpio_Initialize(&AxiGpioInst, XPAR_AXI_GPIO_0_BASEADDR);
    if (status != XST_SUCCESS) return XST_FAILURE;

    XGpio_SetDataDirection(&AxiGpioInst, BTN_CH, 0xF);
    XGpio_SetDataDirection(&AxiGpioInst, LED_CH, 0x0);

    status = XSetupInterruptSystem(&AxiGpioInst, 
                                ISR_AxiGpioHandler,
                                AxiGpioConfig->IntrId,
                                AxiGpioConfig->IntrParent,
                                XINTERRUPT_DEFAULT_PRIORITY);

    XGpio_InterruptGlobalEnable(&AxiGpioInst);
    XGpio_InterruptEnable(&AxiGpioInst, BTN_INT);

    status = XTmrCtr_Initialize(&AxiTimerInst, XPAR_AXI_TIMER_0_BASEADDR);
    if (status != XST_SUCCESS) return XST_FAILURE;

    status = XSetupInterruptSystem(&AxiTimerInst, 
                            (XInterruptHandler)XTmrCtr_InterruptHandler,
                            AxiTimerInst.Config.IntrId,
                            AxiTimerInst.Config.IntrParent,
                            XINTERRUPT_DEFAULT_PRIORITY);
    
    if (status != XST_SUCCESS) return XST_FAILURE;

    XTmrCtr_SetHandler(&AxiTimerInst, ISR_AxiTimerHandler, &AxiTimerInst);

    XTmrCtr_SetResetValue(&AxiTimerInst, TMR_CNT_0, XPAR_AXI_TIMER_0_CLOCK_FREQUENCY);
    XTmrCtr_SetOptions(&AxiTimerInst, TMR_CNT_0, XTC_INT_MODE_OPTION | XTC_DOWN_COUNT_OPTION | XTC_AUTO_RELOAD_OPTION);
    
    printf("Timer initalized\n\r");
    // XGpio_DiscreteWrite(&AxiGpioInst, LED_CH, 0x00);
    data_val = 0;
    timer_interrupt_count = 0;
    data_interval = 1;

    XTmrCtr_Start(&AxiTimerInst, TMR_CNT_0);

    while (1) {
        XGpio_DiscreteWrite(&AxiGpioInst, LED_CH, data_val);
    }

    return 0;
}