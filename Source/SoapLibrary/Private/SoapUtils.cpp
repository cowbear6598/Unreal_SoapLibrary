// Fill out your copyright notice in the Description page of Project Settings.


#include "SoapUtils.h"


#include "HttpModule.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Interfaces/IHttpRequest.h"
#include "JsonUtilities/Public/JsonObjectConverter.h"

UTexture2D* USoapUtils::LoadTexture2DFromDiskFile(const FString& FilePath)
{
    UTexture2D* LoadedT2D = nullptr;

    // 確認檔案是否存在
    if(!FPaths::FileExists(FilePath))
    {
        UE_LOG(LogTemp,Error,TEXT("File not found: %s"), *FilePath);
        return nullptr;
    }

    // 將檔案加載壓縮成 byte
    TArray<uint8> FileData;
					
    if(!FFileHelper::LoadFileToArray(FileData, *FilePath))
    {
        UE_LOG(LogTemp,Error,TEXT("File load fail: %s"), *FilePath);
        return nullptr;
    }
    
    // 使用 Image Wrapper 模組檢測圖片類型
    IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));

    EImageFormat ImageFormat = ImageWrapperModule.DetectImageFormat(FileData.GetData(),FileData.Num());

    if(ImageFormat == EImageFormat::Invalid)
    {
        UE_LOG(LogTemp,Error,TEXT("Unrecognized image format: %s"), *FilePath);
        return nullptr;
    }

    // 創建 Image 包裝
    TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(ImageFormat);

    if(!ImageWrapper.IsValid())
    {
        UE_LOG(LogTemp,Error,TEXT("Create image wrapper fail: %s"), *FilePath);
        return nullptr;
    }

    // 解壓縮 Image 數據
    TArray<uint8> RawData;
    
    if(ImageWrapper->SetCompressed(FileData.GetData(),FileData.Num()) &&
       ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, RawData))
    {
        LoadedT2D = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);
                  
        if (!LoadedT2D) 
        {
            UE_LOG(LogTemp,Error,TEXT("Create texture fail: %s"), *FilePath);
            return nullptr;
        }
                                			
        //Copy!
        void* TextureData = LoadedT2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
        FMemory::Memcpy(TextureData, RawData.GetData(), RawData.Num());
        LoadedT2D->PlatformData->Mips[0].BulkData.Unlock();
                                
        //Update!
        LoadedT2D->UpdateResource();

        UE_LOG(LogTemp,Log,TEXT("Create texture success: %s"), *FilePath);
        return LoadedT2D;
    }
    else
    {
        UE_LOG(LogTemp,Error,TEXT("Decompressed data fail: %s"), *FilePath);
        return nullptr;
    }
}

FString USoapUtils::LoadTextFromDiskFile(const FString& FilePath)
{
    FString FileContent;
    
    if (!FPaths::FileExists(FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("File not found: %s"), *FilePath);
        return "";
    }

    if(FFileHelper::LoadFileToString(FileContent,*FilePath))
    {
        UE_LOG(LogTemp, Log, TEXT("File load success: %s"), *FileContent);
        return FileContent;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("File load fail: %s"), *FilePath);
        return "";
    }
}

bool USoapUtils::StringToJsonObject(FString TextContent,TSharedPtr<FJsonObject>& OutObject)
{
    if(!TextContent.IsEmpty())
    {
        const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(TextContent);
        
        if (FJsonSerializer::Deserialize(JsonReader, OutObject))
        {
            UE_LOG(LogTemp,Log,TEXT("Convert to JsonObject success: %s"), *TextContent);
            return true;
        }
        else
        {
            UE_LOG(LogTemp,Log,TEXT("Text convert to JsonObject fail: %s"), *TextContent);
            return false;
        }
    }
    else
    {
        UE_LOG(LogTemp,Log,TEXT("Text is empty"));
        return false;
    }
}

bool USoapUtils::LoadTextToUStructFromDiskFile(const FString& FilePath,const UStruct* StructType,void* OutStruct)
{
    FString FileContent;

    if (!FPaths::FileExists(FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("File not found: %s"), *FilePath);
        return false;
    }

    if (FFileHelper::LoadFileToString(FileContent, *FilePath))
    {
        UE_LOG(LogTemp, Log, TEXT("File load success: %s"), *FileContent);

        if (!FileContent.IsEmpty())
        {
            TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

            const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(FileContent);

            if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
            {
                if (FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), StructType, OutStruct))
                {
                    UE_LOG(LogTemp, Log, TEXT("Convert to UStruct success: %s"),*FilePath);
                    return true;
                }
                else
                {
                    UE_LOG(LogTemp, Log, TEXT("JsonObject to UStruct fail: %s"),*FilePath);
                    return false;
                }
            }
            else
            {
                UE_LOG(LogTemp, Log, TEXT("JsonObject deserialize failed: %s"), *FileContent);
                return false;
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Text is empty: %s"), *FilePath);
            return false;
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("File load fail: %s"), *FilePath);
        return false;
    }
}
