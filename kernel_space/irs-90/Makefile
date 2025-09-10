# Makefile for TCRT5000 HAL & Device Driver
# TCRT5000 硬體抽象層與裝置驅動程式編譯腳本
# 
# 功能說明：
# 1. 編譯核心模組 (.c → .ko)
# 2. 管理標頭檔路徑
# 3. 自動化建置流程
# 4. 清理編譯產物

# ==================== 模組定義 ====================
# 指定需要編譯的核心模組清單
# obj-m 變數告訴 Linux 核心建置系統要編譯哪些模組

obj-m += irs-90_hal.o      # HAL層模組：tcrt5000_hal.c → tcrt5000_hal.ko
                             # 提供GPIO硬體抽象介面，包含read_gpio()函數

obj-m += irs-90_driverr.o   # 驅動層模組：tcrt5000_driver.c → tcrt5000_driver.ko  
                             # 提供字元裝置介面，讓使用者程式可透過/dev/tcrt5000讀取感測器

# ==================== 編譯選項設定 ====================
# EXTRA_CFLAGS：傳遞給 gcc 編譯器的額外參數
# -I 選項：指定標頭檔搜尋路徑

EXTRA_CFLAGS += -I$(PWD)/../../include
# 解釋：
# - $(PWD)：當前工作目錄 (Present Working Directory)
# - ../../include：相對路徑，往上兩層目錄的include資料夾
# - 讓編譯器能找到 pin_mapping.h, register_map.h 等標頭檔

# ==================== 核心建置系統路徑 ====================
# KDIR：指向當前核心的建置目錄
# 此目錄包含核心標頭檔、Makefile模板、編譯規則等

KDIR := /lib/modules/$(shell uname -r)/build
# 解釋：
# - $(shell uname -r)：執行shell命令取得核心版本，例如 "5.15.0-91-generic"
# - /lib/modules/核心版本/build：指向核心建置目錄的符號連結
# - 此目錄通常連結到 /usr/src/linux-headers-核心版本/

# ==================== 路徑變數定義 ====================
# PWD：取得當前Makefile所在的絕對路徑
PWD := $(shell pwd)
# 用途：讓核心建置系統知道原始碼檔案的位置

# KO_DIR：統一存放編譯完成的 .ko 檔案目錄
KO_DIR := $(PWD)/../../modules
# 解釋：
# - 將所有 .ko 檔集中管理，方便後續載入
# - 相對路徑：往上兩層目錄的modules資料夾
# - 範例：/home/user/project/modules/

# ==================== 主要建置目標 ====================
# all：預設建置目標，執行完整的編譯流程
all:
	@echo "****** Building Modules ******"
	# 第一步：呼叫核心建置系統編譯模組
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	# 解釋：
	# - $(MAKE)：make命令
	# - -C $(KDIR)：切換到核心建置目錄執行
	# - M=$(PWD)：告訴核心建置系統模組原始碼位置
	# - modules：建置目標，編譯所有obj-m指定的模組
	
	# 第二步：建立目標目錄
	mkdir -p $(KO_DIR)
	# -p 選項：如果目錄不存在則建立，存在則不報錯
	
	# 第三步：複製 .ko 檔到統一目錄
	cp -v *.ko $(KO_DIR)/
	# -v 選項：顯示複製的檔案資訊 (verbose)
	# *.ko：所有的核心模組檔案
	
	@echo "****** Created KO_DIR and Copy .ko file successed  ******"
	
	# 第四步：清理編譯過程產生的中間檔案
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	# 清理：.o, .mod.c, .mod, .cmd, modules.order, Module.symvers 等
	
	@echo "Build successed and has cleaned other files~"

# ==================== 清理目標 ====================
# clean：清除所有編譯產物，回復到編譯前狀態
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	# 清理所有編譯過程中產生的檔案
	# 包括：*.o, *.ko, *.mod.c, *.mod, .*.cmd, modules.* 等

# ==================== 使用方式說明 ====================
# 編譯模組：
# $ make
# 或
# $ make all
#
# 清理檔案：
# $ make clean
#
# 載入模組：
# $ sudo insmod ../../modules/tcrt5000_hal.ko
# $ sudo insmod ../../modules/tcrt5000_driver.ko
#
# 卸載模組：
# $ sudo rmmod tcrt5000_driver
# $ sudo rmmod tcrt5000_hal

# ==================== 目錄結構範例 ====================
# 專案目錄結構：
# project/
# ├── include/                    # 標頭檔目錄
# │   ├── pin_mapping.h          # GPIO腳位定義
# │   └── register_map.h         # 暫存器對應
# ├── modules/                   # 編譯完成的.ko檔存放處
# │   ├── tcrt5000_hal.ko       
# │   └── tcrt5000_driver.ko    
# └── src/
#     └── tcrt5000/              # 當前目錄
#         ├── Makefile           # 此檔案
#         ├── tcrt5000_hal.c     # HAL層原始碼
#         └── tcrt5000_driver.c  # 驅動層原始碼

# ==================== 核心建置系統說明 ====================
# Linux 核心使用 Kbuild 系統進行模組編譯：
#
# 1. 核心建置系統會讀取 obj-m 變數
# 2. 根據 .o 檔案名稱找到對應的 .c 檔案
# 3. 使用核心的編譯選項和標頭檔進行編譯
# 4. 產生可載入的 .ko 核心模組檔案
#
# 編譯過程產生的檔案：
# - .o：目標檔案 (object file)
# - .mod.c：模組資訊檔案
# - .mod：模組符號檔案  
# - .ko：核心模組檔案 (kernel object)
# - .cmd：編譯命令記錄檔
# - modules.order：模組載入順序
# - Module.symvers：符號版本資訊

# ==================== 常見問題排解 ====================
# 1. 找不到標頭檔：
#    檢查 EXTRA_CFLAGS 路徑是否正確
#
# 2. 核心版本不符：
#    確認 /lib/modules/$(uname -r)/build 存在
#    可能需要安裝 linux-headers 套件
#
# 3. 權限問題：
#    載入模組需要 root 權限：sudo insmod
#
# 4. 模組相依性：
#    必須先載入 HAL層，再載入驅動層