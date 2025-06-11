#include <iostream>
#include <string>
#include <fstream>
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
        cout<<"ID: "<<id<<endl;
    }
    int getID() const { return id; }
    string getName() const { return name; }
    virtual ~Person() {
        cout<<"Person Destructor called"<<endl;
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
    void setDate(string d) { date = d; }
    void setTotalMinutes(int mins) {
        inHour = 9;
        inMinute = 0;
        outHour = 9 + (mins / 60);
        outMinute = mins % 60;
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
        name = n;
        id = i;
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
    void saveToFile(ofstream& out) const {
        out<<name<<'\n'<<id<<'\n'<<subject<<'\n'<<department<<'\n'<<attendanceCount<<'\n';
        for(int i=0; i<attendanceCount; i++) {
            out<<attendanceRecords[i].getDate()<<'\n'<<attendanceRecords[i].totalMinutes()<<'\n';
        }
    }
    void loadFromFile(ifstream& in) {
        getline(in,name);
        in>>id;
        in.ignore();
        getline(in,subject);
        getline(in,department);
        in>>attendanceCount;
        in.ignore();
        attendanceRecords = new Attendance[attendanceCount];
        for(int i=0; i<attendanceCount; i++) {
            string d;
            int mins;
            getline(in,d);
            in>>mins;
            in.ignore();
            attendanceRecords[i].setDate(d);
            attendanceRecords[i].setTotalMinutes(mins);
            attendanceRecords[i].calculateMetrics();
        }
    }
};
void saveTeachersToFile(Teacher* t,int count) {
    ofstream out("teachers.txt");
    out<<count<<'\n';
    for(int i=0; i<count; i++) {
        t[i].saveToFile(out);
    }
    out.close();
    cout<<"Data saved to teachers.txt"<<endl;
}
void loadTeachersFromFile(Teacher*& t,int& count) {
    ifstream in("teachers.txt");
    if(!in) {
        cout<<"File not found."<<endl;
        return;
    }
    in>>count;
    in.ignore();
    t = new Teacher[count];
    for(int i=0; i<count; i++) {
        t[i].loadFromFile(in);
    }
    in.close();
    cout<<"Data loaded from teachers.txt"<<endl;
}
void searchTeacher(Teacher* t,int count,int id) {
    for(int i=0; i<count; i++) {
        if(t[i].getID() == id) {
            t[i].displayTeacherInfo();
            return;
        }
    }
    cout<<"Teacher with ID "<<id<<" not found."<<endl;
}
void deleteTeacher(Teacher*& t,int& count,int id) {
    int idx = -1;
    for(int i=0; i<count; i++) {
        if(t[i].getID() == id) {
            idx = i;
            break;
        }
    }
    if(idx == -1) {
        cout<<"Teacher not found."<<endl;
        return;
    }
    Teacher* temp = new Teacher[count-1];
    for(int i=0, j=0; i<count; i++) {
        if(i != idx) temp[j++] = t[i];
    }
    delete[] t;
    t = temp;
    count--;
    cout<<"Teacher deleted."<<endl;
    saveTeachersToFile(t,count);
}
int main() {
    Teacher* teachers = nullptr;
    int teacherCount = 0;
    int choice;
    do {
        cout<<endl;
        cout<<"Menu:"<<endl;
        cout<<"1. Add Teacher"<<endl;
        cout<<"2. Display All"<<endl;
        cout<<"3. Save"<<endl;
        cout<<"4. Load"<<endl;
        cout<<"5. Search"<<endl;
        cout<<"6. Delete"<<endl;
        cout<<"7. Exit"<<endl;
        cout<<"Enter choice:"<<endl;
        cin>>choice;
        if(choice == 1) {
            cout<<"Enter number of teachers to add:"<<endl;
            int n;
            cin>>n;
            Teacher* newList = new Teacher[teacherCount+n];
            for(int i=0; i<teacherCount; i++)
                newList[i] = teachers[i];
            for(int i=teacherCount; i<teacherCount+n; i++) {
                newList[i].getTeacherDetails();
            }
            delete[] teachers;
            teachers = newList;
            teacherCount += n;
        }
        else if(choice == 2) {
            for(int i=0; i<teacherCount; i++) {
                teachers[i].displayTeacherInfo();
                cout<<endl;
            }
        }
        else if(choice == 3) {
            saveTeachersToFile(teachers, teacherCount);
        }
        else if(choice == 4) {
            loadTeachersFromFile(teachers, teacherCount);
        }
        else if(choice == 5) {
            int id;
            cout<<"Enter ID to search:"<<endl;
            cin>>id;
            searchTeacher(teachers, teacherCount, id);
        }
        else if(choice == 6) {
            int id;
            cout<<"Enter ID to delete:"<<endl;
            cin>>id;
            deleteTeacher(teachers, teacherCount, id);
        }
    }
    while(choice != 7);
    delete[] teachers;
    return 0;
}
