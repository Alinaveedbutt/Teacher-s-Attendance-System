#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
using namespace std;

class Person {
protected:
    string name;
    int id;
public:
    Person() {
        name = "";
        id = 0;
    }
    Person(string name, int id) {
        this-> name = name;
        this-> id = id;
    }
    virtual void getPersonalInfo() {
        cin.ignore();
        cout<<"Enter name:"<<endl;
        getline(cin, name);
        cout<<"Enter ID:"<<endl;
        cin>>id;
    }
    virtual void displayPersonalInfo() const {
        cout<<"Name: "<<name<<endl;
        cout<<"ID: "<<id<<endl;
    }
    int getID() const {
        return id;
    }
    string getName() const {
        return name;
    }
    virtual ~Person() {
        cout<<"Person Destructor called..."<<endl;
    }
};

class TimeRecord {
protected:
    string date;
    string inTime, outTime;
    static bool isValidTimeString (const string &timeString) {
        if (timeString.length() != 5 || timeString[2] != ':')
            return false;
        for (int i = 0; i < 5; i++) {
            if (i == 2)
                continue;
            if (!isdigit(timeString[i]))
                return false;
        }
        int hour = stoi(timeString.substr(0,2));
        int minute = stoi(timeString.substr(3,2));
        return (hour >=0 && hour <=23 && minute>=0 && minute<=59);
    }
    static int timeToMinutes(const string& timeStr) {
        int hour = stoi(timeStr.substr(0,2));
        int minute = stoi(timeStr.substr(3,2));
        return hour * 60 + minute;
    }
public:
    TimeRecord() {
        date = "";
        inTime = "";
        outTime = "";
    }
    void setDate(string date) {
        this-> date = date;
    }
    string getDate() const {
        return date;
    }
    void setInTime() {
        cin.ignore();
        cout<<"Enter date (DD-MM-YYYY):"<<endl;
        getline(cin, date);
        bool valid = false;
         while (!valid) {
            cin.ignore();
            cout<<"Enter time in (HH:MM):"<<endl;
            getline(cin, inTime);
            valid = isValidTimeString(inTime);
            if (!valid)
                cout<<"Invalid time format (Please Use a colon or try using the 24 hour format)"<<endl;
        }
    }
    void setOutTime() {
        bool valid = false;
        while (!valid) {
            cin.ignore();
            cout<<"Enter time out (HH:MM):"<<endl;
            getline(cin, outTime);
            valid = isValidTimeString(outTime);
            if (!valid)
                cout<<"Invalid time format (Please Use a colon or try using the 24 hour format)"<<endl;
        }
    }
    void setTotalMinutes(int mins) {
        string hourString, minString;
        inTime = "09:00";
        int totalMinutes = 9*60 + mins;
        int outHours = totalMinutes / 60;
        int outMins = totalMinutes % 60;
        if(outHours < 10)
            hourString = "0" + to_string(outHours);
        else
            hourString = to_string(outHours);
        if(outMins < 10)
            minString = "0" + to_string(outMins);
        else
            minString = to_string(outMins);
        outTime = hourString + ":" + minString;
    }
    void displayTimeRecord() const {
        cout<<"Date: "<<date<<endl;
        cout<<"In Time: "<<inTime<<endl;
        cout<<"Out Time: "<<outTime<<endl;
    }
    int totalMinutes() const {
        return timeToMinutes(outTime) - timeToMinutes(inTime);
    }
};

