USER_LIB_PATH=../../../../
SRC_CC=./*.ino $(USER_LIB_PATH)/libILI9486/src/**.c

.PHONY: build
build: $(SRC_CC)
	arduino-cli compile -v --libraries ${USER_LIB_PATH} --fqbn ${BOARD_FQBN} ./

.PHONY: upload
upload: build
	arduino-cli upload -v --fqbn ${BOARD_FQBN} ../simpleDraw
