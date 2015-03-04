################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/sw/Application.cpp \
../src/sw/Task.cpp 

OBJS += \
./src/sw/Application.o \
./src/sw/Task.o 

CPP_DEPS += \
./src/sw/Application.d \
./src/sw/Task.d 


# Each subdirectory must supply rules for building sources it contributes
src/sw/%.o: ../src/sw/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/jonah/Dropbox/U5/Research/workspace/CodeGen/includes" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


