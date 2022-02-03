#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Master_v2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Master_v2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS
SUB_IMAGE_ADDRESS_COMMAND=--image-address $(SUB_IMAGE_ADDRESS)
else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../src/calculations.c ../src/ecan_driver.c ../src/state_and_control.c ../src/main.c ../src/initialization.c ../src/CAN1_mgmt.c ../src/CAN2_mgmt.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1360937237/calculations.o ${OBJECTDIR}/_ext/1360937237/ecan_driver.o ${OBJECTDIR}/_ext/1360937237/state_and_control.o ${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/1360937237/initialization.o ${OBJECTDIR}/_ext/1360937237/CAN1_mgmt.o ${OBJECTDIR}/_ext/1360937237/CAN2_mgmt.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1360937237/calculations.o.d ${OBJECTDIR}/_ext/1360937237/ecan_driver.o.d ${OBJECTDIR}/_ext/1360937237/state_and_control.o.d ${OBJECTDIR}/_ext/1360937237/main.o.d ${OBJECTDIR}/_ext/1360937237/initialization.o.d ${OBJECTDIR}/_ext/1360937237/CAN1_mgmt.o.d ${OBJECTDIR}/_ext/1360937237/CAN2_mgmt.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1360937237/calculations.o ${OBJECTDIR}/_ext/1360937237/ecan_driver.o ${OBJECTDIR}/_ext/1360937237/state_and_control.o ${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/1360937237/initialization.o ${OBJECTDIR}/_ext/1360937237/CAN1_mgmt.o ${OBJECTDIR}/_ext/1360937237/CAN2_mgmt.o

# Source Files
SOURCEFILES=../src/calculations.c ../src/ecan_driver.c ../src/state_and_control.c ../src/main.c ../src/initialization.c ../src/CAN1_mgmt.c ../src/CAN2_mgmt.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/Master_v2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33EP256GM604
MP_LINKER_FILE_OPTION=,--script=p33EP256GM604.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1360937237/calculations.o: ../src/calculations.c  .generated_files/flags/default/50d8b99220efa983063766ffb35d2ee96783dad2 .generated_files/flags/default/cf746db493ae6e9c8dd43321e46753b1122a0084
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/calculations.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/calculations.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/calculations.c  -o ${OBJECTDIR}/_ext/1360937237/calculations.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/calculations.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1360937237/ecan_driver.o: ../src/ecan_driver.c  .generated_files/flags/default/362baeb876dca04f126927ad10cb4cf7828df5ad .generated_files/flags/default/cf746db493ae6e9c8dd43321e46753b1122a0084
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ecan_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ecan_driver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/ecan_driver.c  -o ${OBJECTDIR}/_ext/1360937237/ecan_driver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ecan_driver.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1360937237/state_and_control.o: ../src/state_and_control.c  .generated_files/flags/default/4c7ba8a67122b759eb39b03b06181376fefa5742 .generated_files/flags/default/cf746db493ae6e9c8dd43321e46753b1122a0084
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/state_and_control.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/state_and_control.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/state_and_control.c  -o ${OBJECTDIR}/_ext/1360937237/state_and_control.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/state_and_control.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  .generated_files/flags/default/e73eecb9b249be8440780d4cec94c78f625b296e .generated_files/flags/default/cf746db493ae6e9c8dd43321e46753b1122a0084
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/main.c  -o ${OBJECTDIR}/_ext/1360937237/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1360937237/initialization.o: ../src/initialization.c  .generated_files/flags/default/d073622594342f2db18304d6507b5bffb04cbb3b .generated_files/flags/default/cf746db493ae6e9c8dd43321e46753b1122a0084
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/initialization.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/initialization.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/initialization.c  -o ${OBJECTDIR}/_ext/1360937237/initialization.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/initialization.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1360937237/CAN1_mgmt.o: ../src/CAN1_mgmt.c  .generated_files/flags/default/9f7241b96750c438b1131943054c069e3b725024 .generated_files/flags/default/cf746db493ae6e9c8dd43321e46753b1122a0084
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/CAN1_mgmt.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/CAN1_mgmt.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/CAN1_mgmt.c  -o ${OBJECTDIR}/_ext/1360937237/CAN1_mgmt.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/CAN1_mgmt.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1360937237/CAN2_mgmt.o: ../src/CAN2_mgmt.c  .generated_files/flags/default/a7eb02db2ce75947486b1e2a4d5a8a204ca96ae7 .generated_files/flags/default/cf746db493ae6e9c8dd43321e46753b1122a0084
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/CAN2_mgmt.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/CAN2_mgmt.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/CAN2_mgmt.c  -o ${OBJECTDIR}/_ext/1360937237/CAN2_mgmt.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/CAN2_mgmt.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/_ext/1360937237/calculations.o: ../src/calculations.c  .generated_files/flags/default/431bdcb64818452807196135362635036ad548a7 .generated_files/flags/default/cf746db493ae6e9c8dd43321e46753b1122a0084
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/calculations.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/calculations.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/calculations.c  -o ${OBJECTDIR}/_ext/1360937237/calculations.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/calculations.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1360937237/ecan_driver.o: ../src/ecan_driver.c  .generated_files/flags/default/b7e8dbbad88aa91c4e3d82e43267e5d9ad5faed5 .generated_files/flags/default/cf746db493ae6e9c8dd43321e46753b1122a0084
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ecan_driver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ecan_driver.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/ecan_driver.c  -o ${OBJECTDIR}/_ext/1360937237/ecan_driver.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ecan_driver.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1360937237/state_and_control.o: ../src/state_and_control.c  .generated_files/flags/default/a0e8dbf89f3a4ac6785399d7d52012f1ce2620a0 .generated_files/flags/default/cf746db493ae6e9c8dd43321e46753b1122a0084
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/state_and_control.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/state_and_control.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/state_and_control.c  -o ${OBJECTDIR}/_ext/1360937237/state_and_control.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/state_and_control.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  .generated_files/flags/default/2a3fd3b6a2c293ad099f46109610f04f8aa0ba4f .generated_files/flags/default/cf746db493ae6e9c8dd43321e46753b1122a0084
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/main.c  -o ${OBJECTDIR}/_ext/1360937237/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1360937237/initialization.o: ../src/initialization.c  .generated_files/flags/default/bf9d79cac64958938369acd07b7e3775d0e7c597 .generated_files/flags/default/cf746db493ae6e9c8dd43321e46753b1122a0084
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/initialization.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/initialization.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/initialization.c  -o ${OBJECTDIR}/_ext/1360937237/initialization.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/initialization.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1360937237/CAN1_mgmt.o: ../src/CAN1_mgmt.c  .generated_files/flags/default/a4c884d2f9193dee9cad0c6528efd4c9eb88b899 .generated_files/flags/default/cf746db493ae6e9c8dd43321e46753b1122a0084
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/CAN1_mgmt.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/CAN1_mgmt.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/CAN1_mgmt.c  -o ${OBJECTDIR}/_ext/1360937237/CAN1_mgmt.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/CAN1_mgmt.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1360937237/CAN2_mgmt.o: ../src/CAN2_mgmt.c  .generated_files/flags/default/2ed40e751c69feeca598d21f43f9e44699640dd7 .generated_files/flags/default/cf746db493ae6e9c8dd43321e46753b1122a0084
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/CAN2_mgmt.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/CAN2_mgmt.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/CAN2_mgmt.c  -o ${OBJECTDIR}/_ext/1360937237/CAN2_mgmt.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/CAN2_mgmt.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/Master_v2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/Master_v2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)      -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/Master_v2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/Master_v2.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/Master_v2.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   -mdfp="${DFP_DIR}/xc16" 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
