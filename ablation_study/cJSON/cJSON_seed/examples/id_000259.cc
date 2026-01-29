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
//<ID> 259
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *const strs[] = { "alpha", "beta", "gamma" };
    cJSON *str_array = cJSON_CreateStringArray(strs, 3);
    cJSON *num = cJSON_CreateNumber(13.37);
    cJSON *label_old = cJSON_CreateString("old_label");
    cJSON *label_new = cJSON_CreateString("new_label");

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "strings", str_array);
    cJSON_AddItemToObjectCS(root, "value", num);
    cJSON_AddItemToObjectCS(root, "label", label_old);

    // step 3: Operate & Validate
    cJSON_bool has_label = cJSON_HasObjectItem(root, "label");
    cJSON *has_label_num = cJSON_CreateNumber((double)has_label);
    cJSON_AddItemToObjectCS(root, "has_label", has_label_num);
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, label_old, label_new);
    cJSON *replaced_flag = cJSON_CreateNumber((double)replaced);
    cJSON_AddItemToObjectCS(root, "label_replaced", replaced_flag);
    double val = cJSON_GetNumberValue(num);
    cJSON *val_copy = cJSON_CreateNumber(val);
    cJSON_AddItemToObjectCS(root, "value_copy", val_copy);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}