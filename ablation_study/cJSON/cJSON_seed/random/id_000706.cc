#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 706
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = NULL;
    cJSON *settings = NULL;
    cJSON *true_item = NULL;
    cJSON *false_item = NULL;
    cJSON *retr_settings = NULL;
    cJSON *retr_ref = NULL;
    cJSON *retr_on = NULL;
    cJSON *retr_off = NULL;
    char *printed = NULL;
    int on_is_true = 0;
    int off_is_false = 0;
    int ref_is_object = 0;
    int presence_count = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    if (!root) {
        return 0;
    }

    settings = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToObject(settings, "enabled", true_item);
    cJSON_AddItemToObject(settings, "disabled", false_item);
    cJSON_AddItemToObject(root, "settings", settings);

    // Use the dedicated API to add a reference to 'settings' into 'root'.
    // This avoids double-free issues that can occur when creating a separate
    // reference node and adding it with cJSON_AddItemToObject.
    (void)cJSON_AddItemReferenceToObject(root, "settingsRef", settings);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    retr_settings = cJSON_GetObjectItem(root, "settings");
    retr_ref = cJSON_GetObjectItem(root, "settingsRef");
    retr_on = cJSON_GetObjectItem(retr_settings, "enabled");
    retr_off = cJSON_GetObjectItem(retr_settings, "disabled");
    on_is_true = (int)cJSON_IsTrue(retr_on);
    off_is_false = (int)cJSON_IsFalse(retr_off);
    ref_is_object = (int)cJSON_IsObject(retr_ref);

    // step 5: Validate
    presence_count = (printed != NULL) + on_is_true + off_is_false + ref_is_object;
    (void)presence_count;
    (void)retr_settings;
    (void)retr_ref;
    (void)retr_on;
    (void)retr_off;

    // step 6: Cleanup
    if (printed) {
        cJSON_free(printed);
    }
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}