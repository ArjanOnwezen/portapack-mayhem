/*
 * Copyright (C) 2015 Jared Boone, ShareBrained Technology, Inc.
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

#ifndef __APP_ERT_H__
#define __APP_ERT_H__

#include "ui_console.hpp"
#include "message.hpp"
#include "log_file.hpp"

#include <string>

class ERTModel {
public:
	ERTModel();

	std::string on_packet(const ERTPacketMessage& message);

private:
	LogFile log_file;
};

namespace ui {

class ERTView : public Console {
public:
	void on_show() override;
	void on_hide() override;

private:
	ERTModel model;

	void log(const std::string& s);
};

} /* namespace ui */

#endif/*__APP_ERT_H__*/
