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
//<ID> 1361
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *pi_item = cJSON_CreateNumber(3.14159);
    cJSON *name_item = cJSON_CreateString("cJSON_example");
    cJSON *nested = cJSON_CreateObject();
    cJSON *count_item = cJSON_CreateNumber(0.0);

    // step 2: Configure
    cJSON_AddItemToObject(root, "pi", pi_item);
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddItemToObject(root, "nested", nested);
    cJSON_AddItemToObject(nested, "count", count_item);
    cJSON_AddBoolToObject(nested, "enabled", 1);
    cJSON_SetNumberHelper(count_item, 42.0);
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON_AddBoolToObject(root, "root_is_object", root_is_obj);

    // step 3: Operate & Validate
    double pi_val = cJSON_GetNumberValue(pi_item);
    double count_val = cJSON_GetNumberValue(count_item);
    double total = pi_val + count_val;
    cJSON *total_item = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(root, "total", total_item);
    char *name_val = cJSON_GetStringValue(name_item);
    cJSON *name_copy = cJSON_CreateString(name_val);
    cJSON_AddItemToObject(root, "name_copy", name_copy);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}