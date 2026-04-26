#include "Course.h"

Course::Course(string code, string title, string sec) : courseCode(code), courseTitle(title), section(sec) {}

string Course::getCode() const { return courseCode; }
string Course::getTitle() const { return courseTitle; }
string Course::getSection() const { return section; }

bool Course::operator==(const Course& other) const { 
    return this->courseCode == other.courseCode; 
}