#pragma once

bool group_message_pipeline(
	dice_protocols_manager * dice_protocols, 
	const int32_t i_AuthCode, 
	const char * msg, 
	const int64_t fromGroup, 
	const int64_t fromQQ, 
	bool isfromGroup);
