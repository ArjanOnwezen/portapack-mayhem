/*
 * Copyright (C) 2022 ArjanOnwezen
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

#include "ui_rc_car_tx.hpp"
#include "encoders.hpp"

#include "baseband_api.hpp"
#include "string_format.hpp"


using namespace portapack;
using namespace encoders;

namespace ui {

void RCCarTxView::focus() {
	button_exit.focus();
}

RCCarTxView::~RCCarTxView() {
	transmitter_model.disable();
	baseband::shutdown();
}

void RCCarTxView::stop_tx() {
	transmitter_model.disable();
//	tx_mode = IDLE;
	progressbar.set_value(0);
 	//text_status.set("Ready");

}

void RCCarTxView::on_tx_progress(const uint32_t progress, const bool done) {
	if (!done) {
		// Progress
//		if (tx_mode == SINGLE)
			progressbar.set_value(progress);
//		else if (tx_mode == SCAN)
//			progressbar.set_value((RCCarTx_REPEATS) + progress);
	} else {
		// Done transmitting
//		if (tx_mode == SINGLE) {
			stop_tx();
//		} else if (tx_mode == SCAN) {
//			if (pin == RCCarTx_MAX_PIN) {
//				stop_tx();
//			} else {
//				transmitter_model.disable();
//				pin++;
//				field_pin.set_value(pin);
//				start_tx(scan_button_index);
//			}
//		}
	}
}




void RCCarTxView::start_tx(const uint32_t button_index) {


	std::string fragments = "1110111011101110";
	size_t bit;
	std::string sync_word = "";
	

	//tx_mode = SINGLE;
	progressbar.set_max(RCCarTx_REPEATS);
	//text_status.set("Transmitting...");
	
	// Convert to OOK symbols
	for (bit = 0; bit < button_codes[button_index]; bit++) {
		fragments += "10";
	}


	// Sync and end pulse
	//fragments = "1110111011101110" + fragments;

		text_status.set(fragments);	
	size_t bitstream_length = make_bitstream(fragments);

	#define SAMPLING_RATE 2280000U

	transmitter_model.set_tuning_frequency(27145000);//change into variable
	transmitter_model.set_sampling_rate(SAMPLING_RATE);
	transmitter_model.set_tx_gain(47);
	transmitter_model.set_rf_amp(true); 
	transmitter_model.set_baseband_bandwidth(1750000);
	transmitter_model.enable();
	
	baseband::set_ook_data(
		bitstream_length,
		SAMPLING_RATE / 2018,
		10, //RCCarTx_REPEATS
		200						// Pause
	);
}

RCCarTxView::RCCarTxView(
	NavigationView& nav
) {
	baseband::run_image(portapack::spi_flash::image_tag_ook);

	add_children({
		&text_frequency,
		&options_frequency,
		&text_status,
		&progressbar,
		&button_exit
	});
	
	//transmitter_model.set_tuning_frequency(26995000);


	transmitter_model.set_tuning_frequency(options_frequency.selected_index() + 1);


	button_exit.on_select = [&nav, this](Button&) {
		nav.pop();
	};

	const auto button_fn = [this](Button& button) {
		start_tx(button.id);
	};
	
	size_t n = 0;
	for (auto& entry : remote_layout) {
		buttons[n].on_select = button_fn;
		buttons[n].id = n;
		buttons[n].set_text(entry.text);
		buttons[n].set_parent_rect({
			entry.position + Point(8, 0),
			{ 7 * 8, 6 * 8 }
		});
		add_child(&buttons[n]);
		n++;
	}
}

} /* namespace ui */
