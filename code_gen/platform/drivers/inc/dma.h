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
alt_dma_txchan txchan[NUMCORES];
alt_dma_rxchan rxchan[NUMCORES];


extern OS_EVENT *dmaQ;
extern OS_FLAG_GRP *dmaReadyFlag;
extern CriticalFunctionData critFuncData[NUMCORES];
extern int *core_IRQ[NUMCORES];

void sendDMA(void* sourceAddress, void* destAddress, int size, void *handle) ;
void dma_TASK(void* pdata);
void handleDMA(void* handle, void* data);
void postDmaMessage(INT32U task, bool start);
void initDMA(void);
#endif /* DMA_H_ */
