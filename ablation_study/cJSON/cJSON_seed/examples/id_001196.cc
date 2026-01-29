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
//<ID> 1196
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
    cJSON *n0 = cJSON_CreateNumber(42.0);
    cJSON *f1 = cJSON_CreateFalse();
    cJSON *s2 = cJSON_CreateString("old");
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, f1);
    cJSON_AddItemToArray(arr, s2);

    // step 2: Configure
    cJSON *new_true = cJSON_CreateTrue();
    cJSON_ReplaceItemInArray(arr, 1, new_true);

    // step 3: Operate & Validate
    cJSON_AddItemToObject(root, "array", arr);
    cJSON *arr_dup = cJSON_Duplicate(arr, 1);
    cJSON_bool equal = cJSON_Compare(arr, arr_dup, 1);
    cJSON *eq_num = cJSON_CreateNumber((double)equal);
    cJSON_AddItemToObject(root, "equal", eq_num);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(arr_dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}