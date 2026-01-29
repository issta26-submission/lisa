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
//<ID> 704
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
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(numbers, n1);
    cJSON_AddItemToArray(numbers, n2);
    cJSON_AddItemToArray(numbers, n3);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(99.5);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(numbers, 1, replacement);
    (void)replaced;

    // step 3: Operate
    cJSON *item0 = cJSON_GetArrayItem(numbers, 0);
    cJSON *item1 = cJSON_GetArrayItem(numbers, 1);
    cJSON *item2 = cJSON_GetArrayItem(numbers, 2);
    double v0 = cJSON_GetNumberValue(item0);
    double v1 = cJSON_GetNumberValue(item1);
    double v2 = cJSON_GetNumberValue(item2);
    double sum = v0 + v1 + v2;
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);

    // step 4: Validate & Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}