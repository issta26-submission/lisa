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
//<ID> 787
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char raw_fragment[] = "{\"note\":\"raw\"}";
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_AddArrayToObject(root, "items");

    // step 2: Configure
    cJSON *n = cJSON_CreateNumber(3.14);
    cJSON *t = cJSON_CreateTrue();
    cJSON *r = cJSON_CreateRaw(raw_fragment);
    cJSON_AddItemToArray(array, n);
    cJSON_AddItemToArray(array, t);
    cJSON_AddItemToArray(array, r);

    // step 3: Operate
    int count = cJSON_GetArraySize(array);
    cJSON *first = cJSON_GetArrayItem(array, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *second = cJSON_GetArrayItem(array, 1);
    int second_true = cJSON_IsTrue(second);
    cJSON *third = cJSON_GetArrayItem(array, 2);
    const char *third_raw = cJSON_GetStringValue(third);

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    (void)count;
    (void)first_val;
    (void)second_true;
    (void)third_raw;
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}