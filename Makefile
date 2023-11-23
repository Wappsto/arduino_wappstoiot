.PHONY: setup test

all: example test

lint:
	./bin/arduino-lint --compliance strict --library-manager submit --project-type library

example:
	./bin/arduino-cli compile -b esp32:esp32:esp32 --libraries="." examples/esp32_wappsto/esp32_wappsto.ino

test:
	$(MAKE) -C extras/tests tests
	$(MAKE) -C extras/tests runtests

testcov:
	$(MAKE) -C extras/tests testscov
	$(MAKE) -C extras/tests runtests
	(cd extras/tests/CreateTest; gcov ../../../src/*.cpp | grep 'arduino_wappstoiot/src/' -A 1)

genhtml:
	geninfo src/*.gcda
	genhtml src/*.info --legend -t "Arduino Wappsto IoT test coverage" -q -o ./analyze

clean-cov:
	rm -rf analyze
	find . -type f -name '*.gcda' -delete
	find . -type f -name '*.gcda.info' -delete
	find . -type f -name '*.gcno' -delete
	find . -type f -name '*.gcov.*' -delete
	find . -type f -name '*.*.gcov' -delete
	find . -type f -name '*.gz' -delete

clean-test:
	$(MAKE) -C extras/tests clean

clean: clean-cov clean-test

EpoxyDuino:
	git clone https://github.com/seluxit/EpoxyDuino.git extras/EpoxyDuino
	cp extras/epoxy_local.patch extras/EpoxyDuino
	(cd extras/EpoxyDuino; git apply epoxy_local.patch)

bin/arduino-cli:
	curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
	./bin/arduino-cli config init
	./bin/arduino-cli core update-index

bin/arduino-lint:
	curl -fsSL https://raw.githubusercontent.com/arduino/arduino-lint/main/etc/install.sh | sh

setup: bin/arduino-cli bin/arduino-lint EpoxyDuino
	./bin/arduino-cli config set library.enable_unsafe_install true
	# ESP32 core version 2
	#./bin/arduino-cli config add board_manager.additional_urls https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
	# ESP32 core version 1
	./bin/arduino-cli config add board_manager.additional_urls https://dl.espressif.com/dl/package_esp32_index.json
	./bin/arduino-cli config set directories.user ./extras

	./bin/arduino-cli core update-index
	./bin/arduino-cli core install esp32:esp32@1.0.6

	./bin/arduino-cli lib install ArduinoJson@6.17.3
	./bin/arduino-cli lib install AUnit
	./bin/arduino-cli lib update-index
