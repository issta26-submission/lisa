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
//<ID> 1201
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
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    double retrieved_value = 0.0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Setup - create container and items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(42.5);
    str_item = cJSON_CreateString("fuzz_label");
    true_item = cJSON_CreateTrue();

    // step 3: Configure - assemble object: add array, string and true to root
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemToObject(root, "label", str_item);
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 4: Operate - retrieve the number from the array and obtain numeric value and array size
    retrieved_num = cJSON_GetArrayItem(arr, 0);
    retrieved_value = cJSON_GetNumberValue(retrieved_num);
    arr_size = cJSON_GetArraySize(arr);

    // step 5: Validate - compute a small validation score from retrieved values
    validation_score = (int)retrieved_value + arr_size;

    // step 6: Cleanup - delete the root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}