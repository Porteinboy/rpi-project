/*
 * GPIO�Ȧs����}�����w�q�� (register_map.h)
 * �A�Ω� Raspberry Pi 4 BCM2711 ����
 * 
 * ���ɮשw�q�FBCM2711 GPIO������Ҧ��Ȧs����}�����q
 * ���Broadcom BCM2711�x����W��
 */

#ifndef __GPIO_ADDRESS__
#define __GPIO_ADDRESS__

// ==================== GPIO����򩳦�} ====================
// BCM2711 (Raspberry Pi 4) GPIO���������O�����}
#define GPIO_BASE 0xFE200000
/*
 * �`�N�G���PRaspberry Pi�������򩳦�}���P
 * - Pi 1: 0x20200000
 * - Pi 2/3: 0x3F200000  
 * - Pi 4: 0xFE200000 (��e�w�q)
 */

// ==================== GPIO�\���ܼȦs�� (GPFSEL) ====================
// �C�ӼȦs������10��GPIO�}�쪺�\��]�w
// �C��GPIO�ϥ�3�Ӧ줸�өw�q�\��G000=��J, 001=��X, 010-111=�S��\��

#define GPFSEL0 0x00    // GPIO�\���ܼȦs��0 (����GPIO 0-9)
                        // �줸�t�m�GGPIO0[2:0], GPIO1[5:3], GPIO2[8:6], ..., GPIO9[29:27]

#define GPFSEL1 0x04    // GPIO�\���ܼȦs��1 (����GPIO 10-19)
                        // �줸�t�m�GGPIO10[2:0], GPIO11[5:3], ..., GPIO19[29:27]
                        // TCRT5000�P�����ϥΪ�GPIO16,17,18�N�b���Ȧs����

#define GPFSEL2 0x08    // GPIO�\���ܼȦs��2 (����GPIO 20-29)

#define GPFSEL3 0x0c    // GPIO�\���ܼȦs��3 (����GPIO 30-39)

#define GPFSEL4 0x10    // GPIO�\���ܼȦs��4 (����GPIO 40-49)

#define GPFSEL5 0x14    // GPIO�\���ܼȦs��5 (����GPIO 50-57)
                        // Pi 4�u��54��GPIO�A�ҥH�u�ϥγ����줸

// ==================== GPIO��X�]�w�Ȧs�� (GPSET) ====================
// �g�J1������줸�i�N��GPIO�]�����q��
// �g�J0�h�L�@�Ρ]���|����GPIO���A�^

#define GPSET0 0x1c     // GPIO��X�]�w�Ȧs��0 (����GPIO 0-31)
                        // �C�Ӧ줸�����@��GPIO�Gbit0=GPIO0, bit1=GPIO1, ...

#define GPSET1 0x20     // GPIO��X�]�w�Ȧs��1 (����GPIO 32-53)
                        // bit0=GPIO32, bit1=GPIO33, ..., bit21=GPIO53

// ==================== GPIO��X�M���Ȧs�� (GPCLR) ====================
// �g�J1������줸�i�N��GPIO�]���C�q��
// �g�J0�h�L�@�Ρ]���|����GPIO���A�^

#define GPCLR0 0x28     // GPIO��X�M���Ȧs��0 (����GPIO 0-31)
                        // �PGPSET0�����A�Ω�]�w�C�q��

#define GPCLR1 0x2c     // GPIO��X�M���Ȧs��1 (����GPIO 32-53)
                        // �PGPSET1�����A�Ω�]�w�C�q��

// ==================== GPIO�q��Ū���Ȧs�� (GPLEV) ====================
// Ū��GPIO���Y�ɿ�J/��X���A
// 1��ܰ��q��A0��ܧC�q��]TCRT5000�P�����D�n�ϥΦ��Ȧs���^

#define GPLEV0 0x34     // GPIO�q��Ū���Ȧs��0 (Ū��GPIO 0-31���A)
                        // TCRT5000�P����(GPIO16,17,18)�����A�N�q���Ȧs��Ū��

#define GPLEV1 0x38     // GPIO�q��Ū���Ȧs��1 (Ū��GPIO 32-53���A)

// ==================== GPIO�ƥ󰻴����A�Ȧs�� (GPEDS) ====================
// �O��GPIO�O�_����������w���ƥ�]�W�ɽt�B�U���t���^
// Ū����|�۰ʲM���A�Ω��_�B�z

#define GPEDS0 0x40     // GPIO�ƥ󰻴����A�Ȧs��0 (GPIO 0-31)

#define GPEDS1 0x44     // GPIO�ƥ󰻴����A�Ȧs��1 (GPIO 32-53)

// ==================== GPIO�W�ɽt�����P��Ȧs�� (GPREN) ====================
// �]�w����GPIO�n�����W�ɽt�ƥ�]0��1�ഫ�^
// �Ω��_Ĳ�o�A�i��@�P�������A�ܤƪ��Y�ɦ^��

#define GPREN0 0x4c     // GPIO�W�ɽt�����P��Ȧs��0 (GPIO 0-31)

#define GPREN1 0x50     // GPIO�W�ɽt�����P��Ȧs��1 (GPIO 32-53)

// ==================== GPIO�U���t�����P��Ȧs�� (GPFEN) ====================
// �]�w����GPIO�n�����U���t�ƥ�]1��0�ഫ�^

