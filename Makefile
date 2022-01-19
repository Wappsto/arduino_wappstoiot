.PHONY: setup

all:
	./bin/arduino-cli compile -b esp32:esp32:esp32 --libraries="." examples/esp32_wappsto/esp32_wappsto.ino

lint:
	./bin/arduino-lint --compliance strict --library-manager submit --project-type library

bin/arduino-cli:
	curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
	./bin/arduino-cli config init
	./bin/arduino-cli core update-index

bin/arduino-lint:
	curl -fsSL https://raw.githubusercontent.com/arduino/arduino-lint/main/etc/install.sh | sh

setup: bin/arduino-cli bin/arduino-lint
	./bin/arduino-cli config set library.enable_unsafe_install true
	./bin/arduino-cli config add board_manager.additional_urls https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

	./bin/arduino-cli core update-index
	./bin/arduino-cli core install esp32:esp32

	./bin/arduino-cli lib install ArduinoJson
	./bin/arduino-cli lib update-index