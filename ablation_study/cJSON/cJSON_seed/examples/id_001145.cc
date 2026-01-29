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
//<ID> 1145
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"arr\":[10,20,30],\"name\":\"example\"}";
    size_t json_len = sizeof(json_text) - 1;
    cJSON *parsed = cJSON_ParseWithLength(json_text, json_len);
    cJSON *extra_parsed = cJSON_Parse("{\"flag\":true}");
    cJSON *root = cJSON_CreateObject();
    cJSON *null_item = cJSON_CreateNull();

    // step 2: Configure
    cJSON *arr = cJSON_GetObjectItem(parsed, "arr");
    cJSON *a0 = cJSON_GetArrayItem(arr, 0);
    cJSON *a1 = cJSON_GetArrayItem(arr, 1);
    cJSON *a2 = cJSON_GetArrayItem(arr, 2);
    double v0 = cJSON_GetNumberValue(a0);
    double v1 = cJSON_GetNumberValue(a1);
    double v2 = cJSON_GetNumberValue(a2);
    cJSON *copied = cJSON_CreateArray();
    cJSON_AddItemToArray(copied, cJSON_CreateNumber(v0));
    cJSON_AddItemToArray(copied, cJSON_CreateNumber(v1));
    cJSON_AddItemToArray(copied, cJSON_CreateNumber(v2));
    cJSON_AddItemToObject(root, "copied_numbers", copied);
    cJSON_AddItemToObject(root, "nullable", null_item);
    cJSON_AddItemToObject(root, "meta", extra_parsed);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(copied, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON_AddNumberToObject(root, "first_plus_one_point_five", first_val + 1.5);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}