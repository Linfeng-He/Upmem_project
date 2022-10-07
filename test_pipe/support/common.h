#ifndef _COMMON_H_
#define _COMMON_H_

#define ROUND_UP_TO_MULTIPLE_OF_2(x)    ((((x) + 1)/2)*2)
#define ROUND_UP_TO_MULTIPLE_OF_8(x)    ((((x) + 7)/8)*8)
#define ROUND_UP_TO_MULTIPLE_OF_64(x)   ((((x) + 63)/64)*64)


struct DPUParams {
    uint32_t num;
    uint32_t origin_m;
    uint32_t send_m;
    uint32_t receive_m;
    uint32_t result_m;
    uint32_t offsetdpu;
    uint32_t flagm;
};

struct send_pipe {
    uint32_t startpos;
    uint32_t offset;
    uint32_t flag;
};

// static void send(struct dpu_set_t dpu_src, uint8_t* hostSrc, uint32_t offset, uint32_t mramSrc, uint32_t size){
//     DPU_ASSERT(dpu_copy_from(dpu_src, DPU_MRAM_HEAP_POINTER_NAME, mramSrc, hostSrc + offset, size));
//     //while buffer if full copy to dpu
// }

// //when to do this? after a fixed cycle
// static void receive(struct dpu_set_t dpu_dst, uint8_t* hostDst, uint32_t offset, uint32_t mramDst, uint32_t size){

//     DPU_ASSERT(dpu_copy_to(dpu_dst, DPU_MRAM_HEAP_POINTER_NAME, mramDst, hostDst + offset, size));
//     //after copy to clear buffer
// }
//static void receive(struct dpu_set_t dpu_src, struct dpu_set_t dpu_dst, uint8_t* hostPtr, uint32_t mramIdx, uint32_t size)

#endif