#define GPFEN0 0x58     // GPIO�U���t�����P��Ȧs��0 (GPIO 0-31)

#define GPFEN1 0x5C     // GPIO�U���t�����P��Ȧs��1 (GPIO 32-53)

// ==================== GPIO���q�찻���P��Ȧs�� (GPHEN) ====================
// �]�w����GPIO�n�������q��ƥ�]����1�����A�^

#define GPHEN0 0x64     // GPIO���q�찻���P��Ȧs��0 (GPIO 0-31)

#define GPHEN1 0x68     // GPIO���q�찻���P��Ȧs��1 (GPIO 32-53)

// ==================== GPIO�C�q�찻���P��Ȧs�� (GPLEN) ====================
// �]�w����GPIO�n�����C�q��ƥ�]����0�����A�^

#define GPLEN0 0x70     // GPIO�C�q�찻���P��Ȧs��0 (GPIO 0-31)

#define GPLEN1 0x74     // GPIO�C�q�찻���P��Ȧs��1 (GPIO 32-53)

// ==================== GPIO�D�P�B�W�ɽt�����P��Ȧs�� (GPAREN) ====================
// �D�P�B�����W�ɽt�A�����t�ήɯ߼v�T
// �A�Ω��W�T���ι�ɧǭn�D�Y�檺����

#define GPAREN0 0x7c    // GPIO�D�P�B�W�ɽt�����P��Ȧs��0 (GPIO 0-31)

#define GPAREN1 0x80    // GPIO�D�P�B�W�ɽt�����P��Ȧs��1 (GPIO 32-53)

// ==================== GPIO�D�P�B�U���t�����P��Ȧs�� (GPAFEN) ====================
// �D�P�B�����U���t�A�����t�ήɯ߼v�T

#define GPAFEN0 0x88    // GPIO�D�P�B�U���t�����P��Ȧs��0 (GPIO 0-31)

#define GPAFEN1 0x8c    // GPIO�D�P�B�U���t�����P��Ȧs��1 (GPIO 32-53)

// ==================== GPIO�W��/�U�Ա���Ȧs�� (PUP_PDN_CNTRL) ====================
// BCM2711�s�W���W��/�U�Ա���Ȧs���]���N�ª���GPPUD/GPPUDCLK�^
// �C��GPIO�ϥ�2�Ӧ줸�G00=�L, 01=�W��, 10=�U��, 11=�O�d

#define GPIO_PUP_PDN_CNTRL_REG0 0xe4    // GPIO�W�ԤU�Ա���Ȧs��0 (GPIO 0-15)
                                        // �C��GPIO��2�줸�GGPIO0[1:0], GPIO1[3:2], ...

#define GPIO_PUP_PDN_CNTRL_REG1 0xe8    // GPIO�W�ԤU�Ա���Ȧs��1 (GPIO 16-31)
                                        // TCRT5000�P�������W�ԤU�Գ]�w�b���Ȧs��

#define GPIO_PUP_PDN_CNTRL_REG2 0xec    // GPIO�W�ԤU�Ա���Ȧs��2 (GPIO 32-47)

#define GPIO_PUP_PDN_CNTRL_REG3 0xf0    // GPIO�W�ԤU�Ա���Ȧs��3 (GPIO 48-57)

#endif // __GPIO_ADDRESS__

/*
 * ==================== �Ȧs���ϥνd�� ====================
 * 
 * 1. �]�wGPIO����X�Ҧ��G
 *    val = readl(gpio_base + GPFSEL1);           // Ū��GPFSEL1
 *    val |= (1 << 18);                          // �]�wGPIO16����X(001)
 *    writel(val, gpio_base + GPFSEL1);          // �g�^�Ȧs��
 * 
 * 2. �]�wGPIO��X���q��G
 *    writel(1 << 16, gpio_base + GPSET0);       // GPIO16��X���q��
 * 
 * 3. �]�wGPIO��X�C�q��G
 *    writel(1 << 16, gpio_base + GPCLR0);       // GPIO16��X�C�q��
 * 
 * 4. Ū��GPIO��J���A�G
 *    val = readl(gpio_base + GPLEV0);           // Ū�����A�Ȧs��
 *    bit = (val >> 16) & 1;                     // ����GPIO16����
 * 
 * 5. �]�w�W�Թq���G
 *    val = readl(gpio_base + GPIO_PUP_PDN_CNTRL_REG1);
 *    val |= (1 << 0);                           // GPIO16�]�w�W��(01)
 *    writel(val, gpio_base + GPIO_PUP_PDN_CNTRL_REG1);
 */

/*
 * ==================== TCRT5000���λ��� ====================
 * 
 * TCRT5000�`��P�����ϥΪ��Ȧs���G
 * - GPIO16,17,18�\��]�w�GGPFSEL1�Ȧs��
 * - �P�������AŪ���GGPLEV0�Ȧs��
 * - �W�ԤU�Գ]�w�GGPIO_PUP_PDN_CNTRL_REG1�Ȧs��
 * 
 * �P�������u��ĳ�G
 * - VCC �� 3.3V/5V�]�̷P�����W��^
 * - GND �� Ground
 * - OUT �� GPIO16/17/18�]�Ʀ��X�}��^
 * 
 * �q�`��ĳ�]�w���L�W�Թq���H�T�O�T��í�w��
 */
