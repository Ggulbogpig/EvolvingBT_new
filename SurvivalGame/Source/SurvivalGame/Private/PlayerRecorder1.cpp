// Copyright 2020 Tom Looman, and additional copyright holders as specified in LICENSE.md

//#include "PlayerRecorder1.h"
//#include "GameFramework/Actor.h"
//#include "Serialization/JsonSerializer.h"
//#include "Misc/FileHelper.h"
//#include "Misc/Paths.h"
//
//#include "BehaviorTree/BTNode.h"
//#include "Kismet/KismetMathLibrary.h"
//#include "Kismet/KismetSystemLibrary.h"
//#include "DrawDebugHelpers.h" 
//
//
//
//UPlayerRecorder1::UPlayerRecorder1()
//{
//	PrimaryComponentTick.bCanEverTick = true;
//
//	// 0.1f = 0.1초마다 Tick 실행
//	PrimaryComponentTick.TickInterval = 0.5f;
//
//	//LastSeenPosition = FVector::ZeroVector;
//	//LastSeenTime = -1.0f;
//	
//}
//
//
//
//
//void UPlayerRecorder1::BeginPlay()
//{
//    Super::BeginPlay();
//
//    // 시작 시간
//    StartTime = GetWorld()->GetTimeSeconds();
//
//    // -----------------------------
//    // 1. EnemyList 자동 수집
//    // -----------------------------
//    for (TActorIterator<ACharacter> It(GetWorld()); It; ++It)
//    {
//        ACharacter* Char = *It;
//        if (Char == Cast<ACharacter>(GetOwner())) continue;
//
//        EnemyList.Add(Char);
//    }
//
//    // -----------------------------
//    // 2. GoalActor (Arrive BP) 찾기
//    // -----------------------------
//    // 2-1) Arrive BP 클래스 로드
//    UClass* GoalBPClass = StaticLoadClass(
//        AActor::StaticClass(),
//        nullptr,
//        TEXT("Blueprint'/Game/test_map/Arrive.Arrive'") // <<-- 여기 경로 수정!
//    );
//
//    if (!GoalBPClass)
//    {
//        UE_LOG(LogTemp, Error, TEXT(" Goal BP Class Load FAILED! 경로 확인 필요."));
//        return;
//    }
//
//    // 2-2) 맵에서 Arrive BP로 생성된 액터 찾기
//    for (TActorIterator<AActor> It(GetWorld()); It; ++It)
//    {
//        AActor* Actor = *It;
//
//        if (Actor->IsA(GoalBPClass))
//        {
//            GoalActor = Actor;
//            break;
//        }
//    }
//
//    if (!GoalActor)
//    {
//        UE_LOG(LogTemp, Error, TEXT(" GoalActor Not Found in Level!"));
//        return;
//    }
//
//    // -----------------------------
//    // 3. 목표 위치 및 최대 거리 계산
//    // -----------------------------
//    GoalLocation = GoalActor->GetActorLocation();
//    UE_LOG(LogTemp, Warning, TEXT(" Goal Location Found: %s"), *GoalLocation.ToString());
//
//    if (AActor* Owner = GetOwner())
//    {
//        FVector StartLoc = Owner->GetActorLocation();
//        MaxGoalDistance = FVector::Dist(StartLoc, GoalLocation);
//
//        UE_LOG(LogTemp, Warning, TEXT(" MaxGoalDistance = %f"), MaxGoalDistance);
//    }
//}









