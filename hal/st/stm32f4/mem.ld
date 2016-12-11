/*
 * Memory Spaces Definitions.
 *
 * The values below can be addressed in further linker scripts
 * using functions like 'ORIGIN(RAM)' or 'LENGTH(RAM)'.
 */

/*
 STM32F405VG:
 ============
 RAM   = 112KB + 16KB      = 128KB
 CCM = Core Coupled Memory = 64KB
 Flash = 1MB               = 1024KB
*/

MEMORY
{
  RAM (xrw)           : ORIGIN = 0x20000000, LENGTH = 128K
  CCMRAM (xrw)        : ORIGIN = 0x10000000, LENGTH = 64K
  FLASH (rx)          : ORIGIN = 0x08000000, LENGTH = 1024K
}
