#ifndef TRANSCRIPT_H
#define TRANSCRIPT_H
#include <string>
#include "Course.h"

using namespace std;

class Transcript {
private:
    string courseCodes[5];
    string grades[5];
    int gradeCount;
public:
    Transcript();
    void addGrade(string courseCode, string grade);
    string getGrade(string courseCode) const;
    void displayTranscript(const Course enrolledCourses[], int courseCount) const;
};

#endif