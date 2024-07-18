#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <cmath>
#include <iomanip>
#include <random>
#include <algorithm>

using namespace std;

class Random {
public:
    static double drnd() {
        int x = rand() + 1, y = rand() + 1;
        x %= y;
        return x * 1. / y;
    }

    static int randomNumberInRange(int min, int max) {
        return min + rand() % (max - min + 1);
    }

    static string randomName() {
        string res = "";
        res += (char)(randomNumberInRange(0, 25) + 'A');
        for (int i = 0; i < 5; ++i)
            res += (char)(randomNumberInRange(0, 25) + 'a');
        res += " ";
        res += (char)(randomNumberInRange(0, 25) + 'A');
        for (int i = 0; i < 4; ++i)
            res += (char)(randomNumberInRange(0, 25) + 'a');
        return res;
    }

    static string randomListName() {
        vector<string> randomNames = { "Babakov Ilya", "Beskrovniy Nikolay", "Bryzgalov Vladislave", "Busheva Milana",
        "Vyazovik meTimofey", "Gaysina Sofiya", "Grigoryan Sergey", "Zaporozhets Artyom", "Izotov Vladislave",
        "Kazaku Maxim", "Malyuzhniy Kihail", "Lisenkov Stanislav", "Olhov Ivanuzhka", "Popov Vladimir",
        "Samoylov Anatoliy", "Sheven Veronika", "Shtukmayster Herman", "Eyhman Maxim", "Nazaykin Egor",
        "Mashtaler Maxim", "Makogon Yaroslav", "Typhoon Phil", "Volnov Ivan", "Fomin Ivan", "Doronin Fedorodoro",
        "Kobzev Andrew", "Glukhov Coach", "Tyrin Mik", "Irval Alex", "Dekartach Alex", "Lyazer Stepan",
        "Leshinskiy Artem", "Antonova Anna", "Levushkina Anna", "Zadunaev Vladislave" };
        int ind = randomNumberInRange(0, 34);
        return randomNames[ind];
    }
};

class CourseManagementSystem {
public:
    void addStudent(string name, string language, string level, int intensity, bool isIndividual) {
        if (mp[name] == nullptr)
            mp[name] = new Student(name);
        if (find(_students.begin(), _students.end(), mp[name]) == _students.end())
            _students.push_back(mp[name]);
        if (isIndividual)
            mp[name]->addIndividualCource(language, level, intensity);
        else
            addToGroup(mp[name], language, level, intensity),
            checkGroups();
    }

    void checkPayments(double chance = 0.8) {
        int it = Random::randomNumberInRange(0, _students.size());
        for (int i = 0; i < it; ++i)
            if (Random::drnd() > chance) {
                int ind = Random::randomNumberInRange(0, max(0, (int)_students.size() - 1));
                int kickCource = Random::randomNumberInRange(0, _students[ind]->getBusy() - 1);
                Group* group = _students[ind]->removeCourse(kickCource);
                if (group != nullptr)
                    group->removeStudent(_students[ind]),
                    checkGroups();
                if (_students[ind]->getBusy() == 0)
                    removeStudent(_students[ind]);
            }
    }

    int getNumPeople() {
        return _students.size();
    }

    int getNumGroups() {
        int ans = 0;
        for (int i = 0; i < (int)_groups.size(); ++i)
            if (_groups[i]->getNumStudents() >= 5) ++ans;
        return ans;
    }

    vector<string> getStudents() {
        vector<string> res;
        for (auto student : _students)
            res.push_back(student->getName());
        return res;
    }

    auto getStudentCources(int ind) {
        auto cources = _students[ind]->getCourses();
        vector<int> groups;
        for (auto group : _students[ind]->getGroups())
            groups.push_back(find(_groups.begin(), _groups.end(), group) - _groups.begin() + 1);
        return make_pair(cources, groups);
    }

    pair<string, string> getStudentInfo(int ind) {
        return { _students[ind]->getName(), to_string(_students[ind]->getSumCost()) };
    }

    int getNumStudentsOfGroup(int ind) {
        return _groups[ind]->getNumStudents();
    }

    vector<string> getStudentsOfGroup(int ind) {
        vector<string> res;
        for (auto student : _groups[ind]->getStudents())
            res.push_back(student->getName());
        return res;
    }

    vector<string> getGroupInfo(int ind) {
        vector<string> res;
        res.push_back(to_string(_groups[ind]->getNumStudents()));
        res.push_back(_groups[ind]->getLanguage());
        res.push_back(_groups[ind]->getLevel());
        res.push_back(to_string(_groups[ind]->getIntensity()) + " times/week");
        res.push_back(to_string(_groups[ind]->getCost()));
        return res;
    }

    int totalNumStudents() {
        return mp.size();
    }

    int totalNumGroups() {
        return _cnt_groups;
    }

