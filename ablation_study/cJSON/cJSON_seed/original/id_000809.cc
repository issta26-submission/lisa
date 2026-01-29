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
//<ID> 809
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
    cJSON *list = cJSON_CreateArray();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *num_item = cJSON_CreateNumber(3.1415);
    cJSON *str_item = cJSON_CreateString("example");

    // step 2: Configure
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON_AddItemToObject(settings, "enabled", true_item);
    cJSON_AddRawToObject(settings, "raw_payload", "{\"embedded\":true,\"n\":10}");
    cJSON_AddItemToObject(root, "items", list);
    cJSON_AddItemToArray(list, num_item);
    cJSON_AddItemToArray(list, str_item);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON *retrieved_settings = cJSON_GetObjectItemCaseSensitive(root, "settings");
    cJSON *retrieved_flag = cJSON_GetObjectItemCaseSensitive(retrieved_settings, "enabled");
    cJSON_bool enabled_true = cJSON_IsTrue(retrieved_flag);
    cJSON *retrieved_raw = cJSON_GetObjectItemCaseSensitive(retrieved_settings, "raw_payload");
    char *raw_value = cJSON_GetStringValue(retrieved_raw);
    (void)raw_value;
    (void)enabled_true;

    // step 4: Validation & Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}