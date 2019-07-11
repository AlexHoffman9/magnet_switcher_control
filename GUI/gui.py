import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk
import sys

class GtkHandler:
    def __init__(self):
        self.builder = Gtk.Builder()
        self.builder.add_from_file("form1.glade")
        self.builder.connect_signals(self)
        self.window = self.builder.get_object("window1")
        self.window.show()

    def onQuit(self, window, data=None):
        sys.exit(0)

    def start(self, switch, data=None):
        #self.
        print("START")
        
    def stop(self, switch, data=None):
        print("STOP")

    def freq_change(self, box, data=None):
        print("FREQ " + str(self.builder.get_object("freq_button").get_value()))

    def reverse(self, button, data=None):
        if button.get_active():
            print("DIR -")
        else:
            print("DIR +")


if __name__ == "__main__":
    myclass = GtkHandler()
    Gtk.main()

