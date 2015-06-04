#ifndef CEXTENSIONS_H
#define CEXTENSIONS_H

#include <string>
#include <vector>
#include <set>

//The StdExt namespace contains various tools that extend the standard library:
namespace StdExt
{
/******************************************************************************/
/*                          Constants, typedefs, etc.                         */
/******************************************************************************/
   using std::string;
   using std::vector;
   using std::set;

/******************************************************************************/
/*                                  Functions                                 */
/******************************************************************************/
   //Converts the main()'s list of arguments into a list of strings (more C++
   //friendly):
   vector<string> GetArguments(int argc, const char *const argv[], bool includeSwitches = true);

   //Extracts the switches from main()'s list of arguments (more C++ friendly):
   set<string> GetSwitches(int argc, const char *const argv[]);

} //namespace StdExt
#endif //#ifndef CEXTENSIONS_H

