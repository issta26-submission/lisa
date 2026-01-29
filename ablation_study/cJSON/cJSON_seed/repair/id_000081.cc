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
//<ID> 81
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
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(numbers, n1);
    cJSON *n2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(numbers, n2);

    // step 2: Configure
    cJSON *dup_numbers = cJSON_Duplicate(numbers, 1);
    cJSON_AddItemToObject(root, "backup", dup_numbers);
    cJSON *n3 = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(dup_numbers, n3);

    // step 3: Operate and Validate
    int size_orig = cJSON_GetArraySize(numbers);
    int size_dup = cJSON_GetArraySize(dup_numbers);
    double val0 = cJSON_GetNumberValue(cJSON_GetArrayItem(numbers, 0));
    double val_dup_last = cJSON_GetNumberValue(cJSON_GetArrayItem(dup_numbers, size_dup - 1));
    cJSON_AddNumberToObject(root, "orig_count", (double)size_orig);
    cJSON_AddNumberToObject(root, "dup_count", (double)size_dup);
    cJSON_AddNumberToObject(root, "first_value", val0);
    cJSON_AddNumberToObject(root, "dup_last_value", val_dup_last);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}