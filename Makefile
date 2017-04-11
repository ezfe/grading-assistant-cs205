#
# Simple Makefile currently used for testing purposes.
#
# Able to open, create clean, or load mock data to database
# used to Grading Assistant.
#

ifeq ($(OS),Windows_NT)
    uname_S := Windows
else
    uname_S := $(shell uname -s)
endif

ifeq ($(uname_S), Windows)
    DATAPATH = ./elin-sampsell.grading-assistant
endif
ifeq ($(uname_S), Darwin)
    DATAPATH = ~/Library/Application\ Support/elin-sampsell.grading-assistant
endif
ifeq ($(uname_S), Linux)
    DATAPATH = ~/.elin-sampsell.grading-assistant
endif

# Opens database
opendb:
	@echo "=== Opening Database ==="
	@sqlite3 $(DATAPATH)/database.sqlite3

# Creates a clean (empty) database
createdb:	
	@echo "=== Creating Tables ==="
	@sqlite3 $(DATAPATH)/database.sqlite3 < data/sql/createtables.sql

# Loads the mock data into database. Removed by calling "createdb"
loaddb:
	@echo "=== Loading Test Data ==="
	@sqlite3 $(DATAPATH)/database.sqlite3 < data/sql/loadtables.sql


