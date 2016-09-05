###############################################################################
##
##  $Workfile: Makefile$
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
##      GCC make file for the Application.
##
##  Notes: 
##
##
##  Modification History:
##  $Log:
##  $
##
###############################################################################

.PHONY : all clean pid

include Makefile.inc


#############################################################################
# The main Targets
#############################################################################

all : pid

pid :
	$(CC) $(CCFLAGS) $(INC) -c -o $(BIN_DIR)/PIDTest.o ./PIDTest.cpp
	$(CC) $(CCFLAGS) $(INC) -c -o $(BIN_DIR)/Utils.o ./Utils.cpp
	$(CC) $(CCFLAGS) $(INC) -c -o $(BIN_DIR)/LinuxUtils.o ./LinuxUtils.cpp
	$(CC) $(CCFLAGS) $(INC) -c -o $(BIN_DIR)/RNDOMessage.o ./RNDOMessage.cpp
	$(CC) $(CCFLAGS) $(INC) -c -o $(BIN_DIR)/RNDOTransmitClient.o ./RNDOTransmitClient.cpp


	$(CC) $(CCFLAGS) $(INC) -o $(BIN_DIR)/pid \
    $(BIN_DIR)/PIDTest.o \
    $(BIN_DIR)/Utils.o \
    $(BIN_DIR)/LinuxUtils.o \
    $(BIN_DIR)/RNDOMessage.o \
    $(BIN_DIR)/RNDOTransmitClient.o \
    -lwiringPi -lpthread

clean :
	@rm -f CommTest
	@rm -f output.html
	@rm -f *.o
	@rm -f *.g*
	@rm -f *.out
	@rm -f *.coverout


