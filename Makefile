KDIR := /lib/modules/$(shell uname -r)/build
SRC := $(PWD)/src
BUILD := $(PWD)/.build-artifacts
OUT := $(PWD)/build
TARGET := anakin.ko

all: $(OUT)/$(TARGET)

$(OUT)/$(TARGET):
	mkdir -p $(BUILD) $(OUT)
	$(MAKE) -C $(KDIR) M=$(SRC) modules
	mv $(SRC)/*.o $(SRC)/*.mod* $(SRC)/*.cmd $(SRC)/.*.cmd $(SRC)/.*.o $(SRC)/*.order $(SRC)/*.symvers $(BUILD) 2>/dev/null || true
	mv $(SRC)/$(TARGET) $(OUT)/

clean:
	$(MAKE) -C $(KDIR) M=$(SRC) O=$(BUILD) clean || true
	rm -rf $(BUILD) $(OUT)

.PHONY: all clean