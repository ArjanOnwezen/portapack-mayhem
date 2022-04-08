/*
 * Copyright (C) 2022 Arjan Onwezen
 *
 * This file is part of PortaPack.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#include "ui.hpp"
#include "ui_transmitter.hpp"
#include "transmitter_model.hpp"



#define RCCarTx_MAX_PIN 255
#define RCCarTx_REPEATS 4



namespace ui {

class RCCarTxView : public View {
public:
	RCCarTxView(NavigationView& nav);
	~RCCarTxView();
	
	void focus() override;
	
	std::string title() const override { return "RC Car TX"; };

// Each 16bit button code is actually 8bit followed by its complement
const uint8_t button_codes[9] = {
	28,	// Left Forward
	10,	// Forward
	34,	// Right Forward
	58,	// Left
	4,	// Stop
	64,	// Right
	52,	// Left Backwards
	40,	// Backwards
	46,	// Right Backwards
};

private:
	uint32_t scan_button_index { };

	
	enum tx_modes {
		IDLE = 0,
		SINGLE,
		SCAN
	};
	
	tx_modes tx_mode = IDLE;
	
	void start_tx(const uint32_t button_index);
	void stop_tx();
	void on_tx_progress(const uint32_t progress, const bool done);


	struct remote_layout_t {
		Point position;
		std::string text;
	};
	
	const std::array<remote_layout_t, 9> remote_layout { {
		{ { 3 * 8, 7 * 8 }, "UL" },
		{ { 11 * 8, 7 * 8 }, "U" },
		{ { 19 * 8, 7 * 8 }, "UR" },
		{ { 3 * 8, 14 * 8 }, "L" },
		{ { 11 * 8, 14 * 8 }, "S" },
		{ { 19 * 8, 14 * 8 }, "R" },
		{ { 3 * 8, 21 * 8 }, "DL" },
		{ { 11 * 8, 21 * 8 }, "D" },
		{ { 19 * 8, 21 * 8 }, "DR" },

	} };

	Text text_frequency {
		{ 2 * 8, 2 * 8, 128, 16 },
		"RC car frequency:"
	};

	OptionsField options_frequency {
		{ 2 * 8 , 4 * 8 },
		28,
		{
			{ "26.995MHz - Ch1 (Brown)", 26995000 },
			{ "27.045MHz - Ch2 (Red)", 27045000 },
			{ "27.095MHz - Ch3 (Orange)", 27095000 },
			{ "27.145MHz - Ch4 (Yellow)", 27145000 },
			{ "27.195MHz - Ch5 (Green)", 27195000 },
			{ "27.255MHz - Ch6 (Blue)", 27255000 },
			{ "49.830MHz - Ch1", 26995000 },
			{ "49.845MHz - Ch2", 27045000 },
			{ "49.860MHz - Ch3", 27095000 },
			{ "49.875MHz - Ch4", 27145000 },
			{ "49.890MHz - Ch5", 27195000 },
		}
	};	
/*

*/
	Button button_exit {
		{ 9 * 8, 30 * 8, 13 * 8, 3 * 8 },
		"Exit" 
	};
	
	std::array<Button, 9> buttons { };


	Text text_status {
		{ 2 * 8, 33 * 8, 128, 16 },
		"Ready"
	};
	
	ProgressBar progressbar {
		{ 2 * 8, 35 * 8, 208, 16 }
	};
	
	MessageHandlerRegistration message_handler_tx_progress {
		Message::ID::TXProgress,
		[this](const Message* const p) {
			const auto message = *reinterpret_cast<const TXProgressMessage*>(p);
			this->on_tx_progress(message.progress, message.done);
		}
	};
};

} /* namespace ui */
