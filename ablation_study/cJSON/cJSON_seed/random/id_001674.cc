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
//<ID> 1674
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
    cJSON *detached = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool is_false = 0;
    int arr_size = 0;
    double val0 = 0.0;
    double val1 = 0.0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(3.14);
    num1 = cJSON_CreateNumber(2.0);
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToObject(root, "data", arr);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    val0 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    val1 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    is_false = cJSON_IsFalse(cJSON_GetArrayItem(arr, 2));
    detached = cJSON_DetachItemFromObjectCaseSensitive(root, "data");
    printed = cJSON_PrintUnformatted(detached);

    // step 5: Validate
    validation = (int)(arr_size == 3) + (int)(val0 == 3.14) + (int)(val1 == 2.0) + (int)is_false + (int)(printed != (char *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)num0;
    (void)num1;
    (void)false_item;
    (void)detached;
    (void)is_false;
    (void)arr_size;
    (void)val0;
    (void)val1;
    // API sequence test completed successfully
    return 66;
}