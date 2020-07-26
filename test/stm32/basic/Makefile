USER_LIB_PATH=../../../../
SRC_CC=./*.ino

build: $(SRC_CC)
	arduino-cli compile -v --libraries ${USER_LIB_PATH} --fqbn ${BOARD_FQBN} ./

upload:
	arduino-cli upload -v --fqbn ${BOARD_FQBN} ../basic
