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
//<ID> 593
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
    cJSON *meta = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("fuzzer");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", enabled);

    // step 2: Configure
    cJSON *values = cJSON_CreateArray();
    cJSON *v1 = cJSON_CreateNumber(1.0);
    cJSON *v2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(values, v1);
    cJSON_AddItemToArray(values, v2);
    cJSON_AddItemToObject(root, "values", values);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(child, "inner", cJSON_CreateString("inside"));
    cJSON_AddItemToObject(root, "child", child);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool has_meta = cJSON_HasObjectItem(parsed, "meta");
    cJSON *parsed_meta = cJSON_GetObjectItem(parsed, "meta");
    cJSON *parsed_name = cJSON_GetObjectItem(parsed_meta, "name");
    const char *name_value = cJSON_GetStringValue(parsed_name);
    cJSON *parsed_values = cJSON_GetObjectItem(parsed, "values");
    int values_size = cJSON_GetArraySize(parsed_values);
    cJSON_bool is_invalid_check = cJSON_IsInvalid(parsed_name);

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;has_meta=%d;name=%s;vals=%d;inv=%d",
            version ? version : "null",
            (int)has_meta,
            name_value ? name_value : "null",
            values_size,
            (int)is_invalid_check);
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}