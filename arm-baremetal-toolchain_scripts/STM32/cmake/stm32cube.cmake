# If using the stm32cube library this will configure it for inclusion in the
# project.
#
# The following important variables will be created:
#	STM32CUBE_INCLUDE_DIRS	The include dirs for the library
#	STM32CUBE_CFLAGS		Flags for the C compiler
#	STM32CUBE_DEFINITIONS	Defenition for the c preproccesor
#	STM32CUBE_LINKER_FLAGS	Flags for the linker
#	STM32CUBE_LIBRARIES		the libraries that should be linked to
#
# It will also create the following functions:
#	open_ocd_write_flash(elf_file)	Created a target to flash the elf_file using openOCD
#

option(CHIP_FAMILY
	"CHIP_FAMILY defines what chip the library should be build for.
	Can be one of the following:
	STM32F405xx
	STM32F415xx
	STM32F417xx
	STM32F427xx
	STM32F437xx
	STM32F429xx
	STM32F439xx
	STM32F401xC
	STM32F401xE
	STM32F411xE
	STM32F446xx
	STM32F407xx"
)
if(NOT CHIP_FAMILY)
	message(FATAL_ERROR "Must specify a CHIP_FAMILY with -DCHIP_FAMILY=<value>")
endif()

option(CHIP_FAMILY_TYPE "The family type is the last two identifying letters in the chip name" "VG")
if(NOT CHIP_FAMILY_TYPE)
	message(FATAL_ERROR "Must specify a CHIP_FAMILY_TYPE in hz with -DCHIP_FAMILY_TYPE=<value>")
endif()

# First we need to get hold of the stm32cube library. Download it if the doesnt have have it already
option(EXTERNAL_STM32CUBE_PATH "Set this to the path to the stm32cube library, If not specified the library will be downloaded")
IF(NOT EXTERNAL_STM32CUBE_PATH)
	message(STATUS "Downloading stm32cube library")
	set(STM32CUBE_ZIP_PATH "${CMAKE_BINARY_DIR}/stm32cubef4.zip")
	file(DOWNLOAD
	    "http://www.st.com/st-web-ui/static/active/en/st_prod_software_internet/resource/technical/software/firmware/stm32cubef4.zip"
	    ${STM32CUBE_ZIP_PATH}
	    SHOW_PROGRESS
	)

	message(STATUS "Extracting stm32cube library")
	execute_process(
	    COMMAND ${CMAKE_COMMAND} -E tar xzf ${STM32CUBE_ZIP_PATH} # Works with zip files
	    # WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/stm32cube
	)

	# Remove the zip file now that we have extracted it
	file(REMOVE ${STM32CUBE_ZIP_PATH})

	# Get The extracted dir
	file(GLOB STM32CUBE_DIR ${CMAKE_BINARY_DIR}/STM32Cube_FW_F4_V*)
ELSE()
    set(STM32CUBE_DIR ${EXTERNAL_STM32CUBE_PATH})
ENDIF()

set(STM32CUBE_HAL_DIR ${STM32CUBE_DIR}/Drivers/STM32F4xx_HAL_Driver)
set(STM32CUBE_CMSIS_DIR ${STM32CUBE_DIR}/Drivers/CMSIS)

if(NOT EXISTS ${STM32CUBE_HAL_DIR}/Inc/stm32f4xx_hal_conf.h)
	file(RENAME
	    ${STM32CUBE_HAL_DIR}/Inc/stm32f4xx_hal_conf_template.h
	    ${STM32CUBE_HAL_DIR}/Inc/stm32f4xx_hal_conf.h
	)
endif()

