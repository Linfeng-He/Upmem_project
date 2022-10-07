#include <stdio.h>

#include <alloc.h>
#include <barrier.h>
#include <defs.h>
#include <mram.h>
#include <mutex.h>
#include <perfcounter.h>

#include "dpu-utils.h"
#include "../support/common.h"

#define block_size (1 << 8) 

int main(){
    uint32_t params_m = (uint32_t) DPU_MRAM_HEAP_POINTER;
    struct DPUParams* params_w = (struct DPUParams*) mem_alloc(ROUND_UP_TO_MULTIPLE_OF_8(sizeof(struct DPUParams)));
    uint32_t* cache = mem_alloc(sizeof(uint32_t));
    mram_read((__mram_ptr void const*)params_m, params_w, ROUND_UP_TO_MULTIPLE_OF_8(sizeof(struct DPUParams)));

    struct send_pipe* pipe[NR_TASKLETS];

    uint32_t num = params_w->num;
    uint32_t origin_m = params_w->origin_m;
    uint32_t send_m = params_w->send_m;
    uint32_t receive_m = params_w->receive_m;
    uint32_t result_m = params_w->result_m;

    uint32_t flagm = params_w->flagm;

    uint32_t b_size = block_size/sizeof(uint32_t);
    uint32_t p_num = (num + NR_TASKLETS - 1)/NR_TASKLETS * 2;
    uint32_t startpo = 0;
    uint32_t t_id = me();
    if(me() < NR_TASKLETS/2){
        pipe[t_id] = (struct send_pipe*) mem_alloc(sizeof(struct send_pipe));
        pipe[t_id]->startpos = me() * b_size;
        pipe[t_id]->offset = params_w->offsetdpu;
        pipe[t_id]->flag = 0;
        startpo = me() * p_num;

    }else{
        uint32_t temp_id = me() - NR_TASKLETS/2;
        pipe[t_id] = (struct send_pipe*) mem_alloc(sizeof(struct send_pipe));
        pipe[t_id]->startpos = temp_id * b_size;
        pipe[t_id]->offset = params_w->offsetdpu;
        pipe[t_id]->flag = 0;
        startpo = temp_id * p_num;
    }
    

    uint32_t t_num = 0;
    if(t_num > num) {
        t_num = 0;
    } else if(startpo + p_num > num) {
        t_num = num - startpo;
    } else {
        t_num = p_num;
    }

    if(me() < NR_TASKLETS/2){
        

        for(uint32_t i = startpo; i < startpo + t_num; ){
            pipe[t_id]->flag = params_w->flagm;
            if(pipe[t_id]->flag == 0){
                uint32_t temp = load(origin_m, i, cache);

                uint32_t pos = pipe[t_id]->startpos + pipe[t_id]->offset;
                store(temp, send_m, pos, cache);
                i++;
                pipe[t_id]->offset++;
                if(pipe[t_id]->offset == pipe[t_id + 1]->startpos){
                    params_w->flagm = 1;
                }

            }else{  
                continue;
            }
        }
    }else {
        for(uint32_t i = startpo; i < startpo + t_num;){
            uint32_t pos = pipe[t_id]->startpos + pipe[t_id]->offset;
            uint32_t temp = load(receive_m, pos, cache);
            if(temp != 0){
                uint32_t result_temp = temp * temp;
                store(result_temp, result_m, i, cache);
                i++;
                pipe[t_id]->offset++;
            }else{
                continue;
            }
        }

    }
}