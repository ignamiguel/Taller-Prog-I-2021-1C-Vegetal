#include <string>
using namespace std;

class Logger {
    private:
        string path;
        int level;
        void log(string, string);
    public:
        Logger(string, int);
        void logNewGame ();
        void logError(string);
        void logInformation(string);
        void logDebug(string);
};