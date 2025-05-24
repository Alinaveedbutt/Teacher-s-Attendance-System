#include <iostream>
#include <string>
using namespace std;

class Person {
protected:
    string name;
    int id;
public:
    Person() : name(""), id(0) {}
    Person(string n,int i) { name = n; id = i; }
    virtual void getPersonalInfo() {
        cout<<"Enter name:"<<endl;
        cin.ignore();
        getline(cin,name);
        cout<<"Enter ID:"<<endl;
        cin>>id;
    }
    virtual void displayPersonalInfo() const {
        cout<<"Name: "<<name<<endl;
        cout<< "ID: "<<id<<endl;
    }
    int getID() const { return id; }
    virtual ~Person() {
        cout <<"Person Destructor called"<<endl;
    }
};

class TimeRecord {
protected:
    string date;
    int inHour,inMinute,outHour,outMinute;
public:
    TimeRecord() : date(""), inHour(0), inMinute(0), outHour(0), outMinute(0) {}
    void setInTime() {
        cout<<"Enter date (DD-MM-YYYY):"<<endl;
        cin.ignore();
        getline(cin,date);
        cout<<"Enter time in (HH MM):"<<endl;
        cin>>inHour>>inMinute;
    }
    void setOutTime() {
        cout<<"Enter time out (HH MM):"<<endl;
        cin>>outHour>>outMinute;
    }
    string getDate() const { return date; }
    void displayTimeRecord() const {
        cout<<"Date: "<<date<<", In: ";
        printTime(inHour,inMinute);
        cout<<", Out: ";
        printTime(outHour,outMinute);
        cout<<endl;
    }
    int totalMinutes() const {
        return (outHour * 60 + outMinute) - (inHour * 60 + inMinute);
    }
    void printTime(int h,int m) const {
        if(h<10) cout<<"0";
        cout<<h<<":";
        if(m<10) cout<<"0";
        cout<<m;
    }
};

