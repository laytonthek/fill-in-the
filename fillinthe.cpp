#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <limits>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
using namespace std;

/*
Version 4 of Fill in the $#&%@! is here!

This is a major update as it adds a beautiful title screen and gives the ability to make your own stories! 
the user-made stories are automatically added to master.list and added to the main menu upon creation so 
they can be immediately played without configuration!
*/

unsigned int ID_COUNTER;
string VER = "Beta v4";
string TITLE = "Fill in the $#&%@!";
string LONGTITLE = "" + TITLE + " (" + VER + ")";
string COPYRIGHT = "Developed by Layton Kinyon. August 2026";
string HEADING_BUFFER;

void cls() {
    std::cout << "\033[2J\033[1;1H"; // Clear screen and move cursor to top-left
}

void sleep(unsigned int seconds) {
    std::this_thread::sleep_for(std::chrono::seconds(seconds)); // Makes console wait for x seconds
}

void msleep(unsigned int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
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

        return "error - can't read file.\n";

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

            cout << "\n saving failed...";
            pause();
            return false;

        } else {

            handle << buffer.str();
            handle.close();
            return true;
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
            for (int x = 0; x < 20; x++)
                getline(wiki_buffer, line[x].data);

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

void bufferWiki(Wiki::WikiItem& current_item, string type) {

    if (type == "noun")
        current_item.setupWikiItem("noun");
    if (type == "nouns")
        current_item.setupWikiItem("nouns");
    if (type == "proper_noun")
        current_item.setupWikiItem("proper_noun");
    if (type == "verb")
        current_item.setupWikiItem("verb");
    if (type == "verbed")
        current_item.setupWikiItem("verbed");
    if (type == "verbing")
        current_item.setupWikiItem("verbing");
    if (type == "adverb")
        current_item.setupWikiItem("adverb");
    if (type == "adjective")
        current_item.setupWikiItem("adjective"); 
    if (type == "animal")
        current_item.setupWikiItem("animal");
    if (type == "animals")
        current_item.setupWikiItem("animals");
}

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
    msleep(1777);

    string prompt;

    do {

        cout << "\nWould you like to save? 'yes' or 'no': ";
        cin >> prompt;

    } while (prompt != "yes" and prompt != "no");
    
    if (prompt == "yes") {

        cls();
        cout << "Type a filename: ";

        string maketxt;
        ostringstream txtbuffer;
        bool flag;

        cin >> maketxt;

        txtbuffer << display.heading << " - " << maketxt << endl << endl;

        for (int x = 0; x < 6; x++) 
            txtbuffer << display.hookline[x].data << endl;

        txtbuffer << endl;

        for (int x = 0; x < 6; x++)
            txtbuffer << display.problemline[x].data << endl;

        txtbuffer << endl;

        for (int x = 0; x < 6; x++)
            txtbuffer << display.chaosline[x].data << endl;

        flag = writeFile(maketxt, txtbuffer);

        if (!flag) {

            cout << "\n   error saving file...";
            pause();
        }
    }

    return;
}

void wikiScreen(Wiki::WikiItem& current_item) {

    string dummy;

    cls();

    cout << "<WIKI> " << current_item.proper_name << endl;
    cout << endl;
    for (int x = 0; x < 20; x++)
        cout << current_item.line[x].data << endl;
    cout << endl;
    cout << "Press ENTER to continue...";
    
    getline(cin, dummy);
    return;
}

