/*

 Copied from https://github.com/zserge/tray under the MIT License

*/

#ifndef TRAY_H
#define TRAY_H

struct tray_menu;

struct tray
{
  char *icon;
  struct tray_menu *menu;
};

struct tray_menu
{
  char *text;
  int type; //0: Button, 1: Checkbox, 2: Separator, 3: Image, 4: Radio
  int disabled;
  void *context;

  void (*cb)(struct tray_menu *);

  struct tray_menu *submenu;
};

static void tray_update(struct tray *tray);

#include <gtk/gtk.h>
#include <libappindicator/app-indicator.h>

#define TRAY_APPINDICATOR_ID "tray-id"

static AppIndicator *indicator = NULL;
static int loop_result = 0;

static void _tray_menu_cb(GtkMenuItem *item, gpointer data)
{
  (void)item;
  struct tray_menu *m = (struct tray_menu *)data;
  m->cb(m);
}

static GtkMenuShell *_tray_menu(struct tray_menu *m)
{
  GtkMenuShell *menu = (GtkMenuShell *)gtk_menu_new();
  for (; m != NULL && m->text != NULL; m++)
  {
    GtkWidget *item;
    switch (m->type)
    {
    case 0:

      item = gtk_menu_item_new_with_label(m->text);
      if (m->submenu != NULL)
      {
        gtk_menu_item_set_submenu(GTK_MENU_ITEM(item),
                                  GTK_WIDGET(_tray_menu(m->submenu)));
      }
      break;
    case 1:
      if (m->submenu != NULL)
      {
        item = gtk_menu_item_new_with_label(m->text);
        gtk_menu_item_set_submenu(GTK_MENU_ITEM(item),
                                  GTK_WIDGET(_tray_menu(m->submenu)));
      }
      else
      {
        item = gtk_check_menu_item_new_with_label(m->text);
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item), (int)m->context);
      }
      break;
    case 2:
      item = gtk_separator_menu_item_new();
      break;
    case 3:
      item = gtk_image_menu_item_new();
      gtk_image_menu_item_set_image(item, gtk_image_new_from_file(m->context));
    case 4:
      item = gtk_radio_menu_item_new(m->context);
    break;
    }

    gtk_widget_set_sensitive(item, !m->disabled);
    if (m->cb != NULL)
    {
      g_signal_connect(item, "activate", G_CALLBACK(_tray_menu_cb), m);
    }

    gtk_widget_show(item);
    gtk_menu_shell_append(menu, item);
  }
  return menu;
}

static int tray_init(struct tray *tray)
{
  if (gtk_init_check(0, NULL) == FALSE)
  {
    return -1;
  }
  indicator = app_indicator_new(TRAY_APPINDICATOR_ID, tray->icon,
                                APP_INDICATOR_CATEGORY_APPLICATION_STATUS);
  app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);
  tray_update(tray);
  return 0;
}

static int tray_loop(int blocking)
{
  gtk_main_iteration_do(blocking);
  return loop_result;
}

static void tray_update(struct tray *tray)
{
  app_indicator_set_icon(indicator, tray->icon);
  // GTK is all about reference counting, so previous menu should be destroyed
  // here
  app_indicator_set_menu(indicator, GTK_MENU(_tray_menu(tray->menu)));
}

static void tray_exit() { loop_result = -1; }

#endif