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
//<ID> 665
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *top_array = cJSON_AddArrayToObject(root, "top_array");
    cJSON *child_array = cJSON_CreateArray();
    cJSON_AddItemToArray(top_array, child_array);

    // step 2: Configure
    char *temp_buf = (char *)cJSON_malloc(64);
    memset(temp_buf, 0, 64);
    strcpy(temp_buf, "dynamic_string");
    cJSON *str_const = cJSON_CreateString("const_string");
    cJSON_AddItemToArray(child_array, str_const);
    cJSON *str_dyn = cJSON_CreateString(temp_buf);
    cJSON_AddItemToArray(child_array, str_dyn);
    cJSON *num_item = cJSON_CreateNumber(123.0);
    cJSON_AddItemToArray(child_array, num_item);
    cJSON_free(temp_buf);

    // step 3: Operate and Validate
    cJSON_bool is_first_string = cJSON_IsString(cJSON_GetArrayItem(child_array, 0));
    cJSON_AddBoolToObject(root, "first_is_string", is_first_string);
    cJSON *child_copy = cJSON_Duplicate(child_array, 1);
    cJSON_AddItemToObject(root, "child_copy", child_copy);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}