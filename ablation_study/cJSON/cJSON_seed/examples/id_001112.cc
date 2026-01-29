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
//<ID> 1112
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(1.5);
    cJSON *n1 = cJSON_CreateNumber(2.5);
    cJSON *holder = cJSON_CreateNumber(0.0);
    cJSON *name_ref = cJSON_CreateStringReference("example_name");

    // step 2: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "name", name_ref);
    cJSON_AddTrueToObject(root, "active");
    cJSON_AddItemToObject(root, "computed", holder);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double sum = cJSON_GetNumberValue(first) + cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    double setv = cJSON_SetNumberHelper(holder, sum);
    cJSON_bool holder_is_number = cJSON_IsNumber(holder);
    cJSON_AddNumberToObject(root, "holder_is_number", (double)holder_is_number);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}