    double averageNumGroup() {
        double sum = 0;
        for (auto group : _groups)
            if (group->getNumStudents() >= 5)
                sum += group->getNumStudents();
        if (getNumGroups() == 0) return 0;
        return sum / getNumGroups();
    }

    double averageCostGroup() {
        double sum = 0;
        for (auto group : _groups)
            if (group->getNumStudents() >= 5)
                sum += group->getCost();
        if (getNumGroups() == 0) return 0;
        return sum / getNumGroups();
    }

    double averageCostStudent() {
        double sum = 0;
        for (auto student : _students)
            sum += student->getSumCost();
        if (_students.size() == 0) return 0;
        return sum / _students.size();
    }

private:
    int _cnt_groups = 0;

    class Group;

    class Student {
    public:
        Student(string name) : _name(name) {}

        string getName() const { return _name; }

        vector<vector<string>> getCourses() {
            vector<vector<string>> res;
            for (const auto &course : _courses) {
                res.push_back( { } );
                res.back().push_back(course.getLanguage());
                res.back().push_back(course.getLevel());
                res.back().push_back(to_string(course.getIntensity()) + " times/week");
                res.back().push_back(to_string(course.getCost()));
            }
            return res;
        }

        vector<Group*> getGroups() { return _groups; }

        int getBusy() {
            return _courses.size() + _groups.size();
        }

        int getSumCost() {
            int res = 0;
            for (const auto &course : _courses)
                res += course.getCost();
            for (auto course : _groups)
                res += course->getCost();
            return res;
        }

        void addIndividualCource(string language, string level, int intensity) {
            if (find(_courses.begin(), _courses.end(), Individual_course(language, level, intensity)) == _courses.end())
                _courses.push_back(Individual_course(language, level, intensity));
        }

        void addToGroup(Group* group) {
            _groups.push_back(group);
        }

        Group* removeCourse(int ind) {
            if (ind < (int)_courses.size()) {
                auto it = next(_courses.begin(), ind);
                _courses.erase(it);
                return nullptr;
            }
            ind -= _courses.size();
            Group* group = _groups[ind];
            _groups.erase(_groups.begin() + ind);
            return group;
        }

    private:
        string _name;

        class Individual_course {
        public:
            Individual_course(string language, string level, int intensity) :
                _language(language), _level(level), _intensity(intensity) {
                if (language == "English")
                    _cost = 650;
                else if (language == "French")
                    _cost = 725;
                else if (language == "German")
                    _cost = 700;
                else if (language == "Japanese")
                    _cost = 775;
                if (level == "Middle")
                    _cost *= 2;
                else if (level == "Advanced")
                    _cost *= 3;
                _cost *= intensity * 0.8 * 0.75;
            }

            string getLanguage() const {
                return _language;
            }

            string getLevel() const {
                return _level;
            }

            int getIntensity() const {
                return _intensity;
            }

            int getCost() const {
                return _cost;
            }

            bool operator==(Individual_course ic) const {
                return _language == ic.getLanguage();
            }

        private:
            string _language;
            string _level;
            int _intensity, _cost;
        };

        vector<Individual_course> _courses;
        vector<Group*> _groups;

    };

    class Group {
    public:
        Group(string language, string level, int intensity)
            : _language(language), _level(level), _intensity(intensity) {
            if (language == "English")
                _cost = 575;
            else if (language == "French")
                _cost = 625;
            else if (language == "German")
                _cost = 600;
            else if (language == "Japanese")
                _cost = 675;
            if (level == "Middle")
                _cost *= 2;
            else if (level == "Advanced")
                _cost *= 3;
            _cost *= intensity * 0.8 * 0.75;
        }

        string getLanguage() const { return _language; }

        string getLevel() const { return _level; }

        int getIntensity() const { return _intensity; }

        int getCost() const { return _cost; }

        int getNumStudents() const { return _students.size(); }

        void addStudent(Student* student) {
            _students.push_back(student);
        }

        void removeStudent(Student* student) {
            for (auto it = _students.begin(); it != _students.end(); ++it) {
                if ((*it) == student) {
                    _students.erase(it);
                    break;
                }
            }
        }

        vector<Student*> getStudents() const {
            return _students;
        }

    private:
        string _language;
        string _level;
        int _intensity, _cost;
        vector<Student*> _students;
    };

    vector<Student*> _students;
    vector<Group*> _groups;

    map<string, Student*> mp;

    void addToGroup(Student* student, string language, string level, int intensity) {
        for (auto& group : student->getGroups())
            if (group->getLanguage() == language)
                return;
        for (auto& group : _groups)
            if (group->getLanguage() == language && group->getLevel() == level && group->getIntensity() == intensity) {
                student->addToGroup(group);
                int num_students = group->getNumStudents();
                group->addStudent(student);
                if (num_students < 5 && group->getNumStudents() >= 5)
                    ++_cnt_groups;
                return;
            }
        _groups.push_back(new Group(language, level, intensity));
        student->addToGroup(_groups.back());
        _groups.back()->addStudent(student);
    }

