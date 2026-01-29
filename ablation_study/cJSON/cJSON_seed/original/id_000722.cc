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
//<ID> 722
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"device\":\"sensor\",\"active\":true,\"values\":[1,2,3]}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    cJSON *device_item = cJSON_GetObjectItem(root, "device");
    cJSON *values_arr = cJSON_GetObjectItem(root, "values");

    // step 2: Configure
    int values_count = cJSON_GetArraySize(values_arr);
    cJSON *count_item = cJSON_CreateNumber((double)values_count);
    cJSON_AddItemToObject(root, "values_count", count_item);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_ReplaceItemInObject(root, "active", false_item);

    // step 3: Operate
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    cJSON *device_copy = cJSON_CreateString(cJSON_GetStringValue(device_item));
    cJSON_AddItemToObject(root, "device_copy", device_copy);

    // step 4: Validate & Cleanup
    int final_count = cJSON_GetArraySize(values_arr);
    cJSON_AddItemToObject(root, "final_count", cJSON_CreateNumber((double)final_count));
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}