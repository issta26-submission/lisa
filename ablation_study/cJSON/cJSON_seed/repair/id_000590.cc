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
//<ID> 590
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *list = cJSON_CreateArray();
    cJSON_bool added_list = cJSON_AddItemToObject(root, "list", list);
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON_AddItemToArray(list, num_item);
    cJSON_AddItemToArray(list, str_item);

    // step 2: Configure
    int list_size = cJSON_GetArraySize(list);
    cJSON *enabled = cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate and Validate
    double base_value = cJSON_GetNumberValue(num_item);
    double computed = (double)list_size + base_value;
    cJSON_AddNumberToObject(root, "computed", computed);
    cJSON_bool enabled_flag = cJSON_IsTrue(enabled);
    cJSON *dup_enabled = cJSON_AddBoolToObject(root, "duplicate_enabled", enabled_flag);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}