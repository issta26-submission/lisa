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
//<ID> 87
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
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(numbers, n1);
    cJSON_AddItemToArray(numbers, n2);

    // step 2: Configure
    cJSON *dup_numbers = cJSON_Duplicate(numbers, 1);
    cJSON_AddItemToObject(root, "backup", dup_numbers);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(dup_numbers, n3);

    // step 3: Operate and Validate
    int size_orig = cJSON_GetArraySize(numbers);
    int size_dup = cJSON_GetArraySize(dup_numbers);
    cJSON *item0 = cJSON_GetArrayItem(numbers, 0);
    cJSON *item1 = cJSON_GetArrayItem(numbers, 1);
    double v0 = cJSON_GetNumberValue(item0);
    double v1 = cJSON_GetNumberValue(item1);
    double sum = v0 + v1;
    cJSON *sum_num = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_num);
    cJSON *orig_count = cJSON_CreateNumber((double)size_orig);
    cJSON_AddItemToObject(root, "orig_count", orig_count);
    cJSON *dup_count = cJSON_CreateNumber((double)size_dup);
    cJSON_AddItemToObject(root, "dup_count", dup_count);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}