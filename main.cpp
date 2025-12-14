#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <iomanip>
#include <cctype>
#include <fstream>
#include <sstream>
#include <windows.h> // Colors, Beep, Sleep
#include <conio.h>   // _getch()

using namespace std;

// --- CONFIGURATION ---
bool ENABLE_SOUND = true; 

// --- DATA STRUCTURES ---
struct Course {
    string name;
    int marks;
    int credits;
    string grade;
    double gpa;
};

struct Student {
    string rollNo;
    string name;
    string uni;
    int semester;
    double sgpa;
    double cgpa;
    vector<Course> courses;
};

// --- UTILITY FUNCTIONS ---

void setColor(int colorCode) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, colorCode);
}

void playSound(string type) {
    if (!ENABLE_SOUND) return;
    if (type == "ERROR") Beep(300, 300);
    if (type == "SUCCESS") { Beep(1000, 100); Beep(1500, 150); }
    if (type == "CLICK") Beep(600, 50);
}

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void drawLine(int width = 70) {
    setColor(8); // Gray
    cout << string(width, '=') << endl;
    setColor(7); // Default
}

void drawSingleLine(int width = 70) {
    setColor(8);
    cout << string(width, '-') << endl;
    setColor(7);
}

void printHeader(string title) {
    system("cls");
    cout << "\n";
    drawLine();
    setColor(14); // Yellow
    cout << "  " << title << endl;
    drawLine();
    cout << "\n";
}

// --- VALIDATION MODULE ---

string getValidText(string prompt) {
    string input;
    while (true) {
        setColor(11); // Cyan
        cout << prompt << ": ";
        setColor(15); // White
        
        getline(cin, input);
        
        bool hasAlpha = false;
        for (char c : input) if (isalpha(c)) hasAlpha = true;

        if (!input.empty() && hasAlpha) {
            playSound("CLICK");
            return input;
        } else {
            setColor(12); // Red
            cout << "  [!] Error: Input must contain alphabets.\n";
            playSound("ERROR");
        }
    }
}

string getValidRollNo(string prompt) {
    string input;
    while (true) {
        setColor(11);
        cout << prompt << ": ";
        setColor(15);
        
        getline(cin, input);
        
        bool isNumeric = true;
        if (input.empty()) isNumeric = false;
        for (char c : input) if (!isdigit(c)) isNumeric = false;

        if (isNumeric) {
            playSound("CLICK");
            return input;
        }
        
        setColor(12);
        cout << "  [!] Error: Roll No must be numeric.\n";
        playSound("ERROR");
    }
}

int getValidInt(string prompt, int min, int max) {
    int value;
    while (true) {
        setColor(11);
        cout << prompt << ": ";
        setColor(15);
        
        if (cin >> value) {
            if (value >= min && value <= max) {
                clearInput();
                playSound("CLICK");
                return value;
            }
        }
        
        clearInput();
        setColor(12);
        cout << "  [!] Invalid Input! Range: " << min << "-" << max << endl;
        playSound("ERROR");
    }
}

double getValidDouble(string prompt, double min, double max) {
    double value;
    while (true) {
        setColor(11);
        cout << prompt << ": ";
        setColor(15);
        
        if (cin >> value) {
            if (value >= min && value <= max) {
                clearInput();
                playSound("CLICK");
                return value;
            }
        }
        clearInput();
        setColor(12);
        cout << "  [!] Invalid Value.\n";
        playSound("ERROR");
    }
}

// --- GRADING LOGIC ---

double calculateGPA(int marks) {
    if (marks >= 85) return 4.0;
    if (marks >= 80) return 3.75;
    if (marks >= 75) return 3.5;
    if (marks >= 70) return 3.0;
    if (marks >= 65) return 2.5;
    if (marks >= 60) return 2.0;
    if (marks >= 56) return 1.5;
    if (marks >= 50) return 1.0;
    return 0.0;
}

