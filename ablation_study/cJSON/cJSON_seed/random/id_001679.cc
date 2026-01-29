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
//<ID> 1679
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
    cJSON *falseItem = (cJSON *)0;
    cJSON *first = (cJSON *)0;
    cJSON *second = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool is_false = 0;
    double first_val = 0.0;
    double third_val = 0.0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(3.14);
    falseItem = cJSON_CreateFalse();
    num1 = cJSON_CreateNumber(2.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, falseItem);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToObject(root, "data", arr);

    // step 4: Operate
    first = cJSON_GetArrayItem(arr, 0);
    first_val = cJSON_GetNumberValue(first);
    second = cJSON_GetArrayItem(arr, 1);
    is_false = cJSON_IsFalse(second);
    detached = cJSON_DetachItemFromObjectCaseSensitive(root, "data");
    third_val = cJSON_GetNumberValue(cJSON_GetArrayItem(detached, 2));
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation = (int)(first_val == 3.14) + (int)is_false + (int)(third_val == 2.0) + (int)(printed != (char *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(detached);
    (void)root;
    (void)arr;
    (void)num0;
    (void)num1;
    (void)falseItem;
    (void)first;
    (void)second;
    (void)detached;
    (void)is_false;
    (void)first_val;
    (void)third_val;
    // API sequence test completed successfully
    return 66;
}