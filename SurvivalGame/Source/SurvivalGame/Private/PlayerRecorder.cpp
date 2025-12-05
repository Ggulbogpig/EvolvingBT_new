// Copyright 2020 Tom Looman, and additional copyright holders as specified in LICENSE.md

#include "PlayerRecorder.h"
#include "GameFramework/Actor.h"
#include "Serialization/JsonSerializer.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

UPlayerRecorder::UPlayerRecorder()
{
	PrimaryComponentTick.bCanEverTick = true;

	// 0.1f = 0.1초마다 Tick 실행
	PrimaryComponentTick.TickInterval = 0.2f;
}

void UPlayerRecorder::BeginPlay()
{
	Super::BeginPlay();
	// 시작 시간 기록
	StartTime = GetWorld()->GetTimeSeconds();
}

void UPlayerRecorder::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 주인(플레이어)이 없으면 중단
	AActor* Owner = GetOwner();
	if (!Owner) return;

	// 1. 현재 상태 캡처
	FRecordData Data;
	Data.Time = GetWorld()->GetTimeSeconds() - StartTime;
	Data.Location = Owner->GetActorLocation();
	Data.Rotation = Owner->GetActorRotation();
	Data.Health = CurrentHealth; // 블루프린트가 업데이트해준 값 사용

	// 2. 배열에 저장
	History.Add(Data);
}

void UPlayerRecorder::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	SaveToJson();
	Super::EndPlay(EndPlayReason);
}

void UPlayerRecorder::SaveToJson()
{
	// JSON 배열 생성
	TArray<TSharedPtr<FJsonValue>> JsonArray;

	for (const FRecordData& Data : History)
	{
		TSharedPtr<FJsonObject> Row = MakeShareable(new FJsonObject);

		Row->SetNumberField("Time", Data.Time);
		Row->SetNumberField("Health", Data.Health);

		// 위치 정보
		TSharedPtr<FJsonObject> Pos = MakeShareable(new FJsonObject);
		Pos->SetNumberField("X", Data.Location.X);
		Pos->SetNumberField("Y", Data.Location.Y);
		Pos->SetNumberField("Z", Data.Location.Z);
		Row->SetObjectField("Position", Pos);

		// 배열에 추가
		JsonArray.Add(MakeShareable(new FJsonValueObject(Row)));
	}

	// 최종 객체
	TSharedPtr<FJsonObject> Root = MakeShareable(new FJsonObject);
	Root->SetArrayField("SessionData", JsonArray);

	// 문자열로 변환
	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(Root.ToSharedRef(), Writer);

	// 파일 저장 경로: 프로젝트폴더/Saved/Recordings/
	FString FileName = FString::Printf(TEXT("Record_%s.json"), *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")));
	FString FilePath = FPaths::ProjectSavedDir() + "Recordings/" + FileName;

	FFileHelper::SaveStringToFile(OutputString, *FilePath);
}