class Attendance : public TimeRecord {
    int lateInMinutes;
    int earlyGoMinutes;
    int shortHours;
    int excessHours;
public:
    Attendance() {
        lateInMinutes = 0;
        earlyGoMinutes = 0;
        shortHours = 0;
        excessHours = 0;
    }
    void calculateExcessorLateMins() {
        const int expectedIn = 9 * 60;
        const int expectedOut = 17 * 60;
        int inTimeMins = timeToMinutes (inTime);
        int outTimeMins = timeToMinutes (outTime);
        if (inTimeMins > expectedIn) {
            lateInMinutes = inTimeMins - expectedIn;
        }
        else {
            lateInMinutes = 0;
        }
        if (outTimeMins < expectedOut) {
            earlyGoMinutes = expectedOut - outTimeMins;
        }
        else {
            earlyGoMinutes = 0;
        }
        int workedMinutes = outTimeMins - inTimeMins;
        int requiredMinutes = expectedOut - expectedIn;
        if (workedMinutes < requiredMinutes) {
            shortHours = requiredMinutes - workedMinutes;
            excessHours = 0;
        }
        else {
            shortHours = 0;
            excessHours = workedMinutes - requiredMinutes;
        }
    }
    void displayAttendanceReport() const {
        displayTimeRecord();
        cout<<"Late In (minutes): "<<lateInMinutes<<endl;
        cout<<"Early Go (minutes): "<<earlyGoMinutes<<endl;
        cout<<"Short Hours (minutes): "<<shortHours<<endl;
        cout<<"Excess Hours (minutes): "<<excessHours<<endl;
    }
    int getLateInMinutes() const {
        return lateInMinutes;
    }
    int getEarlyGoMinutes() const {
        return earlyGoMinutes;
    }
    int getShortHours() const {
        return shortHours;
    }
    int getExcessHours() const {
        return excessHours;
    }
};

class Teacher : public Person {
    string subject;
    string department;
    Attendance *attendanceRecords;
    int attendanceCount;
public:
    Teacher() {
        subject = "";
        department = "";
        attendanceCount = 0;
    }
    Teacher(string name, int id, string subject, string department) {
        this-> name = name;
        this-> id = id;
        this-> subject = subject;
        this-> department = department;
        this-> attendanceRecords = nullptr;
        this-> attendanceCount = 0;
    }
    ~Teacher() {
        delete[] attendanceRecords;
        cout<<"Teacher Destructor called..."<<endl;
    }
    void getTeacherDetails() {
        getPersonalInfo();
        cin.ignore();
        cout<<"Enter subject:"<<endl;
        getline(cin, subject);
        cout<<"Enter department:"<<endl;
        getline(cin, department);
        cout<<"Enter number of attendance records:"<<endl;
        cin>>attendanceCount;
        cin.ignore();
        attendanceRecords = new Attendance[attendanceCount];
        for (int i = 0; i < attendanceCount; i++) {
            cout<<"Attendance Record # "<<(i + 1)<<endl;
            cout<<endl;
            attendanceRecords[i].setInTime();
            attendanceRecords[i].setOutTime();
            attendanceRecords[i].calculateExcessorLateMins();
        }
    }
    void displayTeacherInfo() const {
        displayPersonalInfo();
        cout<<"Subject: "<<subject<<endl;
        cout<<"Department: "<<department<<endl;
        for (int i = 0; i < attendanceCount; i++) {
            cout<<"Attendance Record # "<<(i + 1)<<":"<<endl;
            attendanceRecords[i].displayAttendanceReport();
        }
    }
    void saveToFile(ofstream &out) const {
        out<<name<<endl;
        out<<id<<endl;
        out<<subject<<endl;
        out<<department<<endl;
        out<<attendanceCount<<endl;
        for (int i = 0; i < attendanceCount; i++) {
            out<<attendanceRecords[i].getDate()<<endl;
            out<<attendanceRecords[i].totalMinutes()<<endl;
        }
    }
    void loadFromFile(ifstream &in) {
        getline(in, name);
        in>>id;
        in.ignore();
        getline(in, subject);
        getline(in, department);
        in>>attendanceCount;
        in.ignore();
        attendanceRecords = new Attendance[attendanceCount];
        for (int i = 0; i < attendanceCount; i++) {
            string date;
            int mins;
            getline(in, date);
            in>>mins;
            in.ignore();
            attendanceRecords[i].setDate(date);
            attendanceRecords[i].setTotalMinutes(mins);
            attendanceRecords[i].calculateExcessorLateMins();
        }
    }
    int getAttendanceCount() const {
        return attendanceCount;
    }
    const Attendance &getAttendanceRecord(int index) const {
        return attendanceRecords[index];
    }
};

