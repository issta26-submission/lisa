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
//<ID> 922
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *str_item = cJSON_CreateString("alpha");
    cJSON *num_item = cJSON_CreateNumber(1.0);
    cJSON *meta = cJSON_CreateObject();
    cJSON *meta_name = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(meta, "name", meta_name);

    // step 2: Configure
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate & Validate
    cJSON *num_in_array = cJSON_GetArrayItem(arr, 1);
    cJSON_SetNumberHelper(num_in_array, 99.9);
    cJSON *replacement_str = cJSON_CreateString("beta");
    cJSON_ReplaceItemInObject(root, "meta", replacement_str);
    cJSON *got_list = cJSON_GetObjectItem(root, "list");
    cJSON *first_in_list = cJSON_GetArrayItem(got_list, 0);
    cJSON_AddBoolToObject(root, "first_is_string", cJSON_IsString(first_in_list));
    double updated_val = cJSON_GetNumberValue(cJSON_GetArrayItem(got_list, 1));
    cJSON_AddNumberToObject(root, "updated_number", updated_val);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}