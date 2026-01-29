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
//<ID> 1200
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
    cJSON *true_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    double num_val = 0.0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Setup - create root object and an array
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", arr);

    // step 3: Configure - create items and attach to the root/object
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", true_item);
    str_item = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(root, "name", str_item);
    num_item = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToArray(arr, num_item);

    // step 4: Operate - extract the numeric value and the array size
    num_val = cJSON_GetNumberValue(num_item);
    arr_size = cJSON_GetArraySize(arr);

    // step 5: Validate - compute a simple validation score from extracted values
    validation_score = (int)num_val + arr_size;

    // step 6: Cleanup - delete the root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}