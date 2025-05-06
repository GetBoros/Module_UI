#include "Module_UI_HUD.h"
#include "Module_UI.h"

// UADrag_Widget
void UADrag_Widget::NativeConstruct()
{
	Super::NativeConstruct();
}
//-------------------------------------------------------------------------------------------------------------
void UADrag_Widget::Update_State(const UImage *image, const FVector2D desired_size) const
{
	SizeBox_Root->SetWidthOverride(desired_size.X);
	SizeBox_Root->SetHeightOverride(desired_size.Y);  // Set Widget Box Size
	Border_Root->SetBrush(image->GetBrush() );
}
//-------------------------------------------------------------------------------------------------------------




// UAModule_UI_Inventory_Slot
FReply UAModule_UI_Inventory_Slot::NativeOnMouseButtonDown(const FGeometry &in_geometry, const FPointerEvent &in_mouse_event)
{
	Super::NativeOnMouseButtonDown(in_geometry, in_mouse_event);

    const FVector2D mouse_pos = in_geometry.AbsoluteToLocal(in_mouse_event.GetScreenSpacePosition() );

	Drag_Offset = in_geometry.AbsoluteToLocal(in_mouse_event.GetScreenSpacePosition() );  // Save Drag offset

    UE_LOG(LogTemp, Warning, TEXT("Mouse Clicked at: X: %f, Y: %f"), mouse_pos.X, mouse_pos.Y);

	return UWidgetBlueprintLibrary::DetectDragIfPressed(in_mouse_event, this, EKeys::LeftMouseButton).NativeReply;  // if start drag
}
//------------------------------------------------------------------------------------------------------------
void UAModule_UI_Inventory_Slot::NativeOnDragDetected(const FGeometry &in_geometry, const FPointerEvent &in_mouse_event, UDragDropOperation *&out_operation)
{
	UADrag_Widget* drag_visual = 0;

	// 1.0. Display Widget
	drag_visual = CreateWidget<UADrag_Widget>(GetWorld(), Drag_Widget_Class);  // Widget to show where we drag this widget
	drag_visual->Update_State(Image_Root, GetDesiredSize() );

	// 1.1. Operation Drag n Drop Init  NewDragDrop->Offset = in_geometry.AbsoluteToLocal(in_mouse_event.GetScreenSpacePosition());
	Drag_Drop_Operation = NewObject<UADrag_Drop_Operation>(this, Drag_Drop_O_Class);
	Drag_Drop_Operation->DefaultDragVisual = drag_visual;
	Drag_Drop_Operation->Payload = this;  // Is it correct?
	Drag_Drop_Operation->Pivot = EDragPivot::MouseDown;  // where the Drag Widget Visual appears while being dragged relative to the pointer performing the drag operation.
	Drag_Drop_Operation->Widget_Reference = this;  // For recreating
	Drag_Drop_Operation->Drag_Offset = Drag_Offset;  // Off set from mouse to correct add to viewport

	out_operation = Drag_Drop_Operation;  // send operation to drop

	Super::NativeOnDragDetected(in_geometry, in_mouse_event, out_operation);
}
//------------------------------------------------------------------------------------------------------------
bool UAModule_UI_Inventory_Slot::NativeOnDrop(const FGeometry &in_geometry, const FDragDropEvent &in_drag_drop_event, UDragDropOperation *in_operation)
{
	UADrag_Drop_Operation *drop_operations;
	UAModule_UI_Inventory_Slot *drop_widget_ref;

	drop_operations = Cast<UADrag_Drop_Operation>(in_operation);
	drop_widget_ref = Cast<UAModule_UI_Inventory_Slot>(drop_operations->Widget_Reference);

	FSlateBrush brush = Image_Root->GetBrush();

	Image_Root->SetBrush(drop_widget_ref->Image_Root->GetBrush() );
	drop_widget_ref->Image_Root->SetBrush(brush);

	Super::NativeOnDrop(in_geometry, in_drag_drop_event, in_operation);

	return true;
}
//-----------------------------------------------------------------------------------------------------------




// UModule_UI_HUD
void UModule_UI_HUD::NativeOnDragDetected(const FGeometry &in_geometry, const FPointerEvent &in_mouse_event, UDragDropOperation *&out_operation)
{
	Super::NativeOnDragDetected(in_geometry, in_mouse_event, out_operation);
}
//------------------------------------------------------------------------------------------------------------
FReply UModule_UI_HUD::NativeOnMouseButtonDown(const FGeometry &in_geometry, const FPointerEvent &in_mouse_event)
{
	Super::NativeOnMouseButtonDown(in_geometry, in_mouse_event);

    const FVector2D mouse_pos = in_geometry.AbsoluteToLocal(in_mouse_event.GetScreenSpacePosition() );

    UE_LOG(LogTemp, Warning, TEXT("Mouse Clicked at: X: %f, Y: %f"), mouse_pos.X, mouse_pos.Y);

	return UWidgetBlueprintLibrary::DetectDragIfPressed(in_mouse_event, this, EKeys::LeftMouseButton).NativeReply;  // if start drag
}
//------------------------------------------------------------------------------------------------------------
FReply UModule_UI_HUD::NativeOnMouseMove(const FGeometry &in_geometry, const FPointerEvent &in_mouse_event)
{
    const FVector2D mouse_pos = in_mouse_event.GetScreenSpacePosition();

    UE_LOG(LogTemp, Warning, TEXT("Mouse Moved at: X: %f, Y: %f"), mouse_pos.X, mouse_pos.Y);

    return FReply::Unhandled();
}
//------------------------------------------------------------------------------------------------------------


