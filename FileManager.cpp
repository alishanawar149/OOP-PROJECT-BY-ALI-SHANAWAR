#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

void cleanStr(string& s) {
    while(!s.empty() && (s.back() == '\r' || s.back() == ' ' || s.back() == '\n')) s.pop_back();
    while(!s.empty() && (s.front() == ' ')) s.erase(0, 1);
}

string getCourseTitleHelper(string code) {
    if(code == "CL1004") return "Object Oriented Programming";
    if(code == "EE1005") return "Digital Logic Design";
    if(code == "MT1008") return "Multivariable Calculus";
    if(code == "SS1014") return "Expository Writing";
    if(code == "SS1015") return "Pakistan Studies";
    return "Unknown Course";
}

void FileManager::loadStudents(const string& filename, Student students[], int& studentCount) {
    studentCount = 0; 
    try {
        ifstream file(filename);
        if (!file.is_open()) throw runtime_error("Unable to open " + filename + " for reading.");
        
        string line, name, roll, email, pass, feeStr, secStr, coursesStr;
        while (getline(file, line) && studentCount < 100) {
            cleanStr(line); stringstream ss(line);
            getline(ss, name, ','); getline(ss, roll, ','); getline(ss, email, ',');
            getline(ss, pass, ','); getline(ss, feeStr, ','); getline(ss, secStr, ','); 
            getline(ss, coursesStr, ',');

            Student s(name, roll, email, pass, stod(feeStr), secStr);
            
            stringstream cs(coursesStr); string courseData;
            while(getline(cs, courseData, ';')) {
                cleanStr(courseData); if(courseData.empty()) continue;
                stringstream cds(courseData); string cCode, grade, attStr;
                
                getline(cds, cCode, ':'); cleanStr(cCode);
                getline(cds, grade, ':'); cleanStr(grade);
                getline(cds, attStr, ':'); cleanStr(attStr);
                
                Course c(cCode, getCourseTitleHelper(cCode), secStr);
                
                // FIXED: Silent Array Loading (No couts!)
                if(s.courseCount < 5) { 
                    s.enrolledCourses[s.courseCount] = c;
                    s.courseCount++;
                }
                
                s.transcript.addGrade(cCode, grade);

                if(!attStr.empty()) {
                    size_t pos = 0; string token;
                    while (true) {
                        pos = attStr.find(',');
                        token = (pos == string::npos) ? attStr : attStr.substr(0, pos); cleanStr(token);
                        if (!token.empty()) {
                            size_t eqPos = token.find('=');
                            if (eqPos != string::npos) {
                                string dateStr = token.substr(0, eqPos); string statusStr = token.substr(eqPos + 1);
                                cleanStr(dateStr); cleanStr(statusStr);
                                if (!statusStr.empty() && s.attendanceCount < 100) {
                                    // FIXED: Silent Array Loading (No couts!)
                                    s.attendanceHistory[s.attendanceCount] = AttendanceRecord(dateStr, statusStr[0], c);
                                    s.attendanceCount++;
                                }
                            }
                        }
                        if (pos == string::npos) break;
                        attStr.erase(0, pos + 1);
                    }
                }
            }
            students[studentCount] = s; studentCount++;
        }
    } catch (const exception& e) {
        cout << "[!] File Exception: " << e.what() << "\n";
    }
}

void FileManager::saveStudents(const string& filename, const Student students[], int studentCount) {
    try {
        ofstream file(filename);
        if (!file.is_open()) throw runtime_error("Unable to open " + filename + " for writing.");
        
        for (int i = 0; i < studentCount; i++) {
            const Student& s = students[i];
            file << s.getName() << "," << s.getRollNo() << "," << s.getEmail() << "," << s.getPassword() << "," << s.getFeeDue() << "," << s.getSection() << ",";
            for (int j = 0; j < s.getCourseCount(); j++) {
                string cCode = s.getEnrolledCourses()[j].getCode();
                string grade = s.transcript.getGrade(cCode);
                file << cCode << ":" << grade << ":"; 
                
                bool firstAtt = true;
                for (int k = 0; k < s.getAttendanceCount(); k++) {
                    if (s.getAttendanceHistory()[k].getCourseCode() == cCode) {
                        if (!firstAtt) file << ",";
                        file << s.getAttendanceHistory()[k].getDate() << "=" << s.getAttendanceHistory()[k].getStatus(); 
                        firstAtt = false;
                    }
                }
                if (j < s.getCourseCount() - 1) file << ";";
            }
            file << "\n";
        }
    } catch (const exception& e) {
        cout << "[!] File Exception: " << e.what() << "\n";
    }
}

