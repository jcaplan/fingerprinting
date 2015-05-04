APP_NAME:=FuelSensor
OBJ:=FuelSensor.o ert_main.o 
CC:=nios2-elf-gcc
OBJDUMP:=nios2-elf-objdump
CFLAGS:=-fomit-frame-pointer -ggdb

all: $(APP_NAME)
	@echo "Build Complete"

$(APP_NAME): $(OBJ)
	@$(CC) $(OBJ) $(CFLAGS) -o $(APP_NAME)

%.objdump: %.o
	$(OBJDUMP) -d $< > $@

%.o: %.c %.h
	$(CC) -c $(CFLAGS) -o $@ $< 
	

clean:
	rm -f *.o *.objdump $(APP_NAME)