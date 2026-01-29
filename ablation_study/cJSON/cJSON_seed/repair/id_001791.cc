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
//<ID> 1791
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON *flags = cJSON_CreateArray();
    cJSON_AddItemToObject(settings, "flags", flags);

    // step 2: Configure
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(flags, true_item);
    cJSON *dup_true = cJSON_Duplicate(true_item, 1);
    cJSON_AddItemToArray(flags, dup_true);
    cJSON *initial_mode = cJSON_CreateFalse();
    cJSON_AddItemToObject(settings, "mode", initial_mode);
    cJSON *replacement_mode = cJSON_CreateTrue();
    cJSON_ReplaceItemInObject(settings, "mode", replacement_mode);
    cJSON *settings_copy = cJSON_Duplicate(settings, 1);
    cJSON_AddItemToObject(root, "settings_copy", settings_copy);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *flags_ref = cJSON_GetObjectItem(settings, "flags");
    int flags_count = cJSON_GetArraySize(flags_ref);
    cJSON *first_flag = cJSON_GetArrayItem(flags_ref, 0);
    cJSON *second_flag = cJSON_GetArrayItem(flags_ref, 1);
    cJSON_bool first_is_true = cJSON_IsTrue(first_flag);
    cJSON_bool second_is_true = cJSON_IsTrue(second_flag);
    cJSON *mode_item = cJSON_GetObjectItem(settings, "mode");
    cJSON_bool mode_is_true = cJSON_IsTrue(mode_item);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (flags_count % 10));
    buffer[2] = (char)('0' + (first_is_true ? 1 : 0));
    buffer[3] = (char)('0' + (second_is_true ? 1 : 0));
    buffer[4] = (char)('0' + (mode_is_true ? 1 : 0));
    (void)buffer;
    (void)first_is_true;
    (void)second_is_true;
    (void)mode_is_true;
    (void)flags_count;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}