deps_config := \
	/h/Project/ESP/ESP32/esp-idf/components/app_trace/Kconfig \
	/h/Project/ESP/ESP32/esp-idf/components/aws_iot/Kconfig \
	/h/Project/ESP/ESP32/esp-idf/components/bt/Kconfig \
	/h/Project/ESP/ESP32/esp-idf/components/driver/Kconfig \
	/h/Project/ESP/ESP32/esp-idf/components/esp32/Kconfig \
	/h/Project/ESP/ESP32/esp-idf/components/esp_adc_cal/Kconfig \
	/h/Project/ESP/ESP32/esp-idf/components/esp_http_client/Kconfig \
	/h/Project/ESP/ESP32/esp-idf/components/ethernet/Kconfig \
	/h/Project/ESP/ESP32/esp-idf/components/fatfs/Kconfig \
	/h/Project/ESP/ESP32/esp-idf/components/freertos/Kconfig \
	/h/Project/ESP/ESP32/esp-idf/components/heap/Kconfig \
	/h/Project/ESP/ESP32/esp-idf/components/libsodium/Kconfig \
	/h/Project/ESP/ESP32/esp-idf/components/log/Kconfig \
	/h/Project/ESP/ESP32/esp-idf/components/lwip/Kconfig \
	/h/Project/ESP/ESP32/esp-idf/components/mbedtls/Kconfig \
	/h/Project/ESP/ESP32/esp-idf/components/nvs_flash/Kconfig \
	/h/Project/ESP/ESP32/esp-idf/components/openssl/Kconfig \
	/h/Project/ESP/ESP32/esp-idf/components/pthread/Kconfig \
	/h/Project/ESP/ESP32/esp-idf/components/spi_flash/Kconfig \
	/h/Project/ESP/ESP32/esp-idf/components/spiffs/Kconfig \
	/h/Project/ESP/ESP32/esp-idf/components/tcpip_adapter/Kconfig \
	/h/Project/ESP/ESP32/esp-idf/components/vfs/Kconfig \
	/h/Project/ESP/ESP32/esp-idf/components/wear_levelling/Kconfig \
	/h/Project/ESP/ESP32/esp-idf/components/bootloader/Kconfig.projbuild \
	/h/Project/ESP/ESP32/esp-idf/components/esptool_py/Kconfig.projbuild \
	/h/Project/ESP/ESP32/esp-idf/bad_apple/main/Kconfig.projbuild \
	/h/Project/ESP/ESP32/esp-idf/components/partition_table/Kconfig.projbuild \
	/h/Project/ESP/ESP32/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(IDF_CMAKE)" "n"
include/config/auto.conf: FORCE
endif

$(deps_config): ;
