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
    vector<vector<string>> courses;//Course no. eg PC110
    vector<vector<string>> course_name;
    vector<vector<string>> credits;
    vector<vector<string>> faculty;
    vector<vector<int>> no_of_hours;//To be extracted from credits by converting the first character of each credit into string
    int total_lectures;
    int odd_even_sem;//Ask user if timetable is for even sem or odd sem
    vector<string> classrooms;
    // Define time slots (rows) from 8am to 1pm
    vector<string> time_slots = {"8am - 9am", "9am - 10am", "10am - 11am", "11am - 12pm", "12pm - 1pm"};

    // Days (columns) from Monday to Friday
    vector<string> days = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
    // We initialize the timetable grid,three strings in tuple are course_id, classroom and faculty
    vector<vector<vector<tuple<string, string, string>>>> timetable;
    TTGen()
    {
        cout << "ENTER 1 for Autumn Semester and 2 for Winter Semester:" << endl;
        cin >> odd_even_sem;
        cout << "ENTER CLASSROOMS";
        for(int i=0;i<12;i++)
        {
            cout << "ENTER CLASSROOM "<<(i+1)<<" :";
            string t;
            cin >> t;
            classrooms.push_back(t);
        }
        timetable.resize(time_slots.size() * 12, vector<vector<tuple<string, string, string>>>(days.size()));//Resizing the timetable vector
    }
    void input()
    {
        courses.resize(12);
        course_name.resize(12);
        credits.resize(12);
        faculty.resize(12);
        no_of_hours.resize(12);
        int oes=odd_even_sem;//To print the semester in cmd output
        for (int i = 0; i < 12; i++)
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
    
        vector<vector<int>> lectures_assigned(courses.size(), vector<int>(courses[0].size(), 0));
        // Define a map to store the faculty assigned to each time slot and day
        map<pair<int, int>, unordered_set<string>> faculty_assigned;

        for (int i = 0; i < time_slots.size(); ++i)
        {
            // Loop through each semester (subrow)
            for (int semester = 0; semester < 12; ++semester)
            {
                string cr = classrooms[semester]; // Classroom for the semester
                // Loop through each day (column)
                for (int j = 0; j < days.size(); ++j)
                {
                    // Assign courses to this time slot, semester, and day
                    bool course_assigned = false;
                    for (int all_courses = 0; all_courses < courses.size(); ++all_courses)
                    {
                        for (int course_index = 0; course_index < courses[all_courses].size(); ++course_index)
                        {
                            // Check if this course belongs to the current semester
                            if (semester == all_courses)
                            {
                                // Check if there are remaining lectures for this course
                                if (lectures_assigned[all_courses][course_index] < no_of_hours[all_courses][course_index])
                                {
                                    string course_id = courses[all_courses][course_index];
                                    string faculty_assigned_to_course = faculty[all_courses][course_index];

                                    // Assign the course to this time slot, semester, and day
                                    timetable[12* i + semester][j].push_back({course_id, cr, faculty_assigned_to_course});
                                    lectures_assigned[all_courses][course_index]++;
                                    faculty_assigned[{i, j}].insert(faculty_assigned_to_course);
                                    course_assigned = true;
                                    break; // Move to the next semester
                                }
                            }
                        }
                        if (course_assigned)
                        {
                            break; // Move to the next day, so every day maximum there is only one class of a particular course
                        }
                    }

                    // If no course is assigned, mark this slot as "Free" for this particular time_slot and day
                    if (!course_assigned)
                    {
                        timetable[i * 12+ semester][j].push_back({"Free", "", ""});
                    }
                }
            }
        }
        // Write the output to a text file
        ofstream outfile("timetable.txt",ios::app);//opening file in append mode
        if (outfile.is_open()) 
        {
            // Write the time slots header with 15 spaces for alignment
            outfile << setw(10) << "Time Slot\t";
            for (const string &day : days) 
            {
                outfile << setw(20) << day << "\t\t";
            }
            outfile << endl;
            for(int i=0;i<180;i++)
            {
                outfile<<("_");
            }
            outfile << endl;

            // Write the timetable data
            for (int i = 0; i < timetable.size(); ++i) 
            {
                if((i%12)+1==1)
                    outfile << setw(10) << time_slots[i / 12]<<":";
                setw(10);
                if(odd_even_sem==1)
                {
                    int x=i;
                    if(((i%12))%3==0)
                    {  
                        
                        int semester_number = (i / 12 * 4 + (i % 12) / 3) % 4*2 + 1; // Calculating odd semester number
                        outfile << "\n(Semester " << to_string(semester_number) << ")\t"; // Odd semester info
                    }
                    else
                        outfile <<"\n\t\t";
                    
                }
                else
                {
                    if(((i%12))%2==0)
                        outfile<<"\n(Semester " << to_string((i/12)+2) << ")\t"; // Time slot with even semester info
                    else
                        outfile <<"\n\t\t";
                }   
                for (int j = 0; j < timetable[i].size(); ++j) 
                {
                    
                    if (!timetable[i][j].empty()) 
                    {
                        
                        for (const auto &course_classroom_faculty : timetable[i][j]) 
                        {
                            if(get<0>(course_classroom_faculty)=="Free")
                            {
                                outfile<<setw(9)<<"\t\tFREE\t\t";
                            }
                            else
                            {
                                if(get<0>(course_classroom_faculty)[0]=='M'&&get<0>(course_classroom_faculty)[1]=='C')
                                    outfile <<setw(7)<<"  " <<get<0>(course_classroom_faculty) << "[" << get<1>(course_classroom_faculty) << "](" << get<2>(course_classroom_faculty) << ") \t";
                                else
                                {    
                                if(get<1>(course_classroom_faculty)[0]=='L')
                                    outfile <<setw(7)<<"  " <<get<0>(course_classroom_faculty) << "[" << get<1>(course_classroom_faculty) << "](" << get<2>(course_classroom_faculty) << ") \t"; // Course with classroom and faculty
                                else
                                {
                                    outfile<<setw(6) <<"" <<get<0>(course_classroom_faculty) << "[" << get<1>(course_classroom_faculty) << "](" << get<2>(course_classroom_faculty) << ")\t";
                                }
                                }
                            }
                        }
                        
                    } 
                    
                }
                
                outfile <<endl;
                if((i%12)+1==12) 
                {
                    outfile<<endl;
                    for(int i =0;i<180;i++)//after every time slot, we print a dotted line so that the the timetable.txt file looks organized
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

    void displayFacultySchedule(const string& faculty_name) 
    {
        ofstream outfile("fac_tt.txt", ios::app); // Opening file in append mode
        outfile << "Timetable for Faculty: " << faculty_name << endl;

        for (int j = 0; j < days.size(); ++j) 
        {
            outfile << "Day: " << days[j] << endl;
            for (int i = 0; i < time_slots.size(); ++i) 
            {
                
                bool found = false;
                for (int semester = 0; semester < 12; ++semester) {
                    for (const auto& slot : timetable[12 * i + semester][j]) 
                    {
                        if (get<2>(slot) == faculty_name) 
                        {
                            outfile << "\tTime: " << time_slots[i] << endl;
                            outfile << "\tClassroom: " << get<1>(slot) << "\n\tCourse: " << get<0>(slot) << endl;
                            found = true;
                            outfile<<endl;
                        }
                    }
                }
                
            }
            outfile << endl;
        }
    }
};

int main()
{
    remove("timetable.txt");//first we remove any timetable.txt file in our 
    remove("fac_tt.txt");
    TTGen t1;
    t1.input();
    ofstream outfile("timetable.txt",ios::app);// we create a new timetable.txt file in our directory everytime we open our code
    for(int i=0;i<180;i++)//we print 180 _ so that the reader of our text file can easily notice the partition between two timetables
    {
        outfile<<"-";
    }
    t1.generateTimetable();
    for(int i=0;i<180;i++)
    {
        outfile<<"_";
    }
    outfile << endl<<endl;
    cout << "ENTER FACULTY WHOSE TIMETABLE IS TO BE OUTPUTTED:"<<endl;
    string fac_in;
    cin >> fac_in;
    t1.displayFacultySchedule(fac_in);
}