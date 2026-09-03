#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <limits>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <map>
#include <filesystem>
#include <algorithm>
#include <random>
#include <ctime>
using namespace std;

/*
Beta 5 of Fill in the $#&%@! is here!

This is the biggest update to date! There has been a complete UI overhaul that adds colors and
animation. a lot of display options and UI elements are now colorful and more fun to look at.

We added stars, yes. STARS

Also, the content system is complete for now... You can now delete any user-generated content
from within the app, so now made stories and saved played stories are deletable without exit.
*/

void cls() {std::cout << "\033[2J\033[1;1H";}

string color(const string& colorName, int selector) {

    static const map<pair<int, string>, string> ansiColors = {
        {{0, "reset"}, "\033[0m"},
        {{0, "bold"}, "\033[1m"},
        {{3, "black"}, "\033[30m"},
        {{3, "red"}, "\033[31m"},
        {{3, "green"}, "\033[32m"},
        {{3, "yellow"}, "\033[33m"},
        {{3, "blue"}, "\033[34m"},
        {{3, "magenta"}, "\033[35m"},
        {{3, "cyan"}, "\033[36m"},
        {{3, "white"}, "\033[37m"},
        {{4, "black"}, "\033[40m"},
        {{4, "red"}, "\033[41m"},
        {{4, "green"}, "\033[42m"},
        {{4, "yellow"}, "\033[43m"},
        {{4, "blue"}, "\033[44m"},
        {{4, "magenta"}, "\033[45m"},
        {{4, "cyan"}, "\033[46m"},
        {{4, "white"}, "\033[47m"},
        {{9, "black"}, "\033[90m"},
        {{9, "red"}, "\033[91m"},
        {{9, "green"}, "\033[92m"},
        {{9, "yellow"}, "\033[93m"},
        {{9, "blue"}, "\033[94m"},
        {{9, "magenta"}, "\033[95m"},
        {{9, "cyan"}, "\033[96m"},
        {{9, "white"}, "\033[97m"}
    };

    auto match = ansiColors.find({selector, colorName});
    if (match != ansiColors.end()) {
        return match->second;
    }

    return "";
}

void hideCursor() {cout << "\033[?25l";}

//void showCursor() {cout << "\033[?25h";}

void star(int x, int y) {

    random_device rd; //random seed source
    mt19937 gen(rd()); //random engine (Mersenne Twister)
    cout << "\0337";

    hideCursor();

    //vector<string> star = {"*", "^", "#", "+", "|", "?"};

    vector<string> star = {"*", "^", "\""};
    vector<string> color_type = {

        color("red", 3),
        color("green", 3), 
        color("blue", 3),
        color("yellow", 3),
        color("cyan", 3),
        color("magenta", 3)
    };

    uniform_int_distribution<> dist_star(1, star.size() - 1); //number range distributions
    uniform_int_distribution<> dist_color(1, color_type.size() - 1);

    ostringstream current_star;
    current_star << "\033[" << y << ";" << x << "H" << color_type[dist_color(gen)] << star[dist_star(gen)] << color("reset", 0);
    //return current_star.str();

    cout << "\033[" << y << ";" << x << "H" << color_type[dist_color(gen)] << star[dist_star(gen)] << "\0338";

    return;
}

string getEXT(string str) {str.erase(str.begin(), str.end() - 3); return str;}

int visibleLength(const string& text) {
    int length = 0;
    for (size_t i = 0; i < text.size(); ++i) {
        if (text[i] == '\033') {
            ++i;
            if (i < text.size() && text[i] == '[') {
                ++i;
                while (i < text.size() && text[i] != 'm') {
                    ++i;
                }
            }
            continue;
        }
        ++length;
    }
    return length;
}

unsigned int ID_COUNTER;
static const string TITLE = color("cyan", 3) + "Fill in the $#&%@!" + color("reset", 0); string VER = color("red", 3) + "Beta v5" + color("reset", 0);
//string TITLE = "Fill in the $#&%@!"; string VER = "Beta v4";
static const string LONGTITLE = "" + TITLE + " (" + VER + ")";
static const string COPYRIGHT = color("green", 3) + "Developed by Layton Kinyon. August 2026" + color("reset", 0);
string HEADING_BUFFER;

void sleep(unsigned int seconds) {cout << "\n"; std::this_thread::sleep_for(std::chrono::seconds(seconds));}

void msleep(unsigned int milliseconds) {cout << "\n"; std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));}