//void UPlayerRecorder1::TickComponent(
//    float DeltaTime,
//    ELevelTick TickType,
//    FActorComponentTickFunction* ThisTickFunction)
//{
//    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//    AActor* Owner = GetOwner();
//    if (!Owner) return;
//
//    const float CurrentTime = GetWorld()->GetTimeSeconds();
//
//    // -------------------------------------------
//    //   0. PlayerPawn(BP)의 Item 변수 읽기
//    // -------------------------------------------
//    int32 ItemCountValue = 0;
//    if (PlayerPawn_C* PlayerBP = Cast<PlayerPawn_C>(Owner))
//    {
//        ItemCountValue = PlayerBP->Item;        // ← Blueprint 변수
//    }
//
//    // -------------------------------------------
//    //   1. Goal Actor(success 여부)
//    // -------------------------------------------
//    bool bSuccess = false;
//
//    for (TActorIterator<AActor> It(GetWorld()); It; ++It)
//    {
//        AActor* Actor = *It;
//
//        if (Actor->ActorHasTag("Goal"))
//        {
//            GoalActor = Actor;
//            break;
//        }
//    }
//
//    
//
//    // -------------------------------------------
//    //   2. 기록 구조체 생성
//    // -------------------------------------------
//    FRecordData Data;
//
//    Data.Time = CurrentTime - StartTime;
//    Data.Location = Owner->GetActorLocation();
//    Data.Rotation = Owner->GetActorRotation();
//    Data.Health = CurrentHealth;
//    Data.ItemCount = ItemCountValue;
//    Data.bSuccess = bSuccess;
//
//    float DistToGoal = FVector::Dist(Data.Location, GoalLocation);
//    float GoalProgress = 1.f - FMath::Clamp(DistToGoal / MaxGoalDistance, 0.f, 1.f);
//
//    FVector PlayerLoc = Data.Location;
//
//    // -------------------------------------------
//    //   3. 여러 Enemy 중 "가장 가까운 Enemy" 찾기
//    // -------------------------------------------
//    AActor* BestEnemy = nullptr;
//    float BestDist = FLT_MAX;
//
//    for (AActor* Enemy : EnemyList)
//    {
//        if (!IsValid(Enemy)) continue;
//
//        float Dist = FVector::Dist(PlayerLoc, Enemy->GetActorLocation());
//        if (Dist < BestDist)
//        {
//            BestDist = Dist;
//            BestEnemy = Enemy;
//        }
//    }
//
//    // Enemy 없음 → 기본 기록만
//    if (!IsValid(BestEnemy))
//    {
//        Data.DistanceToEnemy = -1.f;
//        Data.bTargetFound = false;
//        Data.bIsAttacking = bIsAttackingFlag;
//        Data.Action = CurrentAction;
//
//        History.Add(Data);
//        return;
//    }
//
//    // -------------------------------------------
//    //   4. 대표 Enemy의 거리 기록
//    // -------------------------------------------
//    Data.DistanceToEnemy = FVector::Dist(PlayerLoc, BestEnemy->GetActorLocation());
//
//    // -------------------------------------------
//    //   5. Enemy 중 하나라도 시야에 있으면 Found=true
//    // -------------------------------------------
//    bool bFoundAny = false;
//
//    for (AActor* Enemy : EnemyList)
//    {
//        if (!IsValid(Enemy)) continue;
//
//        FVector StartLocation;
//        FRotator LookRotation;
//
//        APawn* OwnerPawn = Cast<APawn>(Owner);
//
//        if (OwnerPawn && OwnerPawn->GetController())
//        {
//            OwnerPawn->GetController()->GetPlayerViewPoint(StartLocation, LookRotation);
//        }
//        else
//        {
//            StartLocation = PlayerLoc + FVector(0, 0, 50.f);
//            LookRotation = Owner->GetActorRotation();
//        }
//
//        FVector EndLocation = StartLocation + LookRotation.Vector() * 2000.f;
//
//        FHitResult Hit;
//        FCollisionQueryParams Params;
//        Params.AddIgnoredActor(Owner);
//
//        bool bHit = GetWorld()->LineTraceSingleByChannel(
//            Hit, StartLocation, EndLocation, ECC_Visibility, Params
//        );
//
//        if (bHit && Hit.GetActor() == Enemy)
//        {
//            bFoundAny = true;
//            break;
//        }
//    }
//
//    Data.bTargetFound = bFoundAny;
//    Data.bIsAttacking = bIsAttackingFlag;
//    Data.Action = CurrentAction;
//
//    //----------------------------------------------------------
//// 7차원 Feature Vector 생성
////----------------------------------------------------------
//    TArray<float> Features;
//
//    // (1) Distance Safety: Enemy 멀수록 +
//    float Dist = Data.DistanceToEnemy;
//    float DistanceSafety = (Dist > 0.f) ? FMath::Clamp(Dist / 1500.f, 0.f, 1.f) : 0.f;
//    Features.Add(DistanceSafety);
//
//    // (2) Goal Progress: 목적지와의 거리 감소 =
//    float DistToGoal = FVector::Dist(Data.Location, GoalLocation);
//    float GoalProgress = 1.f - FMath::Clamp(DistToGoal / MaxGoalDistance, 0.f, 1.f);
//    Features.Add(GoalProgress);
//
//    // (3) Visibility Risk: Enemy 보이면 -1
//    float VisibilityRisk = Data.bTargetFound ? -1.f : 0.f;
//    Features.Add(VisibilityRisk);
//
//    // (4) HP Survival: HP 높으면 +
//    float HealthNormalized = FMath::Clamp(Data.Health / 100.f, 0.f, 1.f);
//    Features.Add(HealthNormalized);
//
//    // (5) Item Value: 아이템 개수 정규화
//    float ItemNorm = FMath::Clamp(Data.ItemCount / 10.f, 0.f, 1.f);
//    Features.Add(ItemNorm);
//
//    // (6) Time Efficiency: 시간이 많아질수록 패널티
//    float TimeNorm = FMath::Clamp(Data.Time / 30.f, 0.f, 1.f);
//    float TimeEfficiency = -TimeNorm;
//    Features.Add(TimeEfficiency);
//
//    // (7) Success Bonus: 성공이면 +1, 실패면 -1
//    float SuccessValue = Data.bSuccess ? 1.f : -1.f;
//    Features.Add(SuccessValue);
//
//    //Data.SubRewards.Add(Features);
//    Data.SubRewards = Features;
//
//
//    // -------------------------------------------
//    //   6. 로그 저장
//    // -------------------------------------------
//    History.Add(Data);
//}
//
//
//
//void UPlayerRecorder1::EndPlay(const EEndPlayReason::Type EndPlayReason)
//{
//	SaveToJson();
//	Super::EndPlay(EndPlayReason);
//}
//





