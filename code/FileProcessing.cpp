#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <map>
using namespace std;

struct person{
    string surname;
    string name;
    int age;
    string phoneNumber;
    friend ostream& operator<<(ostream& s, const person &obj){
        s << obj.surname << " " << obj.name << " " << obj.age << " " << obj.phoneNumber << "\n";
        return s;
    }
};

bool compAge(const person &l, const person &r){
    return l.age < r.age;
}

bool compPhoneNumber(const person &l, const person &r){
    return l.phoneNumber < r.phoneNumber;
}

void readFromFile1(const string name, vector<person> &data){
    string s;
    ifstream in(name);
    int cntLines = 0;
    while(getline(in, s)){
        ++cntLines;
    }
    in.close();

    data.reserve(cntLines);
    in.open("input.txt");
    for(int i = 0; i < cntLines; ++i){
        in >> data[i].surname >> data[i].name
           >> data[i].age     >> data[i].phoneNumber;
    }
    in.close();
}

void readFromFile2(const string name, vector<person> &data){
    person tmp;
    ifstream in("input.txt");

    while(in >> tmp.surname){
        in >> tmp.name >> tmp.age >> tmp.phoneNumber;
        data.push_back(tmp);
    }
    in.close();
}

int main(){

    vector<person> data;
    // readFromFile1("input.txt", data);
    readFromFile2("input.txt", data);

    cout << "-----Sorted by age-----\n";
    sort(data.begin(), data.end(), compAge);
    for(int i = 0; i < data.size(); ++i){
        cout << data[i];
    }

    cout << "\n-----Average Age + 10-----\n";
    long double avgAge = 0.0;
    for(int i = 0; i < data.size(); ++i){
        avgAge += data[i].age;
    }
    avgAge /= data.size();
    for(int i = 0; i < data.size(); ++i){
        if( data[i].age >= avgAge + 10.0 ){
            cout << data[i];
        }
    }

    cout << "\n-----Same phone number-----\n";
    sort(data.begin(), data.end(), compPhoneNumber);
    map<string, int> cnt;
    for(int i = 0; i < data.size(); ++i){
        cnt[data[i].phoneNumber]++;
    }
    for(auto it = cnt.begin(); it != cnt.end(); ++it){
        if( it->second > 1 ){
            cout << "with phone number: " << it->first << "\n";
            for(int i = 0; i < data.size(); ++i){
                if( data[i].phoneNumber == it->first ){
                    cout << data[i].surname << " " << data[i].name << "\n";
                }
            }
        }
    }

    return 0;
}