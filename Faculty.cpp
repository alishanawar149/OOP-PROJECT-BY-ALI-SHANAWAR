#include "Faculty.h"
#include <iostream>

Faculty::Faculty() : Employee(), courseCount(0) {}
Faculty::Faculty(string n, string e, string p, double s) : Employee(n, e, p, s) { courseCount = 0; }

void Faculty::viewDashboard() const {
    cout << "\n--- Faculty Dashboard ---\nName: " << name << "\nSections Taught:\n";
    for(int i = 0; i < courseCount; i++) {
        cout << "- " << coursesTaught[i].getCode() << " [Section " << coursesTaught[i].getSection() << "] : " << coursesTaught[i].getTitle() << endl;
    }
}

void Faculty::assignCourse(Course c) { 
    if(courseCount < 10) { coursesTaught[courseCount] = c; courseCount++; }
}

bool Faculty::teachesCourse(string code, string sec) const {
    for(int i = 0; i < courseCount; i++) {
        if(coursesTaught[i].getCode() == code && coursesTaught[i].getSection() == sec) return true;
    }
    return false;
}

void Faculty::markAttendance(Student& s, Course c, string date, char status) {
    if(teachesCourse(c.getCode(), s.getSection())) {
        bool enrolled = false;
        for(int i = 0; i < s.getCourseCount(); i++) {
            if(s.getEnrolledCourses()[i].getCode() == c.getCode()) { enrolled = true; break; }
        }
        if(!enrolled) { cout << "[!] Error: Student is not enrolled in " << c.getCode() << "!\n"; return; }

        if (s.getAttendanceCount() < 100) {
            s.attendanceHistory[s.attendanceCount] = AttendanceRecord(date, status, c);
            s.attendanceCount++;
        } else { cout << "[!] Attendance storage full.\n"; }
    } else { cout << "[!] You do not teach this student's section.\n"; }
}

void Faculty::assignGrade(Student& s, Course c, string grade) {
    if(teachesCourse(c.getCode(), s.getSection())) {
        bool enrolled = false;
        for(int i = 0; i < s.getCourseCount(); i++) {
            if(s.getEnrolledCourses()[i].getCode() == c.getCode()) { enrolled = true; break; }
        }
        if(!enrolled) { cout << "[!] Error: Student is not enrolled in " << c.getCode() << "!\n"; return; }

        s.transcript.addGrade(c.getCode(), grade);
        cout << "[+] Grade " << grade << " assigned to " << s.getRollNo() << endl;
    } else { cout << "[!] You do not teach this student's section.\n"; }
}

const Course* Faculty::getCoursesTaught() const { return coursesTaught; }
int Faculty::getCourseCount() const { return courseCount; }