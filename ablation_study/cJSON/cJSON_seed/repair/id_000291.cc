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
//<ID> 291
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("initial_name");
    cJSON *flag_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(child, "name", name_item);
    cJSON_AddItemToObject(child, "flag", flag_item);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddNumberToObject(root, "count", 42.0);

    // step 2: Configure
    cJSON *extra_num = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(root, "pi_extra", extra_num);

    // step 3: Operate and Validate
    cJSON *child_ptr = cJSON_GetObjectItem(root, "child");
    int is_obj = (int)cJSON_IsObject(child_ptr);
    cJSON *flag_ptr = cJSON_GetObjectItem(child_ptr, "flag");
    int flag_true = (int)cJSON_IsTrue(flag_ptr);
    cJSON *name_ptr = cJSON_GetObjectItem(child_ptr, "name");
    char *name_val = cJSON_GetStringValue(name_ptr);
    (void)name_val;
    cJSON *sum_item = cJSON_CreateNumber((double)(is_obj + flag_true));
    cJSON_AddItemToObject(root, "sum", sum_item);
    cJSON *new_name = cJSON_CreateString("replaced_name");
    cJSON_bool replaced_ok = cJSON_ReplaceItemViaPointer(child_ptr, name_ptr, new_name);
    (void)replaced_ok;
    cJSON *replaced_name_ptr = cJSON_GetObjectItem(child_ptr, "name");
    char *replaced_name_val = cJSON_GetStringValue(replaced_name_ptr);
    (void)replaced_name_val;
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}