#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

int addl(int eax, int ebx);

GtkWidget *fenster;
GtkWidget *button1, *button2, *button3, *button4;
GtkWidget *hbox,*vbox;
GtkWidget *EaxBox, *EbxBox,*FlagBox;
GtkWidget *lEax,*lEbx;
GtkWidget *inputEax,*inputEbx;
GtkWidget *CF_Bit,*OF_Bit;    

gint delete_cb(GtkWidget *widget, GdkEvent *event, gpointer *data)
{
    return(TRUE);
}

void ende_cb(GtkObject *object)
{
    gtk_main_quit();
}

int test(int eax, int ebx)
{
    return eax+ebx;
}

void setFlags(int res)
{
    switch(res)
    {
        case 0: 
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(CF_Bit),FALSE);
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(OF_Bit),FALSE);
            break;
        case 1:
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(CF_Bit),FALSE);
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(OF_Bit),TRUE );
            break;
        case 2:
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(CF_Bit),TRUE );
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(OF_Bit),FALSE);
            break;
        case 3:   
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(CF_Bit),TRUE );
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(OF_Bit),TRUE );
    }
}

gint addl_clicked(GtkObject *object, GdkEvent *event, gpointer *data)
{
    /* Parse Input String as Numbers */
    int eax = (int) strtol(gtk_entry_get_text(GTK_ENTRY(inputEax)),NULL,0);
    int ebx = (int) strtol(gtk_entry_get_text(GTK_ENTRY(inputEbx)),NULL,0);

    int res = 0;
    __asm__("movl %1,%%eax;\n"
            "addl %2,%%eax;\n"
            "movl $3,%%eax\n;"
            "jnc carry;"
            "jno of;"
            "jmp ende;"
            "carry: movl $1, %%eax;"
            "jo ende;"
            "of: subl $1, %%eax;"
            "ende: movl %%eax, %0"
    :"=r" (res)
    :"r" (eax), "r" (ebx)
    :"%eax");
    
    setFlags(res);

    return TRUE;
}

gint subl_clicked(GtkObject *object, GdkEvent *event, gpointer *data)
{
    /* Parse Input String as Numbers */
    int eax = (int) strtol(gtk_entry_get_text(GTK_ENTRY(inputEax)),NULL,0);
    int ebx = (int) strtol(gtk_entry_get_text(GTK_ENTRY(inputEbx)),NULL,0);

    int res = 0;
    __asm__("movl %2,%%eax;\n"
            "subl %1,%%eax;\n"
            "movl $3,%%eax\n;"
            "jnc carry2;"
            "jno of2;"
            "jmp ende2;"
            "carry2: movl $1, %%eax;"
            "jo ende;"
            "of2: subl $1, %%eax;"
            "ende2: movl %%eax, %0"
    :"=r" (res)
    :"r" (eax), "r" (ebx)
    :"%eax");
    
    setFlags(res);

    return TRUE;
}

gint reset_clicked(GtkObject *object, GdkEvent *event, gpointer *data)
{
    setFlags(0);
}

int main(int argc, char **argv)
{
    
    /*
     * GTK Widget Initialization
     */
   
    gtk_init(&argc, &argv);
    /* Button Initialization */
    fenster  = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    button1  = gtk_button_new_with_label("Reset Flags");
    button4  = gtk_button_new_with_label("Quit");
    button2  = gtk_button_new_with_label("addl");
    button3  = gtk_button_new_with_label("subl");
    inputEax = gtk_entry_new();
    inputEbx = gtk_entry_new();
    lEax     = gtk_label_new("%eax");
    lEbx     = gtk_label_new("%ebx");
    
    CF_Bit   = gtk_check_button_new_with_label("Carry Flag");
    OF_Bit   = gtk_check_button_new_with_label("OVerflow Flag");
    
    gtk_widget_set_sensitive(CF_Bit,FALSE);    
    gtk_widget_set_sensitive(OF_Bit,FALSE);    
    
    /* Initialize Signals */
    gtk_signal_connect(GTK_OBJECT(fenster), "delete_event", GTK_SIGNAL_FUNC(delete_cb),NULL);
    gtk_signal_connect(GTK_OBJECT(button2), "clicked", GTK_SIGNAL_FUNC(addl_clicked),NULL);
    gtk_signal_connect(GTK_OBJECT(button3), "clicked", GTK_SIGNAL_FUNC(subl_clicked),NULL);
    gtk_signal_connect(GTK_OBJECT(button4), "clicked", GTK_SIGNAL_FUNC(ende_cb),GTK_OBJECT(fenster));
    gtk_signal_connect(GTK_OBJECT(button1), "clicked", GTK_SIGNAL_FUNC(reset_clicked),NULL);

    /* Window Setup */
    gtk_window_set_title(GTK_WINDOW(fenster), "RS - FlagRegister auslesen - Michael Strassberger");
    gtk_window_set_position(GTK_WINDOW(fenster),GTK_WIN_POS_CENTER);
    
    gtk_container_set_border_width(GTK_CONTAINER(fenster),25);

    vbox    = gtk_vbox_new(FALSE,5);
    hbox    = gtk_hbox_new(FALSE,5);
    EaxBox  = gtk_hbox_new(FALSE,5);
    EbxBox  = gtk_hbox_new(FALSE,5);
    FlagBox = gtk_hbox_new(FALSE,5);

    gtk_box_pack_start(GTK_BOX(FlagBox), CF_Bit,     TRUE,  TRUE, 10);
    gtk_box_pack_start(GTK_BOX(FlagBox), OF_Bit,     TRUE,  TRUE, 10);
    gtk_box_pack_start(GTK_BOX(hbox),    button2,    TRUE , TRUE ,10);
    gtk_box_pack_start(GTK_BOX(hbox),    button3,    TRUE , TRUE ,10);
    gtk_box_pack_start(GTK_BOX(hbox),    button1,    TRUE , TRUE ,10);
    gtk_box_pack_start(GTK_BOX(hbox),    button4,    TRUE , TRUE ,10);
    gtk_box_pack_start(GTK_BOX(EaxBox),  lEax,       FALSE, FALSE,10);
    gtk_box_pack_start(GTK_BOX(EaxBox),  inputEax  , FALSE, FALSE,10);
    gtk_box_pack_start(GTK_BOX(EbxBox),  lEbx,       FALSE, FALSE,10);
    gtk_box_pack_start(GTK_BOX(EbxBox),  inputEbx  , FALSE, FALSE,10);
    gtk_box_pack_start(GTK_BOX(vbox),    EaxBox,     TRUE , FALSE,10);
    gtk_box_pack_start(GTK_BOX(vbox),    EbxBox,     TRUE , FALSE,10);
    gtk_box_pack_start(GTK_BOX(vbox),    hbox,       TRUE , FALSE,10);
    gtk_box_pack_start(GTK_BOX(vbox),    FlagBox,    TRUE , FALSE,10);
    gtk_container_add(GTK_CONTAINER(fenster),vbox);
    

    gtk_widget_show_all(fenster);

    gtk_main();

    return 0;
}
