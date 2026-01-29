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
//<ID> 214
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *fruits[] = {"apple", "banana", "cherry"};
    cJSON *str_arr = cJSON_CreateStringArray(fruits, 3);
    cJSON_AddItemToObject(root, "fruits", str_arr);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToArray(values, n3);

    // step 2: Configure
    double res_set = cJSON_SetNumberHelper(n2, 42.5);
    cJSON *dup_values = cJSON_Duplicate(values, 1);
    cJSON_AddItemToObject(root, "values_copy", dup_values);
    cJSON *detached = cJSON_DetachItemViaPointer(values, n3);
    cJSON_AddItemToObject(root, "detached_value", detached);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    cJSON *parsed = cJSON_Parse(printed);
    int arr_size = cJSON_GetArraySize(values);
    cJSON *first = cJSON_GetArrayItem(values, 0);
    double first_val_before = cJSON_GetNumberValue(first);
    double res_set2 = cJSON_SetNumberHelper(first, 100.25);
    void *tmp = cJSON_malloc(64);
    memset(tmp, 0, 64);
    int summary = arr_size + (int)first_val_before + (int)res_set + (int)res_set2 + root->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}