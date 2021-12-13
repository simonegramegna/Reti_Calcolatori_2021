################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/math_operations.c \
../src/string_parser.c \
../src/udp_server.c 

OBJS += \
./src/math_operations.o \
./src/string_parser.o \
./src/udp_server.o 

C_DEPS += \
./src/math_operations.d \
./src/string_parser.d \
./src/udp_server.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


