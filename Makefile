.PHONY: setup test

all:
	./bin/arduino-cli compile -b esp32:esp32:esp32 --libraries="." examples/esp32_wappsto/esp32_wappsto.ino

lint:
	./bin/arduino-lint --compliance strict --library-manager submit --project-type library

test:
	$(MAKE) -C tests tests
	$(MAKE) -C tests runtests

EpoxyDuino:
	git clone https://github.com/bxparks/EpoxyDuino.git
	cd EpoxyDuino
	git checkout master

bin/arduino-cli:
	curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
	./bin/arduino-cli config init
	./bin/arduino-cli core update-index

bin/arduino-lint:
	curl -fsSL https://raw.githubusercontent.com/arduino/arduino-lint/main/etc/install.sh | sh

setup: bin/arduino-cli bin/arduino-lint EpoxyDuino
	./bin/arduino-cli config set library.enable_unsafe_install true
	./bin/arduino-cli config add board_manager.additional_urls https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
	./bin/arduino-cli config set directories.user .

	./bin/arduino-cli core update-index
	./bin/arduino-cli core install esp32:esp32

	./bin/arduino-cli lib install ArduinoJson
	./bin/arduino-cli lib install AUnit
	./bin/arduino-cli lib update-index
