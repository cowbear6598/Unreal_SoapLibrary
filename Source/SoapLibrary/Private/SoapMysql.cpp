#include "SoapMysql.h"
#include "HttpModule.h"

FString Url = "http://127.0.0.1:8080";

TSharedPtr<IHttpRequest> USoapMysql::HttpRequestPost(const FString Api, int Index, const TSharedPtr<FJsonObject>& JsonObject)
{
    TSharedPtr<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();

    HttpRequest->SetURL(Url + Api);
    HttpRequest->SetVerb("POST");
    HttpRequest->SetHeader("Content-Type","application/json");

    FString PostContent;
        
    const TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&PostContent);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(),JsonWriter);
    
    HttpRequest->SetContentAsString(PostContent);
    
    return HttpRequest;
}

TSharedPtr<IHttpRequest> USoapMysql::HttpRequestGet(const FString Api, int Index)
{
    TSharedPtr<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();

    HttpRequest->SetVerb("GET");
    HttpRequest->SetURL(Url + Api);
    
    return HttpRequest;
}

void USoapMysql::SetUrl(const FString Domain)
{
    Url = "http://" + Domain + "/";
}