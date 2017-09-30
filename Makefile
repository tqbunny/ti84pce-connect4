# ----------------------------
# Set NAME to the program name
# Set DEBUGMODE to "DEBUG" to use debug functions
# Set ICON to the png icon file name
# Set DESCRIPTION to display within a compatible shell
# Set COMPRESSED to "YES" to create a compressed program
# ** Add all shared library names to L **
# ----------------------------

NAME        ?= CONNECT4
DEBUGMODE   ?= NDEBUG
COMPRESSED  ?= YES
ICON        ?= iconc.png
DESCRIPTION ?= "Connect 4 for TI-84 Plus CE"

L ?= graphx fileioc keypadc

# ----------------------------
# Specify source and output locations
# ----------------------------

SRCDIR ?= source
OBJDIR ?= obj
BINDIR ?= bin
GFXDIR ?= data

# ----------------------------
# Use OS helper functions (Advanced)
# ----------------------------

USE_FLASH_FUNCTIONS ?= YES

include $(CEDEV)/include/.makefile
