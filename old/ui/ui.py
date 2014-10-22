import pygtk
pygtk.require("2.0")
import gtk
from bdd import *


class HelloWorld:
    def __init__(self):
        interface = gtk.Builder()
        interface.add_from_file('ui.glade')

        self.clipboard = gtk.clipboard_get()
        self.choix = dict() 
        self.choix["station"] = "20"
        self.choix["horodotage"] = "30"
        self.choix["autre"] = ""
        self.choix["temp"] = True
        self.choix["pluie"] = True
        self.choix["neige"] = False

        self.choix["app_deb"] = ""
        self.choix["app_fin"] = ""
        self.choix["val_deb"] = ""
        self.choix["val_fin"] = ""
        
        fic = open("time_framework","r")
        spl = (fic.read()).split("\n")
        
        app_deb = interface.get_object("app_deb")
        app_deb.set_text(spl[0])        
        self.choix["app_deb"] = spl[0]

        app_fin = interface.get_object("app_fin")
        app_fin.set_text(spl[1])        
        self.choix["app_fin"] = spl[1]
        
        val_deb = interface.get_object("val_deb")
        val_deb.set_text(spl[2])        
        self.choix["val_deb"] = spl[2]
        
        val_fin = interface.get_object("val_fin")
        val_fin.set_text(spl[3])        
        self.choix["val_fin"] = spl[3]
        
        spin = interface.get_object("spinbutton1")        
        spin.set_value(1.0)
        interface.connect_signals(self)
    
    def on_mainWindow_destroy(self, widget):
        gtk.main_quit()
        
    def on_station_20_toggled(self,widget):
        if widget.get_active():
            self.choix["station"] = widget.get_label()

    def on_station_21_toggled(self,widget):
        if widget.get_active():
            self.choix["station"] = widget.get_label()
    
    def on_station_22_toggled(self,widget):
        if widget.get_active():
            self.choix["station"] = widget.get_label()
    
    def on_station_23_toggled(self,widget):
        if widget.get_active():
            self.choix["station"] = widget.get_label()
    
    def on_station_31_toggled(self,widget):
        if widget.get_active():
            self.choix["station"] = widget.get_label()
    
    def on_heure_toggled(self,widget):
        if widget.get_active():
            self.choix["horodotage"] = "60"

    def on_30m_toggled(self,widget):
        if widget.get_active():
            self.choix["horodotage"] = "30"

    def on_20m_toggled(self,widget):
        if widget.get_active():
            self.choix["horodotage"] = "20"
    
    def on_10m_toggled(self,widget):
        if widget.get_active():
            self.choix["horodotage"] = "10"
 
    def on_5m_toggled(self,widget):
        if widget.get_active():
            self.choix["horodotage"] = "5"
    
    def on_autre_toggled(self,widget):
        if widget.get_active():
            self.choix["horodotage"] = "autre"
   
    def on_entry1_changed(self,widget):
        self.choix["autre"] = widget.get_text()

    def on_app_deb_changed(self,widget):
        self.choix["app_deb"] = widget.get_text()

    def on_app_fin_changed(self,widget):
        self.choix["app_fin"] = widget.get_text()
    
    def on_val_deb_changed(self,widget):
        self.choix["val_deb"] = widget.get_text()
    
    def on_val_fin_changed(self,widget):
        self.choix["val_fin"] = widget.get_text()
    
    def on_ok_clicked(self,widget):
       genere_donnees(self.choix)
       gtk.main_quit() 

if __name__ == "__main__":
    HelloWorld()
    gtk.main()
