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
//<ID> 160
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
    cJSON *num1 = cJSON_CreateNumber(1.0);
    cJSON *num2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, num2);
    cJSON_AddItemReferenceToArray(array, num2);

    // step 3: Operate & Validate
    cJSON_SetNumberHelper(num1, 100.5);
    cJSON *item0 = cJSON_GetArrayItem(array, 0);
    cJSON *item1 = cJSON_GetArrayItem(array, 1);
    cJSON *item2 = cJSON_GetArrayItem(array, 2);
    double v0 = cJSON_GetNumberValue(item0);
    double v1 = cJSON_GetNumberValue(item1);
    double v2 = cJSON_GetNumberValue(item2);
    double total = v0 + v1 + v2;
    cJSON *total_num = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(root, "total", total_num);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}