string calculateGrade(int marks) {
    if (marks >= 85) return "A";
    if (marks >= 80) return "A-";
    if (marks >= 75) return "B+";
    if (marks >= 70) return "B";
    if (marks >= 65) return "C+";
    if (marks >= 60) return "C";
    if (marks >= 56) return "D+";
    if (marks >= 50) return "D";
    return "F";
}

// --- FILE SYSTEM (UPDATED FOR FULL DATA LOADING) ---

void saveToCSV(const vector<Student>& students) {
    ofstream file("StudentData.csv");
    
    // Excel Header
    file << "Roll No,Name,University,Semester,SGPA,CGPA,Detailed Course Data (Name:Marks:Grade:Credits)\n";
    
    for (const auto& s : students) {
        file << s.rollNo << "," << s.name << "," << s.uni << ","
             << s.semester << "," << fixed << setprecision(2) << s.sgpa << "," << s.cgpa << ",";
        
        // Save format: "Name:Marks:Grade:Credits | NextCourse..."
        file << "\""; 
        for (size_t i = 0; i < s.courses.size(); i++) {
            file << s.courses[i].name << ":" 
                 << s.courses[i].marks << ":" 
                 << s.courses[i].grade << ":" 
                 << s.courses[i].credits;
            if (i < s.courses.size() - 1) file << "|"; 
        }
        file << "\"\n";
    }
    file.close();
}

vector<Student> loadFromCSV() {
    vector<Student> students;
    ifstream file("StudentData.csv");
    if (!file.is_open()) return students;

    string line;
    getline(file, line); // Skip Header

    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string segment;
        vector<string> row;

        bool inQuotes = false;
        string currentField = "";
        
        // Handle CSV parsing (commas inside quotes)
        for(char c : line) {
            if(c == '"') inQuotes = !inQuotes;
            else if(c == ',' && !inQuotes) {
                row.push_back(currentField);
                currentField = "";
            } else {
                currentField += c;
            }
        }
        row.push_back(currentField);

        if (row.size() >= 7) {
            Student s;
            s.rollNo = row[0];
            s.name = row[1];
            s.uni = row[2];
            try {
                s.semester = stoi(row[3]);
                s.sgpa = stod(row[4]);
                s.cgpa = stod(row[5]);
            } catch (...) { continue; } 

            // **CRITICAL FIX: Parsing Courses Back**
            string allCourses = row[6];
            stringstream courseStream(allCourses);
            string singleCourseStr;
            
            while(getline(courseStream, singleCourseStr, '|')) {
                // Parse "Name:Marks:Grade:Credits"
                stringstream detailStream(singleCourseStr);
                string segment;
                vector<string> details;
                while(getline(detailStream, segment, ':')) {
                    details.push_back(segment);
                }
                
                if(details.size() == 4) {
                    Course c;
                    c.name = details[0];
                    c.marks = stoi(details[1]);
                    c.grade = details[2];
                    c.credits = stoi(details[3]);
                    c.gpa = calculateGPA(c.marks); // Recalculate GPA just in case
                    s.courses.push_back(c);
                }
            }
            students.push_back(s);
        }
    }
    return students;
}

// --- UI SCREENS ---

void showWelcome() {
    system("cls");
    cout << "\n\n";
    drawLine();
    setColor(14);
    cout << "  ACADEMIC MANAGEMENT SYSTEM v5.0\n";
    cout << "  Professional Edition\n";
    drawLine();
    cout << "\n";
    cout << "  System Status: "; setColor(10); cout << "ONLINE\n"; setColor(7);
    cout << "  Database File: "; setColor(10); cout << "StudentData.csv\n"; setColor(7);
    cout << "\n\n";
    
    cout << "  [ PRESS ENTER TO START ]";
    _getch();
    
    // Sound Config
    system("cls");
    drawLine();
    cout << "  SYSTEM CONFIGURATION\n";
    drawLine();
    cout << "\n";
    cout << "  Enable Sound Effects? (y/n): ";
    char snd; cin >> snd;
    if (snd == 'n' || snd == 'N') ENABLE_SOUND = false;
    clearInput();
    cout << "\n  Configuration Saved.\n";
    Sleep(500);
}

