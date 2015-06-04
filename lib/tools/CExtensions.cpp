#include <tools/CExtensions.h>

//The StdExt namespace contains various tools that extend the standard library:
namespace StdExt
{
/******************************************************************************/
/*                          Constants, typedefs, etc.                         */
/******************************************************************************/
   const char SWITCH_CHAR = '-'; //Identifies a command-line argument as a switch

/******************************************************************************/
/*                                  Functions                                 */
/******************************************************************************/
   //GetArguments() - Converts the main()'s list of arguments into a list of
   //strings (more C++ friendly):
   vector<string> GetArguments(int argc, const char *const argv[], bool includeSwitches)
   {
      vector<string> arguments; //Return value

      for (int i=0; i<argc; ++i)
      {
         //Only add argument if either it is not a switch or if switches are to
         //be included:
         if (argv[i][0]!=SWITCH_CHAR || includeSwitches)
            arguments.push_back(argv[i]);
      }

      return arguments;
   } //vector<string> GetArguments(int argc, const char *const argv[], bool includeSwitches)

   //GetSwitches() - Extracts the switches from main()'s list of arguments (more
   //C++ friendly):
   set<string> GetSwitches(int argc, const char *const argv[])
   {
      set<string> switches; //Return value

      for (int i=1; i<argc; ++i) //First element (program path) can be ignored
      {
         //Add each detected switch switch or if switches are to
         //be included:
         if (argv[i][0]==SWITCH_CHAR)
            switches.insert(&(argv[i][1]));
      }

      return switches;
   } //set<string> GetSwitches(int argc, const char *const argv[])

} //namespace StdExt
