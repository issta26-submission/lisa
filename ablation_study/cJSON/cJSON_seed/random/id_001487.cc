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
//<ID> 1487
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
    cJSON *item2 = (cJSON *)0;
    char *str0 = (char *)0;
    char *printed = (char *)0;
    cJSON_bool is_raw0 = 0;
    cJSON_bool is_raw1 = 0;
    int arr_size = 0;
    int validation_score = 0;
    float values[3];

    // step 2: Initialize - populate float buffer and create JSON structures
    values[0] = 1.25f;
    values[1] = 2.5f;
    values[2] = -3.75f;
    root = cJSON_CreateObject();
    arr = cJSON_CreateFloatArray(values, 3);
    cJSON_AddItemToObject(root, "floats", arr);

    // step 3: Operate - inspect array and its items
    arr_size = cJSON_GetArraySize(arr);
    item0 = cJSON_GetArrayItem(arr, 0);
    item1 = cJSON_GetArrayItem(arr, 1);
    item2 = cJSON_GetArrayItem(arr, 2);
    is_raw0 = cJSON_IsRaw(item0);
    is_raw1 = cJSON_IsRaw(item1);
    str0 = cJSON_GetStringValue(item0);

    // step 4: Validate - render and compute a small validation score
    printed = cJSON_PrintUnformatted(root);
    validation_score = arr_size + (int)(item0 != (cJSON *)0) + (int)(item1 != (cJSON *)0) + (int)(item2 != (cJSON *)0) + (int)is_raw0 + (int)is_raw1 + (int)(str0 != (char *)0) + (int)(printed != (char *)0);

    // step 5: Cleanup - free printed buffer and delete JSON tree
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)arr_size;
    (void)item0;
    (void)item1;
    (void)item2;
    (void)str0;
    (void)is_raw0;
    (void)is_raw1;
    (void)values;
    // API sequence test completed successfully
    return 66;
}