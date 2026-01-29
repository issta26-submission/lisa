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
//<ID> 1168
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num0 = NULL;
    cJSON *num1 = NULL;
    cJSON *true_item = NULL;
    cJSON *false_item = NULL;
    cJSON *detached = NULL;
    char *printed = NULL;
    int arr_size = 0;
    double first_value = 0.0;
    double second_value = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(3.1415);
    num1 = cJSON_CreateNumber(2.7182);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToObject(root, "numbers", arr);
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", true_item);
    cJSON_AddItemToObject(root, "disabled", false_item);
    detached = cJSON_CreateNumber(42.0);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    first_value = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    second_value = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}