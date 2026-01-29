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
//<ID> 1488
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
    cJSON *label_node = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    const float numbers[3] = { 1.25f, 2.5f, 3.75f };
    char *label_str = (char *)0;
    int arr_size = 0;
    cJSON_bool added_array = 0;
    cJSON_bool is_raw_first = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object and a float array
    root = cJSON_CreateObject();
    arr = cJSON_CreateFloatArray(numbers, 3);

    // step 3: Configure - attach array to root and add a descriptive string
    added_array = cJSON_AddItemToObject(root, "floats", arr);
    label_node = cJSON_AddStringToObject(root, "label", "float_values");

    // step 4: Operate - inspect array size, get first element, check rawness, and read the label string
    arr_size = cJSON_GetArraySize(arr);
    first_elem = cJSON_GetArrayItem(arr, 0);
    is_raw_first = cJSON_IsRaw(first_elem);
    label_str = cJSON_GetStringValue(label_node);

    // step 5: Validate & compute a small validation score
    validation_score = arr_size + (int)is_raw_first + (int)(label_str != (char *)0) + (int)added_array;

    // step 6: Cleanup - delete the JSON tree and avoid unused warnings
    cJSON_Delete(root);
    (void)validation_score;
    (void)first_elem;
    (void)label_str;
    (void)arr_size;
    (void)added_array;
    (void)is_raw_first;
    // API sequence test completed successfully
    return 66;
}