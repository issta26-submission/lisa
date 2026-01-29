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
//<ID> 332
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(1.5);
    cJSON *n1 = cJSON_CreateNumber(2.5);
    cJSON *n2 = cJSON_CreateNumber(3.5);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemInArray(arr, 1, replacement);

    // step 3: Operate & Validate
    cJSON *numbers_arr = cJSON_GetObjectItem(root, "numbers");
    cJSON *elem0 = cJSON_GetArrayItem(numbers_arr, 0);
    cJSON *elem1 = cJSON_GetArrayItem(numbers_arr, 1);
    cJSON *elem2 = cJSON_GetArrayItem(numbers_arr, 2);
    double v0 = cJSON_GetNumberValue(elem0);
    double v1 = cJSON_GetNumberValue(elem1);
    double v2 = cJSON_GetNumberValue(elem2);
    double sum = v0 + v1 + v2;
    cJSON_AddNumberToObject(root, "sum", sum);
    char *printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}