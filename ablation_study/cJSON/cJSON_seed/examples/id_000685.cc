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
//<ID> 685
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
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *num_item = cJSON_CreateNumber(123.0);
    cJSON *str_item = cJSON_CreateString("example");

    // step 2: Configure
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *true_dup = cJSON_Duplicate(true_item, 0);
    cJSON_bool true_equal = cJSON_Compare(true_item, true_dup, 1);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    int count = cJSON_GetArraySize(arr);
    cJSON *second_elem = cJSON_GetArrayItem(arr, 1);
    double second_value = cJSON_GetNumberValue(second_elem);
    (void)true_equal;
    (void)count;
    (void)second_value;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(true_dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}