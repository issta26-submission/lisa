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
//<ID> 726
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    double numbers[] = {1.5, 2.5, 3.5};
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON *double_array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON *str_item = cJSON_CreateString("example_string");
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *detachable = cJSON_CreateString("to_be_detached");

    // step 2: Configure
    cJSON_AddItemToArray(items, double_array);
    cJSON_AddItemToArray(items, str_item);
    cJSON_AddItemToArray(items, false_item);
    cJSON_AddItemToObject(root, "payload", items);
    cJSON_AddItemToObject(root, "detach_me", detachable);
    (void)version;

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_payload = cJSON_GetObjectItemCaseSensitive(parsed, "payload");
    cJSON *parsed_first_elem = cJSON_GetArrayItem(parsed_payload, 0); // this is the double array
    cJSON *parsed_first_num = cJSON_GetArrayItem(parsed_first_elem, 0);
    double first_value = cJSON_GetNumberValue(parsed_first_num);
    cJSON *parsed_second_elem = cJSON_GetArrayItem(parsed_payload, 1);
    const char *second_value = cJSON_GetStringValue(parsed_second_elem);
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root, "detach_me");
    const char *detached_value = cJSON_GetStringValue(detached);
    (void)first_value;
    (void)second_value;
    (void)detached_value;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    cJSON_Delete(detached);
    // API sequence test completed successfully
    return 66;
}