/*

#include "Personal_Hub_Game_Mode.h"
#include "Personal_Hub_Character.h"

#include "Components/ProgressBar.h"
#include "GameFramework/GameUserSettings.h"





// UAPersonal_Main_Menu
void UAPersonal_Main_Menu::NativeConstruct()
{
	Super::NativeConstruct();
}
//-------------------------------------------------------------------------------------------------------------




// UAPersonal_Main_HUD
void UAPersonal_Main_HUD::NativeConstruct()
{
	WMorph_Param_Array = 0;
	EHUD_Status = EHUD_State::None;

	Super::NativeConstruct();
}
//-------------------------------------------------------------------------------------------------------------
bool UAPersonal_Main_HUD::NativeOnDrop(const FGeometry& in_geometry, const FDragDropEvent& in_drag_drop_event, UDragDropOperation* in_operation)
{
	UUserWidget* slot_widget = 0;  // need to save after
	FVector2D view_pos = {};

	Super::NativeOnDrop(in_geometry, in_drag_drop_event, in_operation);

	Drag_N_Drop_Operation = Cast<UADrag_Drop_Operation>(in_operation);  // or UADrag_Drop_Operation
	slot_widget = Drag_N_Drop_Operation->Widget_Reference;
	slot_widget->RemoveFromParent();
	slot_widget->AddToViewport();
	view_pos = in_geometry.AbsoluteToLocal(in_drag_drop_event.GetScreenSpacePosition()) - Drag_N_Drop_Operation->Drag_Offset;  // Set where to place location
	slot_widget->SetPositionInViewport(view_pos, false);

	return true;
}
//-------------------------------------------------------------------------------------------------------------
void UAPersonal_Main_HUD::Handler(const EWidget_Action widget_main_hud, void* ptr)
{
	switch (widget_main_hud)
	{
	case EWidget_Action::None:
		break;


	case EWidget_Action::Init_Morph_Param:
		WMorph_Param_Array = static_cast<double*>(ptr);  // If action Morph convert to double array, Save to Morphs_Options and send to widget
		Update_Morph_Sliders();
		break;

	case EWidget_Action::Init_Morph_Func:
		Update_Morph = static_cast<std::function<void()> *>(ptr);
		break;

	case EWidget_Action::Update_Morph_Slider:
		Update_Morph_Sliders();
		break;

	case EWidget_Action::Menu_Main_Toogle:
		Handler_State_HUD(EHUD_State::Toogle_Menu_Main);
		break;

	case EWidget_Action::Inventory_Toogle:
		Handler_State_HUD(EHUD_State::Toogle_Inventory);
		break;
	default:
		break;
	}
}
//-------------------------------------------------------------------------------------------------------------
void UAPersonal_Main_HUD::Update_Morph_Sliders()
{
	for (size_t i = 0; i < 5; i++)
		Morphs_Param.Add(WMorph_Param_Array[i]);

	Handler_State_HUD(EHUD_State::Update_Morph);  // In BP Call update morps
}
//-------------------------------------------------------------------------------------------------------------
void UAPersonal_Main_HUD::Handle_Game_State()
{
	(*Update_Morph)();
}
//-------------------------------------------------------------------------------------------------------------
void UAPersonal_Main_HUD::Set_Morph_State(const int index, double value)
{
	WMorph_Param_Array[index] = value;
	(*Update_Morph)();
}
//------------------------------------------------------------------------------------------------------------




// AAPersonal_HUD
AAPersonal_HUD::AAPersonal_HUD()
	: Widget_HUD_State(), Widget_HUD(0), Widget_HUD_Class(0)
{
}
//-----------------------------------------------------------------------------------------------------------
void AAPersonal_HUD::BeginPlay()
{
	//if (!Widget_HUD_Class != 0)
	//	return;
	//Widget_HUD = CreateWidget<UAPersonal_Main_HUD>(GetWorld(), Widget_HUD_Class);
	//Widget_HUD->AddToViewport();

	Super::BeginPlay();
}
//-----------------------------------------------------------------------------------------------------------
void AAPersonal_HUD::HUD_Handler(const EWidget_Action& widgete_action, void* ptr)
{
	switch (widgete_action)
	{
	case EWidget_Action::None:
		Widget_HUD_State = static_cast<EWidget_Action*>(ptr);  // Initialize
		break;
	case EWidget_Action::Update_HUD_State:
		*Widget_HUD_State = (EWidget_Action)Widget_HUD->EHUD_Status;
		break;
	default:
		Widget_HUD->Handler(widgete_action, ptr);
		break;
	}
}
//-----------------------------------------------------------------------------------------------------------




// APersonalHubGameMode
AAPersonal_Hub_Game_Mode::AAPersonal_Hub_Game_Mode()
	: Personal_HUD(0)
{
	DefaultPawnClass = AAPersonal_Hub_Character::StaticClass();
	HUDClass = AAPersonal_HUD::StaticClass();
	PlayerControllerClass = AAPersonal_Player_Controller::StaticClass();
}
//-------------------------------------------------------------------------------------------------------------
void AAPersonal_Hub_Game_Mode::BeginPlay()
{
	Super::BeginPlay();

	//Personal_HUD = Cast<AAPersonal_HUD>(GetWorld()->GetFirstPlayerController()->GetHUD() );
	//Personal_HUD->HUD_Handler(EWidget_Action::Is_New_Game, );
	//UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();

	//UserSettings->SetFullscreenMode(EWindowMode::WindowedFullscreen);
	//UserSettings->SetFrameRateLimit(30.0);
	//UserSettings->ApplySettings(true);
}
//-------------------------------------------------------------------------------------------------------------

*/