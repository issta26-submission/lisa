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
//<ID> 1198
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *created_arr = cJSON_CreateArray();
    cJSON *num0 = cJSON_CreateNumber(42.0);
    cJSON *num1 = cJSON_CreateNumber(7.5);
    cJSON_AddItemToArray(created_arr, num0);
    cJSON_AddItemToArray(created_arr, num1);
    cJSON_AddItemToObject(root, "arr", created_arr);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateTrue();
    cJSON_ReplaceItemInArray(created_arr, 1, replacement);
    cJSON *dup = cJSON_Duplicate(created_arr, 1);
    cJSON_AddItemToObject(root, "arr_copy", dup);
    cJSON_bool equal = cJSON_Compare(created_arr, dup, 1);

    // step 3: Operate & Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}