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
//<ID> 12
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
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, s1);
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, s2);
    cJSON *ver = cJSON_AddNumberToObject(root, "version", 1.2345);
    cJSON_AddItemToObject(root, "list", arr);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("gamma");
    cJSON_bool replace_result = cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON *nullval = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "nullable", nullval);
    cJSON *dup = cJSON_Duplicate(root, 1);

    // step 3: Operate & Validate
    cJSON_bool compare_equal = cJSON_Compare(root, dup, 1);
    cJSON *nullable_item = cJSON_GetObjectItem(root, "nullable");
    cJSON_bool is_null_flag = cJSON_IsNull(nullable_item);
    (void)replace_result;
    (void)compare_equal;
    (void)is_null_flag;
    (void)ver;
    (void)nullable_item;

    // step 4: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}