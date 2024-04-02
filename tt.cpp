#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <unordered_set>
#include <sstream>
#include <set>
#include <unordered_map>
#include <iomanip>
#include <fstream>
using namespace std;

class TTGen
{
public:
    vector<vector<string>> courses;
    vector<vector<string>> course_name;
    vector<vector<string>> credits;
    vector<vector<string>> faculty;
    vector<vector<int>> no_of_hours;
    int total_lectures;
    int odd_even_sem;
    vector<string> classrooms;
    int classroom_index; // Index to keep track of assigned classrooms

    TTGen()
    {
        cout << "ENTER 1 for Autumn Semester and 2 for Winter Semester:" << endl;
        cin >> odd_even_sem;
        cout << "ENTER FOUR CLASSROOMS";
        for(int i=0;i<4;i++)
        {
            cout << "ENTER CLASSROOM "<<(i+1)<<" :";
            string t;
            cin >> t;
            classrooms.push_back(t);
        }
        classroom_index=0;
    }
    void input()
    {
        courses.resize(4);
        course_name.resize(4);
        credits.resize(4);
        faculty.resize(4);
        no_of_hours.resize(4);
        int oes=odd_even_sem;
        for (int i = 0; i < 4; i++)
        {
            cout << "Enter Details for Semester " << oes << endl;
            cout << "Enter No. of Courses for Semester " << oes << endl;
            int num_of_courses;
            cin >> num_of_courses;
            oes+=2;
            courses[i].resize(num_of_courses);
            course_name[i].resize(num_of_courses);
            credits[i].resize(num_of_courses);
            faculty[i].resize(num_of_courses);
            no_of_hours[i].resize(num_of_courses);

            for (int j = 0; j < num_of_courses; j++)
            {
                cout << "Course " << j + 1 << endl;
                cout << "Enter Course No./ Course Id:" << endl;
                string x;
                cin >> x;
                courses[i][j] = x;
                cout << "Enter Course Name:" << endl;
                cin >> x;
                course_name[i][j] = x;
                cout << "Enter Course Credits:" << endl;
                cin >> x;
                int y = x[0] - '0';
                credits[i][j] = x;
                no_of_hours[i][j] = y;
                total_lectures += y;
                cout << "Enter Course Faculty:" << endl;
                cin >> x;
                faculty[i][j] = x;
            }
        }
    }

    void generateTimetable() 
    {
    // Define time slots (rows) from 8am to 1pm
        vector<string> time_slots = {"8am - 9am", "9am - 10am", "10am - 11am", "11am - 12pm", "12pm - 1pm"};

        // Define days (columns) from Monday to Friday
        vector<string> days = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};

        // Initialize the timetable grid
        vector<vector<vector<tuple<string, string, string>>>> timetable(time_slots.size() * 4, vector<vector<tuple<string, string, string>>>(days.size()));

        // Initialize counters to keep track of lectures assigned for each course
        vector<vector<int>> lectures_assigned(courses.size(), vector<int>(courses[0].size(), 0));

        // Map to keep track of assigned classrooms for each course
        map<string, string> assigned_classrooms;

