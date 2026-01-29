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
//<ID> 162
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", array);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(array, n1);
    cJSON *n2 = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(n2, 2.718);
    cJSON_AddItemReferenceToArray(array, n2);
    cJSON *n3 = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToArray(array, n3);

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(array);
    cJSON *summary = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "summary", summary);
    cJSON *countNum = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(countNum, (double)count);
    cJSON_AddItemToObject(summary, "count", countNum);
    cJSON *second = cJSON_GetArrayItem(array, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON *second_copy = cJSON_CreateNumber(second_val);
    cJSON_AddItemToObject(summary, "second_value", second_copy);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}