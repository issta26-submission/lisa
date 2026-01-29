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
//<ID> 1071
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"values\":[1.0,2.0,3.0],\"flag\":false}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);

    // step 2: Configure
    cJSON *values = cJSON_GetObjectItem(root, "values");
    cJSON *first = cJSON_GetArrayItem(values, 0);
    cJSON *second = cJSON_GetArrayItem(values, 1);
    double v1 = cJSON_GetNumberValue(first);
    double v2 = cJSON_GetNumberValue(second);
    double new_numbers[3];
    new_numbers[0] = v1 + 10.0;
    new_numbers[1] = v2 + 20.0;
    new_numbers[2] = 99.9;
    cJSON *new_array = cJSON_CreateDoubleArray(new_numbers, 3);

    // step 3: Operate
    cJSON_ReplaceItemInObjectCaseSensitive(root, "values", new_array);
    cJSON *sum_number = cJSON_CreateNumber(v1 + v2);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "flag", sum_number);
    cJSON_AddTrueToObject(root, "confirmed");
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}