// ------------------------------------------------------------------------
// NOTICE:  All rights reserved.
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//
// Filename:      dma.c
// Author:        Rick Wright
// Revision:      C
//
// Description:  This file contains the DMA support routines
//
// ------------------------------------------------------------------------
//
// ------------------------------------------------------------------------
//
// Revision Log:
//
//  A           CRU   12/21/2021   Baseline Release
//  B           RLW   06/13/2022   Changes per PR #19 in PACT
//  C           CRU   06/30/2022   Changes per PR #22 in PACT
//
// ------------------------------------------------------------------------
//
// Design Details and Explanations:
//
// ------------------------------------------------------------------------
//
//
// ------------------------------------------------------------------------
//
// Implementation Notes:
//
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
//                              Include Files
// ------------------------------------------------------------------------

#include "global.h"

// ------------------------------------------------------------------------
//                               Definitions
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
//                  Local Conditional Compilation Switches
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
//                          Local Variables
// ------------------------------------------------------------------------


// ------------------------------------------------------------------------
//
//  STM32_DMA_Init - This routine is used to initialize the direct memory access (DMA)
//
//  Params: hdma - a DMA_HandleTypeDef structure containing the configuration information
//
//
//  returns:  none
//
void STM32_DMA_Init(DMA_HandleTypeDef *hdma) {

    uint32_t Tmp;

    Tmp = hdma->Instance->CCR;
    Tmp &= ((uint32_t)~(DMA_CCR_PL | DMA_CCR_MSIZE | DMA_CCR_PSIZE | DMA_CCR_MINC | DMA_CCR_PINC | DMA_CCR_CIRC | DMA_CCR_DIR));
    Tmp |= hdma->Init.Direction |
        hdma->Init.PeriphInc | hdma->Init.MemInc |
        hdma->Init.PeriphDataAlignment | hdma->Init.MemDataAlignment |
        hdma->Init.Mode | hdma->Init.Priority;
    hdma->Instance->CCR = Tmp;
    hdma->ChannelIndex = (((uint32_t)hdma->Instance - (uint32_t)DMA1_Channel1) / ((uint32_t)DMA1_Channel2 - (uint32_t)DMA1_Channel1)) << 2u;
    hdma->DmaBaseAddress = DMA1;
    hdma->XferCpltCallback = NULL;
}

// ------------------------------------------------------------------------
//
//  STM32_DMA_IRQHandler - This routine handles DMA interrup requests
//
//  Params: hdma - a DMA_HandleTypeDef structure containing the configuration information
//
//  returns:  none
//
void STM32_DMA_IRQHandler(DMA_HandleTypeDef *hdma) {

    uint32_t FlagIt = hdma->DmaBaseAddress->ISR;
    uint32_t SourceIt = hdma->Instance->CCR;

    /* Transfer Complete Interrupt management ***********************************/

    if ((RESET != (FlagIt & (DMA_FLAG_TC1 << hdma->ChannelIndex))) && (RESET != (SourceIt & DMA_IT_TC))) {
        if ((hdma->Instance->CCR & DMA_CCR_CIRC) == 0u) {
            /* Disable the transfer complete  & transfer error interrupts */
            /* if the DMA mode is not CIRCULAR */
            hdma->Instance->CCR &= ~(DMA_IT_TC | DMA_IT_TE);
        }

        /* Clear the transfer complete flag */
        hdma->DmaBaseAddress->IFCR = DMA_FLAG_TC1 << hdma->ChannelIndex;

        if (hdma->XferCpltCallback != NULL) {
            /* Transfer complete callback */
            hdma->XferCpltCallback(hdma);
        }
        /* Transfer Error Interrupt management ***************************************/
    } else if ((RESET != (FlagIt & (DMA_FLAG_TE1 << hdma->ChannelIndex))) && (RESET != (SourceIt & DMA_IT_TE))) {
        /* When a DMA transfer error occurs */
        /* reset the processor */
        Error_Handler(FILE_DMA, __LINE__);
    } else {
    }
}

