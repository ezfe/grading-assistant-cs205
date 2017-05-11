#include <iostream>

#include "gtest/gtest.h"
#include <../grading-assistant/filemanager.h>
#include <../grading-assistant/usersettings.h>
#include <../grading-assistant/platform.h>
#include <../grading-assistant/databasemanager.h>
#include <../grading-assistant/databasetable.h>
#include <../grading-assistant/gaoutputfile.h>
#include <../grading-assistant/gradingassistant.h>
#include <../grading-assistant/gadata/gaannotation.h>
#include <../grading-assistant/gadata/gaassignment.h>
#include <../grading-assistant/gadata/gaassignmentdata.h>
#include <../grading-assistant/gadata/gaclass.h>
#include <../grading-assistant/gadata/gaidentifiableobject.h>
#include <../grading-assistant/gadata/garubric.h>
#include <../grading-assistant/gadata/garubricrow.h>
#include <../grading-assistant/gadata/gastudent.h>

// The fixture for testing class GA.
class GATest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  GATest() {
    // You can do set-up work for each test here.

  }

  virtual ~GATest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).

  }
};

// === Begin GradingAssistant Unit-tests === //
TEST(general, GradingAssistantTestGetIDClass) {
    GradingAssistant ga;

    GAClass* gac1 = new GAClass("Test101");
    GAClass* gac2 = new GAClass("Test102");
    GAClass* gac3 = new GAClass("Test103");
    std::string id1 = gac1->get_id();
    std::string id2 = gac2->get_id();
    std::string id3 = gac3->get_id();

    ga.add_class(gac1);
    ga.add_class(gac2);
    ga.add_class(gac3);

    ASSERT_EQ(ga.get_class(id1), gac1)
            << "Positive test for correct GAClass return";

    ASSERT_NE(ga.get_class(id1), gac2)
            << "Negative test for correct GAClass return";

}

TEST(general, GradingAssistantTestRemoveClass) {
    GradingAssistant ga;

    GAClass* gac1 = new GAClass("Test101");
    GAClass* gac2 = new GAClass("Test102");
    GAClass* gac3 = new GAClass("Test103");
    std::string id1 = gac1->get_id();
    std::string id2 = gac2->get_id();
    std::string id3 = gac3->get_id();

    ga.add_class(gac1);
    ga.add_class(gac2);
    ga.add_class(gac3);

    ASSERT_EQ(ga.get_class(id1), gac1)
            << "GAClass exists before removal";

    ga.remove_class(gac1);

    ASSERT_EQ(ga.get_class(id1), nullptr)
            << "GAClass no longer exists after removal";

}

TEST(general, GradingAssistantTestGetRubric) {
    GradingAssistant ga;
    GAClass* class_ = new GAClass("CS205");
    GAAssignment* assign1 = new GAAssignment();
    GAAssignment* assign2 = new GAAssignment();
    GAAssignment* assign3 = new GAAssignment();
    class_->add_assignment(assign1);
    class_->add_assignment(assign2);
    class_->add_assignment(assign3);
    ga.add_class(class_);
    std::string rid1, rid2, rid3, rid4;

    GARubric* gar1 = new GARubric("Test 1");
    GARubric* gar2 = new GARubric("Test 2");
    GARubric* gar3 = new GARubric("Test 3");
    rid1 = gar1->get_id();
    rid2 = gar2->get_id();
    rid3 = gar3->get_id();

    GARubric* gar4 = new GARubric("Test 4");
    rid4 = gar4->get_id();

    assign1->set_rubric(gar1);
    assign2->set_rubric(gar2);
    assign3->set_rubric(gar3);

    ASSERT_EQ(ga.get_rubric(rid2), gar2)
            << "Positive test for correct Rubric return";
    ASSERT_EQ(ga.get_rubric(rid4), nullptr)
            << "Test for non-existant Rubric return";
}

TEST(general, GradingAssistantTestRemoveRubric) {

    GradingAssistant ga;
    GAClass* class_ = new GAClass("CS205");
    GAAssignment* assign1 = new GAAssignment();
    class_->add_assignment(assign1);
    ga.add_class(class_);
    std::string rid1;

    GARubric* gar1 = new GARubric("Test 1");
    GARubric* gar2 = new GARubric("Test 2");
    rid1 = gar1->get_id();

    assign1->set_rubric(gar1);

    ASSERT_EQ(ga.get_rubric(rid1), gar1)
            << "Positive test for correct Rubric return";

    assign1->set_rubric(gar2, false);

    ASSERT_EQ(ga.get_rubric(rid1), nullptr)
            << "Negative test for correct Rubric return";
}
// === End GradingAssistant Unit-tests === //

