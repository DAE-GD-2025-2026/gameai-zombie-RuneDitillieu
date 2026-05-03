// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

AWeapon::AWeapon()
{
}

bool AWeapon::Shoot(ASurvivorPawn const & Survivor, FVector const & Direction, APawn*& HitPawnOut) const
{
	FCollisionQueryParams CollisionParams{};
	CollisionParams.AddIgnoredActor(&Survivor);
	auto const End = Survivor.GetActorLocation() + Direction * 10000.0f;
	if (FHitResult HitResult{}; 
		GetWorld()->LineTraceSingleByChannel(HitResult, Survivor.GetActorLocation(), End, 
			ECC_Pawn, CollisionParams))
	{
		if (APawn* const HitPawn = Cast<APawn>(HitResult.GetActor()); HitPawn)
		{
			HitPawnOut = HitPawn;
			return true;
		}
	}
	return false;
}

