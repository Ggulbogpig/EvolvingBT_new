// Copyright 2020 Tom Looman, and additional copyright holders as specified in LICENSE.md

#include "PlayerRecorder.h"
#include "GameFramework/Actor.h"
#include "Serialization/JsonSerializer.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

#include "BehaviorTree/BTNode.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


UPlayerRecorder::UPlayerRecorder()
{
	PrimaryComponentTick.bCanEverTick = true;

<<<<<<< Updated upstream
	// 0.1f = 0.1�ʸ��� Tick ����
	PrimaryComponentTick.TickInterval = 0.2f;
=======
	// 0.1f = 0.1초마다 Tick 실행
	PrimaryComponentTick.TickInterval = 0.5f;

	LastSeenPosition = FVector::ZeroVector;
	LastSeenTime = -1.0f;
	
>>>>>>> Stashed changes
}




void UPlayerRecorder::BeginPlay()
{
	Super::BeginPlay();
	// ���� �ð� ���
	StartTime = GetWorld()->GetTimeSeconds();

    for (TActorIterator<ACharacter> It(GetWorld()); It; ++It)
    {
        ACharacter* Char = *It;

        // 플레이어 자신 제외
        if (Char == Cast<ACharacter>(GetOwner())) continue;

        EnemyList.Add(Char);
    }




//void UPlayerRecorder::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// 플레이어(PlayerRecorder 컴포넌트의 주인)가 없으면 중단
//	AActor* Owner = GetOwner();
//	if (!Owner) return;
//
//	bool bCheckTargetFound = false; // TargetFound 임시 저장 변수
//	FVector LastSeenPosition;
//	float LastSeenTime;
//
//	// 1. 현재 상태 캡처
//	FRecordData Data;
//	Data.Time = GetWorld()->GetTimeSeconds() - StartTime;
//	Data.Location = Owner->GetActorLocation();
//	Data.Rotation = Owner->GetActorRotation();
//	Data.Health = CurrentHealth; // 블루프린트가 업데이트해준 값 사용
//
//	// --- 적 / 플레이어 위치 관련 ---
//	FVector PlayerLoc = Data.Location;
//	FVector EnemyLoc = FVector::ZeroVector;
//
//	// 고도 차이 (플레이어 기준: 양수면 내가 더 높음)
//	float HeightDiff = 0.0f;
//	if (TargetEnemy)
//	{
//		HeightDiff = PlayerLoc.Z - EnemyLoc.Z;
//	}
//
//	// ==== (1) 적과의 거리 ====
//	if (TargetEnemy)
//	{
//		Data.DistanceToEnemy = FVector::Dist(
//			Owner->GetActorLocation(),
//			TargetEnemy->GetActorLocation()
//		);
//
//		// TargetEnemy를 바라보는지 확인
//		FVector StartLocation;
//		FRotator LookRotation;
//
//		APawn* OwnerPawn = Cast<APawn>(Owner);
//		if (OwnerPawn && OwnerPawn->GetController())
//		{
//			OwnerPawn->GetController()->GetPlayerViewPoint(StartLocation, LookRotation);
//		}
//		else
//		{
//			StartLocation = Owner->GetActorLocation() + FVector(0, 0, 50.f);
//			LookRotation = Owner->GetActorRotation();
//		}
//
//		// 10미터(1000.0f) 앞까지 검사
//		FVector EndLocation = StartLocation + (LookRotation.Vector() * 2000.0f);
//
//		FHitResult HitResult;
//		FCollisionQueryParams QueryParams;
//		QueryParams.AddIgnoredActor(Owner);
//
//		// 레이 발사
//		bool bHit = GetWorld()->LineTraceSingleByChannel(
//			HitResult,
//			StartLocation,
//			EndLocation,
//			ECC_Visibility,
//			QueryParams);
//
//		// 맞은 물체가 타겟과 같은지 확인
//		if (bHit && HitResult.GetActor() == TargetEnemy)
//		{
//			bCheckTargetFound = true;
//			// 지금 적을 보고 있다면, 마지막 위치/시간 갱신
//			LastSeenPosition = EnemyLoc;
//			LastSeenTime = CurrentTime;
//		}
//
//		// 마지막 본 위치와의 거리 / 시간
//		float DistFromLastSeen = 0.0f;
//		float TimeSinceSeen = 9999.0f;  // 아직 못 봤다면 큰 값으로
//
//		if (LastSeenTime > 0.0f)
//		{
//			DistFromLastSeen = FVector::Dist(PlayerLoc, LastSeenPosition);
//			TimeSinceSeen = CurrentTime - LastSeenTime;
//		}
//
//		// 정규화 (필요한 범위에 맞게 마음대로 조정)
//		float DistLastNorm = FMath::Clamp(DistFromLastSeen / 1500.0f, 0.0f, 1.0f);
//		float TimeSeenNorm = FMath::Clamp(TimeSinceSeen / 5.0f, 0.0f, 1.0f);
//		float HeightNorm = FMath::Clamp(HeightDiff / 300.0f, -1.0f, 1.0f);
//
//
//		// [디버그 활성화]
//		//DrawDebugLine(GetWorld(), StartLocation, bHit ? HitResult.Location : EndLocation, bHit ? FColor::Green : FColor::Red, false, 0.2f);
//	}
//	else
//	{
//		Data.DistanceToEnemy = -1.f;  // 적이 없으면 -1
//	}
//
//	// ==== (2) 공격 중인지 여부 ====
//	Data.bIsAttacking = bIsAttackingFlag;
//
//	// TargetFound 레이캐스트 결과 저장
//	Data.bTargetFound = bCheckTargetFound;
//
//	// 2. GetCurrentAction() Call on Playerpawn
//	// FString GetCurrentAction() (No input parameter, FString return)
//	FString ActionValue;
//	// 없으면 빈 문자열로 남김
//	//Data.Action =  "Idle" /*8GetNodeName()*/;
//	//실제 행동 이름 가져오기
//	Data.Action = CurrentAction;
//
//
//
//
//float Dist = Data.DistanceToEnemy;
//float Found = Data.bTargetFound ? 1.0f : 0.0f;
//float IsAtk = Data.bIsAttacking ? 1.0f : 0.0f;
//float HP = Data.Health;
//
//float Close = FMath::Clamp((600 - Dist) / 600.f, 0.f, 1.f);
//float Far = FMath::Clamp(Dist / 1200.f, 0.f, 1.f);
//float LowHP = FMath::Clamp((50 - HP) / 50.f, 0.f, 1.f);
//
//
//
//
//TArray<FString> Actions = { "Chase", "Patrol", "Idle", "Attack" };
//
//TArray<TArray<float>> SubRewardList;
//
//for (auto& Act : Actions)
//{
//	float d = 0.f, s = 0.f, safe = 0.f;
//
//	if (Act == "Chase")
//	{
//		d = Found * Close * 2 + (1 - Found) * TimeSinceSeenNorm * 0.5f;
//		s = -Close;
//		safe = -0.5f * Close;
//	}
//	else if (Act == "Patrol")
//	{
//		d = (1 - Found) * (0.3f + DistLastNorm * 0.4f);
//		s = 0.5f + 0.3f * Far;
//		safe = 0.5f;
//	}
//	else if (Act == "Idle")
//	{
//		d = -Found * 0.5f;
//		s = LowHP * 2.0f;
//		safe = 0.2f + 0.3f * Far + HeightNorm * 0.2f;
//	}
//	else if (Act == "Attack")
//	{
//		d = Found * Close * 3 - (1 - Found) * 2;
//		s = -Close - LowHP;
//		safe = -1.5f * Close;
//	}
//
//	TArray<float> vec;
//	vec.Add(d * 5);
//	vec.Add(s * 5);
//	vec.Add(safe * 5);
//	SubRewardList.Add(vec);
//}
//
//Data.SubRewards = SubRewardList;
//
//
//	//==============================================
//	
//	// 2. 배열에 저장
//	History.Add(Data);

}
//void UPlayerRecorder::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//    AActor* Owner = GetOwner();
//    if (!Owner) return;
//
//    const float CurrentTime = GetWorld()->GetTimeSeconds();
//
//    bool bCheckTargetFound = false;
//
//    // 1. 현재 상태 캡처
//    FRecordData Data;
//    Data.Time = CurrentTime - StartTime;
//    Data.Location = Owner->GetActorLocation();
//    Data.Rotation = Owner->GetActorRotation();
//    Data.Health = CurrentHealth;
//
//    // 위치들
//    FVector PlayerLoc = Data.Location;
//    FVector EnemyLoc = FVector::ZeroVector;
//
//    if (IsValid(TargetEnemy))
//    {
//        EnemyLoc = TargetEnemy->GetActorLocation();
//    }
//
//    // 고도 차이
//    float HeightDiff = 0.0f;
//    if (IsValid(TargetEnemy))
//    {
//        HeightDiff = PlayerLoc.Z - EnemyLoc.Z;
//    }
//
//    // ==== (1) 적과의 거리 ====
//    if (IsValid(TargetEnemy))
//    {
//        Data.DistanceToEnemy = FVector::Dist(PlayerLoc, EnemyLoc);
//
//        // 시야 체크
//        FVector StartLocation;
//        FRotator LookRotation;
//
//        APawn* OwnerPawn = Cast<APawn>(Owner);
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
//        FVector EndLocation = StartLocation + (LookRotation.Vector() * 2000.0f);
//
//        FHitResult HitResult;
//        FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(PlayerViewTrace), false);
//        QueryParams.AddIgnoredActor(Owner);
//
//        bool bHit = GetWorld()->LineTraceSingleByChannel(
//            HitResult,
//            StartLocation,
//            EndLocation,
//            ECC_Visibility,
//            QueryParams
//        );
//
//        if (bHit && HitResult.GetActor() == TargetEnemy)
//        {
//            bCheckTargetFound = true;
//
//            // 지금 보고 있으면 마지막 본 위치/시간 갱신
//            LastSeenPosition = EnemyLoc;
//            LastSeenTime = CurrentTime;
//        }
//
//        // 마지막 본 위치와의 거리 / 시간 차이
//        float DistFromLastSeen = 0.0f;
//        float TimeSinceSeen = 9999.0f;
//
//        if (LastSeenTime >= 0.0f)
//        {
//            DistFromLastSeen = FVector::Dist(PlayerLoc, LastSeenPosition);
//            TimeSinceSeen = CurrentTime - LastSeenTime;
//        }
//
//        // 정규화
//        float DistLastNorm = FMath::Clamp(DistFromLastSeen / 1500.0f, 0.0f, 1.0f);
//        float TimeSeenNorm = FMath::Clamp(TimeSinceSeen / 5.0f, 0.0f, 1.0f);
//        float HeightNorm = FMath::Clamp(HeightDiff / 300.0f, -1.0f, 1.0f);
//
//        // ==== (2) 공격 중 여부 / 타겟 찾음 여부 ====
//        Data.bIsAttacking = bIsAttackingFlag;
//        Data.bTargetFound = bCheckTargetFound;
//
//        // 현재 행동 이름
//        Data.Action = CurrentAction;
//
//        // ==== (3) SubReward 계산 ====
//        float Dist = Data.DistanceToEnemy;
//        float Found = Data.bTargetFound ? 1.0f : 0.0f;
//        float IsAtk = Data.bIsAttacking ? 1.0f : 0.0f;
//        float HP = Data.Health;
//
//        float Close = 0.0f;
//        float Far = 0.0f;
//        if (Dist > 0.0f)
//        {
//            Close = FMath::Clamp((600.0f - Dist) / 600.0f, 0.0f, 1.0f);
//            Far = FMath::Clamp(Dist / 1200.0f, 0.0f, 1.0f);
//        }
//        float LowHP = FMath::Clamp((50.0f - HP) / 50.0f, 0.0f, 1.0f);
//
//        TArray<FString> Actions;
//        Actions.Add(TEXT("Chase"));
//        Actions.Add(TEXT("Patrol"));
//        Actions.Add(TEXT("Idle"));
//        Actions.Add(TEXT("Attack"));
//
//        TArray<TArray<float>> SubRewardList;
//
//        for (const FString& Act : Actions)
//        {
//            float d = 0.f, s = 0.f, safe = 0.f;
//
//            if (Act == TEXT("Chase"))
//            {
//                // 적을 보고 있으면 거리가 가까울수록 d↑
//                // 적을 못 보고 있으면, 최근에 봤던 시점에서 오래 지났을수록 "찾으러 가는" 의미로 조금 보상
//                d = Found * Close * 2.0f + (1.0f - Found) * TimeSeenNorm * 0.5f;
//                s = -Close;
//                safe = -0.5f * Close;
//            }
//            else if (Act == TEXT("Patrol"))
//            {
//                // 적을 못 봤을 때 + 마지막 본 위치로부터 거리가 있는 쪽을 탐색
//                d = (1.0f - Found) * (0.3f + DistLastNorm * 0.4f);
//                s = 0.5f + 0.3f * Far;
//                safe = 0.5f;
//            }
//            else if (Act == TEXT("Idle"))
//            {
//                d = -Found * 0.5f;
//                s = LowHP * 2.0f;
//                // 멀수록/높이 차이가 클수록 살짝 더 안전하다고 가정
//                safe = 0.2f + 0.3f * Far + HeightNorm * 0.2f;
//            }
//            else if (Act == TEXT("Attack"))
//            {
// /*               d = Found * Close * 3.0f - (1.0f - Found) * 2.0f;
//                s = -Close - LowHP;
//                safe = -1.5f * Close;*/
//                if (Found > 0.5f)
//                {
//                    // 적을 보고 있음 → 강한 공격 유리함
//                    d = Close * 2.0f;         // 0~2
//                    s = -Close * 0.5f;        // 0~-0.5
//                    safe = -Close * 1.0f;     // 0~-1
//                }
//                else
//                {
//                    // 적을 못 보고 공격 → 약한 패널티지만 너무 강하지 않게
//                    d = -0.2f;
//                    s = -0.2f;
//                    safe = -0.2f;
//                }
//            }
//
//            TArray<float> Vec;
//            Vec.Add(d * 5.0f);    // DamagePotential
//            Vec.Add(s * 5.0f);    // Survivability
//            Vec.Add(safe * 5.0f); // Safety
//
//            SubRewardList.Add(Vec);
//        }
//
//        Data.SubRewards = SubRewardList;
//    }
//    else
//    {
//        // 적이 아예 없을 때 기본값들
//        Data.DistanceToEnemy = -1.0f;
//        Data.bIsAttacking = bIsAttackingFlag;
//        Data.bTargetFound = false;
//        Data.Action = CurrentAction;
//
//        // 필요하면 "적 없음" 상황용 SubRewards도 따로 정의 가능
//    }
//
//    // 4. 히스토리 저장
//    History.Add(Data);
//}

