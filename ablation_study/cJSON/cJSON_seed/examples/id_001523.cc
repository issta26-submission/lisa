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
//<ID> 1523
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *item0 = cJSON_CreateNumber(10.0);
    cJSON *item1 = cJSON_CreateString("one");
    cJSON *item2 = cJSON_CreateBool(0);

    // step 2: Configure
    cJSON_bool added0 = cJSON_AddItemToArray(values, item0);
    cJSON_bool added1 = cJSON_AddItemToArray(values, item1);
    cJSON_bool added2 = cJSON_AddItemToArray(values, item2);
    cJSON_bool added_values = cJSON_AddItemToObject(root, "values", values);
    cJSON *flag = cJSON_CreateBool(1);
    cJSON_bool added_flag = cJSON_AddItemToObject(root, "flag", flag);
    (void)added0; (void)added1; (void)added2; (void)added_values; (void)added_flag;

    // step 3: Operate & Validate
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(values, 1, replacement);
    cJSON *v0 = cJSON_GetArrayItem(values, 0);
    cJSON *v1 = cJSON_GetArrayItem(values, 1);
    cJSON *v2 = cJSON_GetArrayItem(values, 2);
    double v0_val = cJSON_GetNumberValue(v0);
    cJSON_bool v1_is_number = cJSON_IsNumber(v1);
    cJSON_bool v1_is_string = cJSON_IsString(v1);
    char *out = cJSON_PrintUnformatted(root);
    (void)replaced; (void)v0_val; (void)v1_is_number; (void)v1_is_string;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}