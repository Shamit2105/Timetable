#include <iostream>
#include <vector>

using namespace std;

int main()
{
    cout << "Enter no. of courses to be offered:"<<endl;
    int course_total;
    cin >> course_total;
    cout << "Enter no. of instructors for the course:"<<endl;
    int faculty_total;
    cin >> faculty_total;
    vector<string> courses;
    int total_lectures=0;
    int total_tutorials=0;
    int total_labs=0;
    int credit=0;
    for(int i=0;i<course_total;i++)
    {
        int l,t,p,c;
        cout << "Enter the course code"<<endl;
        cin >> courses[i];
        cout << "Enter the L T P C seperated by spaces"<<endl;
        cin >> l>>t>>p>>c;
        total_lectures+=l;
        total_tutorials+=t;
        total_labs+=p;
        credit+=c;
    }
    cout << "Enter Total Number Of Classes Available"<<endl;
    int classrooms;
    cin >> classrooms;
    int class_no[10];
    for(int i=0;i<10;i++)
    {
        cin >> class_no[i];
    }
    
}