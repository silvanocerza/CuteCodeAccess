#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "Runtime/XmlParser/Public/FastXml.h"

class FCuteCodeFastXmlCallback : public IFastXmlCallback
{
public:
	FCuteCodeFastXmlCallback() {};
	virtual ~FCuteCodeFastXmlCallback() {};

	bool ProcessAttribute(const TCHAR* AttributeName, const TCHAR* AttributeValue) override;
	bool ProcessElement(const TCHAR* ElementName, const TCHAR* ElementData, int32 XmlFileLineNumber) override;
	bool ProcessClose(const TCHAR* Element) override;
	bool ProcessComment(const TCHAR* Comment) override;
	bool ProcessXmlDeclaration(const TCHAR * ElementData, int32 XmlFileLineNumber) override;

	// Getters
	FString GetDefines() const { return Defines; };
	FString GetIncludes() const { return Includes;  };
	TArray<FString> GetSources() const { return Sources; };
	TArray<FString> GetHeaders() const { return Headers; };

private:
	FString CurrentElementName = "";

	FString Defines;
	FString Includes;
	TArray<FString> Sources;
	TArray<FString> Headers;
};

class FCuteCodeInitializer
{
public:
	FCuteCodeInitializer(const FString& SolutionPath, const FString& ProjectName);
	~FCuteCodeInitializer();

	void Run() const;

private:
	void CreateProFile() const;

	FString SolutionPath;
	FString ProjectName;

	FCuteCodeFastXmlCallback* FastXmlCallback;

	void AppendFileNames(TArray<FString>& Array, const TArray<FString>& ToAppend) const;
};
