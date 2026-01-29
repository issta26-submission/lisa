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
//<ID> 700
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(1.0);
    cJSON *n1 = cJSON_CreateNumber(2.0);
    cJSON *n2 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(numbers, n0);
    cJSON_AddItemToArray(numbers, n1);
    cJSON_AddItemToArray(numbers, n2);
    cJSON_AddItemToObject(root, "numbers", numbers);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(42.5);
    cJSON_ReplaceItemInArray(numbers, 1, replacement);
    cJSON_AddItemToObject(root, "replacement_index", cJSON_CreateNumber(1.0));

    // step 3: Operate & Validate
    int arr_size = cJSON_GetArraySize(numbers);
    cJSON_AddNumberToObject(root, "numbers_count", (double)arr_size);
    cJSON *second = cJSON_GetArrayItem(numbers, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON_AddNumberToObject(root, "second_value", second_val);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}