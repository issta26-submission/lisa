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
//<ID> 611
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
    cJSON *str_ref = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON_bool is_false = 0;
    int arr_size = 0;
    double retrieved_constant = 0.0;
    cJSON *tmp = (cJSON *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_ref = cJSON_CreateStringReference("example_label");
    num_item = cJSON_CreateNumber(3.1415);
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToArray(arr, str_ref);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddNumberToObject(root, "constant", 1000.0);
    cJSON_AddItemReferenceToObject(root, "label_ref", str_ref);

    // step 4: Operate
    is_false = cJSON_IsFalse(false_item);
    arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "is_false_flag", (double)is_false);
    cJSON_AddNumberToObject(root, "items_count", (double)arr_size);

    // step 5: Validate
    tmp = cJSON_GetObjectItem(root, "constant");
    retrieved_constant = cJSON_GetNumberValue(tmp);
    (void)retrieved_constant;
    (void)tmp;
    (void)is_false;
    (void)arr_size;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}