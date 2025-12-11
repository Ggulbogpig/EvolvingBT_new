//#pragma once
//
//#include "CoreMinimal.h"
//#include "Components/ActorComponent.h"
//#include "Dom/JsonObject.h"
//#include "PlayerRecorder1.generated.h"
//
//
//USTRUCT(BlueprintType)
//struct FRecordData1
//{
//    GENERATED_BODY()
//
//    UPROPERTY()
//    float Time;
//
//    UPROPERTY()
//    FVector Location;
//
//    UPROPERTY()
//    FRotator Rotation;
//
//    UPROPERTY()
//    float Health;
//
//    UPROPERTY()
//    bool bIsAttacking;
//
//    UPROPERTY()
//    bool bTargetFound;
//
//    UPROPERTY()
//    float DistanceToEnemy;
//
//    UPROPERTY()
//    int32 ItemCount;
//
//    UPROPERTY()
//    bool bSuccess;
//
//    UPROPERTY()
//    FString Action;
//
//    // 7차원 feature vector 1개를 저장하는 2D 배열
//    UPROPERTY()
//    TArray<float> SubRewards;
//
//    UPROPERTY(EditAnywhere, Category = "Recorder")
//    TSubclassOf<AActor> GoalActorClass;
//
//};
//
//
//// ========================
//// Player Recorder Component
//// ========================
//UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
//class SURVIVALGAME_API UPlayerRecorder1 : public UActorComponent
//{
//    GENERATED_BODY()
//
//public:
//    UPlayerRecorder1();
//
//protected:
//    virtual void BeginPlay() override;
//    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
//
//public:
//    virtual void TickComponent(
//        float DeltaTime,
//        ELevelTick TickType,
//        FActorComponentTickFunction* ThisTickFunction
//    ) override;
//
//    // Health 업데이트
//    UFUNCTION(BlueprintCallable, Category = "Recorder")
//    void SetRecordedHealth(float NewHealth)
//    {
//        CurrentHealth = NewHealth;
//    }
//
//    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Recorder")
//    float CurrentHealth = 100.f;
//
//    UFUNCTION(BlueprintCallable, Category = "Recorder")
//    void SetIsAttacking(bool bState) { bIsAttackingFlag = bState; }
//
//    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Recorder")
//    bool bIsAttackingFlag = false;
//
//    UFUNCTION(BlueprintCallable, Category = "Recorder")
//    void SetRecordedAction(const FString& NewAction)
//    {
//        CurrentAction = NewAction;
//    }
//
//    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Recorder")
//    FString CurrentAction = "Idle";
//
//    // Enemy 목록
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recorder")
//    TArray<AActor*> EnemyList;
//
//
//private:
//    // 저장 로그
//    UPROPERTY()
//    TArray<FRecordData1> History;
//
//    float StartTime;
//
//    // Goal Actor 정보
//  
//    FVector GoalLocation = FVector::ZeroVector;
//    float MaxGoalDistance = 1.f;
//
//    void SaveToJson();
//};
//
//

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerRecorder1.generated.h"

class AActor;

// ========================
//  기록용 구조체
// ========================
USTRUCT(BlueprintType)
struct FRecordData1
{
    GENERATED_BODY()

    UPROPERTY()
    float Time = 0.f;

    UPROPERTY()
    FVector Location = FVector::ZeroVector;

    UPROPERTY()
    FRotator Rotation = FRotator::ZeroRotator;

    UPROPERTY()
    float Health = 0.f;

    UPROPERTY()
    bool bIsAttacking = false;

    UPROPERTY()
    bool bTargetFound = false;

    UPROPERTY()
    float DistanceToEnemy = 0.f;

    UPROPERTY()
    int32 ItemCount = 0;

    UPROPERTY()
    bool bSuccess = false;

    UPROPERTY()
    FString Action;

    // 7차원 feature vector
    
    TArray<TArray<float>> SubRewards;
};


// ========================
// Player Recorder Component
// ========================
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SURVIVALGAME_API UPlayerRecorder1 : public UActorComponent
{
    GENERATED_BODY()

public:
    UPlayerRecorder1();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    virtual void TickComponent(
        float DeltaTime,
        ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction
    ) override;

    // Health 업데이트
    UFUNCTION(BlueprintCallable, Category = "Recorder")
    void SetRecordedHealth(float NewHealth)
    {
        CurrentHealth = NewHealth;
    }

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Recorder")
    float CurrentHealth = 100.f;

    UFUNCTION(BlueprintCallable, Category = "Recorder")
    void SetIsAttacking(bool bState) { bIsAttackingFlag = bState; }

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Recorder")
    bool bIsAttackingFlag = false;

    UFUNCTION(BlueprintCallable, Category = "Recorder")
    void SetRecordedAction(const FString& NewAction)
    {
        CurrentAction = NewAction;
    }

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Recorder")
    FString CurrentAction = TEXT("Idle");

    // ItemCount BP에서 넘겨주기
    UFUNCTION(BlueprintCallable, Category = "Recorder")
    void SetItemCount(int32 NewItemCount) { CurrentItemCount = NewItemCount; }

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Recorder")
    int32 CurrentItemCount = 0;

    // Enemy 목록
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recorder")
    TArray<AActor*> EnemyList;


private:
    // 저장 로그
    UPROPERTY()
    TArray<FRecordData1> History;

    float StartTime = 0.f;

    // Goal Actor 정보
    UPROPERTY()
    AActor* GoalActor = nullptr;

    FVector GoalLocation = FVector::ZeroVector;
    float MaxGoalDistance = 1.f;

    void SaveToJson();
};
