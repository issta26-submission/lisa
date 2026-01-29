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
//<ID> 169
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

    // step 2: Configure
    cJSON *num_pi = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(num_pi, 3.141592653589793);
    cJSON_AddItemToArray(numbers, num_pi);
    cJSON *original = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "original", original);
    cJSON_AddItemReferenceToArray(numbers, original);

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(numbers);
    cJSON *first = cJSON_GetArrayItem(numbers, 0);
    cJSON *second = cJSON_GetArrayItem(numbers, 1);
    double v0 = cJSON_GetNumberValue(first);
    double v1 = cJSON_GetNumberValue(second);
    cJSON *sum = cJSON_CreateNumber(v0 + v1);
    cJSON_AddItemToObject(root, "sum", sum);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}