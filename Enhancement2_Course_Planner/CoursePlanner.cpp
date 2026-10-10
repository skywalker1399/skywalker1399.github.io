
// CoursePlanner Enhanced

#include <iostream>
#include <fstream>
#include <string>
#include <climits>
#include <vector>

using namespace std;

//Defult number position
const unsigned int DEFAULT_SIZE = 179;


//define structure to hold course information
struct Course {
    string courseId; 
    string courseName;
    vector<string> preReq;
};


//Hash table store course information 
//Completed courses, prerequisite based generated course plan
class HashTable {

private:
    struct Node {

        Course course;
        unsigned int key;
        Node* next;

        //Defult constructor
        Node() {
            key = UINT_MAX;
            next = nullptr;
        }

        //initalize with a course
        Node(Course acourse) : Node() {
            course = acourse;
        }

        //initalize with a course and a key

        Node(Course acourse, unsigned int akey) : Node(acourse) {
            key = akey;
        }

    };

    vector<Node> nodes;

    //saves courseID of user completed courses
    vector<string> completedCourses;

    unsigned int tableSize = DEFAULT_SIZE;

    unsigned int hash(string courseId);

public:
    HashTable();
    HashTable(unsigned int size);
    void PrintAll();
    void Insert(Course acourse);
    Course Search(string courseId);
    //used for listing completed courses
    void AddCompletedCourse(string courseeId);
    void RemoveCompletedCourse(string courseId);
    void PrintCompletedCourses();
    bool IsCompleted(string courseId);
    bool HasPrerequisites(string courseId);
    void PrintMissingPrerequisites(string courseId);
    void PrintEligibleCourses();
    void GenerateCoursePlan();
    vector<Course> GetAllCourses();
    int CountRemainingPrerequisites(string courseId, vector<Course>& plan);
    vector<Course> GetAvailableCourses(vector<Course>& plan);
    bool IsCompletedOrPlanned(string courseId, vector<Course>& plan);
    void PrintCoursePlanEntry(Course course);
    
};

//Create hash table
HashTable::HashTable() {

    // Initalize node structure by resizing tableSize
    nodes.resize(tableSize);
}


//constructor for specifying size of table
HashTable::HashTable(unsigned int size) {

    this->tableSize = size;

    //Create hash table positions
    nodes.resize(size);
}

//Calculates hash value from courseId
unsigned int HashTable::hash(string courseId) {


    unsigned int hashValue = 0;

    //Build hash value using each character in courseId
    //multiplying by 31 helps distribute different courseIds
    //across the avalible hash table positions
    for (char character : courseId) {
        hashValue = (hashValue * 31) + character;
    }
    return hashValue % tableSize;
}

//Add course to hash table
void HashTable::Insert(Course acourse) {

    //make a key for the course
    unsigned key = hash(acourse.courseId);

    //find node given key
    Node* oldNode = &(nodes.at(key));
    //if no key found
    if (oldNode == nullptr) {
        //make node equal at key
        Node* newNode = new Node(acourse, key);
        nodes.insert(nodes.begin() + key, (*newNode));
    }

    else {
        // assing old node key to UNIT_MAX, set to key, set old node to bid and old node next to null pointer
        if (oldNode->key == UINT_MAX) {
            oldNode->key = key;
            oldNode->course = acourse;
            oldNode->next = nullptr;
        }
        // else find the next open node
        else {

            while (oldNode->next != nullptr) {
                oldNode = oldNode->next;
            }
            // add new newNode to end
            oldNode->next = new Node(acourse, key);
        }
    }
}

//adds courses to completed hash table
void HashTable::AddCompletedCourse(string courseId) {
    completedCourses.push_back(courseId);
}

//Displays completed courses
void HashTable::PrintCompletedCourses() {

    cout << endl;
    cout << "Completed Courses:" << endl;
    
    //loop for each course in CompletedCourses
    for (string courseId : completedCourses) {
        std::cout << courseId << endl;
    }
}

void HashTable::RemoveCompletedCourse(string courseId) {

    for (auto it = completedCourses.begin(); it != completedCourses.end(); ++it) {

        if (*it == courseId) {
            completedCourses.erase(it);
            return;
        }
    }
}

//prevents double completed courses
bool HashTable::IsCompleted(string courseId) {

    for (string completedCourse : completedCourses) {
        if (completedCourse == courseId) {
            return true;
        }
    }
    return false;
}

