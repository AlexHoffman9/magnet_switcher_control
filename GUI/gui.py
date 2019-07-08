import gi
from gi.repository import Gtk
import sys

class GtkHandler:
    def onQuit(self, *a, **kv):
        sys.exit(0)

    def stop(self, *a, **kv): # send stop command
        print("STOP")

    def start(self, *a, **kv): # send start command
        print("START")

    def freq(self, *a, **kv):
        print("FREQ ")


if __name__ == "__main__":
    abuilder = Gtk.Builder()
    abuilder.add_from_file("form1.glade")
    abuilder.connect_signals(GtkHandler)
    form1 = abuilder.get_object("window1")
    form1.show()
    Gtk.main()

