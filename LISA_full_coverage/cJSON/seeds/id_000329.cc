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
//<ID> 329
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
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON *n3 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *first_before = cJSON_GetArrayItem(arr, 0);
    cJSON *second_before = cJSON_GetArrayItem(arr, 1);
    cJSON_bool cmp_before = cJSON_Compare(first_before, second_before, 1);
    cJSON *cmp_before_bool = cJSON_CreateBool(cmp_before);
    cJSON_AddItemToObject(meta, "first_eq_second_before", cmp_before_bool);

    // step 3: Operate and Validate
    cJSON_DeleteItemFromArray(arr, 0);
    cJSON *new_first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool cmp_after = cJSON_Compare(second_before, new_first, 1);
    cJSON *cmp_after_bool = cJSON_CreateBool(cmp_after);
    cJSON_AddItemToObject(meta, "second_eq_new_first_after", cmp_after_bool);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}