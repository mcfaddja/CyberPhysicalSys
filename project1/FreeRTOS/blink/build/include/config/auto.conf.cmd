deps_config := \
	/home/jamster/esp/esp-idf/components/app_trace/Kconfig \
	/home/jamster/esp/esp-idf/components/aws_iot/Kconfig \
	/home/jamster/esp/esp-idf/components/bt/Kconfig \
	/home/jamster/esp/esp-idf/components/driver/Kconfig \
	/home/jamster/esp/esp-idf/components/esp32/Kconfig \
	/home/jamster/esp/esp-idf/components/esp_adc_cal/Kconfig \
	/home/jamster/esp/esp-idf/components/esp_http_client/Kconfig \
	/home/jamster/esp/esp-idf/components/ethernet/Kconfig \
	/home/jamster/esp/esp-idf/components/fatfs/Kconfig \
	/home/jamster/esp/esp-idf/components/freertos/Kconfig \
	/home/jamster/esp/esp-idf/components/heap/Kconfig \
	/home/jamster/esp/esp-idf/components/http_server/Kconfig \
	/home/jamster/esp/esp-idf/components/libsodium/Kconfig \
	/home/jamster/esp/esp-idf/components/log/Kconfig \
	/home/jamster/esp/esp-idf/components/lwip/Kconfig \
	/home/jamster/esp/esp-idf/components/mbedtls/Kconfig \
	/home/jamster/esp/esp-idf/components/mdns/Kconfig \
	/home/jamster/esp/esp-idf/components/openssl/Kconfig \
	/home/jamster/esp/esp-idf/components/pthread/Kconfig \
	/home/jamster/esp/esp-idf/components/spi_flash/Kconfig \
	/home/jamster/esp/esp-idf/components/spiffs/Kconfig \
	/home/jamster/esp/esp-idf/components/tcpip_adapter/Kconfig \
	/home/jamster/esp/esp-idf/components/vfs/Kconfig \
	/home/jamster/esp/esp-idf/components/wear_levelling/Kconfig \
	/home/jamster/esp/esp-idf/Kconfig.compiler \
	/home/jamster/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/jamster/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/jamster/esp/blink/main/Kconfig.projbuild \
	/home/jamster/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/jamster/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