class Admin : public Person {
private:
    string role;
public:
    Admin() {
        this-> name = "";
        this-> id = 0;
        this-> role = "";
    }
    Admin(string name, int id, string role) {
        this->name = name;
        this->id = id;
        this->role = role;
    }
    void viewReports(Teacher* teachers, int count) {
        if (count == 0) {
            cout<<"No teachers to display"<<endl;
            return;
        }
        cout<<endl;
        cout<<"ATTENDANCE SUMMARY REPORT"<<endl;
        cout<<"======================================================================================="<<endl;
        cout<<"ID    ";
        cout<<"Name                ";
        cout<<"     Total Late";
        cout<<"     Total Early";
        cout<<"     Total Short";
        cout<<"     Total Excess" << endl;
        for (int i = 0; i < count; i++) {
            int totalLate = 0;
            int totalEarly = 0;
            int totalShort = 0;
            int totalExcess = 0;
            for (int j = 0; j < teachers[i].getAttendanceCount(); j++) {
                const Attendance& record = teachers[i].getAttendanceRecord(j);
                totalLate += record.getLateInMinutes();
                totalEarly += record.getEarlyGoMinutes();
                totalShort += record.getShortHours();
                totalExcess += record.getExcessHours();
            }
            cout<<teachers[i].getID()<<"    ";
            string name = teachers[i].getName();
            if (name.length() > 20)
                name = name.substr(0, 17) + "...";
            cout<<name;
            for (int s = name.length(); s < 20; s++)
                cout << ' ';
            string late = to_string(totalLate);
            cout<<string(15 - late.length(), ' ')<<late;
            string early = to_string(totalEarly);
            cout<<string(15 - early.length(), ' ')<<early;
            string shrt = to_string(totalShort);
            cout<<string(15 - shrt.length(), ' ')<<shrt;
            string exc = to_string(totalExcess);
            cout<<string(15 - exc.length(), ' ')<<exc<<endl;
        }
        cout<<"======================================================================================="<<endl;
    }
    string getRole() const {
        return role;
    }
};

class Department : protected Teacher {
private:
    string departmentName;
    int numberOfTeachers;
public:
    Department() {
        this-> departmentName = "";
        this-> numberOfTeachers = 0;
    }
    void setDepartmentInfo(string name, int count) {
        departmentName = name;
        numberOfTeachers = count;
    }
    void displayDepartmentInfo() {
        cout<<"Department Name: "<<departmentName<<endl;
        cout<<"Number of Teachers: "<<numberOfTeachers<<endl;
    }
};