class Attendance : public TimeRecord {
    int lateInMinutes;
    int earlyGoMinutes;
    int shortHours;
    int excessHours;
public:
    Attendance() : lateInMinutes(0), earlyGoMinutes(0), shortHours(0), excessHours(0) {}
    void calculateMetrics() {
        const int expectedIn = 9*60;
        const int expectedOut = 17*60;
        int inTime = inHour*60 + inMinute;
        int outTime = outHour*60 + outMinute;
        lateInMinutes = (inTime > expectedIn) ? (inTime - expectedIn) : 0;
        earlyGoMinutes = (outTime < expectedOut) ? (expectedOut - outTime) : 0;
        int workedMinutes = outTime - inTime;
        int requiredMinutes = expectedOut - expectedIn;
        if(workedMinutes < requiredMinutes) {
            shortHours = requiredMinutes - workedMinutes;
            excessHours = 0;
        } else {
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
    int getLateInMinutes() const { return lateInMinutes; }
    int getEarlyGoMinutes() const { return earlyGoMinutes; }
    int getShortHours() const { return shortHours; }
    int getExcessHours() const { return excessHours; }
};

class Teacher : public Person {
    string subject;
    string department;
    Attendance* attendanceRecords;
    int attendanceCount;
public:
    Teacher() : Person(), subject(""), department(""), attendanceRecords(nullptr), attendanceCount(0) {}
    Teacher(string n,int i,string sub,string dept) {
        Person::name = n;
        Person::id = i;
        subject = sub;
        department = dept;
        attendanceRecords = nullptr;
        attendanceCount = 0;
    }
    ~Teacher() {
        delete[] attendanceRecords;
        cout<<"Teacher Destructor called"<<endl;
    }
    void getTeacherDetails() {
        getPersonalInfo();
        cin.ignore();
        cout<<"Enter subject:"<<endl;
        getline(cin,subject);
        cout<<"Enter department:"<<endl;
        getline(cin,department);
        cout<<"Enter number of attendance records:"<<endl;
        cin>>attendanceCount;
        attendanceRecords = new Attendance[attendanceCount];
        for(int i=0; i<attendanceCount; i++) {
            cout<<"Attendance Record # "<<(i+1)<<endl;
            attendanceRecords[i].setInTime();
            attendanceRecords[i].setOutTime();
            attendanceRecords[i].calculateMetrics();
        }
    }
    void displayTeacherInfo() const {
        displayPersonalInfo();
        cout<<"Subject: "<<subject<<endl;
        cout<<"Department: "<<department<<endl;
        for(int i=0; i<attendanceCount; i++) {
            cout<<"Attendance Record # "<<(i+1)<<":"<<endl;
            attendanceRecords[i].displayAttendanceReport();
        }
    }
    int getAttendanceCount() const { return attendanceCount; }
    const Attendance* getAttendanceRecords() const { return attendanceRecords; }
};

class Admin : public Person {
    string role;
public:
    Admin() : role("") {}
    Admin(string n,int i,string r) {
        Person::name = n;
        Person::id = i;
        role = r;
    }
    void manageTeachers(Teacher* teachers,int count) {
        cout<<"Manage Teachers Menu:"<<endl;
        cout<<"Press 1 to View all teachers"<<endl;
        cout<<"Press 2 to Search teacher by ID"<<endl;
        cout<<"Press 3 to Exit"<<endl;
        int choice;
        do {
            cout<<"Enter choice:"<<endl;
            cin>>choice;
            if(choice==1) {
                for(int i=0; i<count; i++) {
                    teachers[i].displayTeacherInfo();
                    cout<<endl;
                }
            } else if(choice==2) {
                int tid;
                cout<<"Enter Teacher ID to search:"<<endl;
                cin>>tid;
                bool found=false;
                for(int i=0; i<count; i++) {
                    if(teachers[i].getID()==tid) {
                        teachers[i].displayTeacherInfo();
                        found=true;
                        break;
                    }
                }
                if(!found) cout<<"Teacher with ID "<<tid<<" not found"<<endl;
            }
        } while(choice!=3);
    }
    void viewReports(Teacher* teachers,int count) {
        cout<<"Attendance Summaries for All Teachers:"<<endl;
        for(int i=0; i<count; i++) {
            teachers[i].displayTeacherInfo();
            cout<<endl;
        }
    }
};

class ReportGenerator {
    Teacher* teachers;
    int teacherCount;
public:
    ReportGenerator() : teachers(nullptr), teacherCount(0) {}
    ~ReportGenerator() {
        delete[] teachers;
        cout<<"Report Generator destructor called"<<endl;
    }
    void inputTeachers() {
        cout<<"How many teachers do you want to input?"<<endl;
        cin>>teacherCount;
        teachers = new Teacher[teacherCount];
        for(int i=0; i<teacherCount; i++) {
            cout<<"Enter details for Teacher "<<(i+1)<<endl;
            teachers[i].getTeacherDetails();
        }
    }
    void generateDailyReport() const {
        cout<<"Daily Attendance Report:"<<endl;
        for(int i=0; i<teacherCount; i++) {
            cout<<"Teacher # "<<(i+1)<<endl;
            teachers[i].displayTeacherInfo();
            cout<<endl;
        }
    }
    void generateMonthlyReports() const {
        cout<<"Monthly Attendance Report:"<<endl;
        for(int i=0; i<teacherCount; i++) {
            int totalLate=0,totalEarly=0,totalShort=0,totalExcess=0;
            const Attendance* records=teachers[i].getAttendanceRecords();
            int count=teachers[i].getAttendanceCount();
            for(int j=0; j<count; j++) {
                totalLate += records[j].getLateInMinutes();
                totalEarly += records[j].getEarlyGoMinutes();
                totalShort += records[j].getShortHours();
                totalExcess += records[j].getExcessHours();
            }
            cout<<"Teacher ID: "<<teachers[i].getID()<<endl;
            cout<<"Total Late In (minutes): "<<totalLate<<endl;
            cout<<"Total Early Go (minutes): "<<totalEarly<<endl;
            cout<<"Total Short Hours (minutes): "<<totalShort<<endl;
            cout<<"Total Excess Hours (minutes): "<<totalExcess<<endl;
            cout<<endl;
        }
    }
};
int main() {
    ReportGenerator system;
    system.inputTeachers();
    system.generateDailyReport();
    system.generateMonthlyReports();
    return 0;
}
