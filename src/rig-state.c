/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
    Grig:  Gtk+ user interface for the Hamradio Control Libraries.

    Copyright (C)  2001-2006  Alexandru Csete.

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
    along with this program; if not, visit http://www.fsf.org/

*/
/** \file rig-state.c
 *  \brief Load and save rig state
 *
 * The rig state is a snapshot of the current settings in the rig-data
 * object. The rig state is saved to a file using the glib key/value
 * infrastructure.
 *
 * To begin with we just implement it in a simple straight forward way,
 * but later the could should be checnged to something more generic,
 * if possible.
 */
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <hamlib/rig.h>
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#include "rig-data.h"
#include "rig-state.h"



extern GtkWidget    *grigapp;


/** \brief Load rig state from file
 *  \param file The file to read the rig state from
 *
 * The file parameter may not be NULL. If you need to open
 * the file selector use the callback functions instead.
 */
gint
rig_state_load (const gchar *file)
{
	return 0;
}


/** \brief Save rig state to file
 *  \param file The file to save the rig state to
 *
 * The file parameter may not be NULL. If you need to open
 * the file selector use the callback functions instead.
 * existing file will be replaced without any warning!
 */
gint
rig_state_save (const gchar *file)
{
	return 0;
}


/** \brief Get connection info about radio
 *
 * This fiunction will return connection details found in the
 * specified rig state file. This can be used to re-establish
 * the link to a radio when the rig state file is loaded via
 * the command line. When the rig state is read via the menu
 * bar, grig is already running a connection and this can not
 * be changed. In those cases this function has no use.
 */
/* gint */
/* rig_state_get_link_info (const gchar *file, */
/* 			 rig_model_t *model, */
/* 			 gchar *model, */
/* 			 gchar *mfg, */
/* 			 gchar *port, */
/* 			 int   *ser_rate) */
/* { */

/* } */



/** \brief Load the rig state
 *
 * This function first asks the user for a file name via the
 * GtkFileChooser dialog, whereafter it calls rig_state_load
 * with the specified file name.
 * An error message is generated if the file does not exist or
 * the contents could not be read.
 */
void
rig_state_load_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget     *dialog;     /* file chooser dialog */
	GtkFileFilter *filter1;    /* *.rig filter used in the dialog */
	GtkFileFilter *filter2;    /* filter used in the dialog for all files */
	gchar         *filename;   /* file name selected by user */

	GtkWidget     *msgdiag;    /* message dialog */
	gint           status;     /* error status */


	/* create file chooser dialog */
	dialog = gtk_file_chooser_dialog_new (_("Load Rig State"),
					      GTK_WINDOW (grigapp),
					      GTK_FILE_CHOOSER_ACTION_OPEN,
					      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					      GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
					      NULL);

	/* Add filters for .rig files and all files */
	filter1 = gtk_file_filter_new ();
	gtk_file_filter_set_name (filter1, _("Rig state files (*.rig)"));
	gtk_file_filter_add_pattern (filter1, "*.rig");
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog), filter1);

	filter2 = gtk_file_filter_new ();
	gtk_file_filter_set_name (filter2, _("All files"));
	gtk_file_filter_add_pattern (filter2, "*");
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog), filter2);


	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {

		filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

		/* check that file exists and it is a regular file */
		if (g_file_test (filename, G_FILE_TEST_IS_REGULAR)) {

			status = rig_state_load (filename);

			if (status) {
				msgdiag = gtk_message_dialog_new (GTK_WINDOW (grigapp),
								  GTK_DIALOG_MODAL |
								  GTK_DIALOG_DESTROY_WITH_PARENT,
								  GTK_MESSAGE_ERROR,
								  GTK_BUTTONS_OK,
								  _("There was an error reading "\
								    "the settings from:\n\n "\
								    "%s\n\n "\
								    "Examine the log messages "\
								    "for further info."),
								  filename);
				gtk_dialog_run (GTK_DIALOG (msgdiag));
				gtk_widget_destroy (msgdiag);
			}
			
		}
		else {
			/* tell user to select an existing file */
			msgdiag = gtk_message_dialog_new (GTK_WINDOW (grigapp),
							  GTK_DIALOG_MODAL |
							  GTK_DIALOG_DESTROY_WITH_PARENT,
							  GTK_MESSAGE_ERROR,
							  GTK_BUTTONS_OK,
							  _("The selected file:\n "\
							    "%s\n "\
							    "does not exist or is not "\
							    "a regular file."),
							    filename);
			gtk_dialog_run (GTK_DIALOG (msgdiag));
			gtk_widget_destroy (msgdiag);
		}

		g_free (filename);
	}

	gtk_file_chooser_remove_filter (GTK_FILE_CHOOSER (dialog), filter1);
	gtk_file_chooser_remove_filter (GTK_FILE_CHOOSER (dialog), filter2);
	gtk_widget_destroy (dialog);

}


