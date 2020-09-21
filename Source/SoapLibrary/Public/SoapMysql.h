// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Interfaces/IHttpRequest.h"
#include "SoapMysql.generated.h"

UCLASS()
class SOAPLIBRARY_API USoapMysql : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static TSharedPtr<IHttpRequest> HttpRequestPost(const FString Api,int Index,const TSharedPtr<FJsonObject>& JsonObject);
	static TSharedPtr<IHttpRequest> HttpRequestGet(const FString Api,int Index);
	static void SetUrl(const FString Domain);
};
