#pragma once

#include "Blueprint/DragDropOperation.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "Module_UI_Inventory.generated.h"

//------------------------------------------------------------------------------------------------------------
//#include "MSkillTree.generated.h"
//
//USTRUCT(BlueprintType) struct MYINVENTORY_API FInventoryItem
//{
//    GENERATED_BODY()
//
//public:
//};
//------------------------------------------------------------------------------------------------------------
UENUM(BlueprintType) enum class ESlot_Type : uint8
{
	EST_Equipment,
	EST_Helmet = 0,
	EST_Armor,
	EST_Pants,
	EST_Gloves,
	EST_Boots,
	EST_Coat,
	EST_Talisman_L,
	EST_Talisman_R,
	EST_Weapon_L,
	EST_Accessory,
	EST_0,
	EST_1,
	EST_2,
	EST_3,
	EST_4,
	EST_5,
	EST_6,
	EST_7,
	EST_8,
	EST_Weapon_R,
	EST_Last
};
//------------------------------------------------------------------------------------------------------------
class UImage;
class UUniformGridPanel;
class USizeBox;
class UBorder;
//------------------------------------------------------------------------------------------------------------
UCLASS(meta = (DisableNativeTick) ) class UADrag_Drop_Operation : public UDragDropOperation
{// This specialized Class enables us to pass through information as part of our drag and drop action.

	GENERATED_BODY()

public:
	FVector2D Drag_Offset;  // offsets the location from where we start dragging and dropping the widget.
	UUserWidget *Widget_Reference;  // used to hold the UMG Widget that we want to drag around on screen.
};
//-----------------------------------------------------------------------------------------------------------
UCLASS(meta = (DisableNativeTick) ) class UAModule_UI_Dragged_Widget : public UUserWidget
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
	virtual FReply NativeOnMouseButtonDown(const FGeometry &in_geometry, const FPointerEvent &in_mouse_event);
	virtual void NativeOnDragDetected(const FGeometry &in_geometry, const FPointerEvent &in_mouse_event, UDragDropOperation *&out_operation);
	virtual bool NativeOnDrop(const FGeometry &in_geometry, const FDragDropEvent &in_drag_drop_event, UDragDropOperation *in_operation);  // What to do while on drop widget
	
	bool Is_Empty;
	FVector2D Drag_Offset;
	TObjectPtr<UADrag_Drop_Operation> Drag_Drop_Operation;
	
	UFUNCTION(BlueprintCallable, Category = "Init") void Init(const ESlot_Type slot_type, UTexture2D *texture2d, const FVector base_color);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget | Type") ESlot_Type Slot_Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget | Type") UTexture2D *Textures_Defaults;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget | Components", meta = (BindWidget) ) TObjectPtr<UImage> Image_Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget | Class", meta = (ExposeOnSpawn = true) ) TSubclassOf<UAModule_UI_Dragged_Widget> Drag_Widget_Class;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget | Class", meta = (ExposeOnSpawn = true) ) TSubclassOf<UADrag_Drop_Operation> Drag_Drop_O_Class;
};
//-----------------------------------------------------------------------------------------------------------
UCLASS() class MODULE_UI_API UModule_UI_Inventory : public UUserWidget
{
	GENERATED_BODY()

public:
	TArray<UAModule_UI_Inventory_Slot *> Slot_Add(const int32 column, const int32 type_index, TArray<UTexture2D *> &textures);

	UFUNCTION(BlueprintCallable, Category = "Init") void Create_Slots_At_Column(const ESlot_Type type, const int column, TArray<UAModule_UI_Inventory_Slot *> &array_slot);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget | Type") TArray<UTexture2D *> Textures_Defaults;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget | Type") TArray<UTexture2D *> Textures_Equipment;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget | Type") TArray<UTexture2D *> Textures_Accessory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget | Type", meta = (BindWidget) ) TObjectPtr<UUniformGridPanel> Uniform_Grid_Panel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget | Type", meta = (ToolTip = "A") ) TSubclassOf<UUserWidget> Slot_Template;
};
//-----------------------------------------------------------------------------------------------------------
