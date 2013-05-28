using namespace std;

// Conversion
const GLfloat TO_RADIANS = M_PI/180.0;
const GLfloat TO_DEGREES = 180.0/M_PI;

void Help();
void Print(const std::string c, const std::string s);
void Log(const std::string s);
void Error(const std::string s);
void Warning(const std::string s);

// Reset
const std::string TERM__Off="\e[0m";             // Text Reset

// Regular Colors
const std::string TERM__Black="\e[0;30m";        // Black
const std::string TERM__Red="\e[0;31m";          // Red
const std::string TERM__Green="\e[0;32m";        // Green
const std::string TERM__Yellow="\e[0;33m";       // Yellow
const std::string TERM__Blue="\e[0;34m";         // Blue
const std::string TERM__Purple="\e[0;35m";       // Purple
const std::string TERM__Cyan="\e[0;36m";         // Cyan
const std::string TERM__White="\e[0;37m";        // White

// Bold
const std::string TERM__BBlack="\e[1;30m";       // Black
const std::string TERM__BRed="\e[1;31m";         // Red
const std::string TERM__BGreen="\e[1;32m";       // Green
const std::string TERM__BYellow="\e[1;33m";      // Yellow
const std::string TERM__BBlue="\e[1;34m";        // Blue
const std::string TERM__BPurple="\e[1;35m";      // Purple
const std::string TERM__BCyan="\e[1;36m";        // Cyan
const std::string TERM__BWhite="\e[1;37m";       // White

// Underline
const std::string TERM__UBlack="\e[4;30m";       // Black
const std::string TERM__URed="\e[4;31m";         // Red
const std::string TERM__UGreen="\e[4;32m";       // Green
const std::string TERM__UYellow="\e[4;33m";      // Yellow
const std::string TERM__UBlue="\e[4;34m";        // Blue
const std::string TERM__UPurple="\e[4;35m";      // Purple
const std::string TERM__UCyan="\e[4;36m";        // Cyan
const std::string TERM__UWhite="\e[4;37m";       // White

// Background
const std::string TERM__On_Black="\e[40m";       // Black
const std::string TERM__On_Red="\e[41m";         // Red
const std::string TERM__On_Green="\e[42m";       // Green
const std::string TERM__On_Yellow="\e[43m";      // Yellow
const std::string TERM__On_Blue="\e[44m";        // Blue
const std::string TERM__On_Purple="\e[45m";      // Purple
const std::string TERM__On_Cyan="\e[46m";        // Cyan
const std::string TERM__On_White="\e[47m";       // White

// High Intensity
const std::string TERM__IBlack="\e[0;90m";       // Black
const std::string TERM__IRed="\e[0;91m";         // Red
const std::string TERM__IGreen="\e[0;92m";       // Green
const std::string TERM__IYellow="\e[0;93m";      // Yellow
const std::string TERM__IBlue="\e[0;94m";        // Blue
const std::string TERM__IPurple="\e[0;95m";      // Purple
const std::string TERM__ICyan="\e[0;96m";        // Cyan
const std::string TERM__IWhite="\e[0;97m";       // White

// Bold High Intensity
const std::string TERM__BIBlack="\e[1;90m";      // Black
const std::string TERM__BIRed="\e[1;91m";        // Red
const std::string TERM__BIGreen="\e[1;92m";      // Green
const std::string TERM__BIYellow="\e[1;93m";     // Yellow
const std::string TERM__BIBlue="\e[1;94m";       // Blue
const std::string TERM__BIPurple="\e[1;95m";     // Purple
const std::string TERM__BICyan="\e[1;96m";       // Cyan
const std::string TERM__BIWhite="\e[1;97m";      // White

// High Intensity backgrounds
const std::string TERM__On_IBlack="\e[0;100m";   // Black
const std::string TERM__On_IRed="\e[0;101m";     // Red
const std::string TERM__On_IGreen="\e[0;102m";   // Green
const std::string TERM__On_IYellow="\e[0;103m";  // Yellow
const std::string TERM__On_IBlue="\e[0;104m";    // Blue
const std::string TERM__On_IPurple="\e[10;95m";  // Purple
const std::string TERM__On_ICyan="\e[0;106m";    // Cyan
const std::string TERM__On_IWhite="\e[0;107m";   // White

void Print(const std::string c, const std::string s)
{
    time_t rawtime;
    struct tm * ptm;
    time(&rawtime);
    ptm = gmtime(&rawtime);

    cout << c;  // Set color
    // Print HOUR
    if (ptm->tm_hour < 10){
        cout << 0 << (ptm->tm_hour)%24 << ":";
    } else {
        cout << (ptm->tm_hour)%24 << ":";
    }
    // Print MINUTES
    if (ptm->tm_min < 10){
        cout << 0 << (ptm->tm_min)%60 << ":";
    } else {
        cout << (ptm->tm_min)%60 << ":";
    }
    // Print SECONDS
    if (ptm->tm_sec < 10){
        cout << 0 << (ptm->tm_sec)%60;
    } else {
        cout << (ptm->tm_sec)%60;
    }
    // Print String
    cout << "  " << s << TERM__Off << endl;
}

void Log(const std::string s)
{
    Print(TERM__Blue, s);
}

void Error(const std::string s)
{
    Print(TERM__Red, s);
}

void Warning(const std::string s)
{
    Print(TERM__Yellow, s);
}