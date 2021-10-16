#include "Toolbox.h"
#include <iostream>
#include <sstream>
#include <cstdio>
#include <inttypes.h>

using namespace  std;

namespace Toolbox
{
   VectorString ToVectorString(string monolithicString, char splitCharacter)
   {
      VectorString vectorString;
      string line;
      
      for (auto character : monolithicString)
      {
         if (character != splitCharacter)
         {
            line.append(1, character);
         }
         else
         {
            vectorString.push_back(line);
            line.clear();
         }
      }

      if (line.size())  // collect remaining text
      {
         vectorString.push_back(line);
      }
   
      return vectorString;      
   }  
   
   void PrintAllLines(VectorString reply)
   {
      for (auto &line : reply)
      {
         cout << line << endl;
      }
   }
   
   void PrintAllLines(string reply)
   {
      auto lines = ToVectorString(reply);
      PrintAllLines(lines);
   }

   string GetFirstWord(string &text)
   {
      stringstream textStream(text);
      string firstWord;

      if (getline(textStream, firstWord, ' '))
      {
         return firstWord;
      }
      else
      {
         return text;
      }
   }

   string DescribeOutcome(bool outcome)
   {
      return outcome ? "True" : "False";
   }

   string GetParameter(const string &parameterName, const VectorString &parameterList)
   {
      string parameterValue = "";
      auto paramterValueIndex = 1;

      for (auto parameter : parameterList)
      {
         if (parameter.find(parameterName) == string::npos)
         {
            continue;
         }

         auto decomposed = ToVectorString (parameter, '=');
         if (decomposed.size() == 2)
         {
            parameterValue = decomposed[paramterValueIndex];
            break;
         }
      }

      return parameterValue;
   }

   s32 ToInteger(string parameter, s32 defaultOnConversionFailure)
   {
      s32 convertedValue;
      auto converted = sscanf(parameter.c_str(), "%" PRId32, &convertedValue);

      if (converted != 1)
      {
         convertedValue = defaultOnConversionFailure;
      }
      return convertedValue;
   }

   u32 ToUnsignedInteger(string parameter, u32 defaultOnConversionFailure)
   {
      u32 convertedValue;
      auto converted = sscanf(parameter.c_str(), "%" SCNu32, &convertedValue);

      if (converted != 1)
      {
         convertedValue = defaultOnConversionFailure;
      }
      return convertedValue;
   }
   
   string GetLineContaining(VectorString lines, const char *searchPhrase)
   {
      string foundLine = "";
      for (auto line: lines)
      {
         if (Contains(line, searchPhrase))
         {
            foundLine = line;
         }
      }
      return foundLine;
   }
   
   bool Contains(const string &text, const char *searchString)
   {
      if (text.find(searchString) == std::string::npos)
      {
         return false;
      }
      else
      {
         return true;
      }
   }

   bool IsInVectorString(const VectorString &referenceList, const string &testItem)
   {
      for (const auto& referenceItem : referenceList)
      {
         if (testItem == referenceItem)
         {
            return true;
         }
      }
      return false;
   }

   VectorString ExtractItemsNotInReferenceList(const VectorString &referenceList, const VectorString &testList)
   {
      VectorString deltaItems;
      for (const auto& testItem : testList)
      {
         if (IsInVectorString(referenceList, testItem) == false)
         {
            deltaItems.push_back(testItem);
         }
      }
      return deltaItems;
   }
}
