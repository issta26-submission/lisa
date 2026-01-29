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
//<ID> 1252
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
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *new_num = (cJSON *)0;
    cJSON *arr_ref = (cJSON *)0;
    cJSON_bool str_is_string = 0;
    int size_before = 0;
    int size_after = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object, array and primitive items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(42.0);
    str_item = cJSON_CreateString("payload");

    // step 3: Configure - populate the array with number and string, attach array to root
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "data", arr);

    // step 4: Operate - create an array reference to the array and measure size / type
    arr_ref = cJSON_CreateArrayReference(arr);
    size_before = cJSON_GetArraySize(arr);
    str_is_string = cJSON_IsString(str_item);

    // step 5: Operate continued - add another number to the array and re-measure
    new_num = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(arr, new_num);
    size_after = cJSON_GetArraySize(arr);

    // step 6: Validate and Cleanup - compute a small validation score, free reference and delete root
    validation_score = (int)(root != (cJSON *)0) + size_before + size_after + (int)str_is_string + (int)(arr_ref != (cJSON *)0);
    (void)validation_score;
    cJSON_Delete(arr_ref);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}