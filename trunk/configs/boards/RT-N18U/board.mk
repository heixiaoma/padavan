##################################################################
# Board PID # Board Name       # PRODUCT     # Note
##################################################################
# RT-N18U   # ASUS RT-N18U     # BCM47081A0  #
##################################################################

CFLAGS += -DBOARD_N18U -DVENDOR_ASUS

# required definitions for this board
BOARD_NUM_USB_PORTS=2
BOARD_2G_IN_SOC=0
BOARD_5G_IN_SOC=0
BOARD_HAS_5G_RADIO=0
BOARD_RAM_SIZE=256

# optional customization extra definitions
