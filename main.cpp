//
//  main.cpp skeleton
//  lab3
//
//  Created by Tarek Abdelrahman on 2018-08-25.
//  Copyright © 2018 Tarek Abdelrahman. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

#include "globals.h"
#include "shape.h"

// This is the shape array, to be dynamically allocated
shape** shapesArray;

// The number of shapes in the database, to be incremented
// everytime a shape is successfully created
int shapeCount = 0;

// The value of the argument to the maxShapes command
int max_shapes;

void makeShapesCommand(stringstream& s);
void createCommand(stringstream& s);
void moveCommand(stringstream& s);
void rotateCommand(stringstream& s);
void drawCommand(stringstream& s);
void deleteCommand(stringstream& s);
void errorCheckerStrings(stringstream& s, string& temp, bool typeChecker);
void errorCheckerIntegers(stringstream& s, int& temp);
void resetError();
void setError(const string& msg, int priority);
bool existingInput(stringstream& s);
bool existingError();
int findShape(const string& name);
void deleteShape(shape*& s, bool printOutput = true);

// Priority Declaration
int priorityOfError = 0;
string messageError;

int main() {

    string line;
    string command;

    cout << "> ";
    getline(cin, line);   // Get a line from standard input

    while (!cin.eof()) {
        stringstream lineStream (line);
        
        resetError();
        
        lineStream >> command;

        if (command == "maxShapes") {
            makeShapesCommand(lineStream);
        }

        else if (command == "create") {
            createCommand(lineStream);
        }

        else if (command == "move") {
            moveCommand(lineStream);
        }

        else if (command == "rotate") {
            rotateCommand(lineStream);
        }

        else if (command == "draw") {
            drawCommand(lineStream);
        }

        else if (command == "delete") {
            deleteCommand(lineStream);
        }

        else { // Invalid command.
            setError("invalid command", 1);
        }
        
        //Prints out any existing errors
        if (existingError()) {
            cout << "Error: " << messageError << endl;
        }
        
        // Once the command has been processed, prompt for the
        // next command
        cout << "> ";          // Prompt for input
        getline(cin, line);   // Get the command line

    } 

    // Delete all dynamically allocated memory
    if (shapesArray != nullptr) {
        for (int i=0; i < shapeCount; i++) {
            if (shapesArray[i] != nullptr) {
                delete shapesArray[i];
                shapesArray[i] = nullptr;
            }
        }
        delete [] shapesArray;
    }
    return 0;
}

void makeShapesCommand(stringstream& s) {
    s >> max_shapes;
    shapesArray = new shape*[max_shapes];
    for (int i = 0; i < max_shapes; i++) {
        shapesArray[i] = nullptr;
    }
    cout << "New database: max shapes is " << max_shapes << endl;
}
void createCommand(stringstream& s) {
    // Initialize variables
    string name, type;
    int x_loc, y_loc, x_sz, y_sz;

     // Get argument inputs
    errorCheckerStrings(s, name, false);

    // Check if shape exists
    int shapeIndex = findShape(name);
    if (shapeIndex >= 0) {
        setError( "shape " + name + " exists", 4); // Shape already exists
    }

    // Continue taking arguments
    errorCheckerStrings(s, type, true);
    errorCheckerIntegers(s, x_loc);
    errorCheckerIntegers(s, y_loc);
    errorCheckerIntegers(s, x_sz);
    errorCheckerIntegers(s, y_sz);

    // Check for too many args or full array.
    if (!existingError() && existingInput(s))
        setError("too many arguments", 8);
    if (!existingError() && shapeCount >= max_shapes)
        setError("shape array is full", 10);

    if (!existingError()) {
        shapesArray[shapeCount++] = new shape(name, type, x_loc, y_loc, x_sz, y_sz);
        cout << "Created ";
        shapesArray[shapeCount-1]->draw();
    }
}

void moveCommand(stringstream& s) {
    string name;
    int x_loc, y_loc;

    errorCheckerStrings(s, name, false);

    int shapeIndex = findShape(name);
    // Shape not found, 5th on priority list
    if (!name.empty() && shapeIndex < 0) {
        setError("shape " + name + " not found", 5); 
    }
    
    errorCheckerIntegers(s, x_loc);
    errorCheckerIntegers(s, y_loc);

    // Too many arguments, 8th on priority list
    if (!existingError() && existingInput(s))
        setError("too many arguments", 8);

    // Move if there are no errors detected
    if (!existingError()) {
        // Move shapeIndex
        shapesArray[shapeIndex]->setXlocation(x_loc);
        shapesArray[shapeIndex]->setYlocation(y_loc);
        cout << "Moved " << name << " to " << x_loc << " " << y_loc << endl;
    }
}

