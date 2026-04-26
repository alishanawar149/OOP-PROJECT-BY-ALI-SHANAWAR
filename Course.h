#ifndef COURSE_H
#define COURSE_H
#include <string>

using namespace std;

class Course {
private:
    string courseCode;
    string courseTitle;
    string section; 
public:
    Course(string code = "", string title = "", string sec = "");
    string getCode() const;
    string getTitle() const;
    string getSection() const;
    bool operator==(const Course& other) const;
};

#endif