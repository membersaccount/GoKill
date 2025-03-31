// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAnim.h"
#include "Enemy/Enemy.h"

void UEnemyAnim::AnimNotify_AttackEnd()
{
    AEnemy* enemy = Cast<AEnemy>(TryGetPawnOwner());
    if (enemy != nullptr) {
        enemy->fsm->OnAttackEnd();
    }
}
