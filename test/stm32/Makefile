BOARD_FQBN ?= STM32:stm32:GenF4:pnum=BLACKPILL_F411CE,upload_method=swdMethod,opt=o3std

# ============= BASIC SCREEN TEST
.PHONY: build-basic
build-basic:
	BOARD_FQBN=${BOARD_FQBN} make -C basic/

.PHONY: upload-basic
upload-basic:
	BOARD_FQBN=${BOARD_FQBN} make -C basic/ upload

# ============= SIMPLE DRAW
.PHONY: build-simpleDraw
build-simpleDraw:
	BOARD_FQBN=${BOARD_FQBN} make -C simpleDraw/

.PHONY: upload-simpleDraw
upload-simpleDraw:
	BOARD_FQBN=${BOARD_FQBN} make -C simpleDraw/ upload
