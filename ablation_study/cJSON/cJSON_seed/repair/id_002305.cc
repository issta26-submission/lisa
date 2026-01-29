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
//<ID> 2305
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToArray(list, cJSON_CreateString("hello"));
    cJSON_AddItemToArray(list, cJSON_CreateNumber(10.0));
    cJSON_AddItemToObject(root, "list", list);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "label", "test-label");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddTrueToObject(root, "enabled");

    // step 2: Configure
    cJSON_AddStringToObject(root, "device", "alpha");
    cJSON_AddStringToObject(meta, "status", "ok");
    cJSON *list_copy = cJSON_Duplicate(list, 1);
    cJSON_AddItemToObject(root, "list_copy", list_copy);

    // step 3: Operate
    cJSON *got_list = cJSON_GetObjectItem(root, "list");
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_label = cJSON_GetObjectItem(got_meta, "label");
    cJSON_bool label_is_string = cJSON_IsString(got_label);
    char *label_value = cJSON_GetStringValue(got_label);
    cJSON_bool list_is_array = cJSON_IsArray(got_list);
    cJSON_AddItemToObject(root, "label_is_string", cJSON_CreateBool(label_is_string));
    cJSON_AddItemToObject(root, "list_is_array", cJSON_CreateBool(list_is_array));
    char *json = cJSON_PrintUnformatted(root);

    // step 4: Validate and Cleanup
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = list_is_array ? '1' : '0';
    buffer[2] = (label_value && label_value[0]) ? label_value[0] : '\0';
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}