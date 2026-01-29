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
//<ID> 572
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *str_item = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "name", str_item);
    cJSON *num_item = cJSON_CreateNumber(12.34);
    cJSON_AddItemToObject(root, "value", num_item);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", false_item);

    // step 2: Configure
    char *set_res = cJSON_SetValuestring(str_item, "updated");
    char *got_str = cJSON_GetStringValue(str_item);
    double orig_num = cJSON_GetNumberValue(num_item);
    cJSON_AddNumberToObject(root, "value_scaled", orig_num * 2.0);
    cJSON_AddStringToObject(root, "name_copy_set", set_res);
    cJSON_AddStringToObject(root, "name_copy_get", got_str);

    // step 3: Operate and Validate
    cJSON_bool was_false = cJSON_IsFalse(false_item);
    cJSON_AddBoolToObject(root, "was_false", was_false);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}