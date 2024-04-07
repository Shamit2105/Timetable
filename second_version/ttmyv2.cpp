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
    /*
    We initialize the timetable grid,outermost is the timeslot for each semester(timeslot*semester),middle one is the days of week and
    third one is vector of tuple of three strings, three strings in tuple are course_id, classroom and faculty
    */
    vector<vector<vector<tuple<string, string, string>>>> timetable;
    TTGen()
    {
        cout << "ENTER 1 for Autumn Semester and 2 for Winter Semester:" << endl;
        cin >> odd_even_sem;
        cout << "ENTER CLASSROOMS";
        for(int i=0;i<22;i++)
        {
            cout << "ENTER CLASSROOM "<<(i+1)<<" :";
            string t;
            cin >> t;
            classrooms.push_back(t);
        }
        timetable.resize(time_slots.size() * 22, vector<vector<tuple<string, string, string>>>(days.size()));//Resizing the timetable vector
    }
    void input()
    {
        courses.resize(22);
        course_name.resize(22);
        credits.resize(22);
        faculty.resize(22);
        no_of_hours.resize(22);
        for (int i = 0; i < 22; i++)
        {
            if(i==12||i==13||i==16||i==17||i==18)
            {
                cout << "Enter Details for Masters Semester 1:" <<  endl;
                cout << "Enter No. of Courses for Masters Semester 1:"   << endl;
            }
            else if(i==14||i==15||i==19||i==20||i==21)
            {
                cout << "Enter Details for Masters Semester 3:" <<  endl;
                cout << "Enter No. of Courses for Masters Semester 3:"   << endl;
            }
            else
            {
                cout << "Enter Details for B Tech Semester " << (i / 22 * 4 + (i % 22) / 3) % 4*2 + 1<< endl;
                cout << "Enter No. of Courses for B Tech Semester " << (i / 22 * 4 + (i % 22) / 3) % 4*2 + 1  << endl;
            }

            int num_of_courses;
            
            cin >> num_of_courses;
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
        //2D vector to keep track of lecture assigned
        vector<vector<int>> lectures_assigned(courses.size(), vector<int>(courses[0].size(), 0));
        // Define a map to store the faculty assigned to each time slot and day
        map<pair<int, int>, unordered_set<string>> faculty_assigned;
        //Map to keep track of last lectures of faculty so that if need we can provide them a break
        map<pair<int, int>, string> last_faculty;
        for (int i = 0; i < time_slots.size(); ++i)
        {
            //At every new time slot, faculty will always be available
            bool faculty_available = true;
            // Loop through each semester (subrow)
            for (int semester = 0; semester < 22; ++semester)
            {
                //For our program, no. of classrooms are equal to no. of semesters so each classroom is assigned to semester
                string cr = classrooms[semester]; // Classroom for the semester
                // Loop through each day (column)
                for (int j = 0; j < days.size(); ++j)
                {
                    // Assign courses to this time slot, semester, and day
                    bool course_assigned = false;//Currently no courses have been assigned
                    
                    for (int sem_courses = 0; sem_courses < courses.size(); ++sem_courses)
                    {
                        for (int course_index = 0; course_index < courses[sem_courses].size(); ++course_index)
                        {
                            // Check if this course belongs to the current semester
                            if (semester == sem_courses)
                            {
                                // Check if there are remaining lectures for this course
                                if (lectures_assigned[sem_courses][course_index] < no_of_hours[sem_courses][course_index])
                                {
                                    // Get the course ID and the faculty assigned to teach this course so that we can add it to our timetable vector
                                    string course_id = courses[sem_courses][course_index];
                                    string faculty_assigned_to_course = faculty[sem_courses][course_index];
                                    
                                    // Check if the faculty is already assigned to teach another course during this time slot and day
                                    bool faculty_available = true;
                                    
                                        //Check for faculty clash
                                        if (faculty_assigned[{i, j}].count(faculty_assigned_to_course) > 0)
                                        {
                                            faculty_available = false;
                                            break;
                                        }
                                        // Check if the faculty has taught in previous time slot and if he/she wants a break
                                        for (int prev_slot = 0; prev_slot < i; ++prev_slot) 
                                        {
                                            if (last_faculty.count({prev_slot, j}) && last_faculty[{prev_slot, j}].back() == '!') 
                                            {
                                                // If a break is found in any previous time slot on the same day, faculty is not available
                                                faculty_available = false;
                                                break; // No need to check further
                                            }
                                        }                                    
                                    if (faculty_available)//Assigning courses only if there are no faculty clashes and the faculty does not need a break
                                    {
                                        // Assign the course to this time slot, semester, and day
                                        timetable[22 * i + semester][j].push_back({course_id, cr, faculty_assigned_to_course});
                    // 22*i refers to the base index of time slot andand then adding semester allows us to differentiate between different semesters within that time slot.
                                        lectures_assigned[sem_courses][course_index]++;
                                        faculty_assigned[{i, j}].insert(faculty_assigned_to_course);
                                        course_assigned = true;
                                        last_faculty[{i,j}]=(faculty_assigned_to_course);
                                        break; // Move to the next semester
                                    }
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
                        timetable[i * 22+ semester][j].push_back({"Free", "", ""});
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
                if((i%22)+1==1)
                    outfile << setw(10) << time_slots[i / 22]<<":";
                setw(10);
                if(odd_even_sem==1)
                {
                    int x=i;
                    if(i%22==0)
                    {
                        outfile << "\nBTech:"<<endl;
                    }
                    else if(i%22==12)
                    {
                        outfile<<"\nMTech:"<<endl;
                    }
                    else if (i%22==16)
                        outfile<<"\nMSc:"<<endl;
                    if(((i%22))%3==0&&((i%22)+1!=22)&&((i%22)+1!=13)&&i%22<12)
                    {  
                        
                        int semester_number = (i / 22 * 4 + (i % 22) / 3) % 4*2 + 1; // Calculating odd semester number
                        outfile << "\n(Semester " << to_string(semester_number) << ")\n"; // Odd semester info
                    }
                    else if((i%22)+1==15||(i%22)+1==20)
                    {
                        outfile <<"\n(Semester 3)\n";
                    }
                    else if((i%22)+1==13||(i%22)+1==17)
                    {
                        outfile <<"\n(Semester 1)\n";
                    }
                    
                }
                else
                {
                    int x=i;
                    if(i%22==0)
                    {
                        outfile << "\nBTech:"<<endl;
                    }
                    else if(i%22==12)
                    {
                        outfile<<"\nMTech:"<<endl;
                    }
                    else if (i%22==16)
                        outfile<<"\nMSc:"<<endl;
                    if(((i%22))%3==0&&((i%22)+1!=22)&&((i%22)+1!=13)&&i%22<12)
                    {  
                        
                        int semester_number = (i / 22 * 4 + (i % 22) / 3) % 4*2 + 2; // Calculating even semester number
                        outfile << "\n(Semester " << to_string(semester_number) << ")\n"; // even semester info
                    }
                    else if((i%22)+1==15||(i%22)+1==20)
                    {
                        outfile <<"\n(Semester 4)\n";
                    }
                    else if((i%22)+1==13||(i%22)+1==17)
                    {
                        outfile <<"\n(Semester 2)\n";
                    }
                }    
                    if(i%22<6&&(i%22==0||(i%22)%3==0))
                    {
                        outfile << "ICT   \t\t";
                    }
                    else if(i%22<6&&(i%22==0||(i%22)%3==1))
                    {
                        outfile << "ICT+CS\t\t";
                    }
                    else if(i%22<6&&(i%22==0||(i%22)%3==2))
                    {
                        outfile << "MnC   \t\t";
                    }
                    if(i%22>=6&&i%22<12&&(i%22==0||(i%22)%3==0))
                    {
                        outfile << "ICT+CS\t\t";
                    }
                    else if(i%22>=6&&i%22<12&&(i%22==0||(i%22)%3==1))
                    {
                        outfile << "ICT+CS\t\t";
                    }
                    else if(i%22>=6&&i%22<12&&(i%22==0||(i%22)%3==2))
                    {
                        outfile << "MnC   \t\t";
                    }
                    else if((i%22<16&&i%22>=12)&&((i%22)%2==0))
                    {
                        outfile << "ICT   \t\t";
                    }
                    else if((i%22<16&&i%22>=12)&&((i%22)%2==1))
                        outfile << "EC    \t\t";
                    else if((i%22>=16)&&((i%22)%3==1))
                    {
                        outfile << "IT    \t\t";
                    }
                    else if((i%22>=16)&&((i%22)%3==2))
                    {
                        outfile << "DS    \t\t";
                    }
                    else if((i%22>=16)&&((i%22)%3==0))
                    {
                        outfile << "AA    \t\t";
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
                                else if((i%22)+1>=13)
                                    outfile <<setw(7)<<"  " <<get<0>(course_classroom_faculty) << "[" << get<1>(course_classroom_faculty) << "](" << get<2>(course_classroom_faculty) << ") \t";
                                else
                                {    
                                if(get<1>(course_classroom_faculty)[0]=='L')
                                    outfile <<setw(7)<<"  " <<get<0>(course_classroom_faculty) << "[" << get<1>(course_classroom_faculty) << "](" << get<2>(course_classroom_faculty) << ") \t"; // Course with classroom and faculty
                                else
                                {
                                    outfile<<setw(7) <<"" <<get<0>(course_classroom_faculty) << "[" << get<1>(course_classroom_faculty) << "](" << get<2>(course_classroom_faculty) << ")\t";
                                }
                                }
                            }
                        }
                        
                    } 
                    
                }
                
                outfile <<endl;
                if((i%22)+1==22) 
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
                for (int semester = 0; semester < 14; ++semester) {
                    for (const auto& slot : timetable[14 * i + semester][j]) 
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
            cout << "Faculty Timetable has been written to fac_tt.txt" << endl;
    }

    void displayClassroomTimetable(const string& classroom_name) 
    {
        ofstream outfile("classroom_tt.txt", ios::app); // Opening file in append mode
        outfile << "Timetable for Classroom: " << classroom_name << endl;

        for (int j = 0; j < days.size(); ++j) 
        {
            outfile << "Day: " << days[j] << endl;
            for (int i = 0; i < time_slots.size(); ++i) 
            {
                bool found = false;
                for (int semester = 0; semester < 14; ++semester) 
                {
                    for (const auto& slot : timetable[14 * i + semester][j]) 
                    {
                        if (get<1>(slot) == classroom_name) 
                        {
                            outfile << "\tTime: " << time_slots[i] << endl;
                            outfile << "\tCourse: " << get<0>(slot) << "\n\tFaculty: " << get<2>(slot) << endl;
                            found = true;
                            outfile<<endl;
                        }
                    }
                }
            }
            outfile << endl;
        }
        cout << "Classroom Timetable has been written to classroom_tt.txt" << endl;
    }
};

int main()
{
    remove("timetable.txt");//first we remove any timetable.txt file in our 
    remove("fac_tt.txt");
    remove("classroom_tt.txt");
    TTGen t1;
    t1.input();
    ofstream outfile("timetable.txt",ios::app);// we create a new timetable.txt file in our directory everytime we open our code
    for(int i=0;i<180;i++)//we print 180 _ so that the reader of our text file can easily notice the partition between two timetables
    {
        outfile<<"-";
    }
    outfile << endl;
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
    cout<< "ENTER CLASSROOM WHOSE TIMETABLE IS TO BE OUTPUTTED:"<<endl;
    string classroom_in;
    cin >> classroom_in;
    t1.displayClassroomTimetable(classroom_in);
}