void nlclr() {std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');}

void pause() {nlclr(); cin.get();}

string readFile(string filename) {

    ifstream handle(filename);
    ostringstream buffer;

    if (!handle.is_open()) {

        return "error";

    } else {

        getline(handle, HEADING_BUFFER);
        handle.clear();
        handle.seekg(0);

        buffer << handle.rdbuf();
        handle.close();
        return buffer.str();
    }
}

bool writeFile(string filename, ostringstream& buffer) {

        ofstream handle(filename);

        if (!handle.is_open()) {

            cout << color("red", 3) << "\n saving failed..." << color("reset", 0);
            pause();
            return false;

        } else {

            handle << buffer.str();
            handle.close();
            return true;
        }
}

void deleteFile(string filename) {

}

void saveToList(string new_listing, int list) {

    ostringstream master_buffer;
    string master_list;

    if (list == 0) {master_list = readFile("master.list");}
    else if (list == 1) {master_list = readFile("saved.list");}

    istringstream master_rebuilder(master_list);
    vector<string> master_line = {""};
    string temp_line = "";
    int iteration = 0;

    while (getline(master_rebuilder, temp_line)) {

        master_line[iteration] = temp_line;
        master_line.push_back("");
        if (master_line[iteration] != "") {master_buffer << master_line[iteration] << endl;}

        iteration++;
    }

    master_buffer << new_listing;

    if (list == 0) {writeFile("master.list", master_buffer);}
    else if (list == 1) {writeFile("saved.list", master_buffer);}
}

void deleteFromList(string new_deleted_listing, int list) {

    ostringstream master_buffer;

    string master_list;

    if (list == 0) {master_list = readFile("master.list");}
    else if (list == 1) {master_list = readFile("saved.list");}

    istringstream master_rebuilder(master_list);
    vector<string> master_line = {""};
    string temp_line = "";
    int iteration = 0;

    while (getline(master_rebuilder, temp_line)) {

        //master_line[iteration] = temp_line;
        master_line.push_back(temp_line);
        if (master_line[iteration] != "") {
            if (master_line[iteration] == new_deleted_listing) {master_buffer << "";}
            else {master_buffer << master_line[iteration] << endl;}
        }
        iteration++;
    }

    if (list == 0) {writeFile("master.list", master_buffer);}
    else if (list == 1) {writeFile("saved.list", master_buffer);}
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

int countString(const std::string& str, const std::string& sub) {
    if (sub.empty()) return 0;
    int count = 0;
    size_t pos = 0;
    while ((pos = str.find(sub, pos)) != std::string::npos) {
        ++count;
        pos += sub.length(); // Move past this match for non-overlapping
    }
    return count;
}

class Story {

    public:

    unsigned int id;
    string heading;
    string file;
    bool is_custom = false;

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

    Line backupline[18];

    Answer answer[12];

    void setupStory(string filename, string answers) {

        string extension;
        file = filename;
        extension = getEXT(file);

        if (extension == "SRY") {is_custom = false;}
        else if (extension == "CUS") {is_custom = true;}
        else {cls(); cout << color("red", 3) << "story must end in .CUS or .SRY, case sensitive" << color("reset", 0); exit(1);}

        string storydata = readFile(filename);
        heading = HEADING_BUFFER;

        istringstream stream(storydata);
        string temp;

        getline(stream, temp);
        getline(stream, temp);

        for (int x = 0; x < 6; x++) {getline(stream, hookline[x].data);}

        getline(stream, temp);

        for (int x = 0; x < 6; x++) {getline(stream, problemline[x].data);}

        getline(stream, temp);

        for (int x = 0; x < 6; x++) {getline(stream, chaosline[x].data);}

        for (int x = 0; x < 18; x++) {

            for (int y = 0; y < 6; y++) {

                backupline[x].data = hookline[y].data;
                x++;
            }

            for (int y = 0; y < 6; y++) {

                backupline[x].data = problemline[y].data;
                x++;
            }

            for (int y = 0; y < 6; y++) {

                backupline[x].data = chaosline[y].data;
                x++;
            }
        }

        string answerdata = readFile(answers);
        istringstream answerstream(answerdata);

        for (int x = 0; x < 12; x++) {

            getline(answerstream, answer[x].type);

            if (answer[x].type == "proper_noun") {answer[x].label = "Proper Noun";}
            if (answer[x].type == "noun") {answer[x].label = "Noun";}
            if (answer[x].type == "nouns") {answer[x].label = "Plural Noun";}
            if (answer[x].type == "animal") {answer[x].label = "Animal";}
            if (answer[x].type == "animals") {answer[x].label = "Animal, Plural";}
            if (answer[x].type == "body_part") {answer[x].label = "Body Part";}
            if (answer[x].type == "verb") {answer[x].label = "Verb";}
            if (answer[x].type == "verbed") {answer[x].label = "Verb, past tense";}
            if (answer[x].type == "verbing") {answer[x].label = "Verb ending in -ing";}
            if (answer[x].type == "adverb") {answer[x].label = "Adverb";}
            if (answer[x].type == "adjective") {answer[x].label = "Adjective";}
        }
    }   
};

class Wiki {

    public:

    class WikiItem {

        public:

        string proper_name;
        unsigned int id;

        class Line {

            public:

            string data;
        };

        Line line[20];

        void setupWikiItem(string type) {

            string wiki_data;
            string filename = type + ".WIK";
            wiki_data = readFile(filename);

            istringstream wiki_buffer(wiki_data);
            for (int x = 0; x < 20; x++) {getline(wiki_buffer, line[x].data);}

            if (type == "noun") {proper_name = "Noun"; id = 1;}
            if (type == "nouns") {proper_name = "Plural Noun"; id = 2;}
            if (type == "proper_noun") {proper_name = "Proper Noun"; id = 3;}
            if (type == "verb") {proper_name = "Verb"; id = 4;}
            if (type == "verbed") {proper_name = "Verb, Past Tense"; id = 5;}
            if (type == "verbing") {proper_name = "Verb Ending In -ing"; id = 6;}
            if (type == "adverb") {proper_name = "Adverb"; id = 7;}
            if (type == "adjective") {proper_name = "Adjective"; id = 8;}
            if (type == "animal") {proper_name = "Noun :3"; id = 9;}
            if (type == "animals") {proper_name = "Plural Noun :3"; id = 10;}
        }
    };

    WikiItem noun;
    WikiItem nouns;
    WikiItem proper_noun;
    WikiItem verb;
    WikiItem verbed;
    WikiItem verbing;
    WikiItem adverb;
    WikiItem adjective;
    WikiItem animal;
    WikiItem animals;

    WikiItem buffer;
};

class Save {

    public:

    string file;

    string setupSave(string filename) {

        file = filename;
        return readFile(file);
    }
};

void bufferWiki(Wiki::WikiItem& current_item, string type) {

    if (type == "noun") {current_item.setupWikiItem("noun");}
    if (type == "nouns") {current_item.setupWikiItem("nouns");}
    if (type == "proper_noun") {current_item.setupWikiItem("proper_noun");}
    if (type == "verb") {current_item.setupWikiItem("verb");}
    if (type == "verbed") {current_item.setupWikiItem("verbed");}
    if (type == "verbing") {current_item.setupWikiItem("verbing");}
    if (type == "adverb") {current_item.setupWikiItem("adverb");}
    if (type == "adjective") {current_item.setupWikiItem("adjective");}
    if (type == "animal") {current_item.setupWikiItem("animal");}
    if (type == "animals") {current_item.setupWikiItem("animals");}
}

void storyScreen(Story& display, vector<Save>& current_save) {

    cls();
    cout << color("blue", 3) << display.heading << color("reset", 0) << "\n\n";
    for (int x = 0; x < 6; x++) {cout << display.hookline[x].data << endl;}
    cout << endl;
    for (int x = 0; x < 6; x++) {cout << display.problemline[x].data << endl;}
    cout << endl;
    for (int x = 0; x < 6; x++) {cout << display.chaosline[x].data << endl;}
    msleep(1777);

    string prompt;

    do {

        cout << "\nWould you like to " << color("green", 3) << "save" << color("reset", 0) << "? 'yes' or 'no': ";
        cin >> prompt;

    } while (prompt != "yes" and prompt != "no");
    
    if (prompt == "yes") {

        cls();
        cout << "Type a filename: ";

        string maketxt;
        ostringstream txtbuffer;
        bool flag;

        cin >> maketxt;
        Save new_save;
        new_save.file = maketxt;
        current_save.push_back(new_save);


        txtbuffer << display.heading << " - " << maketxt << endl << endl;

        for (int x = 0; x < 6; x++) {txtbuffer << display.hookline[x].data << endl;}

        txtbuffer << endl;

        for (int x = 0; x < 6; x++) {txtbuffer << display.problemline[x].data << endl;}

        txtbuffer << endl;

        for (int x = 0; x < 6; x++) {txtbuffer << display.chaosline[x].data << endl;}

        flag = writeFile(maketxt, txtbuffer);

        if (!flag) {cout << color("red", 3) << "\n   error saving file..." << color("reset", 0); pause();}

        saveToList(maketxt, 1);
    }

    return;
}

void wikiScreen(Wiki::WikiItem& current_item) {

    string dummy;

    cls();

    cout << color("magenta", 3) << "<WIKI> " << color("cyan", 3) << current_item.proper_name << color("reset", 0) << endl;
    cout << endl;
    for (int x = 0; x < 20; x++) {cout << current_item.line[x].data << endl;}
    cout << endl;
    cout << "Press " << color("yellow", 3) << "ENTER" << color("reset", 0) << " to continue...";
    
    getline(cin, dummy);
    return;
}

void playStory(Story& on_display, Wiki& current_library, vector<Save>& current_save) {

    string current_prompt;

    cin.ignore();

    for (int x = 0; x < 12; x++) {                                           

        bufferWiki(current_library.buffer, on_display.answer[x].type);

        cls();
        cout << on_display.heading << "\n";
        cout << "type '" << color("magenta", 3) << "WIKI" << color("reset", 0) << "' in all caps for an explanation";

        cout << endl << color("cyan", 3) << on_display.answer[x].label << color("reset", 0) << ": ";
        getline(cin, current_prompt);
        msleep(111);
        if (current_prompt != "WIKI") {on_display.answer[x].data = current_prompt;} else {

            wikiScreen(current_library.buffer);
            x--;
        }
    }

    int current = 0;

    for (int x = 0; x < 6; x++)
        while (replaceFirst(on_display.hookline[x].data, "@", on_display.answer[current].data) and current < 12) {current++;}

    for (int x = 0; x < 6; x++)
        while (replaceFirst(on_display.problemline[x].data, "@", on_display.answer[current].data) and current < 12) {current++;}

    for (int x = 0; x < 6; x++)
        while (replaceFirst(on_display.chaosline[x].data, "@", on_display.answer[current].data) and current < 12) {current++;}

    storyScreen(on_display, current_save);

    for (int x = 0; x < 12; x++) {on_display.answer[x].data.clear();}

    for (int x = 0; x < 18; x++) {

        for (int y = 0; y < 6; y++) {on_display.hookline[y].data = on_display.backupline[x].data; x++;}
        for (int y = 0; y < 6; y++) {on_display.problemline[y].data = on_display.backupline[x].data; x++;}
        for (int y = 0; y < 6; y++) {on_display.chaosline[y].data = on_display.backupline[x].data; x++;}
    }
}

void savedScreen() {

    string current_prompt;
    string prompt_file;
    string current_line;

    cls();
    cout << "Type in desired " << color("green", 3) << "filename " << color("reset", 0) << "to " << color("green", 3) << "view" << color("reset", 0) << ": ";
    cin >> current_prompt;

    prompt_file = readFile(current_prompt);
    istringstream current_buffer(prompt_file);

    cls();

    while (getline(current_buffer, current_line)) {cout << current_line << endl;}

    cout << "\nPress " << color("yellow", 3) << "Enter " << color("reset", 0) << "to continue...";

    pause();
    
}

void wikiMenu(Wiki& current_library) {

    string current_prompt;
    int prompt_number;
    do {
        current_prompt.clear();

        cls();
        cout << "Welcome to the built-in " << color("magenta", 3) << "wiki" << color("reset", 0) << ", which helps you play the game better!" << endl;
        cout << endl;
        cout << "(1) " << color("magenta", 3) << current_library.noun.proper_name << color("reset", 0) << endl;
        cout << "(2) " << color("magenta", 3) << current_library.nouns.proper_name << color("reset", 0) << endl;
        cout << "(3) " << color("magenta", 3) << current_library.proper_noun.proper_name << color("reset", 0) << endl;
        cout << "(4) " << color("magenta", 3) << current_library.verb.proper_name << color("reset", 0) << endl;
        cout << "(5) " << color("magenta", 3) << current_library.verbed.proper_name << color("reset", 0) << endl;
        cout << "(6) " << color("magenta", 3) << current_library.verbing.proper_name << color("reset", 0) << endl;
        cout << "(7) " << color("magenta", 3) << current_library.adverb.proper_name << color("reset", 0) << endl;
        cout << "(8) " << color("magenta", 3) << current_library.adjective.proper_name << color("reset", 0) << endl;
        cout << endl;
        cout << "Select an option by number or type " << color("yellow", 3) << "menu" << color("reset", 0) << " to return to main menu: " << color("yellow", 3);
        cin >> current_prompt;
        cout << color("reset", 0);
        cin.ignore();

        if (current_prompt == "1") {wikiScreen(current_library.noun);}
        if (current_prompt == "2") {wikiScreen(current_library.nouns);}
        if (current_prompt == "3") {wikiScreen(current_library.proper_noun);}
        if (current_prompt == "4") {wikiScreen(current_library.verb);}
        if (current_prompt == "5") {wikiScreen(current_library.verbed);}
        if (current_prompt == "6") {wikiScreen(current_library.verbing);}
        if (current_prompt == "7") {wikiScreen(current_library.adverb);}
        if (current_prompt == "8") {wikiScreen(current_library.adjective);}

    } while (current_prompt != "menu");

    return;
}

void titleScreen() {

    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> width(1, 80);
    uniform_int_distribution<> height(1, 25);

    vector<int> widths;
    vector<int> heights;

    for (int x = 0; x < 15; x++) {

        widths.push_back(width(gen));
        heights.push_back(height(gen));
        while (((widths[x] >= 20) && (widths[x] <= 62)) && ((heights[x] >= 11) && (heights[x] <= 14))) {
            
            widths[x] = width(gen);
            heights[x] = height(gen);
        }
    }

    auto endTime = std::chrono::steady_clock::now() + std::chrono::seconds(3);

    while (std::chrono::steady_clock::now() < endTime) {
        cls();
        cout << "\0337";

        hideCursor();
        cls();

        int title_length = visibleLength(LONGTITLE);
        int copyright_length = visibleLength(COPYRIGHT);

        cout << "\n\n\n\n\n\n\n\n\n\n";

        int t_start_pos = 40 - (title_length / 2);
        int c_start_pos = 40 - (copyright_length / 2);

        int box_length = copyright_length + 4;
        int b_start_pos = 40 - (box_length / 2);

        for (int x = 0; x < b_start_pos; x++) {cout << " ";}
            
        cout << color("magenta", 3);

        vector<string> color_selector = {color("magenta", 3), color("cyan", 3)};
        int frame;

        for (int x = 0; x < box_length; x++) {cout << color_selector[(x + (frame % 2)) % color_selector.size()] << "=";}

        frame++;

        cout << endl;

        for (int x = 0; x < c_start_pos - 2; x++) {cout << " ";}

        cout << "| ";

        for (int x = c_start_pos - 2; x < t_start_pos - 2; x++) {cout << " ";}
            
        cout << LONGTITLE;

        for (int x = t_start_pos + title_length; x < c_start_pos + copyright_length; x++) {cout << " ";}

        cout << color("magenta", 3);
        cout << " |" << endl;
            
        for (int x = 0; x < c_start_pos - 2; x++) {cout << " ";}

        cout << color("magenta", 3) << "| " << COPYRIGHT << color("magenta", 3) << " |" << endl;

        for (int x = 0; x < b_start_pos; x++) {cout << " ";}
            
        for (int x = 0; x < box_length; x++) {cout << color_selector[(x + (frame % 2)) % color_selector.size()] << "=";}

        cout << endl;
        cout << endl << color("reset", 0);

        //showCursor();
        for (int x = 0; x < 15; x++)
            star(widths[x], heights[x]);
        
        //cout << "\033[10;0H----5----1----5----2----5----3----5----4----5---=5=---5----612345----7----5----8";

        msleep(160);
            //x20-62    y11-14
    }


    return;
}

void starScreen() {

    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> width(1, 80);
    uniform_int_distribution<> height(1, 25);

    auto endTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(1600);

    vector<int> widths;
    vector<int> heights;
    int stars = 15;

    for (int x = 0; x < stars; x++) {
        widths.push_back(width(gen));
        heights.push_back(height(gen));
    }

    while (std::chrono::steady_clock::now() < endTime) { 

        cls();
        for (int x = 0; x < stars; x++) {
            star(widths[x], heights[x]);
        }
        msleep(160);
    }
}

Story makeScreen() {

    string title;
    string line[18];
    string filename;
    string answer_type[12];

    int at_checker = 0;
    string dummy;

    cls();

    cout << "Type filename (WITHOUT THE EXTENSION): ";
    cin >> filename;

    cls();

    cout << "Come up with a " << color("blue", 3) << "title " << color("reset", 0) << "for your story. Keep in mind this title shows up both\nat the top of your story and in the main menu.\n\n" << color("blue", 3) << "Title: ";
    cin.ignore();
    getline(cin, title);
    cout << color("reset", 0);

    cls();

    cout << color("red", 3);
    cout << "READ ME IF YOURE NEW TO MAKING STORIES!" << endl;
    cout << endl;
    cout << "your " << color("blue", 3) << "story " << color("red", 3) << "MUST be 18 lines, press ENTER at the end of each line to ensure proper formatting." << endl;
    cout << "those 18 lines will be grouped into 3 paragraphs of 6 lines each." << endl;
    cout << endl;
    cout << "use the " << color("green", 3) << "@ " << color("red", 3) << "symbol to insert places for answers to be written in. THERE MUST BE 12 IN YOUR STORY!" << endl;
    cout << "NO MORE, NO LESS, NO EXCEPTIONS! this is for proper formatting\nfor all the old systems I'm porting to later\n";
    cout << "IF YOU DONT HAVE 12 @ PROGRAM WONT CONTINUE!\n\n";
    cout << color("reset", 0);
    cout << "Press " << color("yellow", 3) << "ENTER " << color("yellow", 3) << "to continue...";
    getline(cin, dummy);

    ostringstream s_buffer;

    cls();
    cout << color("blue", 3);
    cout << title << color("reset", 0) << endl;
    s_buffer << title << endl << endl;
    cout << endl;
    for (int x = 0; x < 6; x++) {  

        cout << x + 1 << color("blue", 3) << "> " << color("reset", 0);
        getline(cin, line[x]);
        s_buffer << line[x] << endl;
    }
    cout << endl;
    s_buffer << endl;
    for (int x = 6; x < 12; x++) {

        cout << x + 1 << color("blue", 3) << "> " << color("reset", 0);
        getline(cin, line[x]);
        s_buffer << line[x] << endl;
    }
    cout << endl;
    s_buffer << endl;
    for (int x = 12; x < 18; x++) {

        cout << x + 1 << color("blue", 3) << "> " << color("reset", 0);
        getline(cin, line[x]);
        s_buffer << line[x] << endl;
    }

    for (int x = 0; x < 18; x++) {at_checker = at_checker + countString(line[x], "@");}

    if (at_checker != 12) {
        cls();
        cout << color("red", 4) << color("black", 3);
        cout << "you really need to read the instructions, or count your @'s!\nThere's content guidelines, you know...";
        cout << "";
        cout << "";
        cout << "";
        cout << "";
        cout << "this should teach you.";
        cout << color("reset", 0);
        exit(1);
    }
    
    cls();
    cout << "Now to make your " << color("green", 3) << "answer " << color("reset", 0) << "types... please don't make any typos or put in a nonexistent type\n";
    cout << "Don't use the 'actual:' ones, use the '" << color("green", 3) << "types" << color("reset", 0) << "' ONLY, also they're case-sensitive\n";
    cout << "actual: Noun | Plural Noun | Proper Noun | Verb | Animal | Animal, PLural\n";
    cout << color("green", 3);
    cout << " types: noun |    nouns    | proper_noun | verb | animal | animals\n" << color("reset", 0);
    cout << "actual: Verb, past tense | Verb ending in -ing | Adverb | Adjective\n";
    cout << color("green", 3);
    cout << " types:     verbed       |      verbing        | adverb | adjective\n\n";
    cout << color("reset", 0);

    ostringstream a_buffer;

    for (int x = 0; x < 12; x++) {
        cout << color("reset", 0);
        cout << x + 1 << color("green", 3) << "> ";
        getline(cin, answer_type[x]);
        a_buffer << answer_type[x] << endl;
    }
    cout << color("reset", 0);

    writeFile(filename + ".CUS", s_buffer);
    writeFile(filename + ".ANS", a_buffer);

    saveToList(filename + ".CUS", 0);
    
    ++ID_COUNTER;

    Story newCustomStory;
    newCustomStory.setupStory(filename + ".CUS", filename + ".ANS");

    return newCustomStory;
}

void deleteSavedScreen(vector<Save>& on_display) {

    string current_prompt;
    int deleter;

    static int page = 1;
    static int pages;
    if (on_display.size() > 0) {pages = (on_display.size() + 10 - 1) / 10;}
    else {pages = 1;}

    static int page_marker;

    do {
        deleter = 0;
        cls();
        cout << color("red", 3) << "Warning: you are about to delete your fond memories." << color("reset", 0) << endl;
        cout << color("yellow", 3) << "<back> <next> " << color("green", 3) << "<menu>" << color("reset", 0) << endl << endl;

        page_marker = (page - 1) * 10 + 1;

        for (int x = page_marker; (x <= page_marker + 9) && (x <= on_display.size()); x++)
            cout << x << "> " << color("green", 3) <<  on_display[x - 1].file << ">" << color("reset", 0) << endl;

        cout << "\nPage " << color("yellow", 3) << page << color("reset", 0) << " of " << color("yellow", 3) << pages << color("reset", 0);
        cout << "\ncommand: " << color("red", 3);
        cin >> current_prompt;
        cout << color("reset", 0);

        if (current_prompt == "back") {--page;}
        else if (current_prompt == "next") {++page;}
        if (page < 1) {page = 1;}
        if (page > pages) {page = pages;}

        try {deleter = stoi(current_prompt);}
        catch (const std::invalid_argument&) {}
        catch (const std::out_of_range&) {}

    if ((deleter > 0) && (deleter <= static_cast<int>(on_display.size()))) {

        filesystem::remove(on_display[deleter - 1].file);
        deleteFromList(on_display[deleter - 1].file, 1);
        vector<Save>* p_on_display = &on_display;
        size_t index = deleter - 1;
        p_on_display->erase(p_on_display->begin() + index);
        break;
    }

    } while (current_prompt != "menu");

    return;
}

void deleteStoryScreen(vector<Story>& on_display) {
    
    string current_prompt;
    int deleter;
    static int page = 1;
    static int pages;
    if (on_display.size() > 0) {pages = (on_display.size() + 10 - 1) / 10;}
    else {pages = 1;}
    
    static int page_marker;

    do {
        deleter = 0;
        cls();
        cout << color("red", 3) << "Warning: you are about to delete your hard work." << color("reset", 0) << endl;
        cout << color("yellow", 3) << "<back> <next> " << color("green", 3) << "<menu>" << color("reset", 0) << endl << endl;
        page_marker = (page - 1) * 10 + 1;

        for (int x = page_marker; (x <= page_marker + 9) && (x <= on_display.size()); x++) 
            cout << x << "> " << color("blue", 3) << on_display[x - 1].heading << " <" << on_display[x - 1].file << ">" << color("reset", 0) << endl;

        cout << "\nPage " << color("yellow", 3) << page << color("reset", 0) << " of " << color("yellow", 3) << pages << color("reset", 0);
        cout << "\ncommand: " << color("red", 3);
        cin >> current_prompt;

        if (current_prompt == "back") {--page;}
        else if (current_prompt == "next") {++page;}
        if (page < 1) {page = 1;}
        if (page > pages) {page = pages;}

        cout << color("reset", 0);
        try {deleter = stoi(current_prompt);}
        catch (const std::invalid_argument&) {}
        catch (const std::out_of_range&) {}

    if ((deleter > 0) && (deleter <= static_cast<int>(on_display.size()))) {

        if (!on_display[deleter - 1].is_custom) {

            cls();
            cout << color("red", 3) << "Can't delete built-in story." << color("reset", 0) << "\n\n";
            cout << "" ;
            sleep(3);

        } else {
            deleteFromList(on_display[deleter - 1].file, 0);
            filesystem::remove(on_display[deleter - 1].file);
            string ans = on_display[deleter - 1].file;
            ans.erase(ans.end() - 3, ans.end());
            ans += "ANS";
            filesystem::remove(ans);
            vector<Story>* p_on_display = &on_display;
            size_t index = deleter - 1;
            p_on_display->erase(p_on_display->begin() + index);
            break;
        }
    }

    } while (current_prompt != "menu");

    return;
}

void deleteScreen(vector<Story>& listing, vector<Save>& saved) {

    string current_prompt;
    do {
        cls();
        cout << "Do you want to..." << endl;
        cout << endl;
        cout << "1) Delete a " << color("green", 3) << "saved memory" << color("reset", 0) << "?" << endl;
        cout << "9) Delete a " << color("blue", 3) << "custom story" << color("reset", 0) << "?" << endl;
        cout << endl;
        cout << "Type a number or type " << color("yellow", 3) << "menu" << color("reset", 0) << " to return to main menu: ";
        cout << color("green", 3);
        cin >> current_prompt;
        cout << color("reset", 0);

        if (current_prompt == "1") {deleteSavedScreen(saved);}
        else if (current_prompt == "9") {deleteStoryScreen(listing);}
    } while (current_prompt != "menu"); return;
}

int main() {

    titleScreen();

    random_device rd;
    mt19937 gen(rd());

    int selection;
    string choice;
    int page;
    int pages;
    int pagemarker;
    
    vector<string> c = {
        color("red", 3),
        color("green", 3),
        color("blue", 3),
        color("magenta", 3),
        color("yellow", 3),
        color("cyan", 3)
    };
    uniform_int_distribution<> cr(1, c.size() - 1);

    Wiki library;

    library.noun.setupWikiItem("noun");
    library.nouns.setupWikiItem("nouns");
    library.proper_noun.setupWikiItem("proper_noun");
    library.verb.setupWikiItem("verb");
    library.verbed.setupWikiItem("verbed");
    library.verbing.setupWikiItem("verbing");
    library.adverb.setupWikiItem("adverb");
    library.adjective.setupWikiItem("adjective");
    library.animal.setupWikiItem("animal");
    library.animals.setupWikiItem("animals");

    ifstream masterlist("master.list");
    if (!masterlist.is_open()) {
        cls();
        cout << color("red", 3);
        cout << "uh oh. " << LONGTITLE << color("red", 3) << " closed because master.list is missing." << endl;
        cout << "check that the PATH that the game is running at is the same as master.list" << endl;
        cout << "(if launching from terminal, cd to game folder)" << endl;
        cout << color("reset", 0);
        exit(1);
    }
    string line;
    while (getline(masterlist, line)) {if (line != "") {++ID_COUNTER;}}
    masterlist.clear();
    masterlist.seekg(0);
    vector<Story> listing;

    { //WE NEED THESE BRACES FOR DEALLOCATION
        int tossme = 0;
        string lineMod;
        while (getline(masterlist, line)) {
            if (line != "") {
                ++tossme;
                lineMod = line;
                lineMod.erase(lineMod.end() - 4, lineMod.end());
                lineMod += ".ANS";
                
                if (readFile(line) != "error") {
                    Story temp_story_buffer;
                    listing.push_back(temp_story_buffer);
                    listing[tossme-1].setupStory(line, lineMod);
                }
            }
        }
    } // DONT TOUCH THIS ONE EITHER
    masterlist.close();
    vector<Save> save;
    {
        int tossme = 0;
        ifstream savedlist("saved.list");
        if (!savedlist.is_open()) {cout << "saved.list is missing"; exit(1);}
        while (getline(savedlist, line)) {
            if (line != "") {
                ++tossme;
                Save temp_save_buffer;
                save.push_back(temp_save_buffer);
                save[tossme - 1].setupSave(line);
            }
        }
        savedlist.close();
    }


    if (ID_COUNTER < 1) {

        cls();
        cout << color("red", 3);
        cout << "master.list can not be empty" << endl;
        cout << color("reset", 0);
        exit(1);
    }
    
    pages = (ID_COUNTER + 10 - 1) / 10;
    page = 1;

    do
    {

        cls();
        cout << LONGTITLE << "\n";
        cout << COPYRIGHT << "\n\n";

        cout << "Welcome to " + TITLE + " Choose from a library of stories which\n";
        cout << "you can customize with your own silly words! You can make it serious, funny,\n";
        cout << "creative or just downright dirty! The laughter is best shared with friends!\n";
        cout << endl;
        cout << "Commands:\n" << color("yellow", 3) << "<back> <next>" << color("red", 3) <<" <quit> " << color("green", 3) << "<view> " << color("magenta", 3) << "<wiki> " << color("blue", 3) << "<make>" << color("red", 3) << " <delete> " << color("reset", 0);
        cout << c[cr(gen)] << "<" << c[cr(gen)] << "s" << c[cr(gen)] << "t" << c[cr(gen)] << "a" << c[cr(gen)] << "r" << c[cr(gen)] << "s" << c[cr(gen)] << ">" << color("reset", 0) << endl;
        cout << endl;
        cout << "Choose a " << color("blue", 3) << "story" << color("reset", 0) << ":\n";

        pagemarker = (page - 1) * 10 + 1;

        for (int x = pagemarker; x <= pagemarker + 9 && x <= listing.size() ; x++)
            cout << "(" << x << ") " << color("blue", 3) << listing[x-1].heading << color("reset", 0) << endl; 

        cout << "\nPage " << color("yellow", 3) << page << color("reset", 0) << " of " << color("yellow", 3) << pages << color("reset", 0);
        cout << "\nType a number or command: ";
        cout << color("green", 3);
        cin >> choice;
        cout << color("reset", 0);
        
        try {selection = stoi(choice);}
        catch (const std::invalid_argument&) {}
        catch (const std::out_of_range&) {}

        if (choice == "back" and page > 1) {page = --page;}
        if (choice == "next" and page < pages) {page = ++page;}
        if (choice == "view") {savedScreen();}
        if (choice == "wiki") {wikiMenu(library);}
        if (choice == "make") {Story temp_story_buffer; listing.push_back(temp_story_buffer); listing.back() = makeScreen();}
        if (choice == "delete") {deleteScreen(listing, save);}
        if (choice == "stars") {starScreen();}

        //view saved
        
        if ((selection > 0) and (selection <= listing.size())) {playStory(listing[selection-1], library, save);}
    selection = 0;
    } while (choice != "quit");

    cls();
    cout << "Goodbye!\n";
    return 0;
}

/*
ideas: 

making title screen animations? using my new msleep function I made just for this.
   ascii stars heheheh

make it so when transitioning between certain screens theres a couple secs of ascii stars

rewrite all visible strings so app is more professional

write 8 more stories to include with the base game so its not empty.

figure out more wiki options to add: pronoun, pronouns, pos_pronoun, body_part

chatgpt integration through APIs??? but it'd have to be guided and controlled through the software.
unsure if I really think this is a good idea or not, but we'll see :)

sift through each line of code and look for potential errors/bugs to catch.

STAY ON TOP OF MEMORY LEAKS WITH THESE VECTORS WE ALREADY HAD TWO INCIDENTS BRO!!

*/
