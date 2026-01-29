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
//<ID> 425
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddNumberToObject(config, "scale", 2.5);
    cJSON *label = cJSON_CreateString("example");
    cJSON_AddItemToObject(config, "label", label);
    cJSON_AddItemToObject(root, "config", config);

    // step 2: Configure
    cJSON *items = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(3.0);
    cJSON *n1 = cJSON_CreateNumber(4.0);
    cJSON *n2 = cJSON_CreateNumber(5.0);
    cJSON_AddItemToArray(items, n0);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_bool cfg_is_obj = cJSON_IsObject(config);
    cJSON_AddNumberToObject(root, "config_is_object", (double)cfg_is_obj);

    // step 3: Operate
    cJSON *scale_item = cJSON_GetObjectItem(config, "scale");
    double scale = cJSON_GetNumberValue(scale_item);
    cJSON *it0 = cJSON_GetArrayItem(items, 0);
    cJSON *it1 = cJSON_GetArrayItem(items, 1);
    cJSON *it2 = cJSON_GetArrayItem(items, 2);
    double sum = cJSON_GetNumberValue(it0) + cJSON_GetNumberValue(it1) + cJSON_GetNumberValue(it2);
    double scaled = sum * scale;
    cJSON *result = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(result, scaled);
    cJSON_AddItemToObject(root, "scaled_sum", result);

    // step 4: Validate and Cleanup
    cJSON *retrieved = cJSON_GetObjectItem(root, "scaled_sum");
    double retrieved_value = cJSON_GetNumberValue(retrieved);
    (void)retrieved_value;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}