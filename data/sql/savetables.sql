.mode csv
.separator ","
/*
.output ../grading-assistant-cs205/data/tables/annotations.csv

SELECT * FROM Annotations;
*/
.output ../grading-assistant-cs205/data/mocktables/assignments.csv 

SELECT * FROM Assignments;

.output ../grading-assistant-cs205/data/mocktables/assignmentdata.csv

SELECT * FROM AssignmentData;

.output ../grading-assistant-cs205/data/mocktables/classes.csv

SELECT * FROM Classes;

.output ../grading-assistant-cs205/data/mocktables/rubrics.csv

SELECT * FROM Rubrics;

.output ../grading-assistant-cs205/data/mocktables/rubricrows.csv

SELECT * FROM RubricRows;

.output ../grading-assistant-cs205/data/mocktables/rubricrowvalues.csv

SELECT * FROM RubricRowValues;

.output ../grading-assistant-cs205/data/mocktables/students.csv

.exit