        // Loop through each time slot (row)
        for (int i = 0; i < time_slots.size(); ++i) 
        {
            // Loop through each semester (subrow)
            for (int semester = 0; semester < 4; ++semester) 
            {
                string cr = classrooms[semester];
                // Loop through each day (column)
                for (int j = 0; j < days.size(); ++j) 
                {
                    // Assign courses to this time slot, semester, and day
                    bool course_assigned = false;
                    for (int year = 0; year < courses.size(); ++year) 
                    {
                        for (int course_index = 0; course_index < courses[year].size(); ++course_index) 
                        {
                            // Check if this course belongs to the current semester
                            if (semester == year) 
                            {
                                // Check if there are remaining lectures for this course
                                if (lectures_assigned[year][course_index] < no_of_hours[year][course_index]) 
                                {
                                    string course_id = courses[year][course_index];
                                    // Assign the course to this time slot, semester, and day
                                    timetable[4 * i + semester][j].push_back({course_id,cr, faculty[year][course_index]});
                                    lectures_assigned[year][course_index]++;
                                    course_assigned = true;
                                    break; // Move to the next semester
                                }
                            }
                        }
                        if (course_assigned) 
                        {
                            break; // Move to the next day
                        }
                    }

                    // If no course is assigned, mark this slot as "Free" for this semester
                    if (!course_assigned) 
                    {
                        timetable[i * 4 + semester][j].push_back({"Free", "", ""});
                    }
                }
            }
        }
        for (int semester = 0; semester < 4; ++semester) 
        {
            for (int j = 0; j < days.size(); ++j) 
            {
                // Iterate over each time slot except the last one
                for (int i = 0; i < time_slots.size() - 1; ++i) 
                {
                    // Check if the current slot and the next slot both have classes
                    if (timetable[i * 4 + semester][j].size() == 1 && get<0>(timetable[i * 4 + semester][j][0]) == "Free") 
                    {
                        // If the current slot is a free slot, move to the next slot
                        continue;
                    }

                    if (timetable[(i + 1) * 4 + semester][j].size() == 1 && get<0>(timetable[(i + 1) * 4 + semester][j][0]) == "Free") 
                    {
                        // If the next slot is a free slot, swap the slots
                        swap(timetable[i * 4 + semester][j], timetable[(i + 1) * 4 + semester][j]);
                    }
                }
            }
        }


        // Write the output to a text file
        ofstream outfile("timetable.txt",ios::app);
        if (outfile.is_open()) 
        {
            // Write the time slots header with 15 spaces for alignment
            outfile << setw(10) << "Time Slot\t";
            for (const string &day : days) 
            {
                outfile << setw(15) << day << "\t\t";
            }
            outfile << endl;

            // Write the timetable data
            for (int i = 0; i < timetable.size(); ++i) 
            {
                if((i%4)+1==1)
                    outfile << setw(10) << time_slots[i / 4];
                    if(odd_even_sem==1)
                        outfile<<"\n(Semester " << to_string(2*(i % 4) + 1) << ")\t"; // Time slot with semester info
                    else
                        outfile<<"\n(Semester " << to_string(2*((i % 4)+1)) << ")\t"; // Time slot with semester info
                for (int j = 0; j < timetable[i].size(); ++j) 
                {
                    
                    if (!timetable[i][j].empty()) 
                    {
                        
                        for (const auto &course_classroom_faculty : timetable[i][j]) 
                        {
                            if(get<0>(course_classroom_faculty)=="Free")
                            {
                                outfile<<setw(10)<<"\tFREE\t";
                            }
                            else
                            {
                                outfile << setw(10); // Set width for alignment
                                outfile << get<0>(course_classroom_faculty) << "[" << get<1>(course_classroom_faculty) << "](" << get<2>(course_classroom_faculty) << ")"; // Course with classroom and faculty
                            }
                        }
                    } 
                    outfile << "\t";
                }
                outfile <<endl;
                if((i%4)+1==4) 
                {
                    outfile<<endl;
                    for(int i =0;i<138;i++)
                    {
                        outfile<<"-";
                    }
                    outfile<<endl;
                }
            }
            outfile.close();
            cout << "Timetable has been written to timetable.txt" << endl;
        } 
        else 
        {
            cout << "Error: Unable to write to the file." << endl;
        }
    }

};

int main()
{
    remove("timetable.txt");
    cout << "ENTER INPUT FOR ICT:"<<endl;
    TTGen t1;
    t1.input();
    ofstream outfile("timetable.txt",ios::app);
    outfile << "TIME TABLE FOR BTECH ICT:"<<endl<<endl;
    t1.generateTimetable();
    for(int i=0;i<138;i++)
    {
        outfile<<"_";
    }
    outfile << endl<<endl;
}
