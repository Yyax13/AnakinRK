KDIR := /lib/modules/$(shell uname -r)/build
SRC := $(PWD)/src
BUILD := $(PWD)/.build-artifacts
OUT := $(PWD)/build
TARGET := anakin.ko

all: help

build: $(OUT)/$(TARGET) ## Build the project
	
$(OUT)/$(TARGET):
	mkdir -p $(BUILD) $(OUT)
	$(MAKE) -C $(KDIR) M=$(SRC) modules
	mv $(SRC)/*.o $(SRC)/*.mod* $(SRC)/*.cmd $(SRC)/.*.cmd $(SRC)/.*.o $(SRC)/*.order $(SRC)/*.symvers $(SRC)/*/*.o $(SRC)/*/*.mod* $(SRC)/*/*.cmd $(SRC)/*/.*.cmd $(SRC)/*/.*.o $(SRC)/*/*.order $(SRC)/*/*.symvers $(BUILD) 2>/dev/null || true
	mv $(SRC)/$(TARGET) $(OUT)/

clean: ## Remove build artifacts and binaries
	$(MAKE) -C $(KDIR) M=$(SRC) O=$(BUILD) clean || true
	rm -rf $(BUILD) $(OUT)

compile_commands: ## Write compile_commands.json
	$(MAKE) clean

	cd src && \
	bear -- \
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD)/src V=1 modules

	@echo "[+] fixing directory field for CLion"
	sed -i 's|"directory": "/usr/src[^"]*"|"directory": "$(PWD)/src"|g' src/compile_commands.json

	mv src/compile_commands.json .
	$(MAKE) clean

help: ## Show this menu
	@echo 'Usage:'
	@grep -E '^[a-z.A-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\t\033[36m%-30s\033[0m %s\n", $$1, $$2}'

.PHONY: all clean compile_commands
