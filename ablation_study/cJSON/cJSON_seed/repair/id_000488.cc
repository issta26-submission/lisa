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
//<ID> 488
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.5);
    cJSON *n3 = cJSON_CreateNumber(5.25);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToArray(values, n3);
    cJSON_AddItemToObject(root, "values", values);
    cJSON *message = cJSON_CreateString("sample");
    cJSON_AddItemToObject(root, "message", message);

    // step 2: Configure
    cJSON *item0 = cJSON_GetArrayItem(values, 0);
    cJSON *item1 = cJSON_GetArrayItem(values, 1);
    cJSON *item2 = cJSON_GetArrayItem(values, 2);
    double v0 = cJSON_GetNumberValue(item0);
    double v1 = cJSON_GetNumberValue(item1);
    double v2 = cJSON_GetNumberValue(item2);
    double sum = v0 + v1 + v2;
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate and Cleanup
    double validated_sum = cJSON_GetNumberValue(sum_item);
    (void)validated_sum;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}