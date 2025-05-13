#include "Module_UI_Inventory.h"
#include "Module_UI.h"

// UAModule_UI_Dragged_Widget
void UAModule_UI_Dragged_Widget::NativeConstruct()
{
	Super::NativeConstruct();
}
//-------------------------------------------------------------------------------------------------------------
void UAModule_UI_Dragged_Widget::Update_State(const UImage *image, const FVector2D desired_size) const
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

	Drag_Offset = in_geometry.AbsoluteToLocal(in_mouse_event.GetScreenSpacePosition() );  // Save Drag offset

	return UWidgetBlueprintLibrary::DetectDragIfPressed(in_mouse_event, this, EKeys::LeftMouseButton).NativeReply;  // if start drag
}
//------------------------------------------------------------------------------------------------------------
void UAModule_UI_Inventory_Slot::NativeOnDragDetected(const FGeometry &in_geometry, const FPointerEvent &in_mouse_event, UDragDropOperation *&out_operation)
{
	UAModule_UI_Dragged_Widget *drag_visual;

	// 1.0. Display Widget
	drag_visual = CreateWidget<UAModule_UI_Dragged_Widget>(GetWorld(), Drag_Widget_Class);  // Widget to show where we drag this widget
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
	const FSlateBrush from_drop_slot_brush = Image_Root->GetBrush();  // Save brush for dropped slot || To set in dropped slot

	// 1.0. Init
	drop_operations = Cast<UADrag_Drop_Operation>(in_operation);  // Get Info from other widget(slot)
	drop_widget_ref = Cast<UAModule_UI_Inventory_Slot>(drop_operations->Widget_Reference);  // check if dragged slot not something else


	// 2.0. Check is need to change widgets textures and data
	if (drop_widget_ref->Slot_Type == Slot_Type)
	{
		// 2.1. Current Slot || Switch widgets textures
		Image_Root->SetBrush(drop_widget_ref->Image_Root->GetBrush() );  // Change brush to dropped Slot

		// !!! If equipment empty slot set texture to empty

		// 2.2. Dropped Slot
		drop_widget_ref->Image_Root->SetBrush(from_drop_slot_brush);  // Change brush from dropped slot
	}

	SetFocus();  // Set Focus to dropped widget
	Super::NativeOnDrop(in_geometry, in_drag_drop_event, in_operation);
	return true;
}
//-----------------------------------------------------------------------------------------------------------
void UAModule_UI_Inventory_Slot::Init(const ESlot_Type slot_type, UTexture2D *texture2d, const FVector base_color)
{
	Slot_Type = slot_type;
	Image_Root->GetDynamicMaterial()->SetTextureParameterValue(FName("Base_Slot_Texture"), texture2d);
	Image_Root->GetDynamicMaterial()->SetVectorParameterValue(FName("Base_Color"), base_color);
}
//-----------------------------------------------------------------------------------------------------------






// UModule_UI_Inventory
TArray<UAModule_UI_Inventory_Slot *> UModule_UI_Inventory::Slot_Add(const int32 column, const int32 type_index, TArray<UTexture2D *> &textures)
{
	ESlot_Type slot_type;
	int32 i;
	int32 texture_size;
	UAModule_UI_Inventory_Slot *slot;
	TArray<UAModule_UI_Inventory_Slot *> slot_array;

	texture_size = textures.Num();
	slot_array.Reserve(texture_size);

	for (i = 0; i < texture_size; ++i)
	{
		slot_type = static_cast<ESlot_Type>(i + type_index);
		slot = Cast<UAModule_UI_Inventory_Slot>(CreateWidget(this, Slot_Template) );
		slot->Init(slot_type, textures[i], FVector::ZeroVector);  // !!! Bad type if Accessory

		if (Uniform_Grid_Panel->AddChildToUniformGrid(slot, i, column) )
			slot_array.Emplace(slot);
	}
	return slot_array;
}
//-----------------------------------------------------------------------------------------------------------
void UModule_UI_Inventory::Create_Slots_At_Column(const ESlot_Type type, const int column, TArray<UAModule_UI_Inventory_Slot *> &array_slot)
{
	int32 type_index;
	TArray<UAModule_UI_Inventory_Slot *> test;

	type_index = static_cast<int32>(type);

	switch (type)
	{
	case ESlot_Type::EST_Equipment:
		test = Slot_Add(column, type_index, Textures_Equipment);
		break;

	case ESlot_Type::EST_Accessory:
		test = Slot_Add(column, type_index, Textures_Accessory);
		break;

	default:
		test = Slot_Add(column, type_index, Textures_Defaults);
		break;
	}

	array_slot = test;
}
//-----------------------------------------------------------------------------------------------------------
