/*
 * dma.h
 *
 *  Created on: 2015-07-15
 *      Author: jonah
 */

#ifndef DMA_H_
#define DMA_H_

#include <stdbool.h>
#include "shared_mem.h"
#include "includes.h"
#include "sys/alt_dma.h"
#include "repos.h"
/*****************************************************************************
 * DMA channels for each core
 *****************************************************************************/


extern OS_EVENT *dmaQ;
extern OS_FLAG_GRP *dmaReadyFlag;

extern bool taskFailed;

void sendDMA(void* sourceAddress, void* destAddress, int size, void *handle) ;
void dma_TASK(void* pdata);
void handleDMA(void* handle, void* data);
void postDmaMessage(INT32U task, bool start, bool core_0_safe, bool modeChange);
void initDMA(void);
void resetDMA(void);
void dma_setTxRxPointers(alt_dma_txchan *txp, alt_dma_rxchan *rxp);
void dma_setCoreIRQPointers(int **p);
void dma_setCritFuncDataPointer(CriticalFunctionData *p);
#endif /* DMA_H_ */
