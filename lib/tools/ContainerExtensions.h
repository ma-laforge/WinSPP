#ifndef CONTAINEREXTENSIONS_H
#define CONTAINEREXTENSIONS_H

#include <set>
#include <string>

//The StdExt namespace contains various tools that extend the standard library:
namespace StdExt
{
/******************************************************************************/
/*                          Constants, typedefs, etc.                         */
/******************************************************************************/

/******************************************************************************/
/*                                  Functions                                 */
/******************************************************************************/

/******************************************************************************/
/*                                  Templates                                 */
/******************************************************************************/
   template<class Key, class Compare, class Allocator>
   inline bool IsElementOf(const std::set<Key, Compare, Allocator> &src, const Key &elem)
      {return src.find(elem) != src.end();}
} //namespace StdExt
#endif //#ifndef CONTAINEREXTENSIONS_H

