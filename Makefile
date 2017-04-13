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

# Saves the current data from the database into "data/tables/"
savedb:
	@echo "=== Saving Database Data ==="
	@rm -rf data/tables/
	@mkdir data/tables/
	@touch data/tables/annotations.csv
	@touch data/tables/assignments.csv
	@touch data/tables/assignmentdata.csv
	@touch data/tables/classes.csv
	@touch data/tables/students.csv
	@touch data/tables/rubrics.csv
	@touch data/tables/rubricrows.csv
	@touch data/tables/rubricrowvalues.csv
	@sqlite3 $(DATAPATH)/database.sqlite3 < data/sql/savetables.sql

ssh:
	ssh spr2017_l2g4@139.147.9.185