class ReportGenerator : public Teacher {
private:
    Teacher teachers[3];
    int teacherCount;
    string reportDate;
    int dateToInt(const string& date) const {
        string day = date.substr(0, 2);
        string month = date.substr(3, 2);
        string year = date.substr(6, 4);
        return stoi(year + month + day);
    }
public:
    ReportGenerator() {
        teacherCount = 0;
        reportDate = "";
    }
    void inputTeachers() {
        cout<<"Enter number of teachers (1-3): "<<endl;
        cin>>teacherCount;
        if (teacherCount < 1 || teacherCount > 3) {
            cout<<"Invalid number, add a number between 1-3"<<endl;
            teacherCount = 1;
        }
        for (int i = 0; i < teacherCount; i++) {
            cout<<endl;
            cout<<"Enter details for Teacher "<<(i + 1)<<":"<<endl;
            teachers[i].getTeacherDetails();
        }
    }
    void generateDailyReport() {
        string date;
        cin.ignore();
        cout<<"Enter date for daily report (DD-MM-YYYY): "<<endl;
        getline(cin, date);
        cout<<endl;
        cout<<"DAILY ATTENDANCE REPORT(" << date << ")"<<endl;
        cout<<"=========================================="<<endl;
        for (int i = 0; i < teacherCount; i++) {
            cout<<endl;
            cout<<"Teacher: "<<teachers[i].getName()<<" (ID: "<<teachers[i].getID()<< ")"<<endl;
            bool found = false;
            for (int j = 0; j < teachers[i].getAttendanceCount(); j++) {
                const Attendance& record = teachers[i].getAttendanceRecord(j);
                if (record.getDate() == date) {
                    record.displayAttendanceReport();
                    found = true;
                    break;
                }
            }
            if (!found) {
                cout<<"No attendance record found for this date"<<endl;
            }
        }
        cout<<"=========================================="<<endl;
    }
    void generateMonthlySummary() {
        string monthYear;
        cout<<"Enter month and year for summary (MM-YYYY):"<<endl;
        getline(cin, monthYear);
        cout<<endl;
        cout<<"MONTHLY SUMMARY REPORT(" << monthYear << ")"<<endl;
        cout<<"============================================="<<endl;
        for (int i = 0; i < teacherCount; i++) {
            int totalLate = 0;
            int totalEarly = 0;
            int totalShort = 0;
            int totalExcess = 0;
            int recordsFound = 0;
            for (int j = 0; j < teachers[i].getAttendanceCount(); j++) {
                const Attendance& record = teachers[i].getAttendanceRecord(j);
                string recordMonth = record.getDate().substr(3, 7);
                if (recordMonth == monthYear) {
                    totalLate += record.getLateInMinutes();
                    totalEarly += record.getEarlyGoMinutes();
                    totalShort += record.getShortHours();
                    totalExcess += record.getExcessHours();
                    recordsFound++;
                }
            }
            cout<<endl;
            cout<<"Teacher: "<<teachers[i].getName()<<" (ID: "<<teachers[i].getID() << ")"<<endl;
            cout<<"Records Found: "<<recordsFound<<endl;
            cout<<"Total Late: "<<totalLate<<" minutes"<<endl;
            cout<<"Total Early: "<<totalEarly<<" minutes"<<endl;
            cout<<"Total Short Hours: "<<totalShort<<" minutes"<<endl;
            cout<<"Total Excess Hours: "<<totalExcess<<"minutes"<<endl;
        }
        cout<<"============================================="<<endl;
    }
    void generateUserSelectedDateSummary() {
        string startDate, endDate;
        cout<<"Enter start date (DD-MM-YYYY):"<<endl;
        getline(cin, startDate);
        cout<<"Enter end date (DD-MM-YYYY):"<<endl;
        getline(cin, endDate);
        int start = dateToInt(startDate);
        int end = dateToInt(endDate);
        cout<<endl;
        cout<<"DATE RANGE SUMMARY ("<<startDate<<" to "<<endDate<<")"<<endl;
        cout<<"=================================================="<<endl;
        for (int i = 0; i < teacherCount; i++) {
            int totalLate = 0;
            int totalEarly = 0;
            int totalShort = 0;
            int totalExcess = 0;
            int recordsFound = 0;
            for (int j = 0; j < teachers[i].getAttendanceCount(); j++) {
                const Attendance& record = teachers[i].getAttendanceRecord(j);
                int recordDate = dateToInt(record.getDate());
                if (recordDate >= start && recordDate <= end) {
                    totalLate += record.getLateInMinutes();
                    totalEarly += record.getEarlyGoMinutes();
                    totalShort += record.getShortHours();
                    totalExcess += record.getExcessHours();
                    recordsFound++;
                }
            }
            cout<<endl;
            cout<<"Teacher: "<<teachers[i].getName()<<" (ID: "<<teachers[i].getID()<< ")"<<endl;
            cout<<"Records Found: "<<recordsFound<<endl;
            cout<<"Total Late: "<<totalLate<<"minutes"<<endl;
            cout<<"Total Early: "<<totalEarly<<" minutes"<<endl;
            cout<<"Total Short Hours: "<<totalShort<<"minutes"<<endl;
            cout<<"Total Excess Hours: "<<totalExcess<<"minutes"<<endl;
        }
        cout<<"=================================================="<<endl;
    }
    ~ReportGenerator() {
        cout<<"ReportGenerator Destructor called..."<<endl;
    }
};
void saveTeachersToFile(Teacher *t, int count) {
    ofstream out("teachers.txt");
    out<<count<<endl;
    for (int i = 0; i < count; i++) {
        t[i].saveToFile(out);
    }
    out.close();
    cout<<"Data saved to teachers.txt"<<endl;
}
void loadTeachersFromFile(Teacher*& t, int &count) {
    ifstream in("teachers.txt");
    if (!in) {
        cout<<"File not found"<<endl;
        return;
    }
    in>>count;
    in.ignore();
    t = new Teacher[count];
    for (int i = 0; i < count; i++) {
        t[i].loadFromFile(in);
    }
    in.close();
    cout<<"Data loaded from teachers.txt"<<endl;
}
void searchTeacher(Teacher* t, int count, int id) {
    for (int i = 0; i < count; i++) {
        if (t[i].getID() == id) {
            t[i].displayTeacherInfo();
            return;
        }
    }
    cout<<"Teacher with ID "<<id<<" has not been found"<<endl;
}
void deleteTeacher(Teacher*& t, int &count, int id) {
    int idx = -1;
    for (int i = 0; i < count; i++) {
        if (t[i].getID() == id) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        cout<<"Teacher not found"<<endl;
        return;
    }
    Teacher* temp = new Teacher[count - 1];
    for (int i = 0, j = 0; i < count; i++) {
        if (i != idx) temp[j++] = t[i];
    }
    delete[] t;
    t = temp;
    count--;
    cout<<"Teacher deleted"<<endl;
    saveTeachersToFile(t, count);
}