//Check if prerequisites are completed
bool HashTable::HasPrerequisites(string courseId) {
    
    Course course = Search(courseId);

    if (course.courseId.empty()) {
        return false;
    }

    for (string prerequisite : course.preReq) {
        if (!IsCompleted(prerequisite)) {
            return false;
        }
    }
    return true;
}

void HashTable::PrintMissingPrerequisites(string courseId) {
    Course course = Search(courseId);

    if (course.courseId.empty()) {
        return;
    }

    bool missingPrerequisite = false;

    for (string prerequisite : course.preReq) {

        if (!IsCompleted(prerequisite)) {
            if (!missingPrerequisite) {
                cout << "Missing prerequisites:" << endl;
                missingPrerequisite = true;
            }

            cout << prerequisite << endl;
        }
    }

    if (!missingPrerequisite) {
        cout << "All prerequisites have been completed." << endl;
    }
}

//lists courses avalible based on completed courses
void HashTable::PrintEligibleCourses() {

    cout << endl;
    cout << "Courses you are eligible to take:" << endl;

    //tracks number of eligible courses
    int eligibleCount = 0;

    // for node begin to end iterate
    for (auto it = nodes.begin(); it != nodes.end(); ++it) {
        //   if key not equal to UINT_MAX
        if (it->key != UINT_MAX) {

            if (!IsCompleted(it->course.courseId) && HasPrerequisites(it->course.courseId)) {

                cout << it->course.courseId << ", " << it->course.courseName << endl;

                eligibleCount++;
            }

            Node* node = it->next;

            while (node != nullptr) {

                if (!IsCompleted(node->course.courseId) && HasPrerequisites(node->course.courseId)) {

                    cout << node->course.courseId << ", " << node->course.courseName << endl;

                    eligibleCount++;
                }

                node = node->next;
            }
        }
    }

    if (eligibleCount == 0) {
        cout << "There are currently no courses avalible based on your completed courses." << endl;
    }
}


//Generate course plan by selecting courses whose prerequisites have already been completed or added to the plan.
//Creates a prerequisite respected order similar to topological sorting.
void HashTable::GenerateCoursePlan() {

    cout << endl;
    cout << "Generated Course Plan:" << endl;

    vector<Course> plan;
    vector<Course> allCourses = GetAllCourses();

    while (true) {

        vector<Course> availableCourses = GetAvailableCourses(plan);

        if (availableCourses.empty()) {
            break;
        }

        Course nextCourse = availableCourses.at(0);

        plan.push_back(nextCourse);

        cout << plan.size() << ", ";
        PrintCoursePlanEntry(nextCourse);
    }

    int remainingCourses = 0;

    for (Course course : allCourses) {
        if (!IsCompleted(course.courseId)) {
            remainingCourses++;
        }
    }

    if (plan.size() < remainingCourses) {
        cout << endl;
        cout << "Unable to generate a complete course plan." << endl;
        cout << "Some courses have unresolved prerequisites." << endl;
    }
    else {
        cout << endl;
        cout << "Complete course plan generated successfully." << endl;
    }
}

//used for making course plan table
vector<Course> HashTable::GetAllCourses() {

    vector<Course> allCourses;

    for (auto it = nodes.begin(); it != nodes.end(); it++) {

        if (it->key != UINT_MAX) {
            allCourses.push_back(it->course);

            Node* node = it->next;

            while (node != nullptr) {
                allCourses.push_back(node->course);
                node = node->next;
            }
        }
    }

    return allCourses;
}

//counts unfinished prerequisites
int HashTable::CountRemainingPrerequisites(
    string courseId,
    vector<Course>& plan) {

    Course course = Search(courseId);

    int count = 0;

    for (string prerequisite : course.preReq) {

        if (!IsCompletedOrPlanned(prerequisite, plan)) {
            count++;
        }
    }

    return count;
}

vector<Course> HashTable::GetAvailableCourses(vector<Course>& plan) {
    
    vector<Course> avaliableCourses;
    vector<Course> allCourses = GetAllCourses();

    for (Course course : allCourses) {
        if (!IsCompletedOrPlanned(course.courseId, plan) && CountRemainingPrerequisites(course.courseId, plan) == 0) {

            avaliableCourses.push_back(course);
        }
    }

    return avaliableCourses;
}

