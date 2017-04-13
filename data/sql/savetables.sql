.mode csv
.separator ","

.output ../grading-assistant-cs205/data/tables/annotations.csv

SELECT * FROM Annotations;

.output ../grading-assistant-cs205/data/tables/assignments.csv 

SELECT * FROM Assignments;

.output ../grading-assistant-cs205/data/tables/assignmentdata.csv

SELECT * FROM AssignmentData;

.output ../grading-assistant-cs205/data/tables/classes.csv

SELECT * FROM Classes;

.output ../grading-assistant-cs205/data/tables/rubrics.csv

SELECT * FROM Rubrics;

.output ../grading-assistant-cs205/data/tables/rubricrows.csv

SELECT * FROM RubricRows;

.output ../grading-assistant-cs205/data/tables/rubricrowvalues.csv

SELECT * FROM RubricRowValues;

.output ../grading-assistant-cs205/data/tables/students.csv

SELECT * FROM Students;

.exit


