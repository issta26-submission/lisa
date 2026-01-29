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
//<ID> 322
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(1.0);
    cJSON *str = cJSON_CreateString("greeting");
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(child, "n", num);
    cJSON_AddItemToObject(child, "s", str);
    cJSON_AddItemToObject(child, "nil", null_item);
    cJSON_AddItemToObject(root, "child", child);
    cJSON *arr = cJSON_CreateArray();
    cJSON *num2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "array", arr);

    // step 2: Configure
    cJSON_SetNumberHelper(num, 3.14159);
    cJSON_AddNumberToObject(root, "extra", 10.0);

    // step 3: Operate & Validate
    cJSON *retrieved_num = cJSON_GetObjectItem(child, "n");
    double val_a = cJSON_GetNumberValue(retrieved_num);
    cJSON *arr_second = cJSON_GetArrayItem(arr, 0);
    double val_b = cJSON_GetNumberValue(arr_second);
    cJSON *sum_item = cJSON_CreateNumber(val_a + val_b);
    cJSON_AddItemToObject(root, "sum", sum_item);
    cJSON_DeleteItemFromObjectCaseSensitive(child, "s");
    char *printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}