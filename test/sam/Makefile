ARDMK_DIR ?= /usr/share/arduino # where you installed arduino make
MONITOR_PORT ?= /dev/ttyACM0 # customize to your serial port
ARDUINO_DIR ?=  /usr/share/arduino
BOARD_TAG ?= arduino_due_x
ARCHITECTURE ?= sam

# ============= BASIC SCREEN TEST
.PHONY: build-basic
build-basic:
	ARDMK_DIR=${ARDMK_DIR} MONITOR_PORT=${MONITOR_PORT} ARDUINO_DIR=${ARDUINO_DIR} BOARD_TAG=${BOARD_TAG} ARCHITECTURE=${ARCHITECTURE} make -C basic/

.PHONY: upload-basic
upload-basic:
	ARDMK_DIR=${ARDMK_DIR} MONITOR_PORT=${MONITOR_PORT} ARDUINO_DIR=${ARDUINO_DIR} BOARD_TAG=${BOARD_TAG} ARCHITECTURE=${ARCHITECTURE} make -C basic/ upload


# ============= SIMPLE DRAWING TEST
.PHONY: build-simpleDraw
build-simpleDraw:
	ARDMK_DIR=${ARDMK_DIR} MONITOR_PORT=${MONITOR_PORT} ARDUINO_DIR=${ARDUINO_DIR} BOARD_TAG=${BOARD_TAG} ARCHITECTURE=${ARCHITECTURE} make -C simpleDraw/

.PHONY: upload-simpleDraw
upload-simpleDraw:
	ARDMK_DIR=${ARDMK_DIR} MONITOR_PORT=${MONITOR_PORT} ARDUINO_DIR=${ARDUINO_DIR} BOARD_TAG=${BOARD_TAG} ARCHITECTURE=${ARCHITECTURE} make -C simpleDraw/ upload