# Move all linker scripts to a common folder
file(GLOB_RECURSE STM32CUBE_LINKER_SCRIPTS ${STM32CUBE_DIR}/*.ld)
file(COPY ${STM32CUBE_LINKER_SCRIPTS} DESTINATION ${STM32CUBE_DIR}/linker_scripts)

# These two chips are allmost the same except for the amount of ram.
# change that in the linker file and save it for the new chip
file(READ ${STM32CUBE_DIR}/linker_scripts/STM32F407VG_FLASH.ld STM32F407VG_FLASH_CONTENT)
string(REPLACE "1024K" "512K" STM32F407VE_FLASH_CONTENT "${STM32F407VG_FLASH_CONTENT}")
file(WRITE ${STM32CUBE_DIR}/linker_scripts/STM32F407VE_FLASH.ld "${STM32F407VE_FLASH_CONTENT}")

# All linker scripts are mising a line (fixes link error of undefined reference to __end__)
file(GLOB STM32CUBE_LINKER_SCRIPTS ${STM32CUBE_DIR}/linker_scripts/*.ld) # Note that we overwrite the linker scripts here
foreach(linker_script ${STM32CUBE_LINKER_SCRIPTS})
    file(READ ${linker_script} old_content)
    string(REPLACE
        "    PROVIDE ( _end = . );"
        "    PROVIDE ( _end = . );\n    PROVIDE ( __end__ = . );"
        new_content "${old_content}"
    )
    file(WRITE ${linker_script} "${new_content}")
endforeach()


# Now that the library is ready we pick what we need from it
set(STM32CUBE_INCLUDE_DIRS
    ${STM32CUBE_HAL_DIR}/Inc
    ${STM32CUBE_CMSIS_DIR}/Include
    ${STM32CUBE_CMSIS_DIR}/Device/ST/STM32F4xx/Include
)

# Compile the HAL as a library
file(GLOB STM32CUBE_HAL_SRCS ${STM32CUBE_HAL_DIR}/Src/*.c)
add_library(stm32cube_hal ${STM32CUBE_HAL_SRCS})
target_include_directories(stm32cube_hal PRIVATE ${STM32CUBE_INCLUDE_DIRS})
target_compile_options(stm32cube_hal PRIVATE -Wno-extra) # There is a lot of unused variable warnings in some hal function-prototypes

# Compile the correct startup script
file(GLOB startup_scripts ${STM32CUBE_CMSIS_DIR}/Device/ST/STM32F4xx/Source/Templates/gcc/*.s)
foreach(startup_script ${startup_scripts})
	get_filename_component(ss ${startup_script} NAME_WE)
	string(TOLOWER ${CHIP_FAMILY} cf)

	if(${ss} MATCHES ${cf})
		set(STARTUP_SCRIPT ${startup_script})
		break()
	endif()
endforeach()

add_library(stm32cube_startup
    ${STARTUP_SCRIPT}
    ${STM32CUBE_CMSIS_DIR}/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c
)
target_include_directories(stm32cube_startup PRIVATE ${STM32CUBE_INCLUDE_DIRS})

# Find the correct linker script
foreach(linker_script ${STM32CUBE_LINKER_SCRIPTS})
	get_filename_component(ls ${linker_script} NAME_WE) # Get just the file name so it is easier to match
	string(REGEX REPLACE "xx" "" chip ${CHIP_FAMILY}) # Remove the last xx

    if(${ls} MATCHES ${chip})
        if(${ls} MATCHES ${CHIP_FAMILY_TYPE})
            set(STM32CUBE_LINKER_SCRIPT ${linker_script})
            break()
        endif()
    endif()
endforeach()

set(gc_flags "-ffunction-sections -fdata-sections -Wl,--gc-sections") # These flags helps with dead code elimination. More info can found at http://stackoverflow.com/a/10809541
set(mcu_flags "-mcpu=cortex-m4 -mtune=cortex-m4 -mthumb -mlittle-endian -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb-interwork")
set(linker_flags "-Wl,-T${STM32CUBE_LINKER_SCRIPT} -Wl,-gc-sections -Wl,-LTO")

set(STM32CUBE_CFLAGS "${mcu_flags} ${gc_flags}") #${linker_flags}")
set(STM32CUBE_DEFINITIONS "-D${CHIP_FAMILY}")
set(STM32CUBE_LIBRARIES stm32cube_hal stm32cube_startup ${linker_flags})

# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${STM32CUBE_LINKER_FLAGS}") # Note this has to be set. It is not enough that the linker flags is passed to the CFLAGS


# Make it possible to Flash using openOCD
option(WITHOUT_OPEN_OCD "Should a target to flash using openOCD be made?" OFF)
if(NOT WITHOUT_OPEN_OCD)
	find_program(OPEN_OCD openocd)
	get_filename_component(open_ocd_path ${OPEN_OCD} DIRECTORY)
	set(OPEN_OCD_CONFIG "${open_ocd_path}/../share/openocd/scripts/board/stm32f4discovery.cfg")
	function(open_ocd_write_flash elf_file)
		add_custom_target(${elf_file}_writeflash
			COMMAND ${OPEN_OCD}
						-f ${OPEN_OCD_CONFIG}
						-c "init"
						-c "reset halt"
						-c "flash write_image erase ${elf_file}"
						-c "reset run"
			DEPENDS ${elf_file}
			VERBATIM
		)
	endfunction()
endif()
