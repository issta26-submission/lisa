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
//<ID> 1673
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
    cJSON *bool_false = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *item2 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added0 = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool is_false = 0;
    double first_val = 0.0;
    int arr_size = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(1.0);
    num1 = cJSON_CreateNumber(2.0);
    bool_false = cJSON_CreateFalse();

    // step 3: Configure
    added0 = cJSON_AddItemToArray(arr, num0);
    added1 = cJSON_AddItemToArray(arr, num1);
    added2 = cJSON_AddItemToArray(arr, bool_false);
    cJSON_AddItemToObject(root, "data", arr);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    item0 = cJSON_GetArrayItem(arr, 0);
    first_val = cJSON_GetNumberValue(item0);
    item2 = cJSON_GetArrayItem(arr, 2);
    is_false = cJSON_IsFalse(item2);
    detached = cJSON_DetachItemFromObjectCaseSensitive(root, "data");
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation = (int)(arr_size == 3) + (int)(first_val == 1.0) + (int)is_false + (int)(detached == arr) + (int)(printed != (char *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    (void)arr;
    (void)num0;
    (void)num1;
    (void)bool_false;
    (void)item0;
    (void)item2;
    (void)added0;
    (void)added1;
    (void)added2;
    (void)is_false;
    (void)first_val;
    (void)arr_size;
    // API sequence test completed successfully
    return 66;
}