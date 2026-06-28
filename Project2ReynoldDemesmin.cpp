/*
 * ProjectTwo.cpp
 * CS 300 Project Two - ABCU Advising Assistance Program
 * Author: Reynold Demesmin
 *
 * This program is completely contained in this one CPP file.
 * It does not use CSVParser.hpp or any other custom header file.
 * The #include statements below are standard C++ library headers.
 *
 * AI Use Acknowledgment:
 * I used ChatGPT to help organize and reviewthis code. I reviewed the
 * final program and made sure it reflects my understanding of the assignment. No SNHU property was uploaded into ChatGPT.
 */

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

// Stores the information for one course record.
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Represents one node in the binary search tree.
struct Node {
    Course course;
    Node* left;
    Node* right;

    explicit Node(const Course& courseValue)
        : course(courseValue), left(nullptr), right(nullptr) {
    }
};

// Binary search tree keyed by course number.
class BinarySearchTree {
private:
    Node* root;
    size_t courseCount;

    static void destroy(Node* node) {
        if (node == nullptr) {
            return;
        }

        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    static void insertNode(Node*& node, const Course& course) {
        if (node == nullptr) {
            node = new Node(course);
            return;
        }

        if (course.courseNumber < node->course.courseNumber) {
            insertNode(node->left, course);
        }
        else {
            insertNode(node->right, course);
        }
    }

    static const Course* searchNode(const Node* node,
                                    const string& courseNumber) {
        if (node == nullptr) {
            return nullptr;
        }

        if (courseNumber == node->course.courseNumber) {
            return &node->course;
        }

        if (courseNumber < node->course.courseNumber) {
            return searchNode(node->left, courseNumber);
        }

        return searchNode(node->right, courseNumber);
    }

    static void printInOrder(const Node* node) {
        if (node == nullptr) {
            return;
        }

        printInOrder(node->left);
        cout << node->course.courseNumber << ", "
             << node->course.courseTitle << '\n';
        printInOrder(node->right);
    }

    // Inserts middle items first so the tree remains reasonably balanced.
    static void buildBalancedNodes(BinarySearchTree& tree,
                                   const vector<Course>& courses,
                                   int first,
                                   int last) {
        if (first > last) {
            return;
        }

        const int middle = first + (last - first) / 2;
        tree.insert(courses[static_cast<size_t>(middle)]);
        buildBalancedNodes(tree, courses, first, middle - 1);
        buildBalancedNodes(tree, courses, middle + 1, last);
    }

public:
    BinarySearchTree() : root(nullptr), courseCount(0) {
    }

    ~BinarySearchTree() {
        clear();
    }

    BinarySearchTree(const BinarySearchTree&) = delete;
    BinarySearchTree& operator=(const BinarySearchTree&) = delete;

    BinarySearchTree(BinarySearchTree&& other) noexcept
        : root(other.root), courseCount(other.courseCount) {
        other.root = nullptr;
        other.courseCount = 0;
    }

    BinarySearchTree& operator=(BinarySearchTree&& other) noexcept {
        if (this != &other) {
            clear();
            root = other.root;
            courseCount = other.courseCount;
            other.root = nullptr;
            other.courseCount = 0;
        }

        return *this;
    }

    void insert(const Course& course) {
        insertNode(root, course);
        ++courseCount;
    }

    void buildBalanced(vector<Course> courses) {
        clear();

        sort(courses.begin(), courses.end(),
            [](const Course& firstCourse, const Course& secondCourse) {
                return firstCourse.courseNumber < secondCourse.courseNumber;
            });

        if (!courses.empty()) {
            buildBalancedNodes(*this, courses, 0,
                               static_cast<int>(courses.size()) - 1);
        }
    }

    const Course* search(const string& courseNumber) const {
        return searchNode(root, courseNumber);
    }

    void printCourseList() const {
        printInOrder(root);
    }

    bool empty() const {
        return root == nullptr;
    }

    size_t size() const {
        return courseCount;
    }

    void clear() {
        destroy(root);
        root = nullptr;
        courseCount = 0;
    }
};

// Removes whitespace from the beginning and end of a string.
string trim(const string& text) {
    const size_t first = text.find_first_not_of(" \t\r\n");

    if (first == string::npos) {
        return "";
    }

    const size_t last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

// Converts course numbers to uppercase for case-insensitive searching.
string toUpperCase(string text) {
    transform(text.begin(), text.end(), text.begin(),
        [](unsigned char character) {
            return static_cast<char>(toupper(character));
        });

    return text;
}

// Removes quotation marks from a complete file path copied from File Explorer.
string removeSurroundingQuotes(string text) {
    text = trim(text);

    if (text.size() >= 2 && text.front() == '"' && text.back() == '"') {
        return text.substr(1, text.size() - 2);
    }

    return text;
}

// Splits a comma-separated line and trims every field.
vector<string> splitCsvLine(const string& line) {
    vector<string> fields;
    string field;
    stringstream lineStream(line);

    while (getline(lineStream, field, ',')) {
        fields.push_back(trim(field));
    }

    // Preserve a final empty field when the line ends with a comma.
    if (!line.empty() && line.back() == ',') {
        fields.push_back("");
    }

    return fields;
}

// Attempts to open the exact path and several common Visual Studio locations.
bool openCourseFile(const string& enteredName,
                    ifstream& inputFile,
                    string& openedPath) {
    const string fileName = removeSurroundingQuotes(enteredName);

    vector<string> possiblePaths;
    possiblePaths.push_back(fileName);

    // These locations help when Visual Studio runs from Debug or x64/Debug.
    if (fileName.find('\\') == string::npos &&
        fileName.find('/') == string::npos) {
        possiblePaths.push_back("../" + fileName);
        possiblePaths.push_back("../../" + fileName);
        possiblePaths.push_back("../../../" + fileName);
        possiblePaths.push_back("../../../../" + fileName);
    }

    for (const string& path : possiblePaths) {
        inputFile.clear();
        inputFile.open(path);

        if (inputFile.is_open()) {
            openedPath = path;
            return true;
        }
    }

    return false;
}

// Reads, validates, and loads the CSV data into the binary search tree.
bool loadCoursesFromFile(const string& fileName,
                         BinarySearchTree& courseTree) {
    ifstream inputFile;
    string openedPath;

    if (!openCourseFile(fileName, inputFile, openedPath)) {
        cerr << "Error: The file could not be opened.\n";
        cerr << "Enter the complete file path, or place the CSV in the same "
             << "folder as the Visual Studio project.\n";
        return false;
    }

    vector<Course> courses;
    unordered_set<string> courseNumbers;
    string line;
    size_t lineNumber = 0;

    while (getline(inputFile, line)) {
        ++lineNumber;
        line = trim(line);

        if (line.empty()) {
            continue;
        }

        // Remove a UTF-8 byte-order mark from the first line when present.
        if (lineNumber == 1 && line.size() >= 3 &&
            static_cast<unsigned char>(line[0]) == 0xEF &&
            static_cast<unsigned char>(line[1]) == 0xBB &&
            static_cast<unsigned char>(line[2]) == 0xBF) {
            line.erase(0, 3);
        }

        const vector<string> fields = splitCsvLine(line);

        if (fields.size() < 2) {
            cerr << "Error on line " << lineNumber
                 << ": A course number and title are required.\n";
            return false;
        }

        Course course;
        course.courseNumber = toUpperCase(fields[0]);
        course.courseTitle = fields[1];

        if (course.courseNumber.empty() || course.courseTitle.empty()) {
            cerr << "Error on line " << lineNumber
                 << ": The course number and title cannot be blank.\n";
            return false;
        }

        if (!courseNumbers.insert(course.courseNumber).second) {
            cerr << "Error on line " << lineNumber
                 << ": Duplicate course number "
                 << course.courseNumber << ".\n";
            return false;
        }

        unordered_set<string> prerequisitesOnLine;

        for (size_t index = 2; index < fields.size(); ++index) {
            const string prerequisite = toUpperCase(fields[index]);

            // The supplied CSV includes empty trailing columns.
            if (prerequisite.empty()) {
                continue;
            }

            if (!prerequisitesOnLine.insert(prerequisite).second) {
                cerr << "Error on line " << lineNumber
                     << ": Duplicate prerequisite "
                     << prerequisite << ".\n";
                return false;
            }

            course.prerequisites.push_back(prerequisite);
        }

        courses.push_back(course);
    }

    if (inputFile.bad()) {
        cerr << "Error: A problem occurred while reading the file.\n";
        return false;
    }

    if (courses.empty()) {
        cerr << "Error: The file does not contain any course records.\n";
        return false;
    }

    // Confirm that every prerequisite is also present as a course record.
    for (const Course& course : courses) {
        for (const string& prerequisite : course.prerequisites) {
            if (courseNumbers.find(prerequisite) == courseNumbers.end()) {
                cerr << "Error: " << prerequisite
                     << " is listed as a prerequisite for "
                     << course.courseNumber
                     << " but is missing from the file.\n";
                return false;
            }
        }
    }

    // Use a temporary tree so a failed load does not erase valid data.
    BinarySearchTree newTree;
    newTree.buildBalanced(courses);
    courseTree = move(newTree);

    cout << courseTree.size() << " courses were loaded successfully from "
         << openedPath << ".\n";
    return true;
}

// Prints one course and the numbers and titles of its prerequisites.
void printCourseInformation(const BinarySearchTree& courseTree,
                            const string& requestedNumber) {
    const string courseNumber = toUpperCase(trim(requestedNumber));
    const Course* course = courseTree.search(courseNumber);

    if (course == nullptr) {
        cout << "Course " << courseNumber << " was not found.\n";
        return;
    }

    cout << course->courseNumber << ", " << course->courseTitle << '\n';

    if (course->prerequisites.empty()) {
        cout << "Prerequisites: None\n";
        return;
    }

    cout << "Prerequisites: ";

    for (size_t index = 0; index < course->prerequisites.size(); ++index) {
        if (index > 0) {
            cout << "; ";
        }

        const string& prerequisiteNumber = course->prerequisites[index];
        const Course* prerequisite = courseTree.search(prerequisiteNumber);

        cout << prerequisiteNumber;

        if (prerequisite != nullptr) {
            cout << ", " << prerequisite->courseTitle;
        }
    }

    cout << '\n';
}

// Displays the menu required by the project.
void displayMenu() {
    cout << "1. Load Data Structure.\n";
    cout << "2. Print Course List.\n";
    cout << "3. Print Course.\n";
    cout << "9. Exit\n";
    cout << "What would you like to do? ";
}

// Reads a menu option and rejects non-numeric or extra input.
bool readMenuChoice(int& choice) {
    string input;

    if (!getline(cin, input)) {
        return false;
    }

    stringstream inputStream(input);
    char extraCharacter;

    if (!(inputStream >> choice) || (inputStream >> extraCharacter)) {
        cout << input << " is not a valid option.\n";
        choice = -1;
    }

    return true;
}

int main() {
    BinarySearchTree courseTree;
    bool running = true;

    cout << "Welcome to the course planner.\n";

    while (running) {
        displayMenu();

        int choice = 0;

        if (!readMenuChoice(choice)) {
            cout << "\nThank you for using the course planner!\n";
            break;
        }

        if (choice == -1) {
            continue;
        }

        switch (choice) {
        case 1: {
            cout << "What is the name of the file? ";
            string fileName;
            getline(cin, fileName);

            if (trim(fileName).empty()) {
                cout << "Error: A file name is required.\n";
            }
            else {
                loadCoursesFromFile(fileName, courseTree);
            }
            break;
        }

        case 2:
            if (courseTree.empty()) {
                cout << "Error: Load the course data before printing the course list.\n";
            }
            else {
                cout << "Here is a sample schedule:\n";
                courseTree.printCourseList();
            }
            break;

        case 3: {
            if (courseTree.empty()) {
                cout << "Error: Load the course data before searching for a course.\n";
                break;
            }

            cout << "What course do you want to know about? ";
            string courseNumber;
            getline(cin, courseNumber);

            if (trim(courseNumber).empty()) {
                cout << "Error: A course number is required.\n";
            }
            else {
                printCourseInformation(courseTree, courseNumber);
            }
            break;
        }

        case 9:
            cout << "Thank you for using the course planner!\n";
            running = false;
            break;

        default:
            cout << choice << " is not a valid option.\n";
            break;
        }
    }

    return 0;
}
