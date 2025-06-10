#pragma once

#include "GameFramework/HUD.h"
#include "Module_UI_HUD.generated.h"

//-----------------------------------------------------------------------------------------------------------
UCLASS(meta = (DisableNativeTick) ) class MODULE_UI_API AAModule_UI_HUD : public AHUD
{
	GENERATED_BODY()

public:
	AAModule_UI_HUD();

	virtual void BeginPlay();

private:
	void Inventory_Load_Stream();

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = true) ) void Inventory_Handle(const int equipment, const int slots);
	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = true) ) void Inventory_Toggle();  // If inventory will be large better destroy?

	UPROPERTY(EditDefaultsOnly, Category = "UI", meta = (AllowPrivateAccess = true) ) TSubclassOf<UUserWidget> Menu_Main;
	UPROPERTY(EditDefaultsOnly, Category = "UI", meta = (AllowPrivateAccess = true) ) TSoftClassPtr<UUserWidget> Inventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = true) ) UUserWidget *Menu_Main_Widget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = true) ) UUserWidget *Inventory_Widget;  // Change to TObjPtr

};
//------------------------------------------------------------------------------------------------------------
