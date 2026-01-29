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
//<ID> 1672
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *third_item = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool is_false = 0;
    double first_val = 0.0;
    int arr_size = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(3.14);
    num1 = cJSON_CreateNumber(2.71);
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToObject(root, "data", arr);

    // step 4: Operate
    third_item = cJSON_GetArrayItem(arr, 2);
    is_false = cJSON_IsFalse(third_item);
    first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    detached = cJSON_DetachItemFromObjectCaseSensitive(root, "data");
    printed = cJSON_PrintUnformatted(root);
    arr_size = cJSON_GetArraySize(arr);

    // step 5: Validate
    validation = (int)is_false + (int)(first_val == 3.14) + (int)(detached == arr) + (int)(arr_size == 3) + (int)(printed != (char *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(detached);
    (void)root;
    (void)arr;
    (void)num0;
    (void)num1;
    (void)false_item;
    (void)third_item;
    (void)detached;
    (void)is_false;
    (void)first_val;
    (void)arr_size;
    // API sequence test completed successfully
    return 66;
}