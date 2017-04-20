DROP TABLE IF EXISTS Annotations;
DROP TABLE IF EXISTS Assignments;
DROP TABLE IF EXISTS AssignmentData;
DROP TABLE IF EXISTS Classes;
DROP TABLE IF EXISTS Rubrics;
DROP TABLE IF EXISTS RubricRows;
DROP TABLE IF EXISTS RubricRowValues;
DROP TABLE IF EXISTS Students;

CREATE TABLE Annotations (id TEXT NOT NULL UNIQUE,
       			  assignment_data TEXT,
		          type TEXT,
		       	  title TEXT,
		       	  description TEXT,
		       	  category TEXT,
		       	  filename TEXT,
		       	  line INT);

CREATE TABLE Assignments (id TEXT NOT NULL UNIQUE,
       	                  title TEXT,
		       	          description TEXT,
		                  class TEXT,
			  	          rubric TEXT UNIQUE);

CREATE TABLE AssignmentData (id TEXT NOT NULL UNIQUE,
       	                     student TEXT,
		       	     assignment TEXT,
		       	     manual_score INT);

CREATE TABLE Classes (id TEXT NOT NULL UNIQUE,
       	              name TEXT);

CREATE TABLE Rubrics (id TEXT NOT NULL UNIQUE, 
	              title TEXT);

CREATE TABLE RubricRows (id TEXT NOT NULL UNIQUE,
       	                 category TEXT,
		       	 total_points INT,
		       	 rubric TEXT,
		       	 extra_credit INT);

CREATE TABLE RubricRowValues (id TEXT NOT NULL,
       	                      value TEXT,
		       	      rubric_row TEXT);

CREATE TABLE Students (id TEXT NOT NULL UNIQUE,
       	               name TEXT,
	       	       lafayette_username TEXT,
	       	       class TEXT);
.exit


