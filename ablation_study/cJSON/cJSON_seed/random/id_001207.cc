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
//<ID> 1207
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    double num_value = 0.0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Setup - create container and items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num = cJSON_CreateNumber(42.5);
    str_item = cJSON_CreateString("hello");
    true_item = cJSON_CreateTrue();

    // step 3: Configure - populate the array and attach it to the root object
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToObject(root, "data", arr);

    // step 4: Operate - extract the numeric value and obtain array size
    num_value = cJSON_GetNumberValue(num);
    arr_size = cJSON_GetArraySize(arr);

    // step 5: Validate - compute a small validation score based on expected contents
    validation_score = (int)(num_value == 42.5) + (arr_size == 3);

    // step 6: Cleanup - delete the root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}