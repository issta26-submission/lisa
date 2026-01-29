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
//<ID> 831
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_literal = "{\"name\":\"test\",\"flag\":false}";
    size_t json_len = sizeof("{\"name\":\"test\",\"flag\":false}") - 1;
    cJSON *root = cJSON_ParseWithLength(json_literal, json_len);
    cJSON *arr = cJSON_CreateArray();

    // step 2: Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON_AddRawToObject(root, "payload", "{\"x\":10,\"y\":20}");

    // step 3: Operate
    cJSON_bool is_array = cJSON_IsArray(arr);
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num);
    char *out = cJSON_PrintUnformatted(root);
    double value = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    const char *name = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    (void)is_array;
    (void)value;
    (void)name;
    cJSON_free(out);

    // step 4: Validate & Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}