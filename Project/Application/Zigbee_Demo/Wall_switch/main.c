/** @file
 *
 * @brief
 *
 */


/**************************************************************************************************
 *    INCLUDES
 *************************************************************************************************/
#include "cm3_mcu.h"
#include "project_config.h"

#include "FreeRTOS.h"
#include "task.h"

#include "task_pci.h"

#include "zigbee_app.h"
#include "zigbee_lib_api.h"

pwm_seq_para_head_t pwm_para_config[1];
/**************************************************************************************************
 *    MACROS
 *************************************************************************************************/

/**************************************************************************************************
 *    CONSTANTS AND DEFINES
 *************************************************************************************************/
#define PWM_PWM0_COUNTER_MODE   PWM_COUNTER_MODE_UP         /*UP is up counter Mode ;UD is up and down Mode*/
#define PWM_PWM0_SEQ_ORDER      PWM_SEQ_ORDER_R             /*Rseq or Tseq Selection or both*/
#define PWM_PWM0_ELEMENT_NUM    1                           /*genrator Pulse number*/
#define PWM_PWM0_REPEAT_NUM     0                           /*Repeat Pulse number*/
#define PWM_PWM0_DLY_NUM        0                           /*Pulse delay number*/
#define PWM_PWM0_CNT_END_VAL    10000                       /*Count end Value*/
#define PWM_PWM0_CLK_DIV        PWM_CLK_DIV_4               /*PWM Input Clock Div*/
#define PWM_PWM0_TRIG_SRC       PWM_TRIGGER_SRC_SELF        /*PWM Trigger Source by self or PWM1~PWM4*/
#define PWM_PWM0_SEQ_MODE       PWM_SEQ_MODE_CONTINUOUS     /*Continuous and Noncontinuous mode*/
#define PWM_PWM0_PLAY_CNT       0                           /*0:is infinite*/
#define PWM_PWM0_SEQ_NUM        PWM_SEQ_NUM_1               /*use rdma single simple or two simple*/
#define PWM_PWM0_DMA_SML_FMT    PWM_DMA_SMP_FMT_1           /*Pwm DMA Simple Format 0 or 1*/

/**************************************************************************************************
 *    GLOBAL VARIABLES
 *************************************************************************************************/
uint32_t pwm_rdma0_addr_temp[1];
uint32_t pwm_rdma1_addr_temp[1];
uint16_t pwm_count_end_val[1] = {PWM_PWM0_CNT_END_VAL};
pwm_clk_div_t pwm_clk_div[1] = {PWM_PWM0_CLK_DIV};
pwm_seq_order_t pwm_seq_order[1] = {PWM_PWM0_SEQ_ORDER};
pwm_trigger_src_t pwm_trigger_src[1] = {PWM_PWM0_TRIG_SRC};
uint16_t pwm_play_cnt[1] = {PWM_PWM0_PLAY_CNT};
pwm_seq_num_t pwm_seq_num[1] = {PWM_PWM0_SEQ_NUM};
pwm_seq_mode_t pwm_seq_mode[1] = {PWM_PWM0_SEQ_MODE};
pwm_counter_mode_t pwm_counter_mode[1] = {PWM_PWM0_COUNTER_MODE};
pwm_dma_smp_fmt_t pwm_dma_smp_fmt[1] = {PWM_PWM0_DMA_SML_FMT};
uint8_t pwm_element_arr[1] = {PWM_PWM0_ELEMENT_NUM};
uint8_t pwm_rep_arr[1] = {PWM_PWM0_REPEAT_NUM};
uint8_t pwm_dly_arr[1] = {PWM_PWM0_DLY_NUM};
/**************************************************************************************************
 *    GLOBAL FUNCTIONS
 *************************************************************************************************/
/**
 * @brief Initinal GPIO Pin mux  for PWM
 */
