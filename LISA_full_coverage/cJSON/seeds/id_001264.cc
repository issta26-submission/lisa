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
//<ID> 1264
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *nums = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "nums", nums);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(nums, n1);
    cJSON_AddItemToArray(nums, n2);
    cJSON_AddItemToArray(nums, n3);

    // step 3: Operate
    cJSON *nums_ref = cJSON_GetObjectItem(root, "nums");
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(nums_ref, 1, replacement);
    (void)replaced;
    cJSON *second_item = cJSON_GetArrayItem(nums_ref, 1);
    cJSON_bool is_number = cJSON_IsNumber(second_item);
    (void)is_number;
    double second_value = cJSON_GetNumberValue(second_item);
    (void)second_value;
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}