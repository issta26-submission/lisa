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
//<ID> 355
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *const strs[] = { "one", "two", "three" };
    cJSON *str_arr = cJSON_CreateStringArray(strs, 3);
    cJSON *num_arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(num_arr, n1);
    cJSON_AddItemToArray(num_arr, n2);
    cJSON_AddItemToObject(root, "strings", str_arr);
    cJSON_AddItemToObject(root, "numbers", num_arr);

    // step 2: Configure
    cJSON_SetNumberHelper(n2, 7.75);
    cJSON *replacement = cJSON_CreateNumber(99.0);
    cJSON_ReplaceItemInArray(num_arr, 0, replacement);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(num_arr, 0);
    cJSON *second = cJSON_GetArrayItem(num_arr, 1);
    double v1 = cJSON_GetNumberValue(first);
    double v2 = cJSON_GetNumberValue(second);
    double sum = v1 + v2;
    cJSON_AddNumberToObject(root, "sum", sum);
    char *printed = cJSON_Print(root);
    cJSON *snapshot = cJSON_CreateString(printed);
    cJSON_AddItemToObject(root, "snapshot", snapshot);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}