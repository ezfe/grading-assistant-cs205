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

// I left this fixture structure. Not currently using it, but could be useful.
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
    GAAnnotation gaid1, gaid2, gaid3;

    std::string id1= gaid1.get_id();
    std::string id2 = gaid2.get_id();
    std::string id3 = gaid3.get_id();

    GAClass* gac1 = new GAClass(id1, "Test101");
    GAClass* gac2 = new GAClass(id2, "Test102");
    GAClass* gac3 = new GAClass(id3, "Test103");

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
    GAAnnotation gaid1, gaid2, gaid3;

    std::string id1 = gaid1.get_id();
    std::string id2 = gaid2.get_id();
    std::string id3 = gaid3.get_id();

    GAClass* gac1 = new GAClass(id1, "Test101");
    GAClass* gac2 = new GAClass(id2, "Test102");
    GAClass* gac3 = new GAClass(id3, "Test103");

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
    GAClass* class_ = new GAClass();
    GAAssignment* assign1 = new GAAssignment();
    GAAssignment* assign2 = new GAAssignment();
    GAAssignment* assign3 = new GAAssignment();
    class_->add_assignment(assign1);
    class_->add_assignment(assign2);
    class_->add_assignment(assign3);
    ga.add_class(class_);
    std::string rid1, rid2, rid3, rid4;

    GARubric* gar1 = new GARubric("Test 1");
    rid1 = gar1->get_id();
    GARubric* gar2 = new GARubric("Test 2");
    rid2 = gar2->get_id();
    GARubric* gar3 = new GARubric("Test 3");
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
    GAClass* class_ = new GAClass();
    GAAssignment* assign1 = new GAAssignment();
    class_->add_assignment(assign1);
    ga.add_class(class_);
    std::string rid1;

    GARubric* gar1 = new GARubric("Test 1");
    rid1 = gar1->get_id();
    GARubric* gar2 = new GARubric("Test 2");

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

// *** Having trouble with this test
TEST(general, GAAssignmentDataGetCommentsTest) {

    GAAnnotation* gaa1 = new GAAnnotation(GA_ANNOTATION_COMMENT);
    GAAnnotation* gaa2 = new GAAnnotation("GA_ANNOTATION_PROBLEM");
    GAAnnotation* gaa3 = new GAAnnotation("GA_ANNOTATION_EC");
    GAAnnotation* gaa4 = new GAAnnotation(GA_ANNOTATION_COMMENT);

    std::cout << gaa1->get_type() << std::endl;
    gaa1->set_type(GA_ANNOTATION_COMMENT);
    std::cout << gaa1->get_type() << std::endl;

    GAAssignmentData gaad;

    gaad.add_annotation(gaa1);
    gaad.add_annotation(gaa2);
    gaad.add_annotation(gaa3);
    gaad.add_annotation(gaa4);

    std::vector<GAAnnotation*> rtnann = gaad.get_comments();
    std::cout << rtnann.size() <<std::endl;

//    ASSERT_EQ(rtnann.size(), 2)
//            << "Check correct number of GAAnnotations returned";

//    ASSERT_EQ(rtnann.at(0), gaa1)
//            << "Check correct GAAnnotations added";

//    ASSERT_EQ(rtnann.pop_back(), gaa1)
//            << "Check correct GAAnnotations added";
}

TEST(general, GAAssignmentDataGetProblemsTest) {
    //"GA_ANNOTATION_PROBLEM"
}

TEST(general, GAAssignmentDataGetExtraCreditTest) {

    //"GA_ANNOTATION_EC"
}

TEST(general, GAAssignmentDataGetAnnotationsest) {

}

TEST(general, GAAssignmentDataGetByCategoryTest) {

}

TEST(general, GAAssignmentDataCalculateScoreAssignmentTest) {

}

TEST(general, GAAssignmentDataCalculateScoreRubricRowTest) {

}

TEST(general, GAAssignmentDataScoreOverrideTest) {

}

int main(int argc, char **argv) {

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
