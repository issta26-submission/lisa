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
//<ID> 1522
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
    cJSON *num0 = cJSON_CreateNumber(1.0);
    cJSON *str1 = cJSON_CreateString("hello");
    cJSON *num2 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_bool added = cJSON_AddItemToObject(root, "data", arr);

    // step 2: Configure
    char *printed_before = cJSON_PrintUnformatted(root);
    cJSON *replacement_bool = cJSON_CreateBool(0);

    // step 3: Operate & Validate
    cJSON *item1 = cJSON_GetArrayItem(arr, 1);
    cJSON_bool item1_is_string = cJSON_IsString(item1);
    cJSON *item0 = cJSON_GetArrayItem(arr, 0);
    cJSON_bool item0_is_number = cJSON_IsNumber(item0);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 1, replacement_bool);
    cJSON *item1_after = cJSON_GetArrayItem(arr, 1);
    cJSON_bool item1_after_is_number = cJSON_IsNumber(item1_after);

    // step 4: Cleanup
    cJSON_free(printed_before);
    (void)added;
    (void)item1_is_string;
    (void)item0_is_number;
    (void)replaced;
    (void)item1_after_is_number;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}