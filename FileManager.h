#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include "Student.h"
#include "Faculty.h"
#include "Admin.h"
#include <string>

class FileManager {
public:
    static void loadStudents(const string& filename, Student students[], int& count);
    static void saveStudents(const string& filename, const Student students[], int count);
    
    static void loadFaculty(const string& filename, Faculty facultyList[], int& count);
    static void saveFaculty(const string& filename, const Faculty facultyList[], int count);
    
    static void loadAdmins(const string& filename, Admin adminList[], int& count);
    static void saveAdmins(const string& filename, const Admin adminList[], int count);
};

#endif