/*this is pin mux setting*/
static void init_default_pin_mux(void)
{
    int i;

    /*set GPIO8 and GPIO9 to PWN and  remaining pin to gpio, except GPIO16, GPIO17 */
    for (i = 0; i < 32; i++)
    {

        if (i == 20)
        {
            pin_set_mode(i, MODE_PWM0);
        }
        else if ((i != 16) && (i != 17))
        {
            pin_set_mode(i, MODE_GPIO);
        }
    }

    return;
}
void pwm_init_parameter(pwm_id_t id, pwm_seq_para_head_t *pwm_para_config)
{
    pwm_seq_para_t *pwm_seq = NULL;

    pwm_para_config->pwm_id            = id;
    pwm_para_config->pwm_play_cnt      = pwm_play_cnt[id]; //0 means continuous
    pwm_para_config->pwm_seq_order     = pwm_seq_order[id];
    pwm_para_config->pwm_seq_num       = pwm_seq_num[id];
    pwm_para_config->pwm_seq_mode      = pwm_seq_mode[id];
    pwm_para_config->pwm_triggered_src = pwm_trigger_src[id];
    pwm_para_config->pwm_clk_div       = pwm_clk_div[id];
    pwm_para_config->pwm_counter_mode  = pwm_counter_mode[id];
    pwm_para_config->pwm_dma_smp_fmt   = pwm_dma_smp_fmt[id];

    if (pwm_para_config->pwm_seq_num == PWM_SEQ_NUM_2)
    {
        pwm_seq = &(pwm_para_config->pwm_seq0);
        pwm_seq->pwm_element_num    = pwm_element_arr[id];
        pwm_seq->pwm_repeat_num     = pwm_rep_arr[id];
        pwm_seq->pwm_delay_num      = pwm_dly_arr[id];
        pwm_seq->pwm_rdma_addr      = (uint32_t)&pwm_rdma0_addr_temp[id];

        pwm_seq = &(pwm_para_config->pwm_seq1);
        pwm_seq->pwm_element_num    = pwm_element_arr[id];
        pwm_seq->pwm_repeat_num     = pwm_rep_arr[id];
        pwm_seq->pwm_delay_num      = pwm_dly_arr[id];
        pwm_seq->pwm_rdma_addr      = (uint32_t)&pwm_rdma1_addr_temp[id];
    }
    else if (pwm_para_config->pwm_seq_num == PWM_SEQ_NUM_1)
    {
        if (pwm_para_config->pwm_seq_order == PWM_SEQ_ORDER_R)
        {
            pwm_seq = &(pwm_para_config->pwm_seq0);
            pwm_seq->pwm_rdma_addr  = (uint32_t)&pwm_rdma0_addr_temp[id];
        }
        else if (pwm_para_config->pwm_seq_order == PWM_SEQ_ORDER_T)
        {
            pwm_seq = &(pwm_para_config->pwm_seq1);
            pwm_seq->pwm_rdma_addr  = (uint32_t)&pwm_rdma1_addr_temp[id];
        }

        pwm_seq->pwm_element_num    = pwm_element_arr[id];
        pwm_seq->pwm_repeat_num     = pwm_rep_arr[id];
        pwm_seq->pwm_delay_num      = pwm_dly_arr[id];
    }

    if (pwm_para_config->pwm_dma_smp_fmt == PWM_DMA_SMP_FMT_0)
    {
        pwm_para_config->pwm_count_end_val = pwm_count_end_val[id];
    }
    else
    {
        pwm_para_config->pwm_count_end_val = 0x00;
    }

}
void set_duty_cycle(pwm_seq_para_head_t *pwm_para_config, uint8_t current_lv)
{
    uint32_t dutycycle ;
    dutycycle = 4000 * current_lv / 255;

    pwm_seq_para_t *pwm_seq;
    uint32_t *rdma_addr;

    pwm_seq = &pwm_para_config->pwm_seq0;
    rdma_addr = (uint32_t *)pwm_seq->pwm_rdma_addr;
    *(rdma_addr)  = PWM_FILL_SAMPLE_DATA_MODE1(0, 4000 - dutycycle, 4000);

    Pwm_Start(pwm_para_config);
}
int main(void)
{
    check_reset();
    /*we should set pinmux here or in SystemInit */
    init_default_pin_mux();
    //pwm_seq_para_head_t pwm_para_config[2];

    sys_set_random_seed(get_random_number());

    aes_fw_init();

    task_pci_init();

    zigbee_app_init();

    pwm_init_parameter((pwm_id_t)0, &pwm_para_config[0]);
    Pwm_Init(&pwm_para_config[0]);

    set_duty_cycle(&pwm_para_config[0], 0);

    /* Start the scheduler. */
    vTaskStartScheduler();
    while (1)
    {
    }
}