void UPlayerRecorder::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

<<<<<<< Updated upstream
	// ����(�÷��̾�)�� ������ �ߴ�
	AActor* Owner = GetOwner();
	if (!Owner) return;

	// 1. ���� ���� ĸó
	FRecordData Data;
	Data.Time = GetWorld()->GetTimeSeconds() - StartTime;
	Data.Location = Owner->GetActorLocation();
	Data.Rotation = Owner->GetActorRotation();
	Data.Health = CurrentHealth; // �������Ʈ�� ������Ʈ���� �� ���

	// ==== (1) 적과의 거리 ====
	if (TargetEnemy)
	{
		Data.DistanceToEnemy = FVector::Dist(
			Owner->GetActorLocation(),
			TargetEnemy->GetActorLocation()
		);
	}
	else
	{
		Data.DistanceToEnemy = -1.f;  // 적이 없으면 -1
	}
=======
    AActor* Owner = GetOwner();
    if (!Owner) return;

    const float CurrentTime = GetWorld()->GetTimeSeconds();

    // 1. 기본 데이터 설정
    FRecordData Data;
    Data.Time = CurrentTime - StartTime;
    Data.Location = Owner->GetActorLocation();
    Data.Rotation = Owner->GetActorRotation();
    Data.Health = CurrentHealth;

    FVector PlayerLoc = Data.Location;

    // ----------------------------------------------
    // 2. 여러 Enemy 중 "가장 가까운" Enemy 선택
    // ----------------------------------------------
    AActor* BestEnemy = nullptr;
    float BestDist = FLT_MAX;

    for (AActor* Enemy : EnemyList)
    {
        if (!IsValid(Enemy)) continue;

        float Dist = FVector::Dist(PlayerLoc, Enemy->GetActorLocation());
        if (Dist < BestDist)
        {
            BestDist = Dist;
            BestEnemy = Enemy;
        }
    }

    // Enemy가 한 명도 없으면 기록만 하고 종료
    if (!IsValid(BestEnemy))
    {
        Data.DistanceToEnemy = -1.f;
        Data.bTargetFound = false;
        Data.bIsAttacking = bIsAttackingFlag;
        Data.Action = CurrentAction;

        History.Add(Data);
        return;
    }

    // ----------------------------------------------
    // 3. 거리 계산 (대표 Enemy 기준)
    // ----------------------------------------------
    FVector EnemyLoc = BestEnemy->GetActorLocation();
    Data.DistanceToEnemy = FVector::Dist(PlayerLoc, EnemyLoc);

    float HeightDiff = PlayerLoc.Z - EnemyLoc.Z;
