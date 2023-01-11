// Fill out your copyright notice in the Description page of Project Settings.


#include "BM_UserWidget.h"


UBM_UserWidget::UBM_UserWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FReply UBM_UserWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
	UE_LOG(LogTemp, Log, TEXT("NativeOnPreviewMouseButtonDown fire ...."));
	return FReply::Unhandled();
}