void answerScreen(Story& on_display, Wiki& current_library) {

    string current_prompt;

    cin.ignore();

    for (int x = 0; x < 12; x++) {                                           

        bufferWiki(current_library.buffer, on_display.answer[x].type);

        cls();
        cout << on_display.heading << "\n";
        cout << "type 'WIKI' in all caps for an explanation";

        cout << endl << on_display.answer[x].label << ": ";
        getline(cin, current_prompt);
        msleep(111);
        if (current_prompt != "WIKI") {
            
            on_display.answer[x].data = current_prompt; //need to fix bug where input is only one word, need to be able to have whitespace

        } else {

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
        while (replaceFirst(on_display.chaosline[x].data, "@", on_display.answer[current].data) and current <12) {current++;}

    storyScreen(on_display);

    for (int x = 0; x < 12; x++)
        on_display.answer[x].data.clear();

    for (int x = 0; x < 18; x++) {

        for (int y = 0; y < 6; y++) {

            on_display.hookline[y].data = on_display.backupline[x].data;
            x++;
        }

        for (int y = 0; y < 6; y++) {

            on_display.problemline[y].data = on_display.backupline[x].data;
            x++;
        }

        for (int y = 0; y < 6; y++) {

            on_display.chaosline[y].data = on_display.backupline[x].data;
            x++;
        }
    }
}

void savedScreen() {

    string current_prompt;
    string prompt_file;
    string current_line;

    cls();
    cout << "Type in desired filename to view: ";
    cin >> current_prompt;

    prompt_file = readFile(current_prompt);
    istringstream current_buffer(prompt_file);

    cls();

    while (getline(current_buffer, current_line))
        cout << current_line << endl;

    cout << "\nPress Enter to continue...";

    pause();
    
}

void wikiMenu(Wiki& current_library) {

    string current_prompt;
    int prompt_number;
    do {
        current_prompt.clear();

        cls();
        cout << "Welcome to the built-in wiki, which helps you play the game better!" << endl;
        cout << endl;
        cout << "(1) " << current_library.noun.proper_name << endl;
        cout << "(2) " << current_library.nouns.proper_name << endl;
        cout << "(3) " << current_library.proper_noun.proper_name << endl;
        cout << "(4) " << current_library.verb.proper_name << endl;
        cout << "(5) " << current_library.verbed.proper_name << endl;
        cout << "(6) " << current_library.verbing.proper_name << endl;
        cout << "(7) " << current_library.adverb.proper_name << endl;
        cout << "(8) " << current_library.adjective.proper_name << endl;
        cout << endl;
        cout << "Select an option by number or type 'menu' to return to main menu. ";
        cin >> current_prompt;
        cin.ignore();

        if (current_prompt == "1")
            wikiScreen(current_library.noun);
        if (current_prompt == "2")
            wikiScreen(current_library.nouns);
        if (current_prompt == "3")
            wikiScreen(current_library.proper_noun);
        if (current_prompt == "4")
            wikiScreen(current_library.verb);
        if (current_prompt == "5")
            wikiScreen(current_library.verbed);
        if (current_prompt == "6")
            wikiScreen(current_library.verbing);
        if (current_prompt == "7")
            wikiScreen(current_library.adverb);
        if (current_prompt == "8")
            wikiScreen(current_library.adjective);

    } while (current_prompt != "menu");

    return;
}

void titleScreen() {

    cls();

    int title_length = LONGTITLE.length();
    int copyright_length = COPYRIGHT.length();

    cout << "\n\n\n\n\n\n\n\n\n\n";

    int t_start_pos = 40 - (title_length / 2);
    int c_start_pos = 40 - (copyright_length / 2);

    int box_length = copyright_length + 4;
    int b_start_pos = 40 - (box_length / 2);

    for (int x = 0; x < b_start_pos; x++)
        cout << " ";
        
    for (int x = 0; x < box_length; x++)
        cout << "=";

    cout << endl;

    for (int x = 0; x < c_start_pos - 2; x++)
        cout << " ";

    cout << "| ";

    for (int x = c_start_pos - 2; x < t_start_pos - 2; x++)
        cout << " ";
        
    cout << LONGTITLE;

    for (int x = t_start_pos + title_length; x < c_start_pos + copyright_length; x++)
        cout << " ";

    cout << " |" << endl;
        
    for (int x = 0; x < c_start_pos - 2; x++)
        cout << " ";

    cout << "| " << COPYRIGHT << " |" << endl;

    for (int x = 0; x < b_start_pos; x++)
        cout << " ";
        
    for (int x = 0; x < box_length; x++)
        cout << "=";

    cout << endl;
    cout << endl;

    sleep(4);

    return;
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

    cout << "Come up with a title for your story. Keep in mind this title shows up both\nat the top of your story and in the main menu.\n\nTitle: ";
    cin.ignore();
    getline(cin, title);

    cls();

    cout << "READ ME IF YOURE NEW TO MAKING STORIES!" << endl;
    cout << endl;
    cout << "your story MUST be 18 lines, press ENTER at the end of each line to ensure proper formatting." << endl;
    cout << "those 18 lines will be grouped into 3 paragraphs of 6 lines each." << endl;
    cout << endl;
    cout << "use the @ symbol to insert places for answers to be written in. THERE MUST BE 12 IN YOUR STORY!" << endl;
    cout << "NO MORE, NO LESS, NO EXCEPTIONS! this is for proper formatting\nfor all the old systems I'm porting to later\n"
    cout << "IF YOU DONT HAVE 12 @ PROGRAM WONT CONTINUE!\n\n";
    cout << "Press ENTER to continue...";
    getline(cin, dummy);

    ostringstream s_buffer;

    cls();
    cout << title << endl;
    s_buffer << title << endl << endl;
    cout << endl;
    for (int x = 0; x < 6; x++) {  

        cout << x + 1 << ">";
        getline(cin, line[x]);
        s_buffer << line[x] << endl;
    }
    cout << endl;
    s_buffer << endl;
    for (int x = 6; x < 12; x++) {

        cout << x + 1 << ">";
        getline(cin, line[x]);
        s_buffer << line[x] << endl;
    }
    cout << endl;
    s_buffer << endl;
    for (int x = 12; x < 18; x++) {

        cout << x + 1 << ">";
        getline(cin, line[x]);
        s_buffer << line[x] << endl;
    }

    for (int x = 0; x < 18; x++) {

        at_checker = at_checker + countString(line[x], "@");
    }

    if (at_checker != 12) {
        cls();
        cout << "you really need to read the instructions, or count your @'s!\nThere's content guidelines, you know...";
        sleep(5);
        cout << "";
        cout << "";
        cout << "";
        cout << "";
        cout << "this should teach you.";
        exit(1);
    }
    
    cls();
    cout << "Now to make your answer types... please don't make any typos or put in a nonexistent type\n";
    cout << "Don't use the 'actual:' ones, use the 'types' ONLY, also they're case-sensitive\n";
    cout << "actual: Noun | Plural Noun | Proper Noun | Verb | Animal | Animal, PLural\n";
    cout << " types: noun |    nouns    | proper_noun | verb | animal | animals\n";
    cout << "actual: Verb, past tense | Verb ending in -ing | Adverb | Adjective\n";
    cout << " types:     verbed       |      verbing        | adverb | adjective\n\n";

    ostringstream a_buffer;

    for (int x = 0; x < 12; x++) {
        cout << x + 1 << ": ";
        getline(cin, answer_type[x]);
        a_buffer << answer_type[x] << endl;
    }

    writeFile(filename + ".CUS", s_buffer);
    writeFile(filename + ".ANS", a_buffer);

    ostringstream master_buffer;
    string master_list = readFile("master.list");

    istringstream master_rebuilder(master_list);
    vector<string> master_line = {""};
    string temp_line = "";
    int iteration = 0;

    while (getline(master_rebuilder, temp_line)) {

        master_line[iteration] = temp_line;
        master_line.push_back("");
        if (master_line[iteration] != "")
            master_buffer << master_line[iteration] << endl;

        iteration++;
    }

    master_buffer << filename << ".CUS";

    writeFile("master.list", master_buffer);
    
    ++ID_COUNTER;

    Story newCustomStory;
    newCustomStory.setupStory(filename + ".CUS", filename + ".ANS");

    return newCustomStory;
}

int main() {

    titleScreen();

    int selection;
    string choice;
    int page;
    int pages;
    int pagemarker;

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
        cout << "uh oh. " << LONGTITLE << " closed because master.list is missing." << endl;
        cout << "check that the PATH that the game is running at is the same as master.list" << endl;
        cout << "(if launching from terminal, cd to game folder)" << endl;
        exit(1);
    }
    string line;
    while (getline(masterlist, line)) {
        if (line == "") 
            break;
        ++ID_COUNTER;
    }
    masterlist.clear();
    masterlist.seekg(0);
    vector<Story> listing;
    { //WE NEED THESE BRACES FOR DEALLOCATION
        int tossme = 0;
        string lineMod;
        while (getline(masterlist, line)) {
            ++tossme;
            if (line == "")              
                break;
            lineMod = line;
            lineMod.erase(lineMod.end() - 4, lineMod.end());
            lineMod += ".ANS";
            Story temp_story_buffer;
            listing.push_back(temp_story_buffer);
            listing[tossme-1].setupStory(line, lineMod);
        }
    } // DONT TOUCH THIS ONE EITHER
    masterlist.close();

    if (ID_COUNTER < 1) {

        cls();
        cout << "master.list can not be empty" << endl;
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
        cout << "Commands:\n<back> <next> <quit> <view> <wiki> <make>" << endl;
        cout << endl;
        cout << "Choose a story:\n";

        pagemarker = (page - 1) * 10 + 1;

        for (int x = pagemarker; x <= pagemarker + 9 and x <= ID_COUNTER ; x++)
            cout << "(" << x << ") " << listing[x-1].heading << endl; 

        cout << "\nPage " << page << " of " << pages;
        cout << "\nType a number or command: ";
        cin >> choice;
        
        try {
            selection = stoi(choice);
        }
        catch (const std::invalid_argument&) {}
        catch (const std::out_of_range&) {}

        if (choice == "back" and page > 1) {page = --page;}

        if (choice == "next" and page < pages) {page = ++page;}

        if (choice == "view") {savedScreen();}

        if (choice == "wiki") {wikiMenu(library);}

        if (choice == "make") {Story temp_story_buffer; listing.push_back(temp_story_buffer); listing.back() = makeScreen();}

        //view saved
        
        if ((selection > 0) and (selection <= ID_COUNTER))
            answerScreen(listing[selection-1], library);
    selection = 0;
    } while (choice != "quit");

    cls();
    cout << "Goodbye!\n";
    return 0;
}

/*
ideas: 

making title screen animations? using my new msleep function I made just for this.
maybe using math to make cheap animations..? using only strings tho.

making saved (only saved) content deletable (hence why they have a different extension)
including txt files generated from playing the game.

rewrite all the dumbass error messages and threatening sounding warnings so app is more professional

*/
