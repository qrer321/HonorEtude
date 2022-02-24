#pragma once																															 
																																		 
#include "ThreadHandlerChatMessage.h"														
#include "ThreadHandlerLoginResultMessage.h"																
#include "ThreadHandlerServerDestroyMessage.h"																
#include "ThreadHandlerAIUpdateMessage.h"																
																																		 
template <typename MessageHandler, typename MessageType>																				 
void OnMessageProcess(std::shared_ptr<GameServerMessage> Message, UWorld* World)														 
{																																		 
	std::shared_ptr<MessageType> ConvertMessage = std::static_pointer_cast<MessageType>(MoveTemp(Message));								 
	if (nullptr == ConvertMessage)																										 
	{																																	 
		return;																															 
	}																																	 
																																		 
	UHonorProjectGameInstance* GameInstance = Cast<UHonorProjectGameInstance>(World->GetGameInstance());								 
																																		 
	MessageHandler Handler = MessageHandler(MoveTempIfPossible(ConvertMessage));														 
	Handler.Initialize(GameInstance, World);																							 
	Handler.Start();																													 
}																																		 
																																		 
inline void AddGlobalHandler(Dispatcher& Dis, UWorld* World)																			 
{																																		   
	Dis.AddHandler(MessageType::Chat,
	               [World](std::shared_ptr<GameServerMessage> GameServerMessage)
	               {
		               OnMessageProcess<ThreadHandlerChatMessage, ChatMessage>(MoveTemp(GameServerMessage), World);
	               });
																																	
	Dis.AddHandler(MessageType::LoginResult,																	
	               [World](std::shared_ptr<GameServerMessage> GameServerMessage)													
	               {																												
		               OnMessageProcess<ThreadHandlerLoginResultMessage, LoginResultMessage>(MoveTemp(GameServerMessage), World);															
	               });																												
																																	
	Dis.AddHandler(MessageType::ServerDestroy,																	
	               [World](std::shared_ptr<GameServerMessage> GameServerMessage)													
	               {																												
		               OnMessageProcess<ThreadHandlerServerDestroyMessage, ServerDestroyMessage>(MoveTemp(GameServerMessage), World);															
	               });																												
																																	
	Dis.AddHandler(MessageType::AIUpdate,																	
	               [World](std::shared_ptr<GameServerMessage> GameServerMessage)													
	               {																												
		               OnMessageProcess<ThreadHandlerAIUpdateMessage, AIUpdateMessage>(MoveTemp(GameServerMessage), World);															
	               });																												
}																																  		 
