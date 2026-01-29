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
//<ID> 696
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
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(data, "values", values);

    // step 2: Configure
    cJSON_AddItemToArray(values, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(30.0));
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(data, "explicit_false", false_item);
    int size = cJSON_GetArraySize(values);
    cJSON_AddBoolToObject(root, "has_values", size > 0);

    // step 3: Operate and Validate
    cJSON_bool is_obj = cJSON_IsObject(data);
    cJSON_AddBoolToObject(root, "data_is_object", is_obj);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}