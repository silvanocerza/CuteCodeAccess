#pragma once

#include "Runtime/XmlParser/Public/FastXml.h"

/// Callback used when parsing .vcxproj file
class FCuteCodeVCProjXmlCallback : public IFastXmlCallback
{
public:
	FCuteCodeVCProjXmlCallback() {};
	virtual ~FCuteCodeVCProjXmlCallback() {};

	bool ProcessAttribute(const TCHAR* AttributeName, const TCHAR* AttributeValue) override;
	bool ProcessElement(const TCHAR* ElementName, const TCHAR* ElementData, int32 XmlFileLineNumber) override;
	bool ProcessClose(const TCHAR* Element) override;
	bool ProcessComment(const TCHAR* Comment) override;
	bool ProcessXmlDeclaration(const TCHAR * ElementData, int32 XmlFileLineNumber) override;

	// Getters
	FString GetDefines() const { return Defines; };
	FString GetIncludes() const { return Includes; };
	TArray<FString> GetSources() const { return Sources; };
	TArray<FString> GetHeaders() const { return Headers; };

private:
	FString CurrentElementName = "";

	FString Defines;
	FString Includes;
	TArray<FString> Sources;
	TArray<FString> Headers;
};