// === Begin GAAnnotation Unit-tests === //
TEST(general, GAAnnotationTestGetAssignmentData) {

    GAAnnotation gaan("Test");

    GAAssignmentData* gaad1 = new GAAssignmentData();
    GAAssignmentData* gaad2 = new GAAssignmentData();

    gaan.set_assignment_data(gaad1);

    ASSERT_EQ(gaan.get_assignment_data(), gaad1)
            << "Positive test for GAAssignmentData return";

    ASSERT_NE(gaan.get_assignment_data(), gaad2)
            << "Negative test for GAAssignmentData return";
}
// === End GAAnnotation Unit-tests === //

// === Begin GAAssignment Unit-tests === //
TEST(general, GAAssignmentTestGetRubric) {

    GAAssignment gaa;

    GARubric* gar1 = new GARubric("Test 01");
    GARubric* gar2 = new GARubric("Test 02");

    gaa.set_rubric(gar1);

    ASSERT_EQ(gaa.get_rubric(), gar1)
            << "Positive test for correct GARubric return";

    gaa.set_rubric(gar2);

    ASSERT_EQ(gaa.get_rubric(), gar2)
            << "Negative test for correct GARubric return";
}
// === End GAAssignment Unit-tests === //

// === Begin GAAssignmentData Unit-tests === //
TEST(general, GAAssignmentDataGetGAAssignmentTest) {

    GAAssignment* gaa1 = new GAAssignment();
    GAAssignment* gaa2 = new GAAssignment();

    GAAssignmentData gaad;

    gaad.set_assignment(gaa1);

    ASSERT_EQ(gaad.get_assignment(), gaa1)
            << "Positive test for correct GAAssignment return";

    gaad.set_assignment(gaa2);

    ASSERT_NE(gaad.get_assignment(), gaa1)
            << "Negative test for correct GAAssignment return";
}

TEST(general, GAAssignmentDataGetStudentTest) {

    GAStudent* gas1 = new GAStudent("Test Name1", "nametest1");
    GAStudent* gas2 = new GAStudent("Test Name2", "nametest2");

    GAAssignmentData gaad;

    gaad.set_student(gas1);

    ASSERT_EQ(gaad.get_student(), gas1)
            << "Positive test for correct GAStudent return";

    gaad.set_student(gas2);

    ASSERT_NE(gaad.get_student(), gas1)
            << "Negative test for correct GAStudent return";
}

TEST(general, GAAssignmentDataGetCommentsTest) {
    GAAnnotation* gaa1 = new GAAnnotation(GA_ANNOTATION_COMMENT);
    GAAnnotation* gaa2 = new GAAnnotation(GA_ANNOTATION_PROBLEM);
    GAAnnotation* gaa3 = new GAAnnotation(GA_ANNOTATION_EXTRACREDIT);
    GAAnnotation* gaa4 = new GAAnnotation(GA_ANNOTATION_COMMENT);

    GAAssignmentData gaad;

    gaad.add_annotation(gaa1);
    gaad.add_annotation(gaa2);
    gaad.add_annotation(gaa3);
    gaad.add_annotation(gaa4);

    std::vector<GAAnnotation*> comments = gaad.get_comments();

    ASSERT_EQ(comments.size(), 2)
            << "Check correct number of GAAnnotations returned";
    ASSERT_EQ(comments.at(0), gaa1)
            << "Check correct GAAnnotations added";
    ASSERT_EQ(comments.at(1), gaa4)
            << "Check correct GAAnnotations added";
}

TEST(general, GAAssignmentDataGetProblemsTest) {
    GAAnnotation* gaa1 = new GAAnnotation(GA_ANNOTATION_COMMENT);
    GAAnnotation* gaa2 = new GAAnnotation(GA_ANNOTATION_PROBLEM);
    GAAnnotation* gaa3 = new GAAnnotation(GA_ANNOTATION_EXTRACREDIT);
    GAAnnotation* gaa4 = new GAAnnotation(GA_ANNOTATION_COMMENT);

    GAAssignmentData gaad;

    gaad.add_annotation(gaa1);
    gaad.add_annotation(gaa2);
    gaad.add_annotation(gaa3);
    gaad.add_annotation(gaa4);

    std::vector<GAAnnotation*> problems = gaad.get_problems();

    ASSERT_EQ(problems.size(), 1)
            << "Check correct number of GAAnnotations returned";
    ASSERT_EQ(problems.at(0), gaa2)
            << "Check correct GAAnnotations added";
}

