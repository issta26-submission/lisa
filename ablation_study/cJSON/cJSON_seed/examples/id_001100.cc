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
//<ID> 1100
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
    cJSON *meta = cJSON_CreateObject();
    cJSON *n1 = cJSON_CreateNumber(3.0);
    cJSON *n2 = cJSON_CreateNumber(4.5);
    cJSON *n3 = cJSON_CreateNumber(2.5);
    cJSON *btrue = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    cJSON_AddItemToArray(arr, btrue);
    cJSON_bool arr_check = cJSON_IsArray(arr);
    double sum = cJSON_GetNumberValue(n1) + cJSON_GetNumberValue(n2) + cJSON_GetNumberValue(n3);
    cJSON *sum_item = cJSON_AddNumberToObject(meta, "sum", sum);
    cJSON *count_item = cJSON_AddNumberToObject(root, "count", (double)cJSON_GetArraySize(arr));

    // step 3: Operate & Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON *copied_sum = cJSON_CreateNumber(cJSON_GetNumberValue(sum_item));
    cJSON_AddItemToObject(meta, "sum_copy", copied_sum);

    // step 4: Cleanup
    (void)arr_check;
    (void)count_item;
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}