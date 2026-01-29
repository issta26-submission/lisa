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
//<ID> 161
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
    cJSON_AddItemToObject(root, "values", values);
    cJSON *num1 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(values, num1);

    // step 2: Configure
    cJSON *num2 = cJSON_CreateNumber(2.0);
    cJSON_SetNumberHelper(num1, 3.14159);
    cJSON_AddItemReferenceToArray(values, num2);

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(values);
    cJSON *item0 = cJSON_GetArrayItem(values, 0);
    cJSON *item1 = cJSON_GetArrayItem(values, 1);
    double v0 = cJSON_GetNumberValue(item0);
    double v1 = cJSON_GetNumberValue(item1);
    double sum = v0 + v1;
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToArray(values, sum_item);
    char *dump = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(dump);
    cJSON_Delete(root);
    cJSON_Delete(num2);

    // API sequence test completed successfully
    return 66;
}