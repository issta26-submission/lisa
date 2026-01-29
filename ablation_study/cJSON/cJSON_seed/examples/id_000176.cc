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
//<ID> 176
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *words[] = { "alpha", "beta", "gamma" };
    cJSON *arr = cJSON_CreateStringArray(words, 3);
    cJSON_AddItemToObject(root, "words", arr);
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "info", info);
    cJSON *label = cJSON_CreateString("initial");
    cJSON_AddItemToObject(info, "label", label);

    // step 2: Configure
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    char *second_val = cJSON_GetStringValue(second_item);
    cJSON *new_label = cJSON_CreateString(second_val);

    // step 3: Operate & Validate
    cJSON_bool replaced = cJSON_ReplaceItemInObject(info, "label", new_label);
    char *got_label = cJSON_GetStringValue(cJSON_GetObjectItem(info, "label"));
    const char *label_items[] = { got_label };
    cJSON *label_array = cJSON_CreateStringArray(label_items, 1);
    cJSON_AddItemToObject(root, "label_array", label_array);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}