>>>>>>> Stashed changes

    // ----------------------------------------------
    // 4. 여러 Enemy 중 하나라도 시야에 보이면 Found=true
    // ----------------------------------------------
    bool bFoundAny = false;

<<<<<<< Updated upstream
=======
    for (AActor* Enemy : EnemyList)
    {
        if (!IsValid(Enemy)) continue;
>>>>>>> Stashed changes

        FVector StartLocation;
        FRotator LookRotation;

        APawn* OwnerPawn = Cast<APawn>(Owner);
        if (OwnerPawn && OwnerPawn->GetController())
        {
            OwnerPawn->GetController()->GetPlayerViewPoint(StartLocation, LookRotation);
        }
        else
        {
            StartLocation = PlayerLoc + FVector(0, 0, 50.f);
            LookRotation = Owner->GetActorRotation();
        }

        FVector EndLocation = StartLocation + LookRotation.Vector() * 2000.f;

<<<<<<< Updated upstream
	float DamagePotential = 0.f;
	float Survivability = 0.f;
	float Safety = 0.f;

	if (TargetEnemy)
	{
		float Dist = Data.DistanceToEnemy;

		float NormDist = 1.0f - FMath::Clamp(Dist / 1000.f, 0.f, 1.f);
		float AttackBonus = bIsAttackingFlag ? 0.5f : 0.f;

		DamagePotential = NormDist + AttackBonus;
		Survivability = FMath::Clamp(Dist / 1000.f, 0.f, 1.f);

		if (Dist < 200.f)       Safety = -1.0f;
		else if (Dist < 400.f)  Safety = -0.5f;
		else                    Safety = 0.2f;
	}

	Data.SubReward.Empty();
	Data.SubReward.Add(DamagePotential);
	Data.SubReward.Add(Survivability);
	Data.SubReward.Add(Safety);

	//==============================================
	// 2. �迭�� ����
	History.Add(Data);


}
=======
        FHitResult Hit;
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(Owner);

        bool bHit = GetWorld()->LineTraceSingleByChannel(
            Hit,
            StartLocation,
            EndLocation,
            ECC_Visibility,
            Params
        );

        if (bHit && Hit.GetActor() == Enemy)
        {
            bFoundAny = true;

            // 마지막 본 Enemy는 대표 Enemy로 고정
            LastSeenPosition = Enemy->GetActorLocation();
            LastSeenTime = CurrentTime;
            break;
        }
    }

    Data.bTargetFound = bFoundAny;

    // ----------------------------------------------
    // 5. 마지막 본 위치/시간 기반 확률 계산
    // ----------------------------------------------
    float DistFromLast = 0.f;
    float TimeSinceSeen = 9999.f;

    if (LastSeenTime >= 0.f)
    {
        DistFromLast = FVector::Dist(PlayerLoc, LastSeenPosition);
        TimeSinceSeen = CurrentTime - LastSeenTime;
    }

    float DistLastNorm = FMath::Clamp(DistFromLast / 1500.f, 0.f, 1.f);
    float TimeSeenNorm = FMath::Clamp(TimeSinceSeen / 5.f, 0.f, 1.f);
    float HeightNorm = FMath::Clamp(HeightDiff / 300.f, -1.f, 1.f);

    // ----------------------------------------------
    // 6. 기본 변수 생성
    // ----------------------------------------------
    float Dist = Data.DistanceToEnemy;
    float Found = Data.bTargetFound ? 1.f : 0.f;
    float HP = Data.Health;

    float Close = FMath::Clamp((600.f - Dist) / 600.f, 0.f, 1.f);
    float Far = FMath::Clamp(Dist / 1200.f, 0.f, 1.f);
    float LowHP = FMath::Clamp((50.f - HP) / 50.f, 0.f, 1.f);

    TArray<FString> Actions = { TEXT("Chase"), TEXT("Patrol"), TEXT("Idle"), TEXT("Attack") };
    TArray<TArray<float>> SubRewards;

    // ----------------------------------------------
    // 7. 행동별 SubReward 계산
    // ----------------------------------------------
    for (const FString& Act : Actions)
    {
        float d = 0, s = 0;

        if (Act == TEXT("Chase"))
        {
            d = Found * Close * 2 + (1 - Found) * TimeSeenNorm * 0.5f;
            s = -Close;
            //safe = -0.5f * Close;
        }
        else if (Act == TEXT("Patrol"))
        {
            d = (1 - Found) * (0.3f + DistLastNorm * 0.4f);
            s = 0.5f + 0.3f * Far;
            //safe = 0.5f;
        }
        else if (Act == TEXT("Idle"))
        {
            d = -Found * 0.5f;
            s = LowHP * 2.0f;
            //safe = 0.2f + 0.3f * Far + HeightNorm * 0.2f;
        }
        else if (Act == TEXT("Attack"))
        {
            // 가까우면 공격이 자연스럽다
            d = Found * (1.5f * Close)         // 적이 보이면 근거리에서 공격은 OK
                + (1 - Found) * (-0.5f);       // 아예 공격 못하게 하지 않음 (페널티 완화)

            // 공격 시 생존성↓, HP 낮으면 위험
            s = -0.3f * Close - 0.3f * LowHP;   // 기존 -1.0f에서 완화

            // 공격은 위험하지만, 너무 큰 페널티는 제거
            //safe = -0.5f * Close;               // 기존 -1.5f에서 3배 완화

        }

        TArray<float> Vec;
        Vec.Add(d * 5);
        Vec.Add(s * 5);
        //Vec.Add(safe * 5);

        SubRewards.Add(Vec);
    }

    Data.SubRewards = SubRewards;
    Data.bIsAttacking = bIsAttackingFlag;
    Data.Action = CurrentAction;

    // 저장
    History.Add(Data);
}

