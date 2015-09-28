#include <cyg/kernel/diag.h>
#include <cyg/kernel/kapi.h>
#include <cyg/hal/var_io.h>

#include "F4_BackupDomain.h"

cyg_bool F4_BackupDomain::powerBackup()
{
	cyg_uint32 bkpReg;
	HAL_READ_UINT32(CYGHWR_HAL_STM32_PWR + CYGHWR_HAL_STM32_PWR_CSR, bkpReg);

	if(bkpReg & CYGHWR_HAL_STM32_PWR_CSR_BRE)
		return true;

	cyg_uint32 cnt = 10000;
	do
	{
		bkpReg |= CYGHWR_HAL_STM32_PWR_CSR_BRE;
		HAL_WRITE_UINT32(CYGHWR_HAL_STM32_PWR + CYGHWR_HAL_STM32_PWR_CSR, bkpReg);
		HAL_READ_UINT32(CYGHWR_HAL_STM32_PWR + CYGHWR_HAL_STM32_PWR_CSR, bkpReg);

	}while(!(bkpReg & CYGHWR_HAL_STM32_PWR_CSR_BRR) && (cnt-- >0));

	if(bkpReg & CYGHWR_HAL_STM32_PWR_CSR_BRR)
		return true;

	return false;

}

cyg_bool F4_BackupDomain::disableWRprotection()
{
	cyg_uint32 cnt = 9000;
	cyg_uint32 bkpReg;
	HAL_READ_UINT32(CYGHWR_HAL_STM32_PWR + CYGHWR_HAL_STM32_PWR_CR, bkpReg);

	do
	{
		bkpReg |= CYGHWR_HAL_STM32_PWR_CR_DBP;
		HAL_WRITE_UINT32(CYGHWR_HAL_STM32_PWR + CYGHWR_HAL_STM32_PWR_CR, bkpReg);

		HAL_READ_UINT32(CYGHWR_HAL_STM32_PWR + CYGHWR_HAL_STM32_PWR_CR, bkpReg);
	}while(!(bkpReg & CYGHWR_HAL_STM32_PWR_CR_DBP) && (cnt-- >0));

	if(!(bkpReg & CYGHWR_HAL_STM32_PWR_CR_DBP))
		return false;

	return true;
}

cyg_bool F4_BackupDomain::enableWRprotection()
{
	cyg_uint32 bkpReg;

	HAL_READ_UINT32(CYGHWR_HAL_STM32_PWR + CYGHWR_HAL_STM32_PWR_CR, bkpReg);
	bkpReg &= ~(CYGHWR_HAL_STM32_PWR_CR_DBP);
	HAL_WRITE_UINT32(CYGHWR_HAL_STM32_PWR + CYGHWR_HAL_STM32_PWR_CR, bkpReg);
	return false;
}
