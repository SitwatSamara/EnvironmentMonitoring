#include <libnotify/notify.h>
#include <stdio.h>


// Function to display an alert
int sendAlert(const char *title, const char *body) {
    // Initialize the notification library
    if (!notify_init("Alert Application")) {
        fprintf(stderr, "Failed to initialize libnotify.\n");
        return 1;
    }

    // Create a notification with the given title and body
    NotifyNotification *notification = notify_notification_new(title, body, NULL);
    if (!notification) {
        fprintf(stderr, "Failed to create notification.\n");
        notify_uninit();
        return 1;
    }

    // Send the notification
    GError *error = NULL;
    if (!notify_notification_show(notification, &error)) {
        fprintf(stderr, "Failed to show notification: %s\n", error->message);
        g_error_free(error);
        g_object_unref(G_OBJECT(notification));
        notify_uninit();
        return 1;
    }

    // Cleanup
    g_object_unref(G_OBJECT(notification));
    printf("Notification sent...\n");
    notify_uninit();
    return 0;
}

