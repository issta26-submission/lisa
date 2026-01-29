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
//<ID> 2304
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON_AddStringToObject(data, "name", "device-42");
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(data, "values", values);
    cJSON_AddItemToArray(values, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(20.5));
    cJSON_AddItemToArray(values, cJSON_CreateString("third"));

    // step 2: Configure
    cJSON_AddNumberToObject(root, "count", (double)cJSON_GetArraySize(values));
    cJSON_AddStringToObject(root, "status", "ok");
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "owner", "tester");

    // step 3: Operate
    cJSON *name_item = cJSON_GetObjectItem(data, "name");
    cJSON_bool name_is_str = cJSON_IsString(name_item);
    char *name_val = cJSON_GetStringValue(name_item);
    cJSON *values_item = cJSON_GetObjectItem(data, "values");
    cJSON_bool values_is_arr = cJSON_IsArray(values_item);
    cJSON *values_dup = cJSON_Duplicate(values_item, 1);
    cJSON_AddItemToObject(root, "values_copy", values_dup);

    // step 4: Validate and Cleanup
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (name_val && name_val[0]) ? name_val[0] : '\0';
    buffer[2] = values_is_arr ? '1' : '0';
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}