//void UPlayerRecorder1::SaveToJson(const FString& FileName)
//{
//    UE_LOG(LogTemp, Warning, TEXT("Saving IRL Log to %s"), *FileName);
//
//    // 루트 JSON 객체
//    TSharedPtr<FJsonObject> RootObject = MakeShared<FJsonObject>();
//
//    // 배열 생성
//    TArray<TSharedPtr<FJsonValue>> TrajectoriesArray;
//
//    for (const FRecordData& Data : History)
//    {
//        TSharedPtr<FJsonObject> StepObject = MakeShared<FJsonObject>();
//
//        // --------------------------
//        // STATE 저장
//        // --------------------------
//        TSharedPtr<FJsonObject> StateObj = MakeShared<FJsonObject>();
//        StateObj->SetNumberField("time", Data.Time);
//        StateObj->SetNumberField("x", Data.Location.X);
//        StateObj->SetNumberField("y", Data.Location.Y);
//        StateObj->SetNumberField("z", Data.Location.Z);
//        StateObj->SetNumberField("hp", Data.Health);
//        StateObj->SetNumberField("item", Data.ItemCount);
//        StateObj->SetNumberField("distance_to_enemy", Data.DistanceToEnemy);
//        StateObj->SetBoolField("found_enemy", Data.bTargetFound);
//        StateObj->SetBoolField("success", Data.bSuccess);
//
//        StepObject->SetObjectField("state", StateObj);
//
//        // --------------------------
//        // ACTION 저장
//        // --------------------------
//        StepObject->SetStringField("action", Data.Action);
//
//        // --------------------------
//        // SUB REWARD VECTOR 저장
//        // --------------------------
//
//        TArray<TSharedPtr<FJsonValue>> RewardArray;
//
//        if (Data.SubRewards.Num() > 0)
//        {
//            const TArray<float>& FeatureVec = Data.SubRewards[0]; // 우리는 1개의 Feature Vector만 기록함
//
//            for (float v : FeatureVec)
//            {
//                RewardArray.Add(MakeShared<FJsonValueNumber>(v));
//            }
//        }
//
//        StepObject->SetArrayField("sub_rewards", RewardArray);
//
//
//
//        // 하나의 Step → 배열에 추가
//        TrajectoriesArray.Add(MakeShared<FJsonValueObject>(StepObject));
//    }
//
//    // Root에 trajectories 넣기
//    RootObject->SetArrayField("trajectories", TrajectoriesArray);
//
//    // --------------------------
//    // JSON 문자열로 변환
//    // --------------------------
//    FString OutputString;
//    TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&OutputString);
//    FJsonSerializer::Serialize(RootObject.ToSharedRef(), JsonWriter);
//
//    // --------------------------
//    // 파일로 저장
//    // --------------------------
//    FString FileName = FString::Printf(TEXT("IRLRecord_%s.json"),
//        *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")));
//
//    FString FilePath = FPaths::ProjectSavedDir() + TEXT("Recordings/") + FileName;
//
//    FFileHelper::SaveStringToFile(OutputString, *FilePath);
//}
//


// Copyright 2020 Tom Looman, and additional copyright holders as specified in LICENSE.md

