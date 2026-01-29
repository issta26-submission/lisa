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
//<ID> 2170
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON *name = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *num1 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(values, num1);
    cJSON *num2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(values, num2);
    cJSON *entry = cJSON_CreateObject();
    cJSON *inner = cJSON_CreateString("inner_value");
    cJSON_AddItemToObject(entry, "inner", inner);
    cJSON_AddItemToArray(values, entry);
    cJSON *values_copy = cJSON_Duplicate(values, 1);
    cJSON_AddItemToObject(root, "values_copy", values_copy);
    cJSON_DeleteItemFromObject(root, "values");

    // step 3: Operate and Validate
    cJSON *got_copy = cJSON_GetObjectItem(root, "values_copy");
    cJSON_bool copy_is_array = cJSON_IsArray(got_copy);
    cJSON *copy_flag = cJSON_CreateBool(copy_is_array);
    cJSON_AddItemToObject(meta, "copy_is_array", copy_flag);
    cJSON *got_name = cJSON_GetObjectItem(root, "name");
    char *name_val = cJSON_GetStringValue(got_name);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (name_val && name_val[0]) ? name_val[0] : '\0';
    buffer[2] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_DeleteItemFromObject(root, "name");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}