#include "CuteCodeXmlCallbacks.h"

#define LOCTEXT_NAMESPACE "FCuteCodeXmlCallbacks"

bool FCuteCodeVCProjXmlCallback::ProcessAttribute(const TCHAR* AttributeName, const TCHAR* AttributeValue)
{
	if (CurrentElementName == "ClCompile" && AttributeName == FString{ "Include" })
	{
		Sources.Add(AttributeValue);
	}
	else if (CurrentElementName == "ClInclude" && AttributeName == FString{ "Include" })
	{
		Headers.Add(AttributeValue);
	}
	return true;
}

bool FCuteCodeVCProjXmlCallback::ProcessElement(const TCHAR* ElementName, const TCHAR* ElementData, int32 XmlFileLineNumber)
{
	CurrentElementName = ElementName;

	if (CurrentElementName == "NMakePreprocessorDefinitions")
	{
		Defines = ElementData;
	}
	else if (CurrentElementName == "NMakeIncludeSearchPath")
	{
		Includes = ElementData;
	}
	return true;
}

bool FCuteCodeVCProjXmlCallback::ProcessClose(const TCHAR* Element)
{
	CurrentElementName = "";
	return true;
}

bool FCuteCodeVCProjXmlCallback::ProcessComment(const TCHAR* Comment)
{
	return true;
}

bool FCuteCodeVCProjXmlCallback::ProcessXmlDeclaration(const TCHAR * ElementData, int32 XmlFileLineNumber)
{
	return true;
}

#undef LOCTEXT_NAMESPACE
