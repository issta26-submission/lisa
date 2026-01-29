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
//<ID> 407
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_AddArrayToObject(root, "numbers");
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(array, n1);
    cJSON_AddItemToArray(array, n2);
    cJSON_AddItemToArray(array, n3);

    // step 2: Configure
    cJSON *second = cJSON_GetArrayItem(array, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON *doubled = cJSON_CreateNumber(second_val * 2.0);
    cJSON_AddItemToArray(array, doubled);
    cJSON_bool n3_invalid = cJSON_IsInvalid(n3);
    cJSON *n3_invalid_item = cJSON_CreateBool(n3_invalid);
    cJSON_AddItemToObject(root, "num3_invalid", n3_invalid_item);

    // step 3: Operate
    cJSON_DeleteItemFromArray(array, 0);
    cJSON *replacement = cJSON_CreateString("replaced_numbers");
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "numbers", replacement);

    // step 4: Validate and Cleanup
    cJSON *numbers_lookup = cJSON_GetObjectItemCaseSensitive(root, "numbers");
    cJSON_bool was_array = cJSON_IsArray(numbers_lookup);
    cJSON *was_array_num = cJSON_CreateNumber((double)was_array);
    cJSON_AddItemToObject(root, "numbers_was_array", was_array_num);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}