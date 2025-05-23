#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>  // 添加缺失的头文件

using namespace std;

class Student {
public:
    string name;
    string id;
    vector<double> scores;

    double getTotal() {
        double sum = 0;
        for (auto s : scores) sum += s;
        return sum;
    }
};

class GradeSystem {
private:
    vector<Student> students;
    const string datafile = "grades.dat";

    // 新增行解析方法
    void parseLine(const string& line, Student& s) {
        size_t pos = 0;
        size_t next = line.find(',');

        // 解析姓名
        s.name = line.substr(pos, next - pos);
        pos = next + 1;

        // 解析学号
        next = line.find(',', pos);
        s.id = line.substr(pos, next - pos);
        pos = next + 1;

        // 解析成绩
        s.scores.clear();
        while (pos < line.size()) {
            next = line.find(',', pos);
            if (next == string::npos) next = line.size();

            double score = stod(line.substr(pos, next - pos));
            s.scores.push_back(score);
            pos = next + 1;
        }
    }

public:
    void addStudent() {
        Student s;
        cout << "Enter student name: ";
        cin >> s.name;
        cout << "Enter student ID: ";
        cin >> s.id;

        int subj;
        cout << "Number of subjects: ";
        while (!(cin >> subj) || subj <= 0) {
            cin.clear();
            cin.ignore(1024, '\n');
            cout << "Invalid input! Enter positive integer: ";
        }

        for (int i = 0; i < subj; i++) {
            double score;
            cout << "Score for subject " << i + 1 << ": ";
            while (!(cin >> score) ){
                cin.clear();
                cin.ignore(1024, '\n');
                cout << "Invalid input! Enter numeric value: ";
            }
            s.scores.push_back(score);
        }
        students.push_back(s);
    }

    void showAll() {
        if (students.empty()) {
            cout << "\nNo students in system!\n";
            return;
        }

        for (auto& s : students) {
            cout << "\nName: " << s.name
                << "\nID: " << s.id
                << "\nScores: ";
            for (auto sc : s.scores) cout << sc << " ";
            cout << "\nTotal: " << s.getTotal() << endl;
        }
    }

    void searchStudent() {
        if (students.empty()) {
            cout << "\nNo students in system!\n";
            return;
        }

        string id;
        cout << "Enter student ID: ";
        cin >> id;

        auto it = find_if(students.begin(), students.end(),
            [&id](const Student& s) { return s.id == id; });

        if (it != students.end()) {
            cout << "\nFound student:"
                << "\nName: " << it->name
                << "\nTotal Score: " << it->getTotal() << endl;
        }
        else {
            cout << "Student not found!" << endl;
        }
    }

    void saveToFile() {
        ofstream fout(datafile);
        if (!fout) {
            cerr << "Error saving data!\n";
            return;
        }

        for (auto& s : students) {
            fout << s.name << "," << s.id;
            for (auto score : s.scores) {
                fout << "," << score;
            }
            fout << "\n";
        }
        cout << "Data saved successfully!\n";
    }

    void loadFromFile() {
        ifstream fin(datafile);
        if (!fin) {
            cerr << "No previous data found\n";
            return;
        }

        students.clear();
        string line;
        while (getline(fin, line)) {
            Student s;
            parseLine(line, s);
            students.push_back(s);
        }
        cout << "Loaded " << students.size() << " student records\n";
    }
};

int main() {
    GradeSystem sys;
    sys.loadFromFile();  // 启动时自动加载数据

    int choice;
    do {
        cout << "\n1. Add Student\n2. Show All\n3. Search\n4. Save & Exit\nChoice: ";
        while (!(cin >> choice) || choice < 1 || choice > 4) {
            cin.clear();
            cin.ignore(1024, '\n');
            cout << "Invalid choice! Enter 1-4: ";
        }

        switch (choice) {
        case 1: sys.addStudent(); break;
        case 2: sys.showAll(); break;
        case 3: sys.searchStudent(); break;
        case 4: sys.saveToFile(); break;
        }
    } while (choice != 4);

    return 0;
}