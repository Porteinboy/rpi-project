// Car Sensors GPIO Definitions
// �۰ʾr�p�����P���� GPIO �}������w�q��
// ���ɮשw�q�Ҧ��w�餸��P Raspberry Pi GPIO �}�쪺�������Y

// ==================== �q�T���� ====================
// UART ��C�q�T ( Raspberry Pi 4 <-> Pico W �L��� )
// �Ω� Pi �P Pico W ��������ƥ洫�A�Ҧp�P�����ƾڡB������O��
// GPIO 14 TXD  - �ǰe��ƽu (Transmit Data)
// GPIO 15 RXD  - ������ƽu (Receive Data)

// ==================== TCRT5000 �`��P���� ====================
// ���~�u�Ϯg���P�����A�Ω󰻴��a���½u�i��`��
// �P�����t�m�G�T�ӷP�����e�@���u�ƦC�A���ѥ����k�����u����

// �����P���� (GPIO16)
#define TCRT5000_LEFT 16               // ����TCRT5000�P�����s����GPIO16
#define GPIO16_FSEL  GPFSEL1           // GPIO16�\���ܼȦs����GPFSEL1
#define GPIO16_BIT_SHIFT 18            // GPIO16�bGPFSEL1�����줸��m (16*3=48, 48%32=16, ����ڬO18)
                                       // �p��覡�GGPIO16�ϥ�GPFSEL1����18-20�줸�ӳ]�w�\��

// �k���P���� (GPIO17)  
#define TCRT5000_RIGHT 17              // �k��TCRT5000�P�����s����GPIO17
#define GPIO17_FSEL  GPFSEL1           // GPIO17�\���ܼȦs����GPFSEL1
#define GPIO17_BIT_SHIFT 21            // GPIO17�bGPFSEL1�����줸��m
                                       // �p��覡�GGPIO17�ϥ�GPFSEL1����21-23�줸�ӳ]�w�\��

// �����P���� (GPIO18)
#define TCRT5000_MIDDLE 18             // ����TCRT5000�P�����s����GPIO18
#define GPIO18_FSEL  GPFSEL1           // GPIO18�\���ܼȦs����GPFSEL1
#define GPIO18_BIT_SHIFT 24            // GPIO18�bGPFSEL1�����줸��m
                                       // �p��覡�GGPIO18�ϥ�GPFSEL1����24-26�줸�ӳ]�w�\��

/*
 * TCRT5000 �u�@��z�G
 * - �o�g���~�u�ñ����Ϯg�T��
 * - �զ�/�L����G�j�Ϯg �� ��X�C�q�� (0) �� �L���鰻��
 * - �¦�/�`����G�z�Ϯg �� ��X���q�� (1) �� ������½u
 * 
 * �`���޿�d�ҡG
 * - "0 1 0" �� �½u�b�����A����
 * - "1 0 0" �� �½u�����A�V�k�ץ�
 * - "0 0 1" �� �½u���k�A�V���ץ�
 * - "0 0 0" �� �L�½u�A�i���y�Ψ�F���I
 * - "1 1 1" �� ���¡A�i��O�_�I/���I�аO
 */

// ==================== L298N ���F��� ====================
// ��H�����F�X�ʴ����A�Ω󱱨��Ӫ��y���F����V�M�t��
// �䴩�e�i�B��h�B����B�k��B�٨����ʧ@
// TODO: �w�q���F����}��
// ��ĳ�}��t�m�G
// #define MOTOR_LEFT_EN   12     // �����FPWM�t�ױ���
// #define MOTOR_LEFT_IN1  5      // �����F��V����1
// #define MOTOR_LEFT_IN2  6      // �����F��V����2
// #define MOTOR_RIGHT_EN  13     // �k���FPWM�t�ױ���  
// #define MOTOR_RIGHT_IN1 19     // �k���F��V����1
// #define MOTOR_RIGHT_IN2 26     // �k���F��V����2

