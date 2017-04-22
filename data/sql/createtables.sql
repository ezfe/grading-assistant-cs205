DROP TABLE IF EXISTS Annotations;
DROP TABLE IF EXISTS Assignments;
DROP TABLE IF EXISTS AssignmentData;
DROP TABLE IF EXISTS Classes;
DROP TABLE IF EXISTS Rubrics;
DROP TABLE IF EXISTS RubricRows;
DROP TABLE IF EXISTS RubricRowValues;
DROP TABLE IF EXISTS Students;

CREATE TABLE Annotations (id TEXT PRIMARY KEY NOT NULL,
       			  assignment_data TEXT,
		          type TEXT,
		       	  title TEXT,
		       	  description TEXT,
		       	  category TEXT,
		       	  points INT,
		       	  filename TEXT,
		       	  line INT);

CREATE TABLE Assignments (id TEXT PRIMARY KEY NOT NULL,
       	                  title TEXT,
		       	          description TEXT,
		                  class TEXT,
			  	          rubric TEXT UNIQUE);

CREATE TABLE AssignmentData (id TEXT PRIMARY KEY NOT NULL,
       	                     student TEXT,
		       	     assignment TEXT,
		       	     manual_score INT);

CREATE TABLE Classes (id TEXT PRIMARY KEY NOT NULL,
       	              name TEXT);

CREATE TABLE Rubrics (id TEXT PRIMARY KEY NOT NULL, 
	              title TEXT);

CREATE TABLE RubricRows (id TEXT PRIMARY KEY NOT NULL,
       	                 category TEXT,
		       	 total_points INT,
		       	 rubric TEXT,
		       	 extra_credit INT);

CREATE TABLE RubricRowValues (id INT NOT NULL,
       	                      value TEXT,
		       	      rubric_row TEXT NOT NULL);

CREATE TABLE Students (id TEXT PRIMARY KEY NOT NULL,
       	               name TEXT,
	       	       lafayette_username TEXT,
	       	       class TEXT);
.exit


