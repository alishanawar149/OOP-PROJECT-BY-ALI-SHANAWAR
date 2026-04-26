#include "FileManager.h"
#include "Admin.h"
#include <iostream>
#include <limits> // Included to allow clearing the cin buffer

Course availableCourses[5] = {
    Course("CL1004", "Object Oriented Programming"),
    Course("EE1005", "Digital Logic Design"),
    Course("MT1008", "Multivariable Calculus"),
    Course("SS1014", "Expository Writing"),
    Course("SS1015", "Pakistan Studies")
};

int main() {
    Student students[100]; int studentCount = 0;
    Faculty facultyList[50]; int facultyCount = 0;
    Admin adminList[10]; int adminCount = 0;

    FileManager::loadStudents("students.txt", students, studentCount);
    FileManager::loadFaculty("faculty.txt", facultyList, facultyCount);
    FileManager::loadAdmins("admins.txt", adminList, adminCount);

    if(studentCount == 0) { students[0] = Student("Abdullah", "24K-0001", "abdullah@saos.edu", "pass123", 198500.0, "A"); studentCount++; }
    if(facultyCount == 0) { 
        facultyList[0] = Faculty("Dr. Smith", "smith@saos.edu", "fac123"); 
        facultyList[0].assignCourse(Course("CL1004", "Object Oriented Programming", "A"));
        facultyCount++; 
    }
    if(adminCount == 0) { adminList[0] = Admin("System Admin", "admin@saos.edu", "admin123"); adminCount++; }

    bool systemRunning = true;
    
    while(systemRunning) {
        int userType;
        cout << "\n==============================\n";
        cout << " Welcome to SAOS Portal \n";
        cout << "==============================\n";
        cout << "1. Student\n2. Faculty\n3. Admin\n4. Exit System\nSelect Role: ";
        cin >> userType;

        // NEW: Exception handling for incorrect string input instead of int
        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "[!] Invalid input. Please enter a number.\n";
            continue; 
        }

        if (userType == 4) { cout << "\n[+] Saving all data. Exiting SAOS. Goodbye!\n"; break; }
        if (userType < 1 || userType > 4) { cout << "[!] Invalid menu selection.\n"; continue; }

        string email, pass;
        cout << "Email: "; cin >> email;
        cout << "Password: "; cin >> pass;

        // --- STUDENT PORTAL ---
        if (userType == 1) {
            bool loggedIn = false;
            for (int i = 0; i < studentCount; i++) {
                Student& s = students[i];
                if (s.verifyLogin(email, pass)) {
                    loggedIn = true;
                    cout << "\n[+] Login Successful! Welcome " << s.getName() << ".\n";
                    int choice;
                    do {
                        cout << "\n--- Student Menu ---\n1. Dashboard\n2. Enroll\n3. Pay Fee\n4. View Attendance\n5. View Transcript\n6. Logout\nSelect: ";
                        cin >> choice;
                        
                        // Safety check for inner menu
                        if (cin.fail()) {
                            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "[!] Invalid input. Please enter a number.\n";
                            continue;
                        }
                        
                        if (choice == 1) s.viewDashboard();
                        else if (choice == 2) {
                            cout << "\n--- Available Courses to Enroll ---\n";
                            bool hasOptions = false;
                            const Course* enrolled = s.getEnrolledCourses();
                            for (int j = 0; j < 5; j++) {
                                bool isEnrolled = false;
                                for (int k = 0; k < s.getCourseCount(); k++) {
                                    if (enrolled[k].getCode() == availableCourses[j].getCode()) { isEnrolled = true; break; }
                                }
                                if (!isEnrolled) { 
                                    cout << j << ". " << availableCourses[j].getCode() << " - " << availableCourses[j].getTitle() << "\n";
                                    hasOptions = true;
                                }
                            }
                            if (!hasOptions) cout << "[!] You are already enrolled in all available courses!\n";
                            else {
                                cout << "Enter index to enroll: "; int idx; cin >> idx;
                                if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); cout << "[!] Invalid format.\n"; continue; }
                                if (idx >= 0 && idx <= 4) {
                                    s.enrollCourse(Course(availableCourses[idx].getCode(), availableCourses[idx].getTitle(), s.getSection()));
                                }
                                else cout << "[!] Error: Invalid Selection.\n";
                            }
                        }
                        else if (choice == 3) {
                            cout << "\n--- Fee Payment ---\nCurrent Due Amount: Rs. " << s.getFeeDue() << "\n";
                            if(s.getFeeDue() <= 0) cout << "You have no pending dues.\n";
                            else { 
                                double amt; cout << "Enter amount to pay: "; cin >> amt; 
                                if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); cout << "[!] Invalid amount format.\n"; continue; }
                                s -= amt; 
                            }
                        }
                        else if (choice == 4) {
                            string c; cout << "\nEnter Course Code (e.g., CL1004): "; cin >> c;
                            s.viewAttendance(c);
                        }
                        else if (choice == 5) s.viewTranscript();

                    } while (choice != 6);
                    FileManager::saveStudents("students.txt", students, studentCount);
                    cout << "[+] Logged out successfully. Data Saved.\n";
                    break;
                }
            }
            if(!loggedIn) cout << "[!] Invalid Student Credentials.\n";
        } 
        
        // --- FACULTY PORTAL ---
        else if (userType == 2) {
            bool loggedIn = false;
            for (int i = 0; i < facultyCount; i++) {
                Faculty& f = facultyList[i];
                if (f.verifyLogin(email, pass)) {
                    loggedIn = true;
                    cout << "\n[+] Login Successful! Welcome " << f.getName() << ".\n";
                    int choice;
                    do {
                        cout << "\n--- Faculty Menu ---\n1. Dashboard\n2. Mark Single Attendance\n3. Mark SECTION Attendance\n4. Assign Grade\n5. View Salary\n6. Redeem Salary\n7. Logout\nSelect: ";
                        cin >> choice;
                        
                        if (cin.fail()) {
                            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "[!] Invalid input. Please enter a number.\n";
                            continue;
                        }
                        
                        if (choice == 1) f.viewDashboard();
                        
                        else if (choice == 2) {
                            string cCode; cout << "\nEnter Course Code (e.g., CL1004): "; cin >> cCode;
                            if (cCode.find("K-") != string::npos) { cout << "[!] ERROR: Typed Roll Number instead of Course Code!\n"; continue; }
                            
                            bool teachesAnySection = false;
                            for(int k = 0; k < f.getCourseCount(); k++) {
                                if(f.getCoursesTaught()[k].getCode() == cCode) { teachesAnySection = true; break; }
                            }
                            if(!teachesAnySection) { cout << "[!] Error: You do not teach " << cCode << ". Access Denied.\n"; continue; }
                            
                            string sec; cout << "Enter Section (e.g., A): "; cin >> sec;
                            if (!f.teachesCourse(cCode, sec)) {
                                cout << "[!] Error: You do not teach Section " << sec << " for course " << cCode << ".\n"; 
                                continue;
                            }
                            
                            string roll; cout << "Student Roll No (e.g., 24K-0001): "; cin >> roll; 
                            
                            int sIdx = -1;
                            for(int j = 0; j < studentCount; j++) {
                                if(students[j].getRollNo() == roll) { sIdx = j; break; }
                            }
                            
                            if (sIdx == -1) { cout << "[!] Error: Student " << roll << " not found in system.\n"; continue; }
                            if (students[sIdx].getSection() != sec) { cout << "[!] Error: Student belongs to Section " << students[sIdx].getSection() << ", not Section " << sec << ".\n"; continue; }
                            
                            string date; char stat;
                            cout << "Date (DD/MM/YYYY): "; cin >> date; 
                            cout << "Status (P/A): "; cin >> stat;
                            
                            f.markAttendance(students[sIdx], Course(cCode, "", sec), date, stat); 
                        }
                        
                        else if (choice == 3) { 
                            string cCode; cout << "\nEnter Course Code (e.g., CL1004): "; cin >> cCode;
                            if (cCode.find("K-") != string::npos) { cout << "[!] ERROR: Typed Roll Number instead of Course Code!\n"; continue; }
                            
                            bool teachesAnySection = false;
                            for(int k = 0; k < f.getCourseCount(); k++) {
                                if(f.getCoursesTaught()[k].getCode() == cCode) { teachesAnySection = true; break; }
                            }
                            if(!teachesAnySection) { cout << "[!] Error: You do not teach " << cCode << ". Access Denied.\n"; continue; }
                            
                            string sec; cout << "Enter Section to mark attendance (e.g., A): "; cin >> sec;
                            if (!f.teachesCourse(cCode, sec)) { cout << "[!] Error: You do not teach " << cCode << " to Section " << sec << ".\n"; continue; }
                            
                            string date; cout << "Date (DD/MM/YYYY): "; cin >> date; 
                            
                            cout << "\n--- Section " << sec << " Attendance for " << cCode << " ---\n";
                            bool studentsFound = false;
                            for(int j = 0; j < studentCount; j++) {
                                if(students[j].getSection() == sec) {
                                    bool isEnrolled = false;
                                    for(int k=0; k<students[j].getCourseCount(); k++) {
                                        if(students[j].getEnrolledCourses()[k].getCode() == cCode) { isEnrolled = true; break; }
                                    }
                                    if(isEnrolled) {
                                        studentsFound = true;
                                        char stat;
                                        cout << students[j].getRollNo() << " (" << students[j].getName() << ") - [P/A]: ";
                                        cin >> stat;
                                        f.markAttendance(students[j], Course(cCode,"",sec), date, stat);
                                    }
                                }
                            }
                            if(!studentsFound) cout << "[!] No students from Section " << sec << " are enrolled in " << cCode << ".\n";
                            else cout << "[+] Section attendance complete.\n";
                        }
                        
                        else if (choice == 4) {
                            string cCode; cout << "\nEnter Course Code (e.g., CL1004): "; cin >> cCode;
                            if (cCode.find("K-") != string::npos) { cout << "[!] ERROR: Typed Roll Number instead of Course Code!\n"; continue; }
                            
                            bool teachesAnySection = false;
                            for(int k = 0; k < f.getCourseCount(); k++) {
                                if(f.getCoursesTaught()[k].getCode() == cCode) { teachesAnySection = true; break; }
                            }
                            if(!teachesAnySection) { cout << "[!] Error: You do not teach " << cCode << ". Access Denied.\n"; continue; }
                            
                            string sec; cout << "Enter Section (e.g., A): "; cin >> sec;
                            if (!f.teachesCourse(cCode, sec)) { cout << "[!] Error: You do not teach Section " << sec << " for course " << cCode << ".\n"; continue; }
                            
                            string roll; cout << "Student Roll No (e.g., 24K-0001): "; cin >> roll; 
                            
                            int sIdx = -1;
                            for(int j = 0; j < studentCount; j++) {
                                if(students[j].getRollNo() == roll) { sIdx = j; break; }
                            }
                            
                            if (sIdx == -1) { cout << "[!] Error: Student " << roll << " not found in system.\n"; continue; }
                            if (students[sIdx].getSection() != sec) { cout << "[!] Error: Student belongs to Section " << students[sIdx].getSection() << ", not Section " << sec << ".\n"; continue; }
                            
                            string grade;
                            cout << "Grade (A+, A, B, etc): "; cin >> grade;
                            f.assignGrade(students[sIdx], Course(cCode, "", sec), grade); 
                        }
                        
                        else if (choice == 5) f.viewSalary();
                        else if (choice == 6) { 
                            cout << "\n--- Redeem Salary ---\nCurrent Balance: Rs. " << f.getSalary() << "\n";
                            if (f.getSalary() <= 0) cout << "[!] You have no salary credited to redeem right now.\n";
                            else { 
                                double amt; cout << "Enter amount to redeem: "; cin >> amt; 
                                if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); cout << "[!] Invalid amount format.\n"; continue; }
                                f.redeemSalary(amt); 
                            }
                        }

                    } while (choice != 7);
                    
                    FileManager::saveFaculty("faculty.txt", facultyList, facultyCount);
                    FileManager::saveStudents("students.txt", students, studentCount); 
                    cout << "[+] Logged out successfully. Data Saved.\n";
                    break;
                }
            }
            if(!loggedIn) cout << "[!] Invalid Faculty Credentials.\n";
        }

        // --- ADMIN PORTAL ---
        else if (userType == 3) {
            bool loggedIn = false;
            for (int i = 0; i < adminCount; i++) {
                Admin& sysAdmin = adminList[i];
                if (sysAdmin.verifyLogin(email, pass)) {
                    loggedIn = true;
                    cout << "\n[+] Admin Login Successful.\n";
                    int choice;
                    do {
                        cout << "\n--- Admin Menu ---\n1. Dashboard\n2. Pay Faculty Salary\n3. Enroll Student Course\n4. Remove Student Course\n5. View Section Students\n6. Logout\nSelect: ";
                        cin >> choice;
                        
                        if (cin.fail()) {
                            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "[!] Invalid input. Please enter a number.\n";
                            continue;
                        }
                        
                        if (choice == 1) sysAdmin.viewDashboard();
                        else if (choice == 2) {
                            string fEmail; double amt;
                            cout << "Faculty Email: "; cin >> fEmail; cout << "Amount to pay: "; cin >> amt;
                            if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); cout << "[!] Invalid amount format.\n"; continue; }
                            
                            bool found = false;
                            for(int j = 0; j < facultyCount; j++) {
                                if(facultyList[j].getEmail() == fEmail) { sysAdmin.paySalary(facultyList[j], amt); found = true; break; }
                            }
                            if(found) FileManager::saveFaculty("faculty.txt", facultyList, facultyCount);
                            else cout << "[!] Error: Faculty member not found.\n";
                        }
                        else if (choice == 3) { 
                            string roll, cCode; cout << "Student Roll No: "; cin >> roll; cout << "Course Code to Enroll: "; cin >> cCode;
                            bool validCourse = false; Course cToEnroll;
                            for(int j = 0; j < 5; j++) {
                                if(availableCourses[j].getCode() == cCode) { cToEnroll = availableCourses[j]; validCourse = true; break; }
                            }
                            if(!validCourse) { cout << "[!] Error: Invalid course code.\n"; continue; }

                            bool found = false;
                            for(int j = 0; j < studentCount; j++) {
                                if(students[j].getRollNo() == roll) { 
                                    cToEnroll = Course(cToEnroll.getCode(), cToEnroll.getTitle(), students[j].getSection());
                                    sysAdmin.enrollCourseForStudent(students[j], cToEnroll); 
                                    found = true; break; 
                                }
                            }
                            if(!found) cout << "[!] Error: Student not found.\n";
                        }
                        else if (choice == 4) {
                            string roll, cCode; cout << "Student Roll No: "; cin >> roll; cout << "Course Code to Remove: "; cin >> cCode;
                            bool found = false;
                            for(int j = 0; j < studentCount; j++) {
                                if(students[j].getRollNo() == roll) { sysAdmin.removeCourseFromStudent(students[j], cCode); found = true; break; }
                            }
                            if(!found) cout << "[!] Error: Student not found.\n";
                        }
                        else if (choice == 5) { 
                            string sec; cout << "Enter Section to view (e.g., A): "; cin >> sec;
                            sysAdmin.viewSectionStudents(students, studentCount, sec);
                        }
                    } while (choice != 6);
                    
                    FileManager::saveStudents("students.txt", students, studentCount); 
                    FileManager::saveAdmins("admins.txt", adminList, adminCount);
                    cout << "[+] Admin logged out.\n";
                    break;
                }
            }
            if (!loggedIn) cout << "[!] Invalid Admin Credentials.\n";
        }
    }
    return 0;
}