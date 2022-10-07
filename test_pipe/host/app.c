#include <dpu.h>
#include <dpu_log.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#include "mram-management.h"
#include "../support/common.h"
#include "../support/timer.h"
#include "../support/utils.h"

#define DPU_BINARY "./bin/dpu_code"

#define m_number INT32_MAX
#define dpu_max (64 << 2)  //less than quarter of 64mb
#define block_size (1 << 8) 

void allocate_number(uint32_t max_dpu, uint32_t* allo_num){
    for(uint32_t i = 1; i <= max_dpu; i++){
        allo_num[i] = i;
    }
}

int main(){

    struct mram_heap_allocator_t {
        uint32_t totalAllocated;
    };

    struct dpu_set_t dpu_set, dpu; //how to get symbol for different dpus (dpu_set, dpu, i)?
    uint32_t numDPUs = 0;
    DPU_ASSERT(dpu_alloc(NR_DPUS, NULL, &dpu_set));
    DPU_ASSERT(dpu_load(dpu_set, DPU_BINARY, NULL));
    DPU_ASSERT(dpu_get_nr_dpus(dpu_set, &numDPUs));
    dpu_id_t dpu0, dpu1;

    uint32_t b_num = block_size/sizeof(uint32_t);

    uint32_t max_dpu = dpu_max/(sizeof(uint32_t));
    uint32_t* allo_num = malloc(max_dpu * sizeof(uint32_t));
    uint32_t* result = malloc(max_dpu * sizeof(uint32_t));
    uint32_t* recv_result = malloc(ROUND_UP_TO_MULTIPLE_OF_8(sizeof(uint32_t)));
    uint32_t* host_buffer[NR_TASKLETS/2];
    for(uint32_t i = 0; i < NR_TASKLETS/2; i++)
        host_buffer[i] = malloc(max_dpu * sizeof(uint32_t));
    allocate_number(max_dpu, allo_num);
    
    uint32_t num = max_dpu; //lots of numbers for dpu
    struct DPUParams dpu_params0, dpu_params_back;

    struct mram_heap_allocator_t allocator;
    init_allocator(&allocator);
    //init_allocator(&allocator1);
    //printf("error here!\n");

    uint32_t dpu_params_m = mram_heap_alloc(&allocator, ROUND_UP_TO_MULTIPLE_OF_8(sizeof(struct DPUParams)));
    uint32_t origin_m = mram_heap_alloc(&allocator, (max_dpu * sizeof(uint32_t)));
    uint32_t send_m = mram_heap_alloc(&allocator, (NR_TASKLETS * b_num * sizeof(uint32_t)));
    uint32_t receive_m = mram_heap_alloc(&allocator, (NR_TASKLETS * b_num * sizeof(uint32_t)));

    //printf("error here!\n");

    dpu_params0.num = max_dpu;
    dpu_params0.origin_m = origin_m;
    dpu_params0.send_m = send_m;
    dpu_params0.receive_m = receive_m;
    dpu_params0.offsetdpu = 0;

    dpu_params0.result_m = 0;
    dpu_params0.flagm = 0;

    dpu_params_back.num = max_dpu;
    dpu_params_back.origin_m = origin_m;
    dpu_params_back.send_m = send_m;
    dpu_params_back.receive_m = receive_m;
    dpu_params_back.offsetdpu = 0;

    dpu_params_back.result_m = 0;
    dpu_params_back.flagm = 0;

    //uint32_t dpu_param1 = mram_heap_alloc(&allocator1, sizeof(struct DPUParams));
    //number_m = mram_heap_alloc(&allocator1, (max_dpu * sizeof(uint32_t)));
    //uint32_t result_m = mram_heap_alloc(&allocator1, (max_dpu * sizeof(uint32_t)));

    // dpu_params1.num = max_dpu;
    // dpu_params0.origin_m = 0;
    // dpu_params1.number_m = number_m;
    // dpu_params1.result_m = result_m;
    // dpu_params1.key = 1;
    //printf("error here!\n");

    // for(uint32_t i = 1; i <= max_dpu; i++){
    //     printf("%d ", allo_num[i]);
    // }

    //dpu = 0;
    DPU_FOREACH(dpu_set, dpu){
        copyToDPU(dpu, (uint8_t*)allo_num, origin_m, ROUND_UP_TO_MULTIPLE_OF_8(max_dpu*sizeof(uint32_t)));
        //dpu = 1;
        //printf("error here!\n");

        copyToDPU(dpu, (uint8_t*)&dpu_params0, dpu_params_m, ROUND_UP_TO_MULTIPLE_OF_8(sizeof(struct DPUParams)));
    }
    

    //printf("error here!\n");
    //copyToDPU(dpu, (uint8_t*)dpu_params1, dpu_param1, max_dpu*sizeof(uint32_t));

    // DPU_FOREACH (dpu_set, dpu) {
    //     printf("%d\n", dpu);
    // }

    uint32_t hostPtr = 0;
    uint32_t count = 0;

    //printf("error here!\n");

    DPU_ASSERT(dpu_launch(dpu, DPU_ASYNCHRONOUS));
    //DPU_ASSERT(dpu_launch(dpu1, DPU_ASYNCHRONOUS));

    //

    //printf("error here!\n");
    DPU_FOREACH(dpu_set, dpu){
        printf("error here1!\n");
    while(1){
        printf("error here2!\n");
        if(count >= max_dpu-1){
            printf("error here3!\n");
            break;
        }else{
            while(1){
                printf("error here4!\n");
                for(uint32_t i = 0; i < NR_TASKLETS/2; i++){
                    printf("error here9!\n");
                    uint32_t p = i * b_num + b_num;
                    printf("error here10!\n");
                    copyFromDPU(dpu, 0, (uint8_t*)recv_result, ROUND_UP_TO_MULTIPLE_OF_8(sizeof(uint32_t))); //
                    printf("error here11!\n");
                    if(recv_result != 0){
                        break;
                    }
                }
            }
            for(uint32_t i = 0; i < NR_TASKLETS/2; i++){
                uint32_t p = i * b_num;
                send(dpu, (uint8_t*)host_buffer[i], hostPtr, p, block_size);
                printf("error here6!\n");
                receive(dpu,  (uint8_t*)host_buffer[i + NR_TASKLETS/2], hostPtr, p, block_size);
                printf("error here7!\n");
                dpu_params0.flagm = 0;
                copyToDPU(dpu, (uint8_t*)&dpu_params0, dpu_params_m, ROUND_UP_TO_MULTIPLE_OF_8(sizeof(struct DPUParams)));
                count += b_num;

            }
            //DPU_ASSERT(dpu_launch(dpu, DPU_ASYNCHRONOUS));
        }
            
    }
    }

    DPU_FOREACH(dpu_set, dpu){
        copyFromDPU(dpu, dpu_params0.result_m, (uint8_t*)result, max_dpu*sizeof(uint32_t));
    }

    for(uint32_t i = 0; i < NR_TASKLETS/2; i++){
        free(host_buffer[i]);
    }
    free(allo_num);
    free(result);

    return 0;
   

}
