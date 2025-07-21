About This Program:
The program generates academic timetables for a college. It prompts the user to input details such as courses, course names, credits, faculty, and classroom availability for multiple semesters. Using this input the program
generates a timetable grid, allocating courses to specific time slots, days, classrooms, and faculty members while ensuring that no faculty member is double-booked and courses are evenly distributed throughout the schedule. 
After generating the timetable, it outputs the schedule to a text file named "timetable.txt", organized by time slots, days, and semesters. Additionally, it allows users to inquire about individual faculty members' schedules 
and classroom availability by generating separate text files ("fac_tt.txt" and "classroom_tt.txt", respectively).

The first version of our program is a very basic version consisting only of B Tech ICT and B Tech MnC with very few constraints checked. The second version is the final version of our program which prints timetable of B Tech,
M Tech as well as M Sc and checks many constraints as mentioned in the document file. That is the final version of our program.

Contribution:
All memebers contributed in thinking about the main algorithm of the code present in void generateTimetable()

1)Bhavya Bhanvadiya (202301123) : Input files for even semester and provided code for void displayFacultySchedule() that creates a text file having output of timetable for a faculty for entire week. Also helped in figuring the logic to avoid clashes by use of data structures(set and map).

2)Devansh Shah (202301136) : Input files for odd semester and provied code for void displayClassroomTimetable() that creates a text file having output of timetable for a classroom for entire week.

3)Shamit Gandhi (202301041) : Handled void input(), handled faculty clashes in void generateTimetable(), handled how to output the main file in void generateTimetable(), wrote pseudocode, Time and Space Complexities with 
Tirth Boghani, explained main algorithm, How We Reached Here in the document, and improved formatting of output in both void displayFacultySchedule() and void displayClassroomTimetable().

4)Tirth Boghani (202301025) Handled faculty breaks in void generateTimetable(), wrote Pseudocode,Time and Space Complexities with Shamit Gandhi, and Why Choose This Data Structure?, Constraints Checked in the document file.
