/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
    Grig:  Gtk+ user interface for the Hamradio Control Libraries.

    Copyright (C)  2001-2004  Alexandru Csete.

    Authors: Alexandru Csete <csete@users.sourceforge.net>

    Comments, questions and bugreports should be submitted via
    http://sourceforge.net/projects/groundstation/
    More details can be found at the project home page:

            http://groundstation.sourceforge.net/
 
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
  
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
  
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the
          Free Software Foundation, Inc.,
	  59 Temple Place, Suite 330,
	  Boston, MA  02111-1307
	  USA
*/
#ifndef RIG_DAEMON_H
#define RIG_DAEMON_H 1


/** \brief List of available commands.
 *
 * This enumeration lists the available commands that can be executed by the RIG daemon.
 * These values should be used to define each daemon cycle.
 */
typedef enum {
	RIG_CMD_NONE = 0,     /*!< Not command. Used to end a sequence of commands. */

	RIG_CMD_GET_FREQ_1,   /*!< Command to acquire primary frequency from rig. */
	RIG_CMD_SET_FREQ_1,   /*!< Command to set primary frequency. */
	RIG_CMD_GET_FREQ_2,   /*!< Command to acquire secondary frequency from rig. */
	RIG_CMD_SET_FREQ_2,   /*!< Command to set secondary frequency. */
	RIG_CMD_GET_RIT,      /*!< Command to get current RIT value. */
	RIG_CMD_SET_RIT,      /*!< Command to set new RIT value. */
	RIG_CMD_GET_XIT,      /*!< Command to get current XIT value. */
	RIG_CMD_SET_XIT,      /*!< Command to set new XIT value. */
	RIG_CMD_GET_VFO,      /*!< Command to get currently active VFO. */      
	RIG_CMD_SET_VFO,      /*!< Command to select new VFO. */
	RIG_CMD_GET_PSTAT,    /*!< Command to read power status (mains pwr, ON/OFF/STDBY). */
	RIG_CMD_SET_PSTAT,    /*!< Command to set new power status (ON/OFF/STDBY). */
	RIG_CMD_GET_PTT,      /*!< Command to get the current PTT status. */
	RIG_CMD_SET_PTT,      /*!< Command to set the current PTT status. */
	RIG_CMD_GET_MODE,     /*!< Command to get the current mode and pass band width. */
	RIG_CMD_SET_MODE,     /*!< Command to set the new mode and/or pass band width. */

	RIG_CMD_NUMBER        /*!< Number of available commands. */
} rig_cmd_t;


int  rig_daemon_start (int);
void rig_daemon_stop  (void);


#endif