void rotateCommand(stringstream& s) {
    string name;
    int angle;

    errorCheckerStrings(s, name, false);

    int shapeIndex = findShape(name);
    // Checking if shape exists
    if (!name.empty() && shapeIndex < 0)
        setError("shape " + name + " not found", 5); 

    errorCheckerIntegers(s, angle);
    
    if (angle > 360)
        // Invalid value is 7th on priority list 
        setError("invalid value", 7);

    // Too many arguments
    if (!existingError() && existingInput(s))
        setError("too many arguments", 8);

    // Rotate if no errors detected
    if (!existingError()) {
        // Rotate 
        shapesArray[shapeIndex]->setRotate(angle);
        cout << "Rotated " << name << " by " << angle << endl;
    }
} 

void drawCommand(stringstream& s) {
    string name;

    // Multiple error checking
    errorCheckerStrings(s, name, false);

    if (messageError == "invalid shape name" && name == "all")
        resetError(); // Remove error

    int shapeIndex = findShape(name);
    if (!name.empty() && name != "all" && shapeIndex < 0)
        setError("shape " + name + " not found", 5); //Shape does not exist

    if (!existingError() && existingInput(s))
        setError("too many arguments", 8);

    // Draw object if no error detected
    if (!existingError()) {
        if (name == "all") {
            // Draw all shapes
            cout << "Drew all shapes" << endl;
            for (int i = 0; i < shapeCount; i++) {
                if (shapesArray[i] == nullptr) continue; 
                shapesArray[i]->draw();
            }
        } else {
            cout << "Drew " << name << endl;
            shapesArray[shapeIndex]->draw();
        }
    }
} 

void deleteCommand(stringstream& s) {
    string name;
    
    // Error checking
    errorCheckerStrings(s, name, false);

    if (messageError == "invalid shape name" && name == "all")
        resetError(); 

    int shapeIndex = findShape(name);
    if (!name.empty() && shapeIndex < 0 && name != "all")
        setError("shape " + name + " not found", 5); 

    if (!existingError() && existingInput(s))
        setError("too many arguments", 8);

    // Delete if no errors detected
    if (!existingError()) {
        if (name == "all") {
            // Delete all shapes
            cout << "Deleted: all shapes" << endl;
            for (int i = 0; i < shapeCount; i++) {
                if (shapesArray[i] == nullptr) {
                    continue; 
                }
                deleteShape(shapesArray[i], false);
            }
        } 
        else {
            deleteShape(shapesArray[shapeIndex]);
        }
    }
}

// Checks error only for integers
void errorCheckerIntegers(stringstream& s, int& temp) {
    if (existingError()) {
        return;
    }
    // Too few arguments, 9th on priority list
    if (!existingInput(s)) {
        setError("too few arguments", 9);
        return;
    }

    s >> temp;

    // Check for valid integer input
    char c = (char) s.peek();
    bool whiteSpace = c == ' ' || c == '\n' || c == '\r' ||
                        c == '\t' || c == '\f' || c == '\v';

    if (!s.eof() && (s.fail() || !whiteSpace)) {
        setError("invalid argument", 2);
    }

    // Invalid value of temp cannot be negative
    if (temp < 0) {
        setError("invalid value", 7);
    }
}

// Checks error only for strings
void errorCheckerStrings(stringstream& s, string& temp, bool typeChecker) {

    if (existingError()) {
        return;
    }

    if (!existingInput(s)) {
        setError("too few arguments", 9);
        return;
    }

    s >> temp;
    
    if (typeChecker) {
        
        // Check if shape type is valid
        bool shapeType = false;
        for (int i = 0; i < NUM_TYPES; i++) {
            if (temp == shapeTypesList[i]) {
                shapeType = true;
                break;
            }
        }
        if (!shapeType) {
            setError("invalid shape type", 6);
            return;
        }

    } 
    else {
        int max = NUM_KEYWORDS > NUM_TYPES ? NUM_KEYWORDS : NUM_TYPES;
        for (int i = 0; i < max; i++) {
            if ((i < NUM_KEYWORDS && temp == keyWordsList[i]) || (i < NUM_TYPES && temp == shapeTypesList[i])) {
                // Invalid shape name has 3rd priority
                setError("invalid shape name", 3);
                return;
            }
        }
    }
}

// Called upon to print error messages based on priority of the error
void setError(const string& message, int priority) {
    if (priorityOfError <= 0 || priority < priorityOfError) {
        messageError = message;
        if (priority > 0) {
            priorityOfError = priority;
        }
    }
}

// Reset error in beginning of main's while loop
void resetError() {
    priorityOfError = 0;
    messageError = "";
}

// Uses peek to check for any inputted whitespaces
bool existingInput(stringstream& s) {
    s >> std::ws; 
    return s.peek() != EOF;
}

int findShape(const string& name) {
    if (name == "all") {
        return -1;
    }
    // Checking if shape exists
    for (int i = 0; i < shapeCount; i++) {
        if (shapesArray[i]->getName() == name && shapesArray[i] != nullptr) {
            return i;
        }
    }
    return -1;
}

bool existingError() {
    return !messageError.empty();
}

void deleteShape(shape*& s, bool printOutput) {
    if (printOutput == true) {
        cout << "Deleted shape " << s->getName() << endl;
    }
}