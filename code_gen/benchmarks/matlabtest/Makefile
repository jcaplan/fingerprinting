APP_NAME:=matlabtest
OBJ:=AirbagModel.o AirbagModel_data.o ert_main.o 
CC:=nios2-elf-gcc
all: $(APP_NAME)
	@echo "Build Complete"

$(APP_NAME): $(OBJ)
	@$(CC) $(OBJ) -fomit-frame-pointer -ggdb -o $(APP_NAME)