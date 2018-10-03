// Copyright 2018 Silvano Cerza All Rights Reserved.

#pragma once

#include "Runtime/XmlParser/Public/FastXml.h"

/// Callback used when parsing .vcxproj file
class FCuteCodeVCProjXmlCallback : public IFastXmlCallback
{
public:
    FCuteCodeVCProjXmlCallback();
    virtual ~FCuteCodeVCProjXmlCallback() {}

    bool ProcessAttribute(const TCHAR* AttributeName, const TCHAR* AttributeValue) override;
    bool ProcessElement(const TCHAR* ElementName, const TCHAR* ElementData, int32 XmlFileLineNumber) override;
    bool ProcessClose(const TCHAR* Element) override;
    bool ProcessComment(const TCHAR* Comment) override;
    bool ProcessXmlDeclaration(const TCHAR * ElementData, int32 XmlFileLineNumber) override;

    // Getters
    FString GetDefines() const { return Defines; }
    FString GetIncludes() const { return Includes; }

private:
    FString CurrentElementName;

    FString Defines;
    FString Includes;
};

/// Callback used when parsing Qt Creator profiles.xml
class FCuteCodeProfilesXmlCallback : public IFastXmlCallback
{
public:
    FCuteCodeProfilesXmlCallback();
    virtual ~FCuteCodeProfilesXmlCallback() {}

    bool ProcessAttribute(const TCHAR* AttributeName, const TCHAR* AttributeValue) override;
    bool ProcessElement(const TCHAR* ElementName, const TCHAR* ElementData, int32 XmlFileLineNumber) override;
    bool ProcessClose(const TCHAR* Element) override;
    bool ProcessComment(const TCHAR* Comment) override;
    bool ProcessXmlDeclaration(const TCHAR * ElementData, int32 XmlFileLineNumber) override;

    FString GetKitUuid() const { return KitUuid; }

private:
    FString CurrentElementData;
    FString CurrentElement;

    FString KitUuid;
    FString ExpectedKitName;
};
