#!/bin/bash -x

#
# Generated - do not edit!
#

# Macros
TOP=`pwd`
CND_CONF=default
CND_DISTDIR=dist
TMPDIR=build/${CND_CONF}/${IMAGE_TYPE}/tmp-packaging
TMPDIRNAME=tmp-packaging
<<<<<<<< HEAD:AMS_Master/AMS_Master_SW/AMS_Master.X/nbproject/Package-default.bash
OUTPUT_PATH=dist/${CND_CONF}/${IMAGE_TYPE}/AMS_Master.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
OUTPUT_BASENAME=AMS_Master.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
PACKAGE_TOP_DIR=amsmaster.x/
========
OUTPUT_PATH=dist/${CND_CONF}/${IMAGE_TYPE}/Final_program.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
OUTPUT_BASENAME=Final_program.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
PACKAGE_TOP_DIR=finalprogram.x/
>>>>>>>> main:AMS_Master/AMS_Master_SW/Final_program.X/nbproject/Package-default.bash

# Functions
function checkReturnCode
{
    rc=$?
    if [ $rc != 0 ]
    then
        exit $rc
    fi
}
function makeDirectory
# $1 directory path
# $2 permission (optional)
{
    mkdir -p "$1"
    checkReturnCode
    if [ "$2" != "" ]
    then
      chmod $2 "$1"
      checkReturnCode
    fi
}
function copyFileToTmpDir
# $1 from-file path
# $2 to-file path
# $3 permission
{
    cp "$1" "$2"
    checkReturnCode
    if [ "$3" != "" ]
    then
        chmod $3 "$2"
        checkReturnCode
    fi
}

# Setup
cd "${TOP}"
mkdir -p ${CND_DISTDIR}/${CND_CONF}/package
rm -rf ${TMPDIR}
mkdir -p ${TMPDIR}

# Copy files and create directories and links
cd "${TOP}"
<<<<<<<< HEAD:AMS_Master/AMS_Master_SW/AMS_Master.X/nbproject/Package-default.bash
makeDirectory ${TMPDIR}/amsmaster.x/bin
========
makeDirectory ${TMPDIR}/finalprogram.x/bin
>>>>>>>> main:AMS_Master/AMS_Master_SW/Final_program.X/nbproject/Package-default.bash
copyFileToTmpDir "${OUTPUT_PATH}" "${TMPDIR}/${PACKAGE_TOP_DIR}bin/${OUTPUT_BASENAME}" 0755


# Generate tar file
cd "${TOP}"
<<<<<<<< HEAD:AMS_Master/AMS_Master_SW/AMS_Master.X/nbproject/Package-default.bash
rm -f ${CND_DISTDIR}/${CND_CONF}/package/amsmaster.x.tar
cd ${TMPDIR}
tar -vcf ../../../../${CND_DISTDIR}/${CND_CONF}/package/amsmaster.x.tar *
========
rm -f ${CND_DISTDIR}/${CND_CONF}/package/finalprogram.x.tar
cd ${TMPDIR}
tar -vcf ../../../../${CND_DISTDIR}/${CND_CONF}/package/finalprogram.x.tar *
>>>>>>>> main:AMS_Master/AMS_Master_SW/Final_program.X/nbproject/Package-default.bash
checkReturnCode

# Cleanup
cd "${TOP}"
rm -rf ${TMPDIR}
