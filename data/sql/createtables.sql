DROP TABLE IF EXISTS Annotations;
DROP TABLE IF EXISTS Assignments;
DROP TABLE IF EXISTS AssignmentData;
DROP TABLE IF EXISTS Classes;
DROP TABLE IF EXISTS Rubrics;
DROP TABLE IF EXISTS RubricRows;
DROP TABLE IF EXISTS RubricRowValues;
DROP TABLE IF EXISTS Students;

CREATE TABLE Annotations (id TEXT PRIMARY KEY,
       			  assignment_data TEXT,
		          type TEXT,
		       	  title TEXT,
		       	  description TEXT,
		       	  category TEXT,
		       	  filename TEXT,
		       	  line INT);

CREATE TABLE Assignments (id TEXT PRIMARY KEY,
       	                  title TEXT,
		       	          description TEXT,
		                  class TEXT,
			  	          rubric TEXT UNIQUE);

CREATE TABLE AssignmentData (id TEXT PRIMARY KEY,
       	                     student TEXT,
		       	     assignment TEXT,
		       	     manual_score INT);

CREATE TABLE Classes (id TEXT PRIMARY KEY,
       	              name TEXT);

CREATE TABLE Rubrics (id TEXT PRIMARY KEY, 
	              title TEXT);

CREATE TABLE RubricRows (id TEXT PRIMARY KEY,
       	                 category TEXT,
		       	 total_points INT,
		       	 rubric TEXT,
		       	 extra_credit INT);

CREATE TABLE RubricRowValues (id INT PRIMARY KEY,
       	                      value TEXT,
		       	      rubric_row TEXT);

CREATE TABLE Students (id TEXT PRIMARY KEY,
       	               name TEXT,
	       	       lafayette_username TEXT,
	       	       class TEXT);
.exit


