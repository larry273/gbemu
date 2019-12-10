#include "memory.h"

/*
 * interrupt enable reg     FFFF
 * internal ram             FF80
 * empty, unusable for IO   FF4C
 * IO                       FF00
 * empty, unusable for IO   FEA0
 * sprite attrib mem        FE00 --ram
 * echo of 8kB              E000 --ram
 * 8kB internal             C000 --ram
 * 8kB switch ram bank      A000
 * 8kB video ram            8000 ---
 * 16kB switchable rom bank 4000 cart
 * 16kB rom bank 0          0000 ---
 */

memory::memory()
{

}

