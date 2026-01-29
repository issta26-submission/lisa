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
//<ID> 1676
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
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool is_false = 0;
    int arr_size = 0;
    double first_val = 0.0;
    double second_val = 0.0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item0 = cJSON_CreateNumber(3.14);
    item1 = cJSON_CreateNumber(2.0);
    bool_item = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "flag", bool_item);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    second_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    is_false = cJSON_IsFalse(cJSON_GetObjectItemCaseSensitive(root, "flag"));
    detached = cJSON_DetachItemFromObjectCaseSensitive(root, "numbers");
    printed = cJSON_Print(root);

    // step 5: Validate
    validation = (int)(arr_size == 2) + (int)(first_val == 3.14) + (int)(second_val == 2.0) + (int)is_false + (int)(detached != (cJSON *)0) + (int)(printed != (char *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)item0;
    (void)item1;
    (void)bool_item;
    (void)detached;
    (void)is_false;
    (void)arr_size;
    (void)first_val;
    (void)second_val;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}