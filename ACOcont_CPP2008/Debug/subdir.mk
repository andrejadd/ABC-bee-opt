################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../Functions.o \
../Solution.o \
../Timer.o 

CPP_SRCS += \
../Colony.cpp \
../Functions.cpp \
../Solution.cpp \
../aco-r.cpp 

CC_SRCS += \
../Random.cc \
../Timer.cc 

OBJS += \
./Colony.o \
./Functions.o \
./Random.o \
./Solution.o \
./Timer.o \
./aco-r.o 

CC_DEPS += \
./Random.d \
./Timer.d 

CPP_DEPS += \
./Colony.d \
./Functions.d \
./Solution.d \
./aco-r.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


