/*
 * Copyright (C) 2014 Jared Boone, ShareBrained Technology, Inc.
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

#ifndef __APP_AIS_H__
#define __APP_AIS_H__

#include "ui_console.hpp"
#include "message.hpp"
#include "log_file.hpp"

#include "ais_baseband.hpp"

class AISModel {
public:
	AISModel();

	baseband::ais::decoded_packet on_packet(const AISPacketMessage& message);

private:
	LogFile log_file;
};

namespace ui {

class AISView : public Console {
public:
	void on_show() override;
	void on_hide() override;

private:
	AISModel model;

	void log(const baseband::ais::decoded_packet decoded);
};

} /* namespace ui */

#endif/*__APP_AIS_H__*/
