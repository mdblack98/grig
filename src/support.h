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
#ifndef SUPPORT_H
#define SUPPORT_H 1

#ifdef ENABLE_NLS
#	include <libintl.h>
#	undef _
#	define _(String) dgettext (PACKAGE, String)
#	ifdef gettext_noop
#		define N_(String) gettext_noop (String)
#	else
#		define N_(String) (String)
#	endif
#else
#	define textdomain(String) (String)
#	define gettext(String) (String)
#	define dgettext(Domain,Message) (Message)
#	define dcgettext(Domain,Message,Type) (Message)
#	define bindtextdomain(Domain,Directory) (Domain)
#	define _(String) (String)
#	define N_(String) (String)
#endif

#endif