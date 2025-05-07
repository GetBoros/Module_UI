#pragma once

#include "Blueprint/DragDropOperation.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/HUD.h"

#include "Module_UI_HUD.generated.h"

// UADrag_Drop_Operation
class USizeBox;
class UImage;
class UProgressBar;
class UBorder;
//-----------------------------------------------------------------------------------------------------------  // Module_UI_Inventory_START
UCLASS(meta = (DisableNativeTick) ) class UADrag_Drop_Operation : public UDragDropOperation
{// This specialized Class enables us to pass through information as part of our drag and drop action.

	GENERATED_BODY()

public:
	FVector2D Drag_Offset;  // offsets the location from where we start dragging and dropping the widget.
	UUserWidget *Widget_Reference;  // used to hold the UMG Widget that we want to drag around on screen.
};
//-----------------------------------------------------------------------------------------------------------
UCLASS(meta = (DisableNativeTick) ) class UADrag_Widget : public UUserWidget
{// Dragged Visual Widget, Used to show where we can drag our widget

	GENERATED_BODY()

public:
	virtual void NativeConstruct();
	void Update_State(const UImage *image, const FVector2D desired_size) const;

	UPROPERTY(meta = (BindWidget) ) TObjectPtr<USizeBox> SizeBox_Root;
	UPROPERTY(meta = (BindWidget) ) TObjectPtr<UBorder> Border_Root;
};
//-----------------------------------------------------------------------------------------------------------
UCLASS(meta = (DisableNativeTick) ) class UAModule_UI_Inventory_Slot : public UUserWidget
{
	GENERATED_BODY()

public:
	//virtual void NativeConstruct();
	virtual FReply NativeOnMouseButtonDown(const FGeometry &in_geometry, const FPointerEvent &in_mouse_event);
	virtual void NativeOnDragDetected(const FGeometry &in_geometry, const FPointerEvent &in_mouse_event, UDragDropOperation *&out_operation);
	virtual bool NativeOnDrop(const FGeometry &in_geometry, const FDragDropEvent &in_drag_drop_event, UDragDropOperation *in_operation);  // What to do while on drop widget
	//virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	int Item_Index;  // Store ID Inventory_Item in Player
	FVector2D Drag_Offset;
	TObjectPtr<UADrag_Drop_Operation> Drag_Drop_Operation;

	//UFUNCTION(BlueprintImplementableEvent, Category = "Widget | Event") void Update_State();  // Update Image

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget | Components", meta = (BindWidget)) TObjectPtr<USizeBox> SizeBox_Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget | Components", meta = (BindWidget) ) TObjectPtr<UImage> Image_Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget | Class", meta = (ExposeOnSpawn = true) ) TSubclassOf<UADrag_Widget> Drag_Widget_Class;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget | Class", meta = (ExposeOnSpawn = true) ) TSubclassOf<UADrag_Drop_Operation> Drag_Drop_O_Class;
};
//-----------------------------------------------------------------------------------------------------------  // Module_UI_Inventory_END
UCLASS(meta = (DisableNativeTick) ) class MODULE_UI_API AAModule_UI_HUD : public AHUD
{
	GENERATED_BODY()

public:
	AAModule_UI_HUD();

	virtual void BeginPlay();

	void Inventory_Load_Stream();

	UFUNCTION(BlueprintCallable) void Inventory_Toggle();  // If inventory will be large better destroy?

	UPROPERTY(EditDefaultsOnly, Category = "UI") TSubclassOf<UUserWidget> Menu_Main;
	UPROPERTY(EditDefaultsOnly, Category = "UI") TSoftClassPtr<UUserWidget> Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI") UUserWidget *Menu_Main_Widget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI") UUserWidget *Inventory_Widget;  // Change to TObjPtr
	
};
//------------------------------------------------------------------------------------------------------------
