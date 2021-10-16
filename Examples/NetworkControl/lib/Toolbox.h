#pragma once
#include <vector>
#include <string>
#include "VectorString.h"
#include "stdint_ex.h"

namespace Toolbox
{
   VectorString ToVectorString(std::string reply, char splitCharacter = '\n');
   void PrintAllLines(VectorString reply);
   void PrintAllLines(std::string reply);
   std::string GetFirstWord(std::string &text);
   std::string DescribeOutcome(bool outcome);
   std::string GetParameter(const std::string &parameter, const VectorString &parameterList);
   s32 ToInteger(std::string parameter, s32 defaultOnConversionFailure = 0); 
   u32 ToUnsignedInteger(std::string parameter, u32 defaultOnConversionFailure = 0);
   bool Contains(const std::string &text, const char *searchString);
   std::string GetLineContaining(VectorString lines, const char *searchPhrase);
   VectorString ExtractItemsNotInReferenceList(const VectorString &referenceList, const VectorString &testList);
}