void showTranscript(const Student& s) {
    cout << "\n";
    drawLine();
    setColor(14); cout << "  OFFICIAL TRANSCRIPT\n"; setColor(7);
    drawLine();
    cout << "\n";
    
    cout << "  Name       : " << s.name << endl;
    cout << "  Roll No    : " << s.rollNo << endl;
    cout << "  University : " << s.uni << endl;
    cout << "  Semester   : " << s.semester << endl;
    
    cout << "\n";
    drawSingleLine();
    cout << left << setw(25) << "  COURSE" 
         << setw(10) << "MARKS" 
         << setw(10) << "GRADE" 
         << setw(10) << "CREDITS" << endl;
    drawSingleLine();

    // Loop through courses to display them
    for (const auto& c : s.courses) {
        cout << left << setw(25) << ("  " + c.name) 
             << setw(10) << c.marks 
             << setw(10) << c.grade 
             << setw(10) << c.credits << endl;
    }
    
    // Check if courses vector is empty (Double check logic)
    if(s.courses.empty()) {
        setColor(12);
        cout << "  [!] No detailed course data available for this record.\n";
        setColor(7);
    }

    drawSingleLine();
    
    cout << "\n";
    setColor(14);
    cout << "  SGPA: " << fixed << setprecision(2) << s.sgpa << endl;
    cout << "  CGPA: " << fixed << setprecision(2) << s.cgpa << endl;
    setColor(7);
    drawLine();
}

// --- MAIN FEATURES ---

void registerStudent() {
    vector<Student> students = loadFromCSV();
    printHeader("NEW STUDENT REGISTRATION");

    string roll;
    while(true) {
        roll = getValidRollNo("  Enter Roll No");
        bool exists = false;
        for(const auto& s : students) if(s.rollNo == roll) exists = true;
        
        if(exists) {
            setColor(12); cout << "  [!] Error: ID exists. Try another.\n"; setColor(7);
            playSound("ERROR");
        } else break;
    }

    string name = getValidText("  Enter Name");
    string uni = getValidText("  Enter University");
    int sem = getValidInt("  Current Semester (1-14)", 1, 14);

    cout << "\n";
    drawSingleLine();
    cout << "  COURSE ENTRY\n";
    drawSingleLine();
    
    int nCourses = getValidInt("  Number of courses (1-10)", 1, 10);
    
    vector<Course> currentCourses;
    double pts = 0;
    int crd = 0;

    for(int i=0; i<nCourses; i++) {
        setColor(14);
        cout << "\n  Subject " << i+1 << ":\n";
        setColor(7);
        
        string cn = getValidText("    Name");
        int cm = getValidInt("    Marks (0-100)", 0, 100);
        int cc = getValidInt("    Credits (1-6)", 1, 6);

        Course c;
        c.name = cn; c.marks = cm; c.credits = cc;
        c.grade = calculateGrade(cm);
        c.gpa = calculateGPA(cm);
        currentCourses.push_back(c);
        pts += (c.gpa * cc);
        crd += cc;
    }

    double sgpa = (crd > 0) ? pts / crd : 0.0;
    double prevCGPA = 0; int prevCreds = 0;

    if(sem > 1) {
        cout << "\n";
        drawSingleLine();
        cout << "  ACADEMIC HISTORY\n";
        drawSingleLine();
        prevCGPA = getValidDouble("  Previous Combined CGPA (0-4)", 0.0, 4.0);
        prevCreds = getValidInt("  Total Previous Credits", 1, 250);
    }

    double totalPts = (prevCGPA * prevCreds) + pts;
    int totalCrd = prevCreds + crd;
    double cgpa = (totalCrd > 0) ? totalPts / totalCrd : 0.0;

    Student s = {roll, name, uni, sem, sgpa, cgpa, currentCourses};
    students.push_back(s);
    saveToCSV(students);

    system("cls");
    playSound("SUCCESS");
    cout << "\n\n";
    drawLine();
    setColor(10); cout << "  DATA SAVED SUCCESSFULLY!\n"; setColor(7);
    drawLine();
    Sleep(1000);
    
    showTranscript(s);
    
    cout << "\n  Press Enter to return...";
    _getch();
}