//checks if course is completed or in planner
bool HashTable::IsCompletedOrPlanned(string courseId, vector<Course>& plan) {

    if (IsCompleted(courseId)) {
        return true;
    }

    for (Course course : plan) {
        
        if (course.courseId == courseId) {
            return true;
        }
    }

    return false;
}

void HashTable::PrintCoursePlanEntry(Course course) {

    cout << course.courseId << ", "
        << course.courseName << endl;

    cout << "   Prerequisites: ";

    if (course.preReq.empty()) {
        cout << "None";
    }
    else {
        for (unsigned int i = 0; i < course.preReq.size(); i++) {

            cout << course.preReq.at(i);

            if (i < course.preReq.size() - 1) {
                cout << ", ";
            }
        }
    }

    cout << endl;
}

void HashTable::PrintAll() {

    // Iterate through each position in the as table
    for (auto it = nodes.begin(); it != nodes.end(); ++it) {
        //   if key not equal to UINT_MAX
        if (it->key != UINT_MAX) {
            // output courseId and courseName
            cout << "Key " << it->key << ": " << it->course.courseId << ", " << it->course.courseName << endl;

            Node* node = it->next;
            while (node != nullptr) {
                // output courseId and courseName
                cout << "key " << node->key << ": " << node->course.courseId << ", " << node->course.courseName << endl;
                // node is equal to next node
                node = node->next;
            }
        }
    }
}

Course HashTable::Search(string courseId) {

    Course course;

    unsigned int key = hash(courseId);
    Node* node = &(nodes.at(key));

    if (node != nullptr && node->key != UINT_MAX && node->course.courseId.compare(courseId) == 0) {
        return node->course;
    }
    // if no entry found for the key
    // return course
    if (node == nullptr || node->key == UINT_MAX) {
        return course;
    }


    //node is equal to next node
    while (node != nullptr) {
        if (node->key != UINT_MAX && node->course.courseId.compare(courseId) == 0) {
            return node->course;
        }
        node = node->next;
    }
    return course;
}

//Display course
void displayCourse(Course course) {

    cout << course.courseId << ", " << course.courseName << endl;
    cout << "Prerequisites: ";

    //if there is no prerequisites
    if (course.preReq.empty()) {
        cout << "none." << endl;
    }
    //If there are prerequisites
    else {
        for (unsigned int i = 0; i < course.preReq.size(); i++) {
            cout << course.preReq.at(i);

            if (i < course.preReq.size() - 1) {
                cout << ", ";
            }
        }
    }
    cout << endl;
    return;
}

//Splits the lines from the file at each comma.
vector<string> Split(string lineFeed) {

    char delim = ',';
    //includes a delimiter at the end so last word is also read
    vector<string> lineTokens;
    string temp;
    for (char character : lineFeed)
    {
        if (character == delim)
        {
            //store words in token vector
            lineTokens.push_back(temp);
            temp.clear();
        }
        else {
            temp += character;
        }
    }
    //add final value
    lineTokens.push_back(temp);
    return lineTokens;
}

void loadCourses(string csvPath, HashTable* Courses) {

    cout << "Loading courses" << endl;
    ifstream inFS;
    string line;
    vector<string> stringTokens;


    //tries to open file
    inFS.open(csvPath);

    //Check if file is open.
    if (!inFS.is_open()) {
        cout << "Could not open file. Please check inputs. " << endl;
        return;
    }

    //while the file 
    while (getline(inFS, line)) {

        Course aCourse;

        stringTokens = Split(line);

        //if size is less than 2 gives error
        if (stringTokens.size() < 2) {
            cout << "error. Next line." << endl;
            continue;
        }    

        aCourse.courseId = stringTokens.at(0);
        aCourse.courseName = stringTokens.at(1);

        //accounts for prerequisites
        for (unsigned int i = 2; i < stringTokens.size(); i++) {    
            if (!stringTokens.at(i).empty()) {
                aCourse.preReq.push_back(stringTokens.at(i));
            }
        }

        //Add course to hash table
        Courses->Insert(aCourse);
    }

    inFS.close();
    cout << "Courses loaded successfully." << endl;
}


