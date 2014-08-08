/*
 * main.vala
 * 
 * Copyright 2014 Michael Chen <chenm@chenm-Lenovo-IdeaPad-Y510P>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


using Gtk;

class MainWindow : Window  {

  public static int main (string[] args) 
  {
    Gtk.init(ref args);  //Gtk intialization

    var window = new MainWindow (); //Create a window
    window.destroy.connect (Gtk.main_quit); //Quit app after window is closed
    window.show_all (); //Makes all widgets visible

    Gtk.main(); //Start the main loop

    return 0;
  }

  public MainWindow()
  {
    this.title = "My First App";
    this.border_width = 10;
    this.set_default_size (250, 60);
    this.window_position = WindowPosition.CENTER;
    this.destroy.connect(Gtk.main_quit); //Quit app when window is closed

    var btnClear = new Button.with_label ("Clear Recent Documents");
    btnClear.clicked.connect (clear_history); //Connect event with function
    add(btnClear);  //Add button to window
  }

  public void clear_history () 
  {
    string HOME = Environment.get_home_dir ();

    try {
      Process.spawn_command_line_sync (@"rm $HOME/.local/share/recently-used.xbel");
      Process.spawn_command_line_sync (@"touch $HOME/.local/share/recently-used.xbel");
      messagebox_show ("Success", "Your recent documents history has been cleared");
    }
    catch (Error e){
      string msg = e.message;
      stderr.printf(msg);
      messagebox_show ("Error", msg);
    }
  }

  public void messagebox_show(string title, string message)
  {
    var dialog = new Gtk.MessageDialog(
       null,
       Gtk.DialogFlags.MODAL, 
       Gtk.MessageType.INFO, 
       Gtk.ButtonsType.OK, 
       message);

    dialog.set_title(title);
    dialog.run();
    dialog.destroy();
  }
}
