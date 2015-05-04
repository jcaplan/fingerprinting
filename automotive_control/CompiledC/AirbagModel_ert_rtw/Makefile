APP_NAME:=AirbagModel
OBJ:=AirbagModel.o ert_main.o 
CC:=nios2-elf-gcc
OBJDUMP:=nios2-elf-objdump
CFLAGS:=-fomit-frame-pointer -ggdb

all: $(APP_NAME)
	@echo "Build Complete"

$(APP_NAME): $(OBJ)
	@$(CC) $(OBJ) $(CFLAGS) -o $(APP_NAME) -lm

%.objdump: %.o
	$(OBJDUMP) -d $< > $@ -lm

clean:
	rm -f *.o *.objdump $(APP_NAME)