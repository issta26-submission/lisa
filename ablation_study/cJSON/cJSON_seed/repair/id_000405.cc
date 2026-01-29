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
//<ID> 405
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
    cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    cJSON *arr_dup = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "items_copy", arr_dup);
    cJSON *label = cJSON_CreateString("primary");
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate
    cJSON *items = cJSON_GetObjectItem(root, "items");
    int count = cJSON_GetArraySize(items);
    cJSON *second = cJSON_GetArrayItem(items, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON *copied_number = cJSON_Duplicate(second, 0);
    cJSON_AddItemToArray(arr_dup, copied_number);
    cJSON *items_copy = cJSON_GetObjectItem(root, "items_copy");
    int copy_count = cJSON_GetArraySize(items_copy);
    (void)second_val;
    (void)count;
    (void)copy_count;

    // step 4: Validate and Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}