Admin adminLogin() {
    string name, role;
    int id;
    cout<<"Enter admin name:"<<endl;
    getline(cin, name);
    cout<<"Enter admin ID:"<<endl;
    cin>>id;
    cin.ignore();
    cout<<"Enter admin role (Head/Manager/Supervisor):"<<endl;
    getline(cin, role);
    cout<<"Welcome "<<role<<" "<<name<<"!"<<endl;
    return Admin(name, id, role);
}
int main() {
    Admin admin = adminLogin();
    string role = admin.getRole();
    if (role != "Head" && role != "head" && role != "Manager" && role != "manager" && role != "Supervisor" && role != "supervisor") {
        cout<<"Access denied, You are not authorized to use this system"<<endl;
        return 0;
    }
    Teacher *teachers = nullptr;
    int teacherCount = 0;
    int choice;
    do {
        cout<<endl;
        cout<<"Main Menu:"<<endl;
        cout<<"1. Add Teacher"<<endl;
        cout<<"2. Display All Teachers"<<endl;
        cout<<"3. Save Data to File"<<endl;
        cout<<"4. Load Data from File"<<endl;
        cout<<"5. Search Teacher by ID"<<endl;
        cout<<"6. Delete Teacher by ID"<<endl;
        cout<<"7. View Attendance Summary Reports"<<endl;
        cout<<"8. Generate Custom Reports"<<endl;
        cout<<"9. Exit"<<endl;
        cout<<"Enter choice:"<<endl;
        cin>>choice;
        cin.ignore();
        if (choice == 1) {
            int n;
            cout<<"Enter number of teachers to add:"<<endl;
            cin>>n;
            Teacher *newList = new Teacher[teacherCount + n];
            for (int i = 0; i < teacherCount; i++)
                newList[i] = teachers[i];
            for (int i = teacherCount; i < teacherCount + n; i++) {
                bool validation = false;
                while (!validation) {
                    newList[i].getTeacherDetails();
                    int ID = newList[i].getID();
                    validation = true;
                    for (int j = 0; j < i; j++) {
                        if (newList[j].getID() == ID) {
                            cout<<"This ID "<<ID<<" already exists,Please Re-enter details"<<endl;
                            validation = false;
                            break;
                        }
                    }
                }
            }
            delete[] teachers;
            teachers = newList;
            teacherCount += n;
        }
        else if (choice == 2) {
            for (int i = 0; i < teacherCount; i++) {
                teachers[i].displayTeacherInfo();
                cout<<endl;
            }
        }
        else if (choice == 3) {
            saveTeachersToFile(teachers, teacherCount);
        }
        else if (choice == 4) {
            loadTeachersFromFile(teachers, teacherCount);
        }
        else if (choice == 5) {
            int id;
            cout<<"Enter ID to search:"<<endl;
            cin>>id;
            searchTeacher(teachers, teacherCount, id);
        }
        else if (choice == 6) {
            int id;
            cout<<"Enter ID to delete: "<<endl;
            cin>>id;
            deleteTeacher(teachers, teacherCount, id);
        }
        else if (choice == 7) {
            admin.viewReports(teachers, teacherCount);
        }
        else if (choice == 8) {
            ReportGenerator rg;
            rg.inputTeachers();
            int reportChoice;
            do {
                cout<<endl;
                cout<<"Report Generator Menu:"<<endl;
                cout<<"1. Generate Daily Report"<<endl;
                cout<<"2. Generate Monthly Summary"<<endl;
                cout<<"3. Generate Date Range Summary"<<endl;
                cout<<"4. Return to Main Menu"<<endl;
                cout<<"Enter choice: ";
                cin>>reportChoice;
                if (reportChoice == 1) rg.generateDailyReport();
                else if (reportChoice == 2) rg.generateMonthlySummary();
                else if (reportChoice == 3) rg.generateUserSelectedDateSummary();
            } while (reportChoice != 4);
        }
    }
    while (choice != 9);
    delete[] teachers;
    return 0;
}

