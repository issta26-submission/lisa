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
//<ID> 236
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *inner = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "inner", inner);

    // step 2: Configure
    cJSON *false_flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(inner, "flag", false_flag);
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(inner, "value", num);

    // step 3: Operate and Validate
    cJSON *entry = cJSON_CreateObject();
    cJSON *entry_name = cJSON_CreateString("entry0");
    cJSON_AddItemToObject(entry, "name", entry_name);
    cJSON_InsertItemInArray(items, 0, entry);
    cJSON *retrieved_inner = cJSON_GetObjectItem(root, "inner");
    cJSON *retrieved_flag = cJSON_GetObjectItem(retrieved_inner, "flag");
    cJSON_bool flag_is_false = cJSON_IsFalse(retrieved_flag);
    cJSON_AddBoolToObject(root, "inner_flag_is_false", flag_is_false);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}