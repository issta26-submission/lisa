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
//<ID> 368
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    double numbers[] = {1.5, 2.5, 3.0};
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateDoubleArray(numbers, 3);
    cJSON_AddItemToObject(root, "values", values);
    cJSON *extra_num = cJSON_CreateNumber(4.25);
    cJSON_AddItemToArray(values, extra_num);

    // step 2: Configure
    int count = cJSON_GetArraySize(values);
    cJSON *item0 = cJSON_GetArrayItem(values, 0);
    cJSON *item1 = cJSON_GetArrayItem(values, 1);
    cJSON *item2 = cJSON_GetArrayItem(values, 2);
    cJSON *item3 = cJSON_GetArrayItem(values, 3);

    // step 3: Operate and Validate
    double v0 = cJSON_GetNumberValue(item0);
    double v1 = cJSON_GetNumberValue(item1);
    double v2 = cJSON_GetNumberValue(item2);
    double v3 = cJSON_GetNumberValue(item3);
    double total = v0 + v1 + v2 + v3;
    cJSON_AddNumberToObject(root, "count", (double)count);
    cJSON_AddNumberToObject(root, "total", total);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}