#include "PlayerRecorder1.h"

#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "EngineUtils.h"

#include "Serialization/JsonSerializer.h"
#include "Dom/JsonObject.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

UPlayerRecorder1::UPlayerRecorder1()
{
    PrimaryComponentTick.bCanEverTick = true;
    // 0.5초마다 Tick
    PrimaryComponentTick.TickInterval = 0.1f;
}

void UPlayerRecorder1::BeginPlay()
{
    Super::BeginPlay();

    StartTime = GetWorld()->GetTimeSeconds();

    // 1. EnemyList 자동 수집 (자기 자신 제외)
    for (TActorIterator<ACharacter> It(GetWorld()); It; ++It)
    {
        ACharacter* Char = *It;
        if (Char == Cast<ACharacter>(GetOwner()))
        {
            continue;
        }
        EnemyList.Add(Char);
    }

    // 2. GoalActor 찾기 (Tag = "Goal" 기준)
    GoalActor = nullptr;

    for (TActorIterator<AActor> It(GetWorld()); It; ++It)
    {
        AActor* Actor = *It;
        if (Actor && Actor->ActorHasTag("Goal"))
        {
            GoalActor = Actor;
            break;
        }
    }

    if (GoalActor)
    {
        GoalLocation = GoalActor->GetActorLocation();

        if (AActor* Owner = GetOwner())
        {
            const FVector StartLoc = Owner->GetActorLocation();
            MaxGoalDistance = FVector::Dist(StartLoc, GoalLocation);
            if (MaxGoalDistance <= KINDA_SMALL_NUMBER)
            {
                MaxGoalDistance = 1.f;
            }
        }
    }

    if (GoalActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("GOAL = %s (%.1f, %.1f, %.1f)"),
            *GoalActor->GetName(),
            GoalLocation.X, GoalLocation.Y, GoalLocation.Z);

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1, 1.0f, FColor::Yellow,
                FString::Printf(
                    TEXT("GOAL = %s (%.1f, %.1f, %.1f)"),
                    *GoalActor->GetName(),
                    GoalLocation.X,
                    GoalLocation.Y,
                    GoalLocation.Z
                )
            );
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("NO GOAL FOUND! TAG 'Goal' missing?"));

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1, 1.0f, FColor::Yellow,
                TEXT("NO GOAL FOUND! TAG 'Goal' missing?")
            );
        }
    }


}

