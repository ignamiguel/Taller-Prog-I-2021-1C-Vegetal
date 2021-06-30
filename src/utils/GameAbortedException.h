#include <exception>

using namespace std;

class GameAborted : public exception
{
  virtual const char *what() const throw()
  {
    return "Game has been aborted by user";
  }
} GameAborted;