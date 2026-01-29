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
//<ID> 1191
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
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *true_item = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToArray(created_arr, false_item);
    cJSON_AddItemToArray(created_arr, true_item);
    cJSON_AddItemToObject(root, "arr", created_arr);

    // step 3: Operate & Validate
    cJSON *replacement_true = cJSON_CreateTrue();
    cJSON_ReplaceItemInArray(created_arr, 0, replacement_true);
    cJSON *item0 = cJSON_GetArrayItem(created_arr, 0);
    cJSON *item1 = cJSON_GetArrayItem(created_arr, 1);
    cJSON_bool equal = cJSON_Compare(item0, item1, 1);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}