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
//<ID> 705
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize & Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(1.0);
    cJSON *n1 = cJSON_CreateNumber(2.0);
    cJSON *n2 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(numbers, n0);
    cJSON_AddItemToArray(numbers, n1);
    cJSON_AddItemToArray(numbers, n2);
    cJSON_AddItemToObject(root, "numbers", numbers);

    // step 2: Operate
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemInArray(numbers, 1, replacement);

    // step 3: Validate
    int count = cJSON_GetArraySize(numbers);
    cJSON *replaced_item = cJSON_GetArrayItem(numbers, 1);
    double replaced_value = cJSON_GetNumberValue(replaced_item);
    cJSON *replaced_copy = cJSON_CreateNumber(replaced_value);
    cJSON_AddItemToObject(root, "replaced_value", replaced_copy);
    (void)count;
    (void)replaced_value;

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}