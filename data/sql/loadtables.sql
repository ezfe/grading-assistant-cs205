DELETE FROM Annotations;
DELETE FROM Assignments;
DELETE FROM AssignmentData;
DELETE FROM Classes;
DELETE FROM Rubrics;
DELETE FROM RubricRows;
DELETE FROM RubricRowValues;
DELETE FROM Students;

.mode csv
.separator ","
.import ../grading-assistant-cs205/data/mocktables/annotations.csv Annotations
.import ../grading-assistant-cs205/data/mocktables/assignments.csv Assignments
.import ../grading-assistant-cs205/data/mocktables/assignmentdata.csv AssignmentData
.import ../grading-assistant-cs205/data/mocktables/classes.csv Classes
.import ../grading-assistant-cs205/data/mocktables/rubrics.csv Rubrics
.import ../grading-assistant-cs205/data/mocktables/rubricrowvalues.csv RubricRowValues
.import ../grading-assistant-cs205/data/mocktables/students.csv Students

.exit


