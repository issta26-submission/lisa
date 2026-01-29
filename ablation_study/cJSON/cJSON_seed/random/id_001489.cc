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
//<ID> 1489
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
    cJSON *first = (cJSON *)0;
    cJSON *second = (cJSON *)0;
    cJSON *third = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    const char *raw_strval = (const char *)0;
    int arr_size = 0;
    cJSON_bool is_raw = 0;
    int validation_score = 0;
    float numbers[2];

    // step 2: Initialize
    root = cJSON_CreateObject();
    numbers[0] = 1.25f;
    numbers[1] = 2.5f;
    arr = cJSON_CreateFloatArray(numbers, 2);

    // step 3: Configure
    cJSON_AddItemToObject(root, "float_array", arr);
    raw_item = cJSON_CreateRaw("\"raw_payload\"");
    cJSON_AddItemToArray(arr, raw_item);

    // step 4: Operate
    first = cJSON_GetArrayItem(arr, 0);
    second = cJSON_GetArrayItem(arr, 1);
    third = cJSON_GetArrayItem(arr, 2);
    arr_size = cJSON_GetArraySize(arr);
    is_raw = cJSON_IsRaw(third);
    raw_strval = cJSON_GetStringValue(third);

    // step 5: Validate
    validation_score = arr_size + (int)is_raw + (int)(raw_strval != (const char *)0) + (int)(first != (cJSON *)0) + (int)(second != (cJSON *)0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)raw_strval;
    (void)first;
    (void)second;
    (void)third;
    (void)raw_item;
    // API sequence test completed successfully
    return 66;
}