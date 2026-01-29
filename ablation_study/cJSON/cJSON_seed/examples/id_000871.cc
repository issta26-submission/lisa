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
//<ID> 871
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *str_item = cJSON_CreateString("example");
    cJSON *arr = cJSON_CreateArray();
    cJSON *arr_elem = cJSON_CreateNumber(7.0);

    // step 2: Configure
    cJSON_AddItemToObject(root, "value", num_item);
    cJSON_AddItemToObject(root, "name", str_item);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToArray(arr, arr_elem);

    // step 3: Operate & Validate
    cJSON *fetched_value = cJSON_GetObjectItem(root, "value");
    cJSON_bool is_number = cJSON_IsNumber(fetched_value);
    cJSON *is_number_flag = cJSON_CreateNumber((double)is_number);
    cJSON_AddItemToObject(root, "is_number", is_number_flag);
    cJSON *missing_item = cJSON_GetObjectItem(root, "missing_key");
    cJSON_bool is_invalid = cJSON_IsInvalid(missing_item);
    cJSON *invalid_flag = cJSON_CreateBool(is_invalid);
    cJSON_AddItemToObject(root, "is_invalid", invalid_flag);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}