    void removeStudent(Student* student) {
        for (auto it = _students.begin(); it != _students.end(); ++it) {
            if ((*it) == student) {
                _students.erase(it);
                break;
            }
        }
    }

    void removeGroup(Group* group) {
        for (auto it = _groups.begin(); it != _groups.end(); ++it) {
            if ((*it) == group) {
                _groups.erase(it);
                break;
            }
        }
    }

    void checkGroups() {
        for (int i = 0; i < (int)_groups.size(); ++i) {
            if (_groups[i]->getNumStudents() == 0) {
                removeGroup(_groups[i]);
                --i;
            } else if (_groups[i]->getNumStudents() < 5) {
                for (int j = i + 1; j < (int)_groups.size(); ++j)
                    if (_groups[j]->getNumStudents() < 5 &&
                        _groups[j]->getLanguage() == _groups[i]->getLanguage() &&
                        _groups[j]->getLevel() == _groups[i]->getLevel() &&
                        _groups[j]->getIntensity() == _groups[i]->getIntensity()) {
                        auto students = _groups[j]->getStudents();
                        removeGroup(_groups[j]);
                        for (auto& student : students)
                            _groups[i]->addStudent(student);
                        --j;
                    }
                if (_groups[i]->getNumStudents() >= 5)
                    ++_cnt_groups;
            } else if (_groups[i]->getNumStudents() > 10) {
                auto students = _groups[i]->getStudents();
                for (auto& student : students)
                    _groups[i]->removeStudent(student);
                _groups.push_back(new Group(*_groups[i]));
                for (int j = 0; j < 10; ++j)
                    _groups[i]->addStudent(students[j]);
                for (int i = 10; i < (int)students.size(); ++i)
                    _groups.back()->addStudent(students[i]);
                if (_groups.back()->getNumStudents() >= 5)
                    ++_cnt_groups;
            }
        }
        sort(_groups.begin(), _groups.end(), [](Group* a, Group* b) {
                                                return a->getNumStudents() > b->getNumStudents();
                                                });
    };

} cms;

class Modeling {
public:

    Modeling() {}

    Modeling(int m) : _resWeek(4 * m) {}

    Modeling(int m, double kick_chance, double add_chance) :
        _resWeek(4 * m), _kick_chance(kick_chance), _add_chance(add_chance)
    {}

    pair<int, int> getDate() {
        return { _weeks / 4, _weeks % 4 };
    }

    bool modelingNextStep() {
        _weeks += 2;

        cms.checkPayments(_kick_chance);

        int cntPeople = Random::randomNumberInRange(
                                                     (cms.getNumPeople() >= 15 ? 0 : 15 - cms.getNumPeople()),
                                                     30 - cms.getNumPeople()
                                                   );
        for (int i = 0; i < cntPeople; ++i)
            if (Random::drnd() > _add_chance) {
                string name = Random::randomListName();
                generateStudent(name);
            }

        cntPeople = Random::randomNumberInRange(
                                                 (cms.getNumPeople() >= 350 ? 0 : 350 - cms.getNumPeople()),
                                                 700 - cms.getNumPeople()
                                               );
        for (int i = 0; i < cntPeople; ++i)
            if (Random::drnd() > _add_chance) {
                string name = Random::randomName();
                generateStudent(name);
            }

        if (_weeks == _resWeek + 2) return 0;

        _average_num_group += cms.averageNumGroup();
        _average_cost_group += cms.averageCostGroup();
        _average_cost_student += cms.averageCostStudent();

        return 1;
    }

    vector<double> result() {
        vector<double> res;
        res.push_back(cms.totalNumStudents());
        res.push_back(cms.totalNumGroups());
        res.push_back(2 * _average_num_group / _resWeek);
        res.push_back(2 * _average_cost_student / _resWeek);
        res.push_back(2 * _average_cost_group / _resWeek);
        return res;
    }

private:
    int _weeks = 0, _resWeek;

    double _kick_chance = 0., _add_chance = 0.;

    double _average_num_group = 0, _average_cost_group = 0, _average_cost_student = 0;

    void generateStudent(string name)
    {
        int lng = Random::randomNumberInRange(1, 4);
        int lvl = Random::randomNumberInRange(1, 3);
        int intensity = Random::randomNumberInRange(1, 5);
        bool isIndividual = Random::drnd() > 0.5;
        string language, level;
        switch (lng) {
        case 1:
            language = "English";
            break;
        case 2:
            language = "French";
            break;
        case 3:
            language = "German";
            break;
        case 4:
            language = "Japanese";
            break;
        }
        switch (lvl) {
        case 1:
            level = "Beginner";
            break;
        case 2:
            level = "Middle";
            break;
        case 3:
            level = "Advanced";
            break;
        }
        cms.addStudent(name, language, level, intensity, isIndividual);
    }

} modeling;
