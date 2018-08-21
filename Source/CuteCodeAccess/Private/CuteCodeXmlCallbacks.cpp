#include "CuteCodeXmlCallbacks.h"
#include "CuteCodeEditorSettings.h"

DEFINE_LOG_CATEGORY_STATIC(LogCuteXmlCallbacks, Log, All);

#define LOCTEXT_NAMESPACE "FCuteCodeXmlCallbacks"

FCuteCodeVCProjXmlCallback::FCuteCodeVCProjXmlCallback()
    : CurrentElementName{""}
{

}

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

FCuteCodeProfilesXmlCallback::FCuteCodeProfilesXmlCallback()
    : CurrentElementData{ "" }
{
    const UCuteCodeEditorSettings* Settings = GetDefault<UCuteCodeEditorSettings>();

    if (Settings)
    {
        ExpectedKitName = Settings->UnrealKitName;
    }
}

bool FCuteCodeProfilesXmlCallback::ProcessAttribute(const TCHAR* AttributeName, const TCHAR* AttributeValue)
{
    if (AttributeName == FString{"key"} && AttributeValue == FString{"PE.Profile.Id"})
    {
        KitUuid = CurrentElementData;
    }
    else if (AttributeName == FString{"key"}
        && AttributeValue == FString{"PE.Profile.Name"}
        && CurrentElementData == ExpectedKitName)
    {
        // We found our kit, no need to parse the rest of the file
        return false;
    }
    return true;
}

bool FCuteCodeProfilesXmlCallback::ProcessElement(const TCHAR* ElementName, const TCHAR* ElementData, int32 XmlFileLineNumber)
{
    CurrentElementData = ElementData;
    CurrentElement = ElementName;
    return true;
}

bool FCuteCodeProfilesXmlCallback::ProcessClose(const TCHAR* Element)
{
    CurrentElementData = "";
    return true;
}

bool FCuteCodeProfilesXmlCallback::ProcessComment(const TCHAR* Comment)
{
    return true;
}

bool FCuteCodeProfilesXmlCallback::ProcessXmlDeclaration(const TCHAR* ElementData, int32 XmlFileLineNumber)
{
    return true;
}

#undef LOCTEXT_NAMESPACE
