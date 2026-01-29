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
//<ID> 614
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
    cJSON *sref = (cJSON *)0;
    cJSON *n0 = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *retr_arr = (cJSON *)0;
    cJSON *third_item = (cJSON *)0;
    cJSON *first_plus_two = (cJSON *)0;
    cJSON_bool is_false = 0;
    int arr_size = 0;
    double first_val = 0.0;
    double check_val = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    sref = cJSON_CreateStringReference("example-label");
    n0 = cJSON_CreateNumber(1.5);
    n1 = cJSON_CreateNumber(4.5);
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemReferenceToObject(root, "label", sref);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "count", (double)arr_size);
    retr_arr = cJSON_GetObjectItem(root, "values");
    third_item = cJSON_GetArrayItem(retr_arr, 2);
    is_false = cJSON_IsFalse(third_item);
    first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(retr_arr, 0));
    first_plus_two = cJSON_AddNumberToObject(root, "first_plus_two", first_val + 2.0);

    // step 5: Validate
    check_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "first_plus_two"));
    (void)is_false;
    (void)check_val;
    (void)first_plus_two;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}