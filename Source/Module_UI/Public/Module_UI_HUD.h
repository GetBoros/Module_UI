#pragma once

#include "Blueprint/DragDropOperation.h"
#include "Blueprint/UserWidget.h"

#include "Module_UI_HUD.generated.h"

// UADrag_Drop_Operation
class USizeBox;
class UImage;
class UProgressBar;
class UBorder;
//-----------------------------------------------------------------------------------------------------------
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

	FVector2D Drag_Offset;
	TObjectPtr<UADrag_Drop_Operation> Drag_Drop_Operation;

	//UFUNCTION(BlueprintImplementableEvent, Category = "Widget | Event") void Update_State();  // Update Image

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget | Components", meta = (BindWidget)) TObjectPtr<USizeBox> SizeBox_Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget | Components", meta = (BindWidget) ) TObjectPtr<UImage> Image_Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget | Class", meta = (ExposeOnSpawn = true) ) TSubclassOf<UADrag_Widget> Drag_Widget_Class;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget | Class", meta = (ExposeOnSpawn = true) ) TSubclassOf<UADrag_Drop_Operation> Drag_Drop_O_Class;
};
//-----------------------------------------------------------------------------------------------------------
UCLASS(meta = (DisableNativeTick) ) class MODULE_UI_API UModule_UI_HUD : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnDragDetected(const FGeometry &in_geometry, const FPointerEvent &in_mouse_event, UDragDropOperation *&out_operation);
	virtual FReply NativeOnMouseButtonDown(const FGeometry &in_geometry, const FPointerEvent &in_mouse_event);
	virtual FReply NativeOnMouseMove(const FGeometry &in_geometry, const FPointerEvent &in_mouse_event);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget | Components", meta = (BindWidget) ) TObjectPtr<UImage> Image_Root;
};
//------------------------------------------------------------------------------------------------------------



/*

#pragma once

#include "Personal_Hub_Interaction.h"  // Interface
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "GameFramework/GameModeBase.h"
#include "Personal_Hub_Game_Mode.generated.h"

//-----------------------------------------------------------------------------------------------------------
UENUM(BlueprintType) enum class EHUD_State : uint8
{
	None,
	Is_New_Game,
	Toogle_Menu_Main,
	Toogle_Inventory,
	Toogle_Menu_Option,
	Update_Morph
	//	Main_Menu_HUD UMETA(DisplayName = "Main_Menu_HUD"),
};
//-----------------------------------------------------------------------------------------------------------
UCLASS(meta = (DisableNativeTick)) class UAPersonal_Main_Menu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct();
};
//-----------------------------------------------------------------------------------------------------------
UCLASS(meta = (DisableNativeTick)) class UAPersonal_Main_HUD : public UUserWidget
{// Create Main Menu, Settings Option + Customization(Morph)

	GENERATED_BODY()

public:
	virtual void NativeConstruct();
	virtual bool NativeOnDrop(const FGeometry& in_geometry, const FDragDropEvent& in_drag_drop_event, UDragDropOperation* in_operation);  // What to do while widget drop on

	void Handler(const EWidget_Action widget_main_hud, void* ptr = 0);  // Controll all widgets from here

	UFUNCTION(BlueprintImplementableEvent, Category = "Event | Handler") void Handler_State_HUD(EHUD_State hud_state_update);  // Call needed func from BP

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Props | State", meta = (AllowPrivateAccess = "true")) EHUD_State EHUD_Status;

private:
	void Update_Morph_Sliders();  // Update Sliders in Widgets BP

	double* WMorph_Param_Array;
	std::function<void()>* Update_Morph;
	UADrag_Drop_Operation* Drag_N_Drop_Operation;

	UFUNCTION(BlueprintCallable, Category = "Widget | Morph") void Handle_Game_State();  // Update from BP Slider to WMorph_Param_Array
	UFUNCTION(BlueprintCallable, Category = "Widget | Morph") void Set_Morph_State(const int index, double value);  // Update from BP Slider to WMorph_Param_Array

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Props | Morph", meta = (AllowPrivateAccess = "true")) TArray<double> Morphs_Param;
};
//-----------------------------------------------------------------------------------------------------------
UCLASS(meta = (DisableNativeTick)) class AAPersonal_HUD : public AHUD, public IIPersonal_Hub_Interaction
{
	GENERATED_BODY()

public:
	AAPersonal_HUD();

	virtual void BeginPlay();
	virtual void HUD_Handler(const EWidget_Action& widgete_action, void* ptr);  // Say widget what to do, and set params

private:
	EWidget_Action* Widget_HUD_State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Widget") TObjectPtr<UAPersonal_Main_HUD> Widget_HUD;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Widget") TSubclassOf<UUserWidget> Widget_HUD_Class;
};
//-----------------------------------------------------------------------------------------------------------
UCLASS(minimalapi) class AAPersonal_Hub_Game_Mode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAPersonal_Hub_Game_Mode();

	virtual void BeginPlay();

private:
	AAPersonal_HUD* Personal_HUD;
};
//-------------------------------------------------------------------------------------------------------------

*/