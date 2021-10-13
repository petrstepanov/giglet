/* main.c
 *
 * Copyright 2021 Petr Stepanov
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <glib/gi18n.h>
#include <gtk/gtk-a11y.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>

#include "giglet-config.h"
#include "giglet-window.h"

int
load_fonts(GtkWindow *window)
{
  // Load list of figlet fonts
  // See example here: https://cpp.hotexamples.com/examples/-/-/g_spawn_command_line_async/cpp-g_spawn_command_line_async-function-examples.html
  gchar *command = g_strdup_printf ("showfigfonts");
  gchar *standard_output;
  gchar *standard_error;
  GError *error = NULL;
  gboolean success = g_spawn_command_line_sync (command, &standard_output, &standard_error, NULL, &error);

  // TODO: figure why i get error here
  if (!success){
    g_printerr("Couldn't execute command \"%s\"\n%s", command, standard_error);
    // g_free (command);
    // return -1;
  }
  g_message("Command \"%s\"executed successffully", command);
  g_free (command);

  // Read template UI from file
  GtkBuilder* builder = gtk_builder_new_from_resource ("font-preview.glade");
  // Obtain child widget
  // GtkWidget *fontPreview = GTK_WIDGET(gtk_builder_get_object (builder, "font_preview"));
  // Add child to parent
  // gtk_container_add (GTK_CONTAINER (window), fontPreview);
  g_object_unref (builder);

  return 0;
  // g_log_writer_standard_streams("asdasdasd")
}

static void
on_activate (GtkApplication *app)
{
	GtkWindow *window;

	/* It's good practice to check your parameters at the beginning of the
	 * function. It helps catch errors early and in development instead of
	 * by your users.
	 */
	g_assert (GTK_IS_APPLICATION (app));

	/* Get the current window or create one if necessary. */
	window = gtk_application_get_active_window (app);
	if (window == NULL)
		window = g_object_new (GIGLET_TYPE_WINDOW,
		                       "application", app,
		                       "default-width", 600,
		                       "default-height", 300,
		                       NULL);

	/* Ask the window manager/compositor to present the window. */
	gtk_window_present (window);

  // Find window font widget container
  //
  load_fonts(window);
}

int main (int argc, char *argv[]){
	g_autoptr(GtkApplication) app = NULL;
	int ret;

	/* Set up gettext translations */
	bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);

	/*
	 * Create a new GtkApplication. The application manages our main loop,
	 * application windows, integration with the window manager/compositor, and
	 * desktop features such as file opening and single-instance applications.
	 */
	app = gtk_application_new ("org.gnome.Giglet", G_APPLICATION_FLAGS_NONE);

	/*
	 * We connect to the activate signal to create a window when the application
	 * has been lauched. Additionally, this signal notifies us when the user
	 * tries to launch a "second instance" of the application. When they try
	 * to do that, we'll just present any existing window.
	 *
	 * Because we can't pass a pointer to any function type, we have to cast
	 * our "on_activate" function to a GCallback.
	 */
	g_signal_connect (app, "activate", G_CALLBACK (on_activate), NULL);

	/*
	 * Run the application. This function will block until the applicaiton
	 * exits. Upon return, we have our exit code to return to the shell. (This
	 * is the code you see when you do `echo $?` after running a command in a
	 * terminal.
	 *
	 * Since GtkApplication inherits from GApplication, we use the parent class
	 * method "run". But we need to cast, which is what the "G_APPLICATION()"
	 * macro does.
	 */
	ret = g_application_run (G_APPLICATION (app), argc, argv);

	return ret;
}
