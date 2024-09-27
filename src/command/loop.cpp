#include "loop.h"
#include "util/logger.h"
#include "util/parser.h"

void Loop::Exec(const dpp::slashcommand_t &event) {
	/* Get the loop type slash command parameter */
	dpp::command_value loop_type_par = event.get_parameter("type");

	/* If there is no parameters */
	if (loop_type_par.index() == 0) {
		event.reply(GuildPlayer::Get(event.command.guild_id)->Loop(event.command.channel_id));
		return;
	}

	/* Get the loop type in string */
	const std::string loop_type_str = std::get<std::string>(loop_type_par);

	/* Send the message to the channel */
	if (loop_type_str == "loop_track")
		return event.reply(GuildPlayer::Get(event.command.guild_id)->Loop(event.command.channel_id, GuildPlayer::LoopType::TRACK));
	else if (loop_type_str == "loop_playlist")
		return event.reply(GuildPlayer::Get(event.command.guild_id)->Loop(event.command.channel_id, GuildPlayer::LoopType::PLAYLIST));
	else
		return event.reply(GuildPlayer::Get(event.command.guild_id)->Loop(event.command.channel_id, GuildPlayer::LoopType::DISABLED));
}

void Loop::Exec(const dpp::message_create_t &event, const std::string &argument) {
	/* If the argument is empty */
	if (argument.empty()) {
		event.send(GuildPlayer::Get(event.msg.guild_id)->Loop(event.msg.channel_id));
		return;
	}

	/* Get the loop type */
	u_int8 loop_type_int = Parser::ToUInt8(argument.c_str() + argument.find_first_not_of(' '));
	event.send(GuildPlayer::Get(event.msg.guild_id)->Loop(event.msg.channel_id, (GuildPlayer::LoopType)loop_type_int));
}