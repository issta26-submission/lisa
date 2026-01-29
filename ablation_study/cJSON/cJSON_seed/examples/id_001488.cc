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
//<ID> 1488
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize & Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(2.718);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddNumberToObject(root, "count", 2.0);

    // step 2: Operate & Validate
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *copy = (char *)cJSON_malloc(printed_len + 1);
    memcpy(copy, printed, printed_len + 1);
    cJSON_bool arr_is_array = cJSON_IsArray(arr);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first);

    // step 3: Cleanup
    cJSON_free(copy);
    cJSON_free(printed);
    cJSON_Delete(root);

    (void)arr_is_array;
    (void)first_val;

    // API sequence test completed successfully
    return 66;
}