>>>>>>> Stashed changes

void UPlayerRecorder::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	SaveToJson();
	Super::EndPlay(EndPlayReason);
}

void UPlayerRecorder::SaveToJson()
{
<<<<<<< Updated upstream
	// JSON �迭 ����
	TArray<TSharedPtr<FJsonValue>> JsonArray;
=======
	// ---- Root JSON 객체 ----
	TSharedPtr<FJsonObject> Root = MakeShareable(new FJsonObject);

	// (1) 목적 함수 개수 K
	Root->SetNumberField(TEXT("K"), 2);

	// (2) objective_ids
	TArray<TSharedPtr<FJsonValue>> ObjIds;
	ObjIds.Add(MakeShareable(new FJsonValueString(TEXT("DamagePotential"))));
	ObjIds.Add(MakeShareable(new FJsonValueString(TEXT("Survivability"))));
	//ObjIds.Add(MakeShareable(new FJsonValueString(TEXT("Safety"))));
	Root->SetArrayField(TEXT("objective_ids"), ObjIds);


	// (3) steps 배열
	TArray<TSharedPtr<FJsonValue>> StepsArray;

	// 행동 목록 (Tick에서 SubRewards를 계산할 때 사용한 순서와 동일해야 함)
	TArray<FString> Actions = { TEXT("Chase"), TEXT("Patrol"), TEXT("Idle"), TEXT("Attack") };
>>>>>>> Stashed changes

	for (const FRecordData& Data : History)
	{
		TSharedPtr<FJsonObject> Row = MakeShareable(new FJsonObject);

		Row->SetNumberField("Time", Data.Time);
		Row->SetNumberField("Health", Data.Health);

		// ��ġ ����
		TSharedPtr<FJsonObject> Pos = MakeShareable(new FJsonObject);
		Pos->SetNumberField("X", Data.Location.X);
		Pos->SetNumberField("Y", Data.Location.Y);
		Pos->SetNumberField("Z", Data.Location.Z);
		Row->SetObjectField("Position", Pos);

		// DistanceToEnemy
		Row->SetNumberField("DistanceToEnemy", Data.DistanceToEnemy);

		// IsAttacking
		Row->SetBoolField("IsAttacking", Data.bIsAttacking);


		// action 
		Row->SetStringField("Action", Data.Action);

	
		// SubReward 저장
		TArray<TSharedPtr<FJsonValue>> RewardArray;

		for (float v : Data.SubReward)
		{
			RewardArray.Add(MakeShareable(new FJsonValueNumber(v)));
		}

		Row->SetArrayField("SubReward", RewardArray);



		// �迭�� �߰�
		JsonArray.Add(MakeShareable(new FJsonValueObject(Row)));


	}

	// ���� ��ü
	TSharedPtr<FJsonObject> Root = MakeShareable(new FJsonObject);
	Root->SetArrayField("SessionData", JsonArray);

	// ���ڿ��� ��ȯ
	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(Root.ToSharedRef(), Writer);

	// ���� ���� ���: ������Ʈ����/Saved/Recordings/
	FString FileName = FString::Printf(TEXT("Record_%s.json"), *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")));
	FString FilePath = FPaths::ProjectSavedDir() + "Recordings/" + FileName;

	FFileHelper::SaveStringToFile(OutputString, *FilePath);
}

