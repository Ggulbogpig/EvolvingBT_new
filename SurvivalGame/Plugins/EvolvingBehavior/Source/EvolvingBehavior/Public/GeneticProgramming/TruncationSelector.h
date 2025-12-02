#pragma once

#include "CoreMinimal.h"
#include "ParentSelector.h"
#include "TruncationSelector.generated.h"

/**
 * @class UTruncationSelector
 * @brief Selects parents only from the top fraction of the population,
 *        then chooses uniformly at random among that truncated set.
 */
UCLASS()
class EVOLVINGBEHAVIOR_API UTruncationSelector : public UParentSelector
{
    GENERATED_BODY()

public:
    /** 상위 몇 %를 부모 후보 풀로 쓸지 결정 */
    UPROPERTY(EditAnywhere, Category = "EvolvingBehavior", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float TruncationFraction = 0.3f;

    /** 한 번의 선택에서 실제로 반환할 부모 수(기존 TournamentSelector가 2명을 뽑는 것과 비슷하게, 기본 2로 두었다). */
    UPROPERTY(EditAnywhere, Category = "EvolvingBehavior", meta = (ClampMin = "2"))
    int32 NumParentsToSelect = 2;

    void Init(float InTruncationFraction, int32 InNumParents)
    {
        TruncationFraction = InTruncationFraction;
        NumParentsToSelect = InNumParents;
    }

    virtual TArray<FParentFitnessInfo> SelectParents(TArray<FParentFitnessInfo> parentInfo,
        URandomGen* randomGen) override;
};