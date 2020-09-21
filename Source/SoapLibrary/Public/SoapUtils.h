// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SoapUtils.generated.h"

UCLASS()
class SOAPLIBRARY_API USoapUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
		/**
    	 * 讀取本地圖片轉換成可用的 Texture
    	 *
    	 * @param FilePath: 圖片位置
    	 * 
    	 */
    	UFUNCTION(BlueprintCallable,Category="Soap")
    	static UTexture2D* LoadTexture2DFromDiskFile(const FString& FilePath);
    
    	/**
    	 * 從本機讀取文字檔
    	 *
    	 * @param FilePath: 文字檔位置
    	 *
    	 */
    	UFUNCTION(BlueprintCallable,Category="Soap")
    	static FString LoadTextFromDiskFile(const FString& FilePath);
           
		/**
		 * 將文字檔轉成 JsonObject
		 * 
		  */
    	static bool StringToJsonObject(FString TextContent,TSharedPtr<FJsonObject>& OutObject);
        
     	/**
    	 *
    	 * 從本機讀取文字檔直接轉成 UStruct 格式
    	 * 
    	 * @param FilePath: 文字檔位置
    	 * @param StructType: UStruct 格式
    	 * @param OutStruct: 寫入哪一個 UStruct
    	 */
    	static bool LoadTextToUStructFromDiskFile(const FString& FilePath,const UStruct* StructType,void* OutStruct);
};