void UPlayerRecorder1::TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    AActor* Owner = GetOwner();
    if (!Owner || !GetWorld())
    {
        return;
    }

    const float CurrentTime = GetWorld()->GetTimeSeconds();

    // -------------------------------
    // 1. 기본 기록 구조체 생성
    // -------------------------------
    FRecordData1 Data;

    Data.Time = CurrentTime - StartTime;
    Data.Location = Owner->GetActorLocation();
    Data.Rotation = Owner->GetActorRotation();
    Data.Health = CurrentHealth;
    Data.ItemCount = CurrentItemCount;  // BP에서 SetItemCount로 넣어준 값
    Data.bSuccess = false;              // 필요하면 BP에서 따로 플래그 넘기도록 설계 가능

    // Goal 관련
    float DistToGoal = 0.f;
    //float GoalProgress = 0.f;

    //if (GoalActor)
    //{
    //    GoalLocation = GoalActor->GetActorLocation();
    //    DistToGoal = FVector::Dist(Data.Location, GoalLocation);

    //    // 안정적인 거리 스코어: 가까울수록 1, 멀면 0
    //    GoalProgress = 1.f / (1.f + DistToGoal / 2);
    //   // GoalProgress = 1.f / (1.f + FMath::Loge(DistToGoal + 1));

    //}

    const FVector PlayerLoc = Data.Location;

    // -------------------------------
    // 2. 여러 Enemy 중 가장 가까운 Enemy 찾기
    // -------------------------------
    AActor* BestEnemy = nullptr;
    float BestDist = FLT_MAX;

    for (AActor* Enemy : EnemyList)
    {
        if (!IsValid(Enemy)) continue;

        const float Dist = FVector::Dist(PlayerLoc, Enemy->GetActorLocation());
        if (Dist < BestDist)
        {
            BestDist = Dist;
            BestEnemy = Enemy;
        }
    }

    // Enemy 없음 → 기본 기록만 하고 종료
    if (!IsValid(BestEnemy))
    {
        Data.DistanceToEnemy = -1.f;
        Data.bTargetFound = false;
        Data.bIsAttacking = bIsAttackingFlag;
        Data.Action = CurrentAction;
        Data.SubRewards.Empty();

        History.Add(Data);
        return;
    }

    // -------------------------------
    // 3. 대표 Enemy 거리 기록
    // -------------------------------
    Data.DistanceToEnemy = FVector::Dist(PlayerLoc, BestEnemy->GetActorLocation());

    // -------------------------------
    // 4. Enemy 시야 체크 (하나라도 보이면 Found = true)
    // -------------------------------
    bool bFoundAny = false;

    for (AActor* Enemy : EnemyList)
    {
        if (!IsValid(Enemy)) continue;

        FVector StartLocation;
        FRotator LookRotation;

        APawn* OwnerPawn = Cast<APawn>(Owner);
        if (OwnerPawn && OwnerPawn->GetController())
        {
            OwnerPawn->GetController()->GetPlayerViewPoint(StartLocation, LookRotation);
        }
        else
        {
            StartLocation = PlayerLoc + FVector(0.f, 0.f, 50.f);
            LookRotation = Owner->GetActorRotation();
        }

        const FVector EndLocation = StartLocation + LookRotation.Vector() * 2000.f;

        FHitResult Hit;
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(Owner);

        const bool bHit = GetWorld()->LineTraceSingleByChannel(
            Hit, StartLocation, EndLocation, ECC_Visibility, Params);

        if (bHit && Hit.GetActor() == Enemy)
        {
            bFoundAny = true;
            break;
        }
    }

    Data.bTargetFound = bFoundAny;
    Data.bIsAttacking = bIsAttackingFlag;
    Data.Action = CurrentAction;

    // -------------------------------
 // 5. 6차원 Feature 생성 (공통 Feature)
 // -------------------------------
    float Dist = Data.DistanceToEnemy;

    // (1) Distance Safety
    float DistanceSafety = (Dist > 0.f)
        ? FMath::Clamp(Dist / 1500.f, 0.f, 1.f)
        : 0.f;

    // (2) Goal Progress
    float GoalProgress = 0.f;
    if (GoalActor)
    {
        float DistGoal = FVector::Dist(PlayerLoc, GoalLocation);
        GoalProgress = 1.f / (1.f + DistGoal / 200.f); // 가까울수록 1
    }

    // (3) Visibility Risk
    float VisibilityRisk = Data.bTargetFound ? 1.f : 0.f;

    // (4) Health Survival (가까우면 더 위험 → 생존력 더 낮게 스케일링)
    float HealthSurvival = (Data.Health / 100.f) * (Dist / (Dist + 300.f));

    // (5) Item Value
    float ItemValue = FMath::Clamp((float)Data.ItemCount / 10.f, 0.f, 1.f);

    // (6) Time Efficiency
    float TimeEfficiency = -FMath::Clamp(Data.Time / 30.f, 0.f, 1.f);


    // -------------------------------
    // 6. 액션 3개 정의
    // -------------------------------
    TArray<FString> Actions = { TEXT("SafeMove"), TEXT("RushMove"), TEXT("FightBack") };
    TArray<TArray<float>> SubRewards;

    DistanceSafety = DistanceSafety * 30.f;
    GoalProgress = GoalProgress * 40.f;
    VisibilityRisk = VisibilityRisk * 50.f;
    HealthSurvival = HealthSurvival * 50.f;
    ItemValue = ItemValue * 20.f;
    TimeEfficiency = TimeEfficiency * 20.f;
    // -------------------------------
    // Action 0: SafeMove
    // -------------------------------
    {
        TArray<float> V;

        V.Add(DistanceSafety * 1.2f);      // 안전 ↑
        V.Add(GoalProgress * 0.8f);        // 목적지 영향 약함
        V.Add(VisibilityRisk * -1.0f);     // 들키면 손해
        V.Add(HealthSurvival * 0.6f);      // HP 중간 영향
        V.Add(ItemValue * 0.5f);           // 아이템 약간 영향
        V.Add(TimeEfficiency * -0.3f);     // 시간은 좀 손해

        SubRewards.Add(V);
    }

    // -------------------------------
    // Action 1: RushMove
    // -------------------------------
    {
        TArray<float> V;

        V.Add(DistanceSafety * 0.4f);      // 안전 거의 없음
        V.Add(GoalProgress * 1.5f);        // 목적지 우선
        V.Add(VisibilityRisk * 1.0f);      // 매우 노출
        V.Add(HealthSurvival * 0.1f);      // HP 영향 거의 없음
        V.Add(ItemValue * 0.1f);           // 아이템 영향 거의 없음
        V.Add(-TimeEfficiency * 1.4f);     // 매우 효율적(+) (TimeEfficiency는 -값)

        SubRewards.Add(V);
    }

    // -------------------------------
    // Action 2: FightBack
    // -------------------------------
    {
        TArray<float> V;

        V.Add(DistanceSafety * 0.1f);                        // 거리 무관
        V.Add(GoalProgress * 0.5f);               // 목적지 영향 없음
        V.Add(VisibilityRisk * 1.5f);      // 매우 들킴
        V.Add(HealthSurvival * 0.7f);      // HP 중요
        V.Add(0.f);                        // 아이템 무관
        V.Add(TimeEfficiency * -0.5f);     // 시간 비효율

        SubRewards.Add(V);
    }

    // 최종 저장
    Data.SubRewards = SubRewards;


    // -------------------------------
    // 6. 로그 저장
    // -------------------------------
    History.Add(Data);
}