//I chose a random name for these variables for the file
int main(int bob, char* bobs[])
{
    string csvPath;
    string acourseKey;

    switch (bob) {
    case 2:
        csvPath = bobs[1];
        break;

    case 3:
        csvPath = bobs[1];
        acourseKey = bobs[2];
        break;
    default:
        csvPath = "CS 300 ABCU_Advising_Program_Input.csv";
    }

    //define a new hashtable to hold the course info
    HashTable* coursesList;

    coursesList = new HashTable();

    //Menu
    Course course;
    int choice = 0;
    while (choice != 9) {
        choice = 0;
        cout << "Welcome to the course planner." << endl;
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List" << endl;
        cout << "3. Print Course." << endl;
        cout << "4. Completed course Menu." << endl;
        cout << "5. Generate Course Plan." << endl;
        cout << "9. Exit." << endl;
        cout << "What would you like to do?" << endl;
        string menuInput;
        cin >> menuInput;

        //check the input
        if (menuInput == "1") {
            choice = 1;
        }
        else if (menuInput == "2") {
            choice = 2;
        }
        else if (menuInput == "3") {
            choice = 3;
        }
        else if (menuInput == "4") {
            choice = 4;
        }
        else if (menuInput == "5") {
            choice = 5;
        }
        else if (menuInput == "9") {
            choice = 9;
        }
        else {
            cout << "Invalid input. Enter 1, 2, 3, 4, 5, or 9." << endl;
            continue;
        }
        switch (choice) {
            //Loads the file
            case 1:

                loadCourses(csvPath, coursesList);
                break;

            //Prints all items in the file
            case 2:

                coursesList->PrintAll();
                break;

            //asks for input for searching for a course
            case 3:

                cout << "What course do you want to know about? It is case sensitive!" << endl;

                cin >> acourseKey;

                course = coursesList->Search(acourseKey);

                //if course id found
                if (!course.courseId.empty()) {

                    displayCourse(course);
                }

                //if course id not found
                else {

                    cout << "Course ID " << acourseKey << " not found." << endl;
                }

                break;

            //Completed Courses submenu
            case 4: {
                string completedMenuInput;
                int completedChoice = 0;

                while (completedChoice != 9) {

                    cout << endl;
                    cout << "Completed Course Menu" << endl;
                    cout << "-------------------------" << endl;
                    cout << "1. Enter Completed Course" << endl;
                    cout << "2. Remove Completed Course" << endl;
                    cout << "3. View Completed Courses" << endl;
                    cout << "9. Return to Main Menu" << endl;
                    cout << "Enter your choice: ";

                    cin >> completedMenuInput;

                    if (completedMenuInput == "1") {
                        completedChoice = 1;
                    }
                    else if (completedMenuInput == "2") {
                        completedChoice = 2;
                    }
                    else if (completedMenuInput == "3") {
                        completedChoice = 3;
                    }
                    else if (completedMenuInput == "9") {
                        completedChoice = 9;
                    }
                    else {
                        cout << "Invalid input. Please enter 1, 2, 3, or 9." << endl;
                        continue;
                    }

                    switch (completedChoice) {

                    case 1:
                        cout << "Enter the course ID you have completed: " << endl;
                        cin >> acourseKey;

                        course = coursesList->Search(acourseKey);

                        if (course.courseId.empty()) {
                            cout << "Course ID " << acourseKey << " was not found." << endl;
                        }
                        else if (coursesList->IsCompleted(acourseKey)) {
                            cout << acourseKey << " is already listed as completed." << endl;
                        }
                        else {
                            coursesList->AddCompletedCourse(acourseKey);
                            cout << acourseKey
                                << " has been added to your completed courses."
                                << endl;
                        }
                        break;

                    case 2:
                        cout << "Enter the course ID you want to remove: " << endl;
                        cin >> acourseKey;

                        if (!coursesList->IsCompleted(acourseKey)) {
                            cout << acourseKey
                                << " is not listed as a completed course."
                                << endl;
                        }
                        else {
                            coursesList->RemoveCompletedCourse(acourseKey);

                            cout << acourseKey
                                << " has been removed from your completed courses."
                                << endl;
                        }
                        break;

                    case 3:
                        coursesList->PrintCompletedCourses();
                        break;

                    case 9:
                        cout << "Returning to Main Menu." << endl << endl;
                        break;
                    }
                }

                break;
            }

            //Generate course plan
            case 5:
                coursesList->GenerateCoursePlan();
                break;
            }
        }
    cout << "Thank you for using the course planner" << endl;
    return 0;
}
