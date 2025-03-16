#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Module_UI_HUD.generated.h"

//------------------------------------------------------------------------------------------------------------
UCLASS() class MODULE_UI_API UModule_UI_HUD : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	
};
//------------------------------------------------------------------------------------------------------------