void UPlayerRecorder1::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    SaveToJson();
    Super::EndPlay(EndPlayReason);
}


// -------------------------------
// JSON 저장
// -------------------------------
void UPlayerRecorder1::SaveToJson()
{
    if (History.Num() == 0)
    {
        return;
    }

    const FString FileName = FString::Printf(
        TEXT("IRLRecord_%s.json"),
        *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")));

    UE_LOG(LogTemp, Warning, TEXT("Saving IRL IRL Log to %s"), *FileName);

    // 루트 JSON 객체
    TSharedPtr<FJsonObject> RootObject = MakeShared<FJsonObject>();
    TArray<TSharedPtr<FJsonValue>> TrajectoriesArray;

    for (const FRecordData1& Data : History)
    {
        TSharedPtr<FJsonObject> StepObject = MakeShared<FJsonObject>();

        // ----------------------
        // STATE
        // ----------------------
        TSharedPtr<FJsonObject> StateObj = MakeShared<FJsonObject>();
        StateObj->SetNumberField(TEXT("time"), Data.Time);
        StateObj->SetNumberField(TEXT("x"), Data.Location.X);
        StateObj->SetNumberField(TEXT("y"), Data.Location.Y);
        StateObj->SetNumberField(TEXT("z"), Data.Location.Z);
        StateObj->SetNumberField(TEXT("hp"), Data.Health);
        StateObj->SetNumberField(TEXT("item"), Data.ItemCount);
        StateObj->SetNumberField(TEXT("distance_to_enemy"), Data.DistanceToEnemy);
        StateObj->SetBoolField(TEXT("found_enemy"), Data.bTargetFound);
        StateObj->SetBoolField(TEXT("success"), Data.bSuccess);

        StepObject->SetObjectField(TEXT("state"), StateObj);

        // ----------------------
        // ACTION (문자열 그대로 저장)
        // ----------------------
        StepObject->SetStringField(TEXT("action"), Data.Action);

        // ----------------------
        // SUB REWARD (2D 배열 저장)
        // ----------------------
        TArray<TSharedPtr<FJsonValue>> SubRewardsOuter;

        // Data.SubRewards 는 TArray<TArray<float>> 이어야 함!
        for (const TArray<float>& OneActionRewards : Data.SubRewards)
        {
            TArray<TSharedPtr<FJsonValue>> Inner;

            for (float v : OneActionRewards)
            {
                Inner.Add(MakeShared<FJsonValueNumber>(v));
            }

            SubRewardsOuter.Add(
                MakeShared<FJsonValueArray>(Inner)
            );
        }

        StepObject->SetArrayField(TEXT("sub_rewards"), SubRewardsOuter);

        // Step 추가
        TrajectoriesArray.Add(MakeShared<FJsonValueObject>(StepObject));
    }

    RootObject->SetArrayField(TEXT("trajectories"), TrajectoriesArray);

    // ----------------------
    // JSON 문자열 변환
    // ----------------------
    FString OutputString;
    TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&OutputString);
    FJsonSerializer::Serialize(RootObject.ToSharedRef(), JsonWriter);

    // ----------------------
    // 파일 저장
    // ----------------------
    const FString FullPath = FPaths::ProjectSavedDir() / TEXT("Recordings") / FileName;
    FFileHelper::SaveStringToFile(OutputString, *FullPath);

    UE_LOG(LogTemp, Warning, TEXT("IRL JSON Saved: %s"), *FullPath);
}
