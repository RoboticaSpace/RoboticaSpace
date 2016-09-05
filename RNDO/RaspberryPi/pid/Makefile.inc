###############################################################################
##
##  $Workfile: Makefile.inc$
##
##  $Revision: 1$
##
##  Project:   Ibbottanator
##
##                           Copyright (c) 2016
##                             James A Wright
##                           All Rights Reserved
##
##    This software may not be reproduced, in part or in its entirety,
##       without the express written permission of James A Wright.
##
##  File Description:
##      GCC make include file for the Application.
##
##  Notes: 
##
##
##  Modification History:
##  $Log:
##  $
##
###############################################################################

.PHONY : all clean
                
SRC_DIR         = .
BIN_DIR         = .
APP_COMMON_DIR  = ./src/common
APP_OP_DIR      = ./src/op
APP_HW_DIR      = ./src/hw
TEST_DIR        = ./test
TEST_OP_DIR     = ./test/op/op
OBJ_DIR         = ./bins
                  
INC =   -I. \
        -I$(APP_COMMON_DIR) \
        -I$(APP_HW_DIR) \
        -I$(APP_OP_DIR) \
        -I$(TEST_DIR)/cppunit/include

CC       = g++
C        = gcc
LD       = gcc
AR       = ar
COPY     = cp
COVER    = gcov
PROFILE  = gprof
PERL     = perl
TODOS    = todos

CFLAGS   = -Wall \
           -Wcomment \
           -ansi \
           -std=c99 \
           -fprofile-arcs \
           -ftest-coverage \
           -pg

CCFLAGS  = -Wall \
           -Wcomment \
           -ansi \
           -fprofile-arcs \
           -ftest-coverage \
           -pg

OBJCPYFLAGS = --output-target=srec
