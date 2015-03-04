################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/hw/Bus.cpp \
../src/hw/BusBridge.cpp \
../src/hw/Comparator.cpp \
../src/hw/DMA.cpp \
../src/hw/Fprint.cpp \
../src/hw/MPU.cpp \
../src/hw/Memory.cpp \
../src/hw/Platform.cpp \
../src/hw/Port.cpp \
../src/hw/Processor.cpp \
../src/hw/TLB.cpp \
../src/hw/Timer.cpp \
../src/hw/Wire.cpp 

OBJS += \
./src/hw/Bus.o \
./src/hw/BusBridge.o \
./src/hw/Comparator.o \
./src/hw/DMA.o \
./src/hw/Fprint.o \
./src/hw/MPU.o \
./src/hw/Memory.o \
./src/hw/Platform.o \
./src/hw/Port.o \
./src/hw/Processor.o \
./src/hw/TLB.o \
./src/hw/Timer.o \
./src/hw/Wire.o 

CPP_DEPS += \
./src/hw/Bus.d \
./src/hw/BusBridge.d \
./src/hw/Comparator.d \
./src/hw/DMA.d \
./src/hw/Fprint.d \
./src/hw/MPU.d \
./src/hw/Memory.d \
./src/hw/Platform.d \
./src/hw/Port.d \
./src/hw/Processor.d \
./src/hw/TLB.d \
./src/hw/Timer.d \
./src/hw/Wire.d 


# Each subdirectory must supply rules for building sources it contributes
src/hw/%.o: ../src/hw/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/jonah/Dropbox/U5/Research/workspace/CodeGen/includes" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


