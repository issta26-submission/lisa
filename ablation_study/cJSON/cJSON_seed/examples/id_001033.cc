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
//<ID> 1033
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("device-007");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *values = cJSON_CreateArray();
    cJSON *v1 = cJSON_CreateNumber(1.0);
    cJSON *v2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(values, v1);
    cJSON_AddItemToArray(values, v2);
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON *root_copy = cJSON_Duplicate(root, 1);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", true_item);

    // step 3: Operate & Validate
    cJSON_bool is_true_flag = cJSON_IsTrue(true_item);
    cJSON *is_true_num = cJSON_CreateNumber((double)is_true_flag);
    cJSON_AddItemToObject(root, "active_as_number", is_true_num);
    cJSON *meta_item = cJSON_GetObjectItem(root, "meta");
    cJSON *name_item = cJSON_GetObjectItem(meta_item, "name");
    char *name_str = cJSON_GetStringValue(name_item);
    cJSON *name_copy = cJSON_CreateString(name_str);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    cJSON_bool equal_flag = cJSON_Compare(root, root_copy, 1);
    cJSON *equal_num = cJSON_CreateNumber((double)equal_flag);
    cJSON_AddItemToObject(root, "equal_to_copy", equal_num);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(root_copy);
    return 66; // API sequence test completed successfully
}