TEST(general, GAAssignmentDataGetExtraCreditTest) {
    GAAnnotation* gaa1 = new GAAnnotation(GA_ANNOTATION_COMMENT);
    GAAnnotation* gaa2 = new GAAnnotation(GA_ANNOTATION_PROBLEM);
    GAAnnotation* gaa3 = new GAAnnotation(GA_ANNOTATION_EXTRACREDIT);
    GAAnnotation* gaa4 = new GAAnnotation(GA_ANNOTATION_COMMENT);

    GAAssignmentData gaad;

    gaad.add_annotation(gaa1);
    gaad.add_annotation(gaa2);
    gaad.add_annotation(gaa3);
    gaad.add_annotation(gaa4);

    std::vector<GAAnnotation*> extra = gaad.get_extra_credit();

    ASSERT_EQ(extra.size(), 1)
            << "Check correct number of GAAnnotations returned";
    ASSERT_EQ(extra.at(0), gaa3)
            << "Check correct GAAnnotations added";
}

TEST(general, GAAssignmentDataGetByTypeTest) {
    GAAnnotation* gaa1 = new GAAnnotation(GA_ANNOTATION_COMMENT);
    GAAnnotation* gaa2 = new GAAnnotation(GA_ANNOTATION_PROBLEM);
    GAAnnotation* gaa3 = new GAAnnotation(GA_ANNOTATION_EXTRACREDIT);
    GAAnnotation* gaa4 = new GAAnnotation(GA_ANNOTATION_COMMENT);

    GAAssignmentData gaad;

    gaad.add_annotation(gaa1);
    gaad.add_annotation(gaa2);
    gaad.add_annotation(gaa3);
    gaad.add_annotation(gaa4);

    std::vector<GAAnnotation*> comments = gaad.get_by_type(GA_ANNOTATION_COMMENT);

    ASSERT_EQ(comments.size(), 2)
            << "Check correct number of GAAnnotations returned";
    ASSERT_EQ(comments.at(0), gaa1)
            << "Check correct GAAnnotations [Comment 1]";
    ASSERT_EQ(comments.at(1), gaa4)
            << "Check correct GAAnnotations [Comment 2]";
}

TEST(general, GAAssignmentDataGetByCategoryTest) {
    GAAnnotation* gaa1 = new GAAnnotation(GA_ANNOTATION_COMMENT);
    GAAnnotation* gaa2 = new GAAnnotation(GA_ANNOTATION_PROBLEM);
    GAAnnotation* gaa3 = new GAAnnotation(GA_ANNOTATION_EXTRACREDIT);
    GAAnnotation* gaa4 = new GAAnnotation(GA_ANNOTATION_COMMENT);

    GAAssignmentData gaad;

    gaad.add_annotation(gaa1);
    gaad.add_annotation(gaa2);
    gaad.add_annotation(gaa3);
    gaad.add_annotation(gaa4);

    std::vector<GAAnnotation*> comments = gaad.get_by_category(GA_ANNOTATION_COMMENT);

    ASSERT_EQ(comments.size(), 2)
            << "Check correct number of GAAnnotations returned";
    ASSERT_EQ(comments.at(0), gaa1)
            << "Check correct GAAnnotations [Comment 1]";
    ASSERT_EQ(comments.at(1), gaa4)
            << "Check correct GAAnnotations [Comment 2]";
}

TEST(general, GAAssignmentDataGetAnnotationsTest) {
    GAAnnotation* gaa1 = new GAAnnotation(GA_ANNOTATION_COMMENT);
    GAAnnotation* gaa2 = new GAAnnotation(GA_ANNOTATION_PROBLEM);
    GAAnnotation* gaa3 = new GAAnnotation(GA_ANNOTATION_EXTRACREDIT);
    GAAnnotation* gaa4 = new GAAnnotation(GA_ANNOTATION_COMMENT);

    GAAssignmentData gaad;

    gaad.add_annotation(gaa1);
    gaad.add_annotation(gaa2);
    gaad.add_annotation(gaa3);
    gaad.add_annotation(gaa4);

    std::vector<GAAnnotation*> annots = gaad.get_annotations();

    ASSERT_EQ(annots.size(), 4)
            << "Check correct number of GAAnnotations returned";
    ASSERT_EQ(annots.at(0), gaa1)
            << "Check correct GAAnnotations [Comment]";
    ASSERT_EQ(annots.at(1), gaa2)
            << "Check correct GAAnnotations [Problem]";
    ASSERT_EQ(annots.at(2), gaa3)
            << "Check correct GAAnnotations [Extra Credit]";
    ASSERT_EQ(annots.at(3), gaa4)
            << "Check correct GAAnnotations [Comment]";
}

