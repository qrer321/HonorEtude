// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatWindow.h"
#include "ChatMessageWidget.h"
#include "ChatMessageObject.h"

void UChatWindow::SendChat(FString ChatMessage, ETextCommit::Type CommitType)
{
	if (ETextCommit::Type::OnEnter != CommitType)
	{
		return;
	}

	UChatMessageObject* ChatMessageObject = NewObject<UChatMessageObject>();
	ChatMessageObject->SetID(TEXT("TestObject"));
	ChatMessageObject->SetMessage(ChatMessage);

	m_MessageListView->AddItem(ChatMessageObject);
}

void UChatWindow::AddNewMessage(UObject* Object, UUserWidget* UserWidget)
{
	if (false == IsValid(Object))
	{
		return;
	}

	if (false == IsValid(UserWidget))
	{
		return;
	}
	
	UChatMessageObject* MessageObject = Cast<UChatMessageObject>(Object);
	if (false == IsValid(MessageObject))
	{
		return;
	}

	UChatMessageWidget* MessageWidget = Cast<UChatMessageWidget>(UserWidget);
	if (false == IsValid(MessageWidget))
	{
		return;
	}

	MessageWidget->SetChatString(MessageObject->GetID() + TEXT(" : ") + MessageObject->GetMessage());
}
