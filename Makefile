all:
	./bin/arduino-cli compile -b esp32:esp32:esp32wrover examples/esp32_wappsto/esp32_wappsto.ino

bin/arduino-cli:
	curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh

setup: bin/arduino-cli
	./bin/arduino-cli core update-index --additional-urls https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
