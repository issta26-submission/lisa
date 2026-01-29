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
//<ID> 723
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *payload = cJSON_CreateArray();
    double numbers[3] = {1.5, 2.5, 3.5};
    cJSON *dbl_array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON *str_item = cJSON_CreateString("sample_item");
    cJSON *false_in_array = cJSON_CreateFalse();
    cJSON *false_for_object = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToArray(payload, str_item);
    cJSON_AddItemToArray(payload, dbl_array);
    cJSON_AddItemToArray(payload, false_in_array);
    cJSON_AddItemToObject(root, "payload", payload);
    cJSON_AddItemToObject(root, "enabled", false_for_object);
    cJSON_AddItemToObject(root, "label", cJSON_CreateString("test_label"));

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *detached_enabled = cJSON_DetachItemFromObjectCaseSensitive(parsed, "enabled");
    cJSON *parsed_payload = cJSON_GetObjectItemCaseSensitive(parsed, "payload");
    cJSON *first_elem = cJSON_GetArrayItem(parsed_payload, 0);
    const char *first_value = cJSON_GetStringValue(first_elem);
    cJSON_bool is_array = cJSON_IsArray(parsed_payload);
    (void)version;
    (void)first_value;
    (void)is_array;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(detached_enabled);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}