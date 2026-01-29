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
//<ID> 225
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"value\": 3.14, \"name\":\"alpha\"}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);
    cJSON *newobj = cJSON_CreateObject();

    // step 2: Configure
    cJSON *val_item = cJSON_GetObjectItem(root, "value");
    double val = cJSON_GetNumberValue(val_item);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *name_str = cJSON_GetStringValue(name_item);
    cJSON_AddStringToObject(newobj, "original_name", name_str);
    cJSON *num_copy = cJSON_CreateNumber(val);
    cJSON_AddItemToObject(newobj, "copied_value", num_copy);
    cJSON_AddStringToObject(newobj, "status", "copied");

    // step 3: Operate
    cJSON_AddItemToObject(root, "copy", newobj);
    cJSON *retrieved_copy = cJSON_GetObjectItem(root, "copy");
    cJSON *retrieved_num = cJSON_GetObjectItem(retrieved_copy, "copied_value");
    double retrieved_val = cJSON_GetNumberValue(retrieved_num);
    cJSON_AddNumberToObject(root, "retrieved_val", retrieved_val);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}