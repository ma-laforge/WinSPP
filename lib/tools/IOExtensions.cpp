#include <sstream>
#include <string.h>
#include <stdexcept>
#include <cstring>
#include <cctype>

#include <tools/IOExtensions.h>

//The StdExt namespace contains various tools that extend the standard library:
namespace StdExt
{
/******************************************************************************/
/*                          Constants, typedefs, etc.                         */
/******************************************************************************/
   using std::ostringstream;

/******************************************************************************/
/*                             Manipulator Classes                            */
/******************************************************************************/
/*----------------SkipChars Class: Member Function Definitions----------------*/
   //IsUnwanted() - Returns true if 'c' is part of 'skipList':
   bool SkipChars::IsUnwanted(char c) const
   {
      int i = 0; //Counter

      while (skipList[i]) //While not at the end of the 'skipList' string
      {
         if (skipList[i] == c) return true;
         ++i;
      }

      return false; //'c' was not found in 'skipList'
   } //bool SkipChars::IsUnwanted(char c) const

   //setSkipList() - Sets up 'skipList
   void SkipChars::setSkipList(const char *const skipList)
   {
      if (strlen(skipList)>MAX_LIST_SIZE) throw std::overflow_error(
         "skipList exceeds 'MAX_LIST_SIZE'");
      strcpy(this->skipList, skipList);
   } //void SkipChars::setSkipList(const char *const skipList)

   //Constructor - Create an object which skips over arbitrary characters:
   SkipChars::SkipChars(const char *const skipList)
      {setSkipList(skipList);}

   //Skip() - Skips over the unwanted characters in stream 's':
   istream &SkipChars::manipulateStream(istream &s) const
   {
      char c;

      while (s.get(c)) //Relies on istream's "operator void*()" member function
      {
         if (!IsUnwanted(c))
         {
            s.putback(c);
            break;
         }
      }

      return s;
   } //istream &SkipChars::manipulateStream(istream &s) const

/*-------------SkipCommentLines Class: Member Function Definitions------------*/
   //Constructor - Creates an object which skips over the lines starting with
   //the string 'commentToken':
   SkipCommentLines::SkipCommentLines(const char *const commentToken):
      commentToken(0), nextChars(0) //Set to 0 in case an exception occurs
   {
      bufLength = strlen(commentToken)+1;
      nextChars = new char[bufLength]; //Allocate space for nextChars
      this->commentToken = new char[bufLength]; //Allocate space for commentToken

      //Copy provided 'commentToken' to space pointed to by local variable:
      strcpy(this->commentToken, commentToken);
   }

   //Destructor - Performs necessary clean-up:
   SkipCommentLines::~SkipCommentLines()
   {
      delete[] nextChars; //Release memory
      delete[] commentToken; //Release memory
   }

   //manipulateStream() - Skips over all the comment lines found at the current
   //position of the provided stream:
   istream &SkipCommentLines::manipulateStream(istream &s) const
   {
      static const int STRINGS_MATCH = 0;
      std::ios::pos_type startPos; //The position in 's' to revert to if no comment lines are found

      startPos = s.tellg(); //Move back here if no comments follow the following whitespaces
      s >> SkipWhiteSpaces;

      //While the next token in 's' is the comment token:
      while (s.get(nextChars, bufLength, '\n')
         && (strcmp(nextChars, commentToken)==STRINGS_MATCH))
      {
         s >> SkipLine; //Skip over this line
         startPos = s.tellg(); //Move back here if no comments follow the following whitespaces
         s >> SkipWhiteSpaces;
      }

      if (s.eof()) s.clear(); //Clear errors if end of file was reached
      return s.seekg(startPos); //Move back to last 'startPos' marker
   } //istream &SkipCommentLines::manipulateStream(istream &s) const

/*----------------SkipLines Class: Member Function Definitions----------------*/
   //manipulateStream() - Skips over the 'nLines' next lines of the provided
   //stream:
   istream &SkipLines::manipulateStream(istream &s) const
   {
      char c;

      for (int i=0; i<nLines; ++i)
      {
         while (s.get(c)) //Relies on istream's "operator void*()" member function
            if (c=='\n') break;
         if (!s) break; //Relies on istream's "operator!()" member function
      }

      return s;
   } //istream &SkipLines::manipulateStream(istream &s) const

/******************************************************************************/
/*                                  Functions                                 */
/******************************************************************************/
   //GetLine() - Returns the next line of the provided stream:
   string GetLine(istream &s)
   {
      ostringstream line; //Return value
      char c;

      while (s.get(c) && c!='\n') line << c;
      //NOTE: Relies on istream's "operator void*()" member function

      return line.str();
   } //string GetLine(istream &s)

   //IsNext() - Peeks to see if next token is sought one (restores state):
   bool IsNext(istream &s, const string& token)
   {
      std::ios::pos_type oldPos; //The position in 's' to revert to after peeking at next token
      string nextToken; //String storing next token

      oldPos = s.tellg(); //Move back here after text is read
      s >> nextToken;
      if (s.eof()) s.clear(); //Clear errors if end of file was reached
      s.seekg(oldPos); //Move back 'oldPos'

      return (nextToken==token);
   }

   //SkipWhiteSpaces() - Skips over all the white spaces found at the current
   //position of the provided stream:
   istream &SkipWhiteSpaces(istream &s)
   {
      char c;

      //Skip over whitespaces:
      while (s.get(c)) //Relies on istream's "operator void*()" member function
      {
         if (!std::isspace(c))
         {
            s.putback(c);
            break;
         }
      }
      
      return s;
   } //istream &SkipWhiteSpaces(istream &s)
} //namespace StdExt
