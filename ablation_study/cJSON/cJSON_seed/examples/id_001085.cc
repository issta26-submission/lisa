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
//<ID> 1085
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
    cJSON *n0 = cJSON_CreateNumber(1.0);
    cJSON *n1 = cJSON_CreateNumber(2.0);
    cJSON *n2 = cJSON_CreateNumber(3.0);
    cJSON *n_ins = cJSON_CreateNumber(1.5);
    cJSON *n_rep = cJSON_CreateNumber(2.5);

    // step 2: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON *count_num = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(root, "count", count_num);

    // step 3: Operate & Validate
    cJSON_bool inserted = cJSON_InsertItemInArray(arr, 1, n_ins);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 2, n_rep);
    cJSON *checked = cJSON_GetArrayItem(arr, 2);
    cJSON_bool is_number = cJSON_IsNumber(checked);
    int sz = cJSON_GetArraySize(arr);
    cJSON_SetNumberHelper(count_num, (double)sz);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    (void)inserted;
    (void)replaced;
    (void)is_number;
    (void)checked;
    (void)sz;
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}