void searchStudent() {
    vector<Student> students = loadFromCSV();
    
    // Check if Database is Empty
    if(students.empty()) {
        printHeader("DATABASE EMPTY");
        setColor(12); cout << "  [!] No records found in 'StudentData.csv'.\n"; setColor(7);
        playSound("ERROR");
        
        cout << "\n";
        // User MUST press 0 to go back
        getValidInt("  Press 0 to go back and enter data", 0, 0); 
        return;
    }

    while(true) {
        printHeader("SEARCH RECORDS");
        string tRoll = getValidRollNo("  Enter Roll No to Search");
        
        int idx = -1;
        for(size_t i=0; i<students.size(); i++) {
            if(students[i].rollNo == tRoll) { idx = i; break; }
        }

        if(idx == -1) {
            setColor(12); cout << "\n  [!] Student not found.\n"; setColor(7);
            playSound("ERROR");
            cout << "\n  1. Try Again\n  2. Main Menu\n";
            int ch = getValidInt("  Select", 1, 2);
            if(ch==2) return;
        } else {
            playSound("SUCCESS");
            system("cls");
            showTranscript(students[idx]); // Now this shows FULL TABLE
            
            while(true) {
                cout << "\n";
                drawSingleLine();
                cout << "  ACTIONS\n";
                drawSingleLine();
                cout << "  1. Update Info (Name/Uni)\n";
                cout << "  2. Search Another\n";
                cout << "  3. Main Menu\n";
                int op = getValidInt("  Select", 1, 3);
                
                if(op == 3) return;
                if(op == 2) break; 
                if(op == 1) {
                    while(true) {
                        printHeader("UPDATE RECORD: " + students[idx].name);
                        cout << "  1. Update Name\n";
                        cout << "  2. Update University\n";
                        cout << "  3. Back\n";
                        int uOp = getValidInt("  Select", 1, 3);
                        if(uOp == 3) break;
                        
                        if(uOp == 1) {
                            students[idx].name = getValidText("  Enter New Name");
                            saveToCSV(students);
                            cout << "\n  [OK] Name Updated.\n"; Sleep(800);
                        }
                        if(uOp == 2) {
                            students[idx].uni = getValidText("  Enter New Uni");
                            saveToCSV(students);
                            cout << "\n  [OK] University Updated.\n"; Sleep(800);
                        }
                        
                        cout << "\n  Update anything else? (1:Yes / 2:No)\n";
                        int more = getValidInt("  Select", 1, 2);
                        if(more == 2) break;
                    }
                    system("cls");
                    showTranscript(students[idx]);
                }
            }
        }
    }
}

// --- MAIN ---

int main() {
    SetConsoleTitleA(" CGPA Calculator ");
    showWelcome();

    while(true) {
        system("cls");
        cout << "\n";
        drawLine();
        setColor(14); cout << "  MAIN DASHBOARD\n"; setColor(7);
        drawLine();
        cout << "\n";
        
        cout << "  1. Register Student & Calculate GPA\n";
        cout << "  2. Search & Update Records\n";
        cout << "  3. Exit System\n";
        cout << "\n";
        
        int choice = getValidInt("  Select Option", 1, 3);
        
        if(choice == 1) registerStudent();
        else if(choice == 2) searchStudent();
        else if(choice == 3) {
            cout << "\n  Exiting...\n";
            Sleep(800);
            break;
        }
    }
    return 0;
}
