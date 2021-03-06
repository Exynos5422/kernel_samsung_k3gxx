/*
 * sound/soc/samsung/idma.h
 *
 * Copyright (c) 2011 Samsung Electronics Co., Ltd
 *		http://www.samsung.com
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 */

#ifndef __SND_SOC_SAMSUNG_IDMA_H_
#define __SND_SOC_SAMSUNG_IDMA_H_

<<<<<<< HEAD
void idma_reg_addr_init(void __iomem *regs, dma_addr_t addr);
int asoc_idma_platform_register(struct device *dev);
void asoc_idma_platform_unregister(struct device *dev);
=======
extern void idma_reg_addr_init(void __iomem *regs, dma_addr_t addr);
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83

/* dma_state */
#define LPAM_DMA_STOP	0
#define LPAM_DMA_START	1

#define MAX_IDMA_PERIOD (128 * 1024)
#define MAX_IDMA_BUFFER (160 * 1024)

#endif /* __SND_SOC_SAMSUNG_IDMA_H_ */
