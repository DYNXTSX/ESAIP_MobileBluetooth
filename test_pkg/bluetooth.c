#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
//gcc `pkg-config --cflags gtk+-3.0` -o programme bluetooth.c `pkg-config --libs gtk+-3.0`

static GtkWidget *appareil, *label2;
static GtkWidget *comboH, *comboM;

void reloadBluetooth(GtkWidget* pWidget, gpointer data)
{
   gchar *newText = "Appareil Bluetooth : <b>";
   newText += "ABDEL";
   newText += "</b>";
   gtk_label_set_markup(GTK_LABEL(appareil), newText);
}

int main(int argc, char *argv[]) {
   GtkWidget *window;
   GtkWidget *table, *table2, *table3, *table4, *table5, *table6;

   GtkWidget *frame1, *frame2;
   GtkWidget *button1, *button2, *button3;
   GtkWidget *hbox, *hbox2;
   GtkWidget *halign, *halign2;

   gtk_init(&argc, &argv);

   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
   gtk_window_set_default_size(GTK_WINDOW(window), 300, 500);
   gtk_window_set_title(GTK_WINDOW(window), "GtkStatusbar");

   gtk_container_set_border_width(GTK_CONTAINER(window), 10);

   table = gtk_table_new(2, 1, TRUE);
   gtk_table_set_row_spacings(GTK_TABLE(table), 10);
   gtk_table_set_col_spacings(GTK_TABLE(table), 10);
   gtk_container_add(GTK_CONTAINER(window), table);

   // Creation des deux sections
   frame1 = gtk_frame_new("Partie Connection");
   gtk_frame_set_shadow_type(GTK_FRAME(frame1), GTK_SHADOW_OUT);
   frame2 = gtk_frame_new("Partie Actionnement");
   gtk_frame_set_shadow_type(GTK_FRAME(frame2), GTK_SHADOW_OUT);

   //Disposition des sections
   gtk_table_attach_defaults(GTK_TABLE(table), frame1, 0, 1, 0, 1);
   gtk_table_attach_defaults(GTK_TABLE(table), frame2, 0, 1, 1, 2);


   //Partie haute
   table2 = gtk_table_new(1, 1, TRUE);
   gtk_table_set_row_spacings(GTK_TABLE(table2), 10);
   gtk_table_set_col_spacings(GTK_TABLE(table2), 10);
   gtk_container_add(frame1, table2);

   gchar *str = "Appareil Bluetooth : <b>ALEXANDRE</b>";
   appareil = gtk_label_new(NULL);
   gtk_label_set_markup(GTK_LABEL(appareil), str);

   button3 = gtk_button_new_with_label("Actualiser");
   gtk_widget_set_size_request(button3, 70, 50 );

   gtk_table_attach_defaults(GTK_TABLE(table2), GTK_LABEL(appareil), 0, 1, 0, 1);
   gtk_table_attach_defaults(GTK_TABLE(table2), button3, 0, 1, 1, 2);


   //Partie basse
   table4 = gtk_table_new(2, 1, TRUE);
   gtk_table_set_row_spacings(GTK_TABLE(table4), 10);
   gtk_table_set_col_spacings(GTK_TABLE(table4), 10);
   gtk_container_add(frame2, table4);

   table3 = gtk_table_new(1, 2, TRUE);
   gtk_table_set_row_spacings(GTK_TABLE(table3), 10);
   gtk_table_set_col_spacings(GTK_TABLE(table3), 10);
   gtk_table_attach_defaults(GTK_TABLE(table4), GTK_TABLE(table3), 0, 1, 1, 2);

   table5 = gtk_table_new(1, 1, TRUE);
   gtk_table_set_row_spacings(GTK_TABLE(table5), 10);
   gtk_table_set_col_spacings(GTK_TABLE(table5), 10);
   gtk_table_attach_defaults(GTK_TABLE(table4), GTK_TABLE(table5), 0, 1, 0, 1);
   gchar *str2 = "Heure Hebdomadaire : <b>15h30</b>";
   label2 = gtk_label_new(NULL);
   gtk_label_set_markup(GTK_LABEL(label2), str2);
   gtk_table_attach_defaults(GTK_TABLE(table5), GTK_LABEL(label2), 0, 1, 0, 1);

   table6 = gtk_table_new(1, 2, TRUE);
   gtk_table_set_row_spacings(GTK_TABLE(table6), 10);
   gtk_table_set_col_spacings(GTK_TABLE(table6), 10);
   gtk_table_attach_defaults(GTK_TABLE(table5), GTK_TABLE(table6), 0, 1, 1, 2);

   //716
   comboH = gtk_combo_box_text_new();
   gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(comboH), NULL, "Heure");
   gtk_combo_box_set_active(GTK_COMBO_BOX(comboH), 0);

   comboM = gtk_combo_box_text_new();
   gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(comboM), NULL, "Minutes");
   gtk_combo_box_set_active(GTK_COMBO_BOX(comboM), 0);

   gtk_table_attach_defaults(GTK_TABLE(table6), comboH, 0, 1, 0, 1);
   gtk_table_attach_defaults(GTK_TABLE(table6), comboM, 1, 2, 0, 1);


   button1 = gtk_button_new_with_label("Mettre à jour");
   gtk_widget_set_size_request(button1, 70, 50 );
   button2 = gtk_button_new_with_label("Activer un coup");
   gtk_widget_set_size_request(button2, 70, 50 );

   gtk_table_attach_defaults(GTK_TABLE(table3), button1, 0, 1, 0, 1);
   gtk_table_attach_defaults(GTK_TABLE(table3), button2, 1, 2, 0, 1);

   //Signaux
   g_signal_connect((button3),"clicked",G_CALLBACK(reloadBluetooth),NULL);

   gtk_widget_show_all(window);
   gtk_main();

   return 0;
}
