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
//<ID> 709
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
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *n1 = cJSON_CreateNumber(20.0);
    cJSON *n2 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(numbers, n0);
    cJSON_AddItemToArray(numbers, n1);
    cJSON_AddItemToArray(numbers, n2);
    cJSON_AddItemToObject(root, "numbers", numbers);

    // step 2: Configure
    cJSON *initial_count = cJSON_CreateNumber((double)cJSON_GetArraySize(numbers));
    cJSON_AddItemToObject(root, "initial_count", initial_count);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);

    // step 3: Operate
    cJSON *replacement = cJSON_CreateNumber(99.5);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(numbers, 1, replacement);
    cJSON *replaced_flag = cJSON_CreateNumber((double)replaced);
    cJSON_AddItemToObject(root, "replaced_flag", replaced_flag);

    // step 4: Validate & Cleanup
    int final_count = cJSON_GetArraySize(numbers);
    cJSON_AddItemToObject(root, "final_count", cJSON_CreateNumber((double)final_count));
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}