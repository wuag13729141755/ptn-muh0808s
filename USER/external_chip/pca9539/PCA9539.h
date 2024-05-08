#ifndef __PCA9539_H_
#define __PCA9539_H_

#define DEVICE_ADDR_PCA9539			0xE8

#define PCA_REG_INPUT_PORT0			0
#define PCA_REG_INPUT_PORT1			1
#define PCA_REG_OUTPUT_PORT0		2
#define PCA_REG_OUTPUT_PORT1		3
#define PCA_REG_POL_INV_PORT0		4
#define PCA_REG_POL_INV_PORT1		5
#define PCA_REG_CONFIG_PORT0		6
#define PCA_REG_CONFIG_PORT1		7

typedef void (*StartupPCA9539Init)(void);

extern uint16_t pca_output_val, pca_output_val_keep;
extern uint16_t g_u16PcaPortConfig;
extern StartupPCA9539Init pPCA9539InitFunc;

uint16_t pca9539_inputport_read(void);
void pca9539_outputport_write(uint16_t port_val);
uint16_t pca9539_outputport_read(void);
void pca9539_pol_inv_write(uint16_t inv_val);
void pca9539_port_configuration(uint16_t cfg_val);
void pca9539_init(void);

#endif











