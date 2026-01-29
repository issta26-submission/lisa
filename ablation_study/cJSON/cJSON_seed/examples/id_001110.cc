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
//<ID> 1110
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metadata = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *n1 = cJSON_CreateNumber(20.0);
    cJSON *sref = cJSON_CreateStringReference("sensor");

    // step 2: Configure
    cJSON_AddItemToObject(root, "meta", metadata);
    cJSON_AddItemToObject(metadata, "name", sref);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddTrueToObject(root, "active");

    // step 3: Operate & Validate
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    double before = cJSON_GetNumberValue(second);
    cJSON_bool was_number = cJSON_IsNumber(second);
    double updated = cJSON_SetNumberHelper(second, before + 5.5);
    cJSON_AddNumberToObject(root, "updated_value", updated);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}