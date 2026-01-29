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
//<ID> 591
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
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *n1 = cJSON_CreateNumber(20.0);
    cJSON *n2 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(numbers, n0);
    cJSON_AddItemToArray(numbers, n1);
    cJSON_AddItemToArray(numbers, n2);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddBoolToObject(meta, "active", 1);
    cJSON *name = cJSON_CreateString("sensor");
    cJSON_AddItemToObject(root, "name", name);

    // step 2: Configure
    cJSON *extra = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(root, "extra", extra);
    char *updated = cJSON_SetValuestring(name, "sensor-updated");
    cJSON_AddStringToObject(root, "name_copy", updated);

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(numbers);
    cJSON *a0 = cJSON_GetArrayItem(numbers, 0);
    cJSON *a1 = cJSON_GetArrayItem(numbers, 1);
    cJSON *a2 = cJSON_GetArrayItem(numbers, 2);
    double sum = cJSON_GetNumberValue(a0) + cJSON_GetNumberValue(a1) + cJSON_GetNumberValue(a2) + cJSON_GetNumberValue(extra);
    double average = sum / (double)count;
    cJSON_AddNumberToObject(meta, "average", average);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}