void FileManager::loadFaculty(const string& filename, Faculty facultyList[], int& facultyCount) {
    facultyCount = 0; 
    try {
        ifstream file(filename);
        if (!file.is_open()) throw runtime_error("Unable to open " + filename + " for reading.");
        
        string line, name, email, pass, salStr, coursesStr;
        while(getline(file, line) && facultyCount < 50) {
            cleanStr(line); stringstream ss(line);
            getline(ss, name, ','); getline(ss, email, ','); getline(ss, pass, ','); 
            getline(ss, salStr, ','); getline(ss, coursesStr, ',');
            
            double salary = salStr.empty() ? 0.0 : stod(salStr);
            Faculty f(name, email, pass, salary); 
            
            stringstream cs(coursesStr); string courseToken;
            while(getline(cs, courseToken, ';')) {
                cleanStr(courseToken); if(courseToken.empty()) continue;
                size_t dash = courseToken.find('-'); 
                if(dash != string::npos) {
                    string cCode = courseToken.substr(0, dash);
                    string cSec = courseToken.substr(dash+1);
                    f.assignCourse(Course(cCode, getCourseTitleHelper(cCode), cSec));
                }
            }
            facultyList[facultyCount] = f; facultyCount++;
        }
    } catch (const exception& e) {
        cout << "[!] File Exception: " << e.what() << "\n";
    }
}

void FileManager::saveFaculty(const string& filename, const Faculty facultyList[], int facultyCount) {
    try {
        ofstream file(filename);
        if (!file.is_open()) throw runtime_error("Unable to open " + filename + " for writing.");
        
        for(int i = 0; i < facultyCount; i++) {
            const Faculty& f = facultyList[i];
            file << f.getName() << "," << f.getEmail() << "," << f.getPassword() << "," << f.getSalary() << ",";
            for (int j = 0; j < f.getCourseCount(); j++) {
                file << f.getCoursesTaught()[j].getCode() << "-" << f.getCoursesTaught()[j].getSection();
                if(j < f.getCourseCount() - 1) file << ";";
            }
            file << "\n";
        }
    } catch (const exception& e) {
        cout << "[!] File Exception: " << e.what() << "\n";
    }
}

void FileManager::loadAdmins(const string& filename, Admin adminList[], int& adminCount) {
    adminCount = 0; 
    try {
        ifstream file(filename);
        if (!file.is_open()) throw runtime_error("Unable to open " + filename + " for reading.");
        
        string line, name, email, pass;
        while(getline(file, line) && adminCount < 10) {
            cleanStr(line); stringstream ss(line); getline(ss, name, ','); getline(ss, email, ','); getline(ss, pass, ',');
            if(!name.empty()) { adminList[adminCount] = Admin(name, email, pass); adminCount++; }
        }
    } catch (const exception& e) {
        cout << "[!] File Exception: " << e.what() << "\n";
    }
}

void FileManager::saveAdmins(const string& filename, const Admin adminList[], int adminCount) {
    try {
        ofstream file(filename);
        if (!file.is_open()) throw runtime_error("Unable to open " + filename + " for writing.");
        
        for(int i = 0; i < adminCount; i++) file << adminList[i].getName() << "," << adminList[i].getEmail() << "," << adminList[i].getPassword() << "\n";
    } catch (const exception& e) {
        cout << "[!] File Exception: " << e.what() << "\n";
    }
}