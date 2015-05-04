APP_NAME:=matlabtest
OBJ:=AirbagModel.o AirbagModel_data.o ert_main.o 
CC:=nios2-elf-gcc
all: $(APP_NAME)
	@echo "Build Complete"

$(APP_NAME): $(OBJ)
	@$(CC) $(OBJ) -O2 -fomit-frame-pointer -ggdb -pie -o $(APP_NAME)