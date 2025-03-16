#include "Module_UI_HUD.h"

//------------------------------------------------------------------------------------------------------------
FReply UModule_UI_HUD::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    // �������� ���������� ����� � ��������� ����������� �������
    const FVector2D mouse_pos = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

    // �������� ���������� �����
    UE_LOG(LogTemp, Warning, TEXT("Mouse Clicked at: X: %f, Y: %f"), mouse_pos.X, mouse_pos.Y);

    return FReply::Handled(); // ��������, ��� ������� ����������
}
//------------------------------------------------------------------------------------------------------------
FReply UModule_UI_HUD::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    // �������� ���������� ���� � ���������� ����������� ������
    const FVector2D mouse_pos = InMouseEvent.GetScreenSpacePosition();

    // �������� ���������� ����
    UE_LOG(LogTemp, Warning, TEXT("Mouse Moved at: X: %f, Y: %f"), mouse_pos.X, mouse_pos.Y);

    return FReply::Unhandled(); // ���������� Unhandled, ����� ������� ������������ ������
}
//------------------------------------------------------------------------------------------------------------
