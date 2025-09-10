/*
 * TCRT5000 �w���H�h���Y�� (tcrt5000_hal.h)
 * 
 * �\�໡���G
 * 1. �w�qHAL�h�P�X�ʼh����������
 * 2. ���Ѩ�ƭ쫬�ŧi�A�קK�sĶ�ɪ����w�q���~
 * 3. �إ߼Ҳն����зǤƳq�T��w
 * 
 * �]�p�ت��G
 * - �����w��ާ@�P�޿豱��
 * - ���ѲΤ@��GPIOŪ������
 * - �䴩�Ҳդƶ}�o�[�c
 */

// TCRT5000 HAL�P�X�ʼh���ϥΪ����Y��
// �קKread_gpio()��Ʀb�sĶ�ɥX�{���w�q���~

#ifndef __TCRT5000_HAL_H__
#define __TCRT5000_HAL_H__

/*
 * ==================== ���Y�ɫO�@���� ====================
 * 
 * #ifndef __TCRT5000_HAL_H__�G�ˬd�����O�_���w�q
 * #define __TCRT5000_HAL_H__�G�w�q�O�@����
 * #endif�G��������sĶ�϶�
 * 
 * �@�ΡG
 * - ������Y�ɳQ���ƥ]�t (Multiple Inclusion Guard)
 * - �קK���Ʃw�q���~
 * - ���ɽsĶ�Ĳv
 * 
 * ��h���ɮ׳� #include "tcrt5000_hal.h" �ɡA
 * �u���Ĥ@���]�t�|��ڳB�z�ɮפ��e�A
 * ���򪺥]�t�|�Q���L�C
 */

// ==================== ��ƭ쫬�ŧi ====================

/**
 * read_gpio - Ū�����wGPIO�}�쪺��J���A
 * @gpio: GPIO�}��s���A�ϥ�pin_mapping.h���w�q���`��
 *        - TCRT5000_LEFT   (GPIO16) - �����P����
 *        - TCRT5000_MIDDLE (GPIO18) - �����P����  
 *        - TCRT5000_RIGHT  (GPIO17) - �k���P����
 * 
 * �^�ǭ�:
 *   0 - GPIO���C�q�� (�P�����������쪫��/������սu)
 *   1 - GPIO�����q�� (�P���������쪫��/������½u)
 *  -1 - ���~�G�L�Ī�GPIO�s��
 * 
 * �ϥνd��:
 *   int left_sensor = read_gpio(TCRT5000_LEFT);
 *   if (left_sensor == 1) {
 *       // �����P����������½u�A����k��ץ�
 *   }
 * 
 * �`�N�ƶ�:
 *   1. ����ƥ�HAL�h�Ҳ�(tcrt5000_hal.ko)���ѹ�@
 *   2. �X�ʼh�Ҳ�(tcrt5000_driver.ko)�z�L������Ū���P����
 *   3. ���������JHAL�h�ҲաA�X�ʼh�~�ॿ�`�B�@
 *   4. ��Ƴz�LEXPORT_SYMBOL()�ץX����L�֤߼Ҳըϥ�
 */
int read_gpio(int gpio);

#endif // __TCRT5000_HAL_H__

/*
 * ==================== �Ҳլ̩ۨʻ��� ====================
 * 
 * �̿����Y�G
 *   tcrt5000_driver.c �� #include "tcrt5000_hal.h" �� read_gpio()���
 *   tcrt5000_hal.c    �� ��@read_gpio()��ƨöץX�Ÿ�
 * 
 * �sĶ�ɡG
 *   - �X�ʼh�sĶ�ɳz�L�����Y�ɱo��read_gpio()�����ñ��
 *   - �קK"implicit function declaration"ĵ�i
 *   - �T�O��ƩI�s���ѼƩM�^�ǭȫ��O���T
 * 
 * ����ɡG
 *   - HAL�h���J��Aread_gpio()�Ÿ��Q���U��֤߲Ÿ���
 *   - �X�ʼh���J�ɡA�ʺA�s�����èϥθӨ��
 *   - �p�GHAL�h�����J�A�X�ʼh���J�|����
 */

/*
 * ==================== �]�p�Ҧ��G�w���H�h ====================
 * 
 * �u�I�G
 * 1. �w��j���G�X�ʼh���ݪ����ާ@GPIO�Ȧs��
 * 2. �����Τ@�G�Ҧ��P����Ū�����ϥάۦP���
 * 3. ������աG�i�H����HAL��@�i��椸����
 * 4. ���x���ӡG�󴫵w�饭�x�ɥu�ݭק�HAL�h
 * 5. �{���X�ƥΡG�h���X�ʥi�@�ΦP�@HAL�h
 * 
 * �h�Ŭ[�c�G
 *   ���ε{��
 *       �� (�t�ΩI�s)
 *   �˸m�X�ʼh (tcrt5000_driver.ko)
 *       �� (��ƩI�s)
 *   �w���H�h (tcrt5000_hal.ko)
 *       �� (�Ȧs���ާ@)
 *   GPIO����w��
 * 
 * ���Y�ɧ@�ΡG
 *   �w�q�U�h������������(Interface Contract)
 */

/*
 * ==================== �X�i�ʦҶq ====================
 * 
 * ���ӥi�઺����X�i�G
 * 
 * // GPIO�g�J���
 * int write_gpio(int gpio, int value);
 * 
 * // GPIO��V�]�w
 * int set_gpio_direction(int gpio, int direction);
 * 
 * // ���_�B�z����
 * int enable_gpio_interrupt(int gpio, int edge_type);
 * int disable_gpio_interrupt(int gpio);
 * 
 * // �h�P�����妸Ū��
 * int read_multiple_gpio(int *gpios, int *values, int count);
 * 
 * // �P�������A�ѪR
 * typedef struct {
 *     int left;
 *     int middle; 
 *     int right;
 *     unsigned long timestamp;
 * } tcrt5000_status_t;
 * 
 * int read_tcrt5000_status(tcrt5000_status_t *status);
 */

/*
 * ==================== �ϥΫ�ĳ ====================
 * 
 * �b��L�֤߼Ҳդ��ϥΦ�HAL�G
 * 
 * 1. �]�t���Y�ɡG
 *    #include "tcrt5000_hal.h"
 * 
 * 2. �I�s��ơG
 *    int sensor_value = read_gpio(TCRT5000_LEFT);
 * 
 * 3. ���~�B�z�G
 *    if (sensor_value < 0) {
 *        printk(KERN_ERR "Failed to read sensor\n");
 *        return -EIO;
 *    }
 * 
 * 4. Makefile�̩ۨʡG
 *    obj-m += your_driver.o
 *    your_driver-objs := your_driver.o
 *    # �T�OHAL�Ҳե��sĶ
 */