// ==================== SRF05 �W���i���Z�P���� ====================
// �Ω�e���ê�������A�׻٥\���@
// �u�@��z�G�o�g�W���i�߽ġA�p��^�i�ɶ��Ӵ��q�Z��
// TODO: �w�q�W���i�P�����}��
// ��ĳ�}��t�m�G
// #define ULTRASONIC_TRIG 23     // Ĳ�o�}�� (Trigger) - �o�g�W���i
// #define ULTRASONIC_ECHO 24     // �^���}�� (Echo) - �����Ϯg�i
// ���Z�d��G��2cm-400cm
// ��סG��3mm

// ==================== LED ���A���ܿO ====================
// �h��LED�O���t�ΡA�Ω���ܨ�����e���A
// �����u�ꨮ�����O���t�ΡA���ѵ�ı�ƪ����A�^�X

// �D�n���A�O
//#define GREEN_LED    xx        // ��O - �������`��p���A
                                 // ��ܡG�`�񥿱`�B�L��ê���B�t�ιB�@���`

//#define YELLOW_LED   xx        // ���O - ���`���pĵ��
                                 // ��ܡG�������ê���B�P�������`�B�ݭn�H�u���J

//#define RED_LED      xx        // ���O - ��������A  
                                 // ��ܡG��氱��B�t�ο��~�B���ݫ��O

// ��V���ܿO
//#define ORANGE_LEFT_LED  xx    // ����O - �����V�O
                                 // ��ܡG�����ǳƩΥ��b����

//#define ORANGE_RIGHT_LED xx    // �k��O - �k���V�O
                                 // ��ܡG�����ǳƩΥ��b�k��

// �\��O��
//#define WHITE_LED    xx        // �տO - �˨��O
                                 // ��ܡG�������b��h

/*
 * LED �����޿��ĳ�G
 * 
 * ���`�`��GGREEN ��G
 * ����ץ��GGREEN + ORANGE_LEFT �{�{
 * �k��ץ��GGREEN + ORANGE_RIGHT �{�{  
 * ��ê�������GYELLOW �{�{
 * ��氱��GRED ��G�A��L�O������
 * �˨��׻١GWHITE + RED �P�ɫG�_
 * �t�Ϊ�l�ơG�Ҧ��O���̧ǰ{�{����
 */

// ==================== �}����t�`�N�ƶ� ====================
/*
 * Raspberry Pi 4 GPIO �}��ϥέ���G
 * 
 * �w�ϥθ}��G
 * - GPIO 14, 15: UART �q�T (�PPico W�q�T)
 * - GPIO 16, 17, 18: TCRT5000 �`��P����
 * 
 * ��ĳ�קK���}��G
 * - GPIO 0, 1: I2C ID EEPROM (�t�ΫO�d)
 * - GPIO 2, 3: I2C1 SDA/SCL (�i�Φ���ĳ�O�d��I2C�˸m)
 * - GPIO 4: 1-Wire (�ū׷P������)
 * - GPIO 7, 8, 9, 10, 11: SPI0 (�i�Ω��LSPI�˸m)
 * 
 * �i�θ}����ˡG
 * - PWM�}��: GPIO 12, 13 (�w��PWM�A�A�X���F����)
 * - �@��IO: GPIO 5, 6, 19, 20, 21, 22, 23, 24, 25, 26, 27
 * 
 * �q���P���a�G
 * - 3.3V: Pin 1, 17
 * - 5V: Pin 2, 4 (L298N���F�X�ʾ��ݭn5V�ѹq)
 * - GND: Pin 6, 9, 14, 20, 25, 30, 34, 39
 */

// ==================== �t�ξ�X��ĳ ====================
/*
 * �Ҳո��J���ǡG
 * 1. tcrt5000_hal.ko     (�w���H�h)
 * 2. tcrt5000_driver.ko  (�`��P�����X��)
 * 3. motor_driver.ko     (���F�����X��)
 * 4. ultrasonic_driver.ko (�W���i�P�����X��)
 * 5. led_driver.ko       (LED�����X��)
 * 6. car_control.ko      (�D����{��)
 * 
 * �˸m�`�I��ĳ�G
 * - /dev/tcrt5000        (�`��P����)
 * - /dev/motor           (���F����)
 * - /dev/ultrasonic      (�W���i���Z)
 * - /dev/car_leds        (LED���A�O)
 */
