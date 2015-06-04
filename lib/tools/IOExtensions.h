#ifndef IOEXTENSIONS_H
#define IOEXTENSIONS_H

#include <string>
#include <istream>

//The StdExt namespace contains various tools that extend the standard library:
namespace StdExt
{
/******************************************************************************/
/*                          Constants, typedefs, etc.                         */
/******************************************************************************/
   using std::string;
   using std::istream;
   using std::size_t;

/******************************************************************************/
/*                             Manipulator Classes                            */
/******************************************************************************/
/*--------------StreamManipulator Virtual Base Class Declaration--------------*/
   //The StreamManipulator class is an abstract base class used to derive all
   //stream manipulator classes
   class StreamManipulator
   {
   public:
      virtual istream &manipulateStream(istream &s) const = 0; //Performs the stream manipulation task
   }; //class StreamManipulator

/*-------------------------SkipChars Class Declaration------------------------*/
   //The SkipChars class is used to skip over unwanted characters found at the
   //current position of an istream object
   class SkipChars: public StdExt::StreamManipulator
   {
   private:
      static const int MAX_LIST_SIZE = 10; //Maximum size for 'skipList'
      char skipList[MAX_LIST_SIZE + 1]; //List of characters to skip over

      bool IsUnwanted(char c) const; //Returns true if 'c' is part of 'skipList'
      void setSkipList(const char *const skipList); //Sets up 'skipList

   public:
      SkipChars() {setSkipList(" \t\v\r\n\f");} //Creates an object which skips over whitespaces
      SkipChars(const char *const skipList); //Creates an object which skips over arbitrary characters

      istream &manipulateStream(istream &s) const; //Skips over the unwanted characters in stream 's'
   }; //class SkipChars

/*---------------------SkipCommentLines Class Declaration---------------------*/
   //The SkipCommentLines class is used to skip over all comment lines found at
   //the current position of the provided stream
   class SkipCommentLines: public StdExt::StreamManipulator
   {
   private:
      char *commentToken; //The token used to identify a comment (ex: #, //, !, ...)
      char *nextChars; //Buffer storing the next few characters of the stream
      size_t bufLength; //length of buffers (= sizeof(nextChars) =  sizeof(commentToken))

      SkipCommentLines(); //Forbidden
      SkipCommentLines(const SkipCommentLines &src); //Forbidden
      SkipCommentLines &operator=(const SkipCommentLines &src); //Forbidden

   public:
      SkipCommentLines(const char *const commentToken); //Creates an object which skips over the lines starting with the string 'commentToken'
      ~SkipCommentLines(); //Performs necessary clean-up

      istream &manipulateStream(istream &s) const; //Skips over all the comment lines found at the current position of the provided stream
   }; //class SkipCommentLines

/*-------------------------SkipLines Class Declaration------------------------*/
   //The SkipLines class is used to skip over unwanted characters found at the
   //current position of an istream object
   class SkipLines: public StdExt::StreamManipulator
   {
   private:
      int nLines; //Number of lines to skip over

      SkipLines(); //Forbidden

   public:
      SkipLines(int nLines): nLines(nLines) {} //Creates an object which skips over 'nLines' lines

      istream &manipulateStream(istream &s) const; //Skips over the 'nLines' following lines of the provided stream
   }; //class SkipLines

/******************************************************************************/
/*                                  Operators                                 */
/******************************************************************************/
   inline istream &operator>>(istream &s, const StreamManipulator &m) //Executes a stream manipulator
      {return m.manipulateStream(s);}

/******************************************************************************/
/*                                  Functions                                 */
/******************************************************************************/
   string GetLine(istream &s); //Returns the next line of the provided stream
   bool IsNext(istream &s, const string& token); //Peeks to see if next token is sought one (restores state)
   //****IsNext is not yet tested!!!!!****
   inline istream &SkipLine(istream &s) {return s >> SkipLines(1);} //Skips over the next line of the provided stream
   istream &SkipWhiteSpaces(istream &s); //Skips over all the white spaces found at the current position of the provided stream
} //namespace StdExt
#endif //#ifndef IOEXTENSIONS_H