/** \brief Save the rig state
 *
 * This function first asks the user for a file name via the
 * GtkFileChooser dialog, whereafter it calls rig_state_save
 * with the specified file name.
 * If the file already exists it will ask the user whether to
 * replace the contents or not. If not, the code returns to the
 * file chooser.
 *
 * The complete algorithm:
 *
 *   while (!done) {
 *       if (run_file_chooser == YES) {
 *           get_filename
 *           if (file_exists) {
 *               create_cfm_dialog
 *               if (do_overwrite) {
 *                   save_file
 *                   if (error)
 *                       show_error_message
 *                   done = TRUE
 *               } else {
 *                   done = FALSE
 *               }
 *               destroy_cfm_dialog
 *           } else {
 *               save_file
 *               if (error)
 *                   show_error_message
 *               done = TRUE
 *           }
 *       } else {
 *           done = TRUE
 *       }
 *   }
 */
void
rig_state_save_cb (GtkWidget *widget, gpointer data)
{
	GtkWidget     *dialog;        /* file chooser dialog */
	GtkFileFilter *filter1;       /* *.rig filter used in the dialog */
	GtkFileFilter *filter2;       /* filter used in the dialog for all files */
	gchar         *filename;      /* file name selected by user */

	GtkWidget     *msgdiag;       /* message dialog */
	gint           status;        /* error status */

	gboolean       done = FALSE;  /* flag to indicate whether we are done or not */
	GtkWidget     *cfmdiag;       /* configrmation dialog */


	/* create file chooser dialog */
	dialog = gtk_file_chooser_dialog_new (_("Save Rig State"),
					      GTK_WINDOW (grigapp),
					      GTK_FILE_CHOOSER_ACTION_SAVE,
					      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					      GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
					      NULL);

	/* Add filters for .rig files and all files */
	filter1 = gtk_file_filter_new ();
	gtk_file_filter_set_name (filter1, _("Rig state files (*.rig)"));
	gtk_file_filter_add_pattern (filter1, "*.rig");
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog), filter1);

	filter2 = gtk_file_filter_new ();
	gtk_file_filter_set_name (filter2, _("All files"));
	gtk_file_filter_add_pattern (filter2, "*");
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog), filter2);


	/* loop until we save the settings or the user selects cancel
	   in the file chooser dialog
	*/
	while (!done) {

		if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {

			/* user selected OK */
			filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

			/* if file exists warn user and ask for confirmation */
			if (g_file_test (filename, G_FILE_TEST_EXISTS)) {

				/* create warning/confirmation dialog */
				cfmdiag = gtk_message_dialog_new (GTK_WINDOW (dialog),
								  GTK_DIALOG_MODAL |
								  GTK_DIALOG_DESTROY_WITH_PARENT,
								  GTK_MESSAGE_QUESTION,
								  GTK_BUTTONS_YES_NO,
								  _("Selected file already exists.\n"\
								    "Overwrite file?"));

				/* if user says YES, save file and bail out */
				if (gtk_dialog_run (GTK_DIALOG (cfmdiag)) == GTK_RESPONSE_YES) {

					status = rig_state_save (filename);

					if (status) {

						/* save function returned non-zero value
						   => show error dialog
						*/
						msgdiag = gtk_message_dialog_new (GTK_WINDOW (grigapp),
										  GTK_DIALOG_MODAL |
										  GTK_DIALOG_DESTROY_WITH_PARENT,
										  GTK_MESSAGE_ERROR,
										  GTK_BUTTONS_OK,
										  _("There was an error saving "\
										    "the settings to:\n\n "\
										    "%s\n\n "\
										    "Examine the log messages "\
										    "for further info."),
										  filename);
						gtk_dialog_run (GTK_DIALOG (msgdiag));
						gtk_widget_destroy (msgdiag);

					}
					done = TRUE;

				} else {
					/* else bail out and re-run file chooser */
					done = FALSE;
				}

				gtk_widget_destroy (cfmdiag);

			} else {

				/* otherwise just save the file and we are done */
				status = rig_state_save (filename);

				if (status) {

					/* save function returned non-zero value
					   => show error dialog
					*/
					msgdiag = gtk_message_dialog_new (GTK_WINDOW (grigapp),
									  GTK_DIALOG_MODAL |
									  GTK_DIALOG_DESTROY_WITH_PARENT,
									  GTK_MESSAGE_ERROR,
									  GTK_BUTTONS_OK,
									  _("There was an error saving "\
									    "the settings to:\n\n "\
									    "%s\n\n "\
									    "Examine the log messages "\
									    "for further info."),
									  filename);
					gtk_dialog_run (GTK_DIALOG (msgdiag));
					gtk_widget_destroy (msgdiag);

				}

				/* set flag so that process can terminate */
				done = TRUE;
			}

			g_free (filename);

		} else {

			/* user pressed CANCEL; terminate process */
			done = TRUE;

		}
	}

	gtk_file_chooser_remove_filter (GTK_FILE_CHOOSER (dialog), filter1);
	gtk_file_chooser_remove_filter (GTK_FILE_CHOOSER (dialog), filter2);
	gtk_widget_destroy (dialog);
}