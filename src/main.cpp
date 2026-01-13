#include <dpp/dpp.h>

#include <bismuth/classes/logger.hpp>
#include <aurora/aurora.hpp>

#include "secrets.hpp"

using namespace dpp;
using namespace aurora;


int main() {
	cluster bot(secrets::BOT_TOKEN, i_default_intents | i_message_content);

	log::set12hTimeEnabled(true);
	log::setFileLogLevel(log::LogLevel::Debug);
	TargetManager::get()->logToDir(std::format("{}/Desktop/logs/sopher", std::getenv("HOME")), "Sopher");
	ThreadManager::get()->addThread("Main");
	bot.on_log(bismuth::logger());


	bot.on_ready([&bot](ready_t const& event) {
		if (run_once<struct CmdRegister>()) {
			std::vector<slashcommand> commandsVector {
				{ "meow", ":3", bot.me.id }
			};
			for (auto& command : commandsVector)
				command.set_interaction_contexts({ itc_guild });

			bot.global_bulk_command_create(commandsVector);
		}

		bot.set_presence(presence(ps_idle, at_watching, "at u"));
	});

	bot.on_slashcommand([](slashcommand_t const& event) {
		auto commandName = event.command.get_command_name();

		if (commandName == "meow")
			event.reply("meow");
	});

	bot.on_message_create([](message_create_t const& event) {
		if (event.msg.author.format_username() == "sopher#7829")
			return;

		auto const& msg = event.msg.content;

		if (msg.contains("meow"))
			event.reply("meow");
		else if (msg.contains(":3"))
			event.reply(":3");
	});

	bot.start(dpp::st_wait);

	return 0;
}