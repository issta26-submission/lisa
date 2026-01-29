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
//<ID> 1192
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
    cJSON *t0 = cJSON_CreateTrue();
    cJSON *f1 = cJSON_CreateFalse();
    cJSON *n2 = cJSON_CreateNumber(7.0);

    // step 2: Configure
    cJSON_AddItemToArray(arr, t0);
    cJSON_AddItemToArray(arr, f1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "flags", arr);

    // step 3: Operate & Validate
    cJSON *new_true = cJSON_CreateTrue();
    cJSON_ReplaceItemInArray(arr, 1, new_true);
    cJSON *item0 = cJSON_GetArrayItem(arr, 0);
    cJSON *item1 = cJSON_GetArrayItem(arr, 1);
    cJSON_bool equal01 = cJSON_Compare(item0, item1, 1);
    cJSON_AddItemToObject(root, "equal01", cJSON_CreateBool(equal01));
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}