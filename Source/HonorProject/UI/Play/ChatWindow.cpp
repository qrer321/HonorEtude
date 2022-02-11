// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatWindow.h"
#include "ChatMessageWidget.h"
#include "ChatMessageObject.h"
#include "HonorProject/Global/HonorProjectGameInstance.h"
#include "HonorProject/Global/ClientBlueprintFunctionLibrary.h"
#include "HonorProject/Message/Messages.h"

void UChatWindow::SendChat(FString Text, ETextCommit::Type CommitType)
{
	if (ETextCommit::Type::OnEnter != CommitType)
	{
		return;
	}

	UChatMessageObject* ChatMessageObject = NewObject<UChatMessageObject>();
	ChatMessageObject->SetID(TEXT("TestObject"));
	ChatMessageObject->SetMessage(Text);

	m_MessageListView->AddItem(ChatMessageObject);

	UHonorProjectGameInstance* GameInstance = Cast<UHonorProjectGameInstance>(GetGameInstance());
	ChatMessage Message;

	std::string ID;
	std::string SendMessage;

	const FString ObjectID = ChatMessageObject->GetID();
	const FString ObjectMessage = ChatMessageObject->GetMessage();
	UClientBlueprintFunctionLibrary::FStringToUTF8(ObjectID, ID);
	UClientBlueprintFunctionLibrary::FStringToUTF8(ObjectMessage, SendMessage);

	Message.m_ID = ID;
	Message.m_Message = SendMessage;

	GameServerSerializer Serializer;
	Message.Serialize(Serializer);

	GameInstance->Send(Serializer.GetData());
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
