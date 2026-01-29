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
//<ID> 612
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
    cJSON *str_ref = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *retr_num_obj = (cJSON *)0;
    double attached_number = 0.0;
    double sum_value = 0.0;
    cJSON_bool was_false = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(42.0);
    str_ref = cJSON_CreateStringReference("hello");
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, str_ref);
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemReferenceToObject(root, "array_ref", arr);
    cJSON_AddNumberToObject(root, "attached_number", 3.14);

    // step 4: Operate
    was_false = cJSON_IsFalse(false_item);
    retr_num_obj = cJSON_GetObjectItem(root, "attached_number");
    attached_number = cJSON_GetNumberValue(retr_num_obj);
    sum_value = attached_number + (double)was_false;
    cJSON_AddNumberToObject(root, "sum_with_false_flag", sum_value);

    // step 5: Validate
    (void)cJSON_GetArraySize(cJSON_GetObjectItem(root, "array_ref"));
    (void)cJSON_GetStringValue(cJSON_GetArrayItem(arr, 1));
    (void)was_false;
    (void)attached_number;
    (void)sum_value;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}