TEST(general, GAAssignmentDataGetLineNumbersTest) {
    GAAnnotation* gaa1 = new GAAnnotation(GA_ANNOTATION_COMMENT);
    GAAnnotation* gaa2 = new GAAnnotation(GA_ANNOTATION_PROBLEM);
    GAAnnotation* gaa3 = new GAAnnotation(GA_ANNOTATION_EXTRACREDIT);
    GAAnnotation* gaa4 = new GAAnnotation(GA_ANNOTATION_COMMENT);

    GAAssignmentData gaad;

    gaad.add_annotation(gaa1);
    gaad.add_annotation(gaa2);
    gaad.add_annotation(gaa3);
    gaad.add_annotation(gaa4);

    // Set similar file for 2/4 annotations
    gaa1->set_filename("test1");
    gaa2->set_filename("test1");

    gaa3->set_filename("test2");
    gaa4->set_filename("test2");

    gaa1->set_line(11);
    gaa2->set_line(22);
    gaa3->set_line(33);
    gaa4->set_line(44);

    std::vector<int> nums1 = gaad.get_line_numbers("test1");
    std::vector<int> nums2 = gaad.get_line_numbers("test2");

    ASSERT_EQ(nums1.at(0), 11)
            << "Check correct line number for file returned";
    ASSERT_EQ(nums1.at(1), 22)
            << "Check correct line number for file returned";
    ASSERT_EQ(nums2.at(0), 33)
            << "Check correct line number for file returned";
    ASSERT_EQ(nums2.at(1), 44)
            << "Check correct line number for file returned";
}

TEST(general, GAAssignmentDataGetAnnotationNonExistentTest) {
    GAAnnotation* gaa1 = new GAAnnotation(GA_ANNOTATION_COMMENT);
    GAAnnotation* gaa2 = new GAAnnotation(GA_ANNOTATION_PROBLEM);
    GAAnnotation* gaa3 = new GAAnnotation(GA_ANNOTATION_EXTRACREDIT);
    GAAnnotation* gaa4 = new GAAnnotation(GA_ANNOTATION_COMMENT);

    GAAssignmentData gaad;

    gaad.add_annotation(gaa1);
    gaad.add_annotation(gaa2);
    gaad.add_annotation(gaa3);
    gaad.add_annotation(gaa4);

    // Set similar file for 2/4 annotations
    gaa1->set_filename("test1");
    gaa2->set_filename("test1");

    gaa3->set_filename("test2");
    gaa4->set_filename("test2");

    gaa1->set_line(11);
    gaa2->set_line(22);
    gaa3->set_line(33);
    gaa4->set_line(44);

    ASSERT_EQ(gaad.get_annotation("test0", 11), nullptr)
            << "Check return when annotation is non-existent";
    ASSERT_EQ(gaad.get_annotation("test2", 33), gaa3)
            << "Check return when annotation exists";
}

TEST(general, GAAssignmentDataScoreOverrideTest) {
    GAAssignmentData gaad;

    gaad.override_score(5);

    ASSERT_EQ(gaad.calculate_score(), 5)
            << "Check that score is overridden.";

    gaad.override_score(0);

    ASSERT_EQ(gaad.calculate_score(), 0)
            << "Check that score is overridden.";
}

TEST(general, GAAssignmentDataResetScoreTest) {
    GAAssignmentData gaad;

    gaad.override_score(5);

    ASSERT_EQ(gaad.calculate_score(), 5)
            << "Check that score is overridden.";

    gaad.reset_score();

    ASSERT_EQ(gaad.is_overriden(), false)
            << "Check that score is overridden.";
}

TEST(general, GAAssignmentDataIsOverriddenTest) {
    GAAssignmentData gaad;
    gaad.reset_score();

    ASSERT_EQ(gaad.is_overriden(), false)
            << "Check that score is overridden.";

    gaad.override_score(5);

    ASSERT_EQ(gaad.is_overriden(), true)
            << "Check that score is overridden.";
}

// === End GAAssignment Unit-tests === //

// === Begin GAClass Unit-tests === //
TEST(general, GAClassAddStudentClassTest) {

    GAClass gac("CS205");

    GAStudent* gas1 = new GAStudent("Student 1", "onestudent");

    std::vector<GAStudent*> students = gac.get_students();

    ASSERT_EQ(students.size(), 0)
            << "Check that no students exist";

    gac.add_student(gas1);
    students = gac.get_students();

    ASSERT_EQ(students.size(), 1)
            << "Check student is added";

}

TEST(general, GAClassAddAssignmentTest) {

    GAClass gac("CS205");

    GAAssignment* gaa1 = new GAAssignment();

    std::vector<GAAssignment*> assigns = gac.get_assignments();

    ASSERT_EQ(assigns.size(), 0)
            << "Check that no assignments exist";

    gac.add_assignment(gaa1);
    assigns = gac.get_assignments();

    ASSERT_EQ(assigns.size(), 1)
            << "Check assignment is added";

}

// === End GAClass Unit-tests === //

int main(int argc, char **argv) {

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
