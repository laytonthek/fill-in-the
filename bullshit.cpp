#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <limits>
#include <fstream>
#include <sstream>
using namespace std;

unsigned int ID_COUNTER;
string VER = "Beta v3.3.0";
string TITLE = "Fill in the $#&%@!";
string LONGTITLE = "" + TITLE + " (" + VER + ")";
string COPYRIGHT = "Developed by Layton Kinyon. August 2026";
string HEADING_BUFFER;

void cls() {
    std::cout << "\033[2J\033[1;1H"; // Clear screen and move cursor to top-left
}

void sleep(unsigned int x) {
    std::this_thread::sleep_for(std::chrono::seconds(x)); // Makes console wait for x seconds
}

void nlclr() {std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');}

void pause() {
    nlclr(); //clrs buffer of newlines
    cin.get(); //wait for new line
}

string readFile(string filename) {

    ifstream handle(filename);
    ostringstream buffer;

    if (!handle.is_open()) {
        return "error\n";
    } else {
        getline(handle, HEADING_BUFFER);
        handle.clear();
        handle.seekg(0);

        buffer << handle.rdbuf();
        handle.close();
        return buffer.str();
    }
}

bool replaceFirst(std::string& str, const std::string& from, const std::string& to) {

    size_t pos = str.find(from);
    if (pos != std::string::npos) {

        str.replace(pos, from.length(), to);
        return true;

    } else {

        return false;
    }
}

class Story {

    public:

    unsigned int id;
    string heading;
    ifstream handle;

    class Line {

        public:

        string data;
    };

    class Answer {

        public:

        string type;
        string label;
        string data;
    };

    Line hookline[6];
    Line problemline[6];
    Line chaosline[6];

    Answer answer[12];

    void setupStory(string filename, string answers) {

        string storydata = readFile(filename);
        heading = HEADING_BUFFER;

        istringstream stream(storydata);
        string temp;

        getline(stream, temp);
        getline(stream, temp);

        for (int x = 0; x < 6; x++)
            getline(stream, hookline[x].data);

        getline(stream, temp);

        for (int x = 0; x < 6; x++)
            getline(stream, problemline[x].data);

        getline(stream, temp);

        for (int x = 0; x < 6; x++)
            getline(stream, chaosline[x].data);

        string answerdata = readFile(answers);
        istringstream answerstream(answerdata);

        for (int x = 0; x < 12; x++) {

            getline(answerstream, answer[x].type);

            if (answer[x].type == "proper_noun")
                answer[x].label = "Proper Noun";
            if (answer[x].type == "noun")
                answer[x].label = "Noun";
            if (answer[x].type == "nouns")
                answer[x].label = "Plural Noun";
            if (answer[x].type == "animal")
                answer[x].label = "Animal";
            if (answer[x].type == "animals")
                answer[x].label = "Animal, Plural";
            if (answer[x].type == "body_part")
                answer[x].label = "Body Part";
            if (answer[x].type == "verb")
                answer[x].label = "Verb";
            if (answer[x].type == "verbed")
                answer[x].label = "Verb, past tense";
            if (answer[x].type == "verbing")
                answer[x].label = "Verb ending in -ing";
            if (answer[x].type == "adverb")
                answer[x].label = "Adverb";
            if (answer[x].type == "adjective")
                answer[x].label = "Adjective";
        }
    }   
};

void storyScreen(Story& display) {

    cls();
    cout << display.heading << "\n\n";
    for (int x = 0; x < 6; x++)
        cout << display.hookline[x].data << endl;
    cout << endl;
    for (int x = 0; x < 6; x++)
        cout << display.problemline[x].data << endl;
    cout << endl;
    for (int x = 0; x < 6; x++)
        cout << display.chaosline[x].data << endl;
    sleep(3);
    cout << "\nPress ENTER/RETURN to continue...";
    string prompt;

    //implement save to txt feature here

    pause();
}

void answerScreen(Story& display) {

    int current = 0;

    for (int x = 0; x < 12; x++) {

        cls();
        cout << display.heading << "\n";
        //cout << "type ? for an explanation";

        cout << endl << display.answer[x].label << ": ";
        nlclr();
        cin >> display.answer[x].data; //need to fix bug where input is only one word, need to be able to have whitespace
        sleep(0.5);
    }

    for (int x = 0; x < 6; x++)
        while (replaceFirst(display.hookline[x].data, "@", display.answer[current].data) and current < 12) {current++;}

    for (int x = 0; x < 6; x++)
        while (replaceFirst(display.problemline[x].data, "@", display.answer[current].data) and current < 12) {current++;}

    for (int x = 0; x < 6; x++)
        while (replaceFirst(display.chaosline[x].data, "@", display.answer[current].data) and current <12) {current++;}

    storyScreen(display);
}

bool explanationScreen(string type) {

    //gotta make this screen
    return true;
}

int main() {
    int selection;
    string choice;

    ifstream masterlist("master.list");
    if (!masterlist.is_open()) {
        cls();
        cout << "uh oh. " << LONGTITLE << " closed because master.list is missing." << endl;
        cout << "check that the PATH that the game is running at is the same as master.list" << endl;
        cout << "(if launching from terminal, cd to game folder)" << endl;
        exit(1);
    }
    string line;
    while (getline(masterlist, line)) {
        ++ID_COUNTER;
    }
    masterlist.clear();
    masterlist.seekg(0);
    Story listing[ID_COUNTER];
    { //WE NEED THESE BRACES FOR DEALLOCATION
        int tossme = 0;
        string lineMod;
        while (getline(masterlist, line)) {
            ++tossme;
            lineMod = line;
            lineMod.erase(lineMod.end() - 4, lineMod.end());
            lineMod += ".ANS";
            listing[tossme-1].setupStory(line, lineMod);
        }
    } // DONT TOUCH THIS ONE EITHER
    masterlist.close();

    do
    {
        cls();
        cout << LONGTITLE << "\n";
        cout << COPYRIGHT << "\n\n";

        cout << "Welcome to " + TITLE + " Choose from a library of stories which\n";
        cout << "you can customize with your own silly words! You can make it serious, funny,\n";
        cout << "creative or just downright dirty! The laughter is best shared with friends!\n\n";

        cout << "Choose a story:\n";

        for (int x = 1; x <= ID_COUNTER; x++)
            cout << "(" << x << ") " << listing[x-1].heading << endl; 

        cout << "\nType a number or 'quit': ";
        cin >> choice;
        
        try {
            selection = stoi(choice);
        }
        catch (const std::invalid_argument&) {}
        catch (const std::out_of_range&) {}
        if (selection == 1)
            answerScreen(listing[0]);
    selection = 0;
    } while (choice != "quit");

    cls();
    cout << "Goodbye!\n";
    return 0;
}