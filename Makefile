#
# Simple Makefile currently used for testing purposes.
#
# Able to open, create clean, or load mock data to database
# used to Grading Assistant.
#

# Opens database
opendb:
	@echo "=== Opening Database ==="
	@sqlite3 ../.elin-sampsell.grading-assistant/database.sqlite3

# Creates a clean (empty) database
createdb:	
	@echo "=== Creating Tables ==="
	@sqlite3 ../.elin-sampsell.grading-assistant/database.sqlite3 < data/sql/createtables.sql

# Loads the mock data into database. Removed by calling "createdb"
loaddb:
	@echo "=== Loading Test Data ==="
	@sqlite3 ../.elin-sampsell.grading-assistant/database.sqlite3 < data/sql/loadtables.sql


