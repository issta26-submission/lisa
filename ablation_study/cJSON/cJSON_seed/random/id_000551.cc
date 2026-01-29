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
//<ID> 551
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
    cJSON *num_item1 = (cJSON *)0;
    cJSON *num_item2 = (cJSON *)0;
    cJSON *num_item3 = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_item = (cJSON *)0;
    double retrieved_val1 = 0.0;
    double retrieved_val2 = 0.0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    cJSON_bool add_ok4 = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item1 = cJSON_CreateNumber(10.5);
    num_item2 = cJSON_CreateNumber(-3.25);
    num_item3 = cJSON_CreateNumber(42.0);
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(arr, num_item1);
    add_ok2 = cJSON_AddItemToArray(arr, num_item2);
    add_ok3 = cJSON_AddItemToArray(arr, num_item3);
    add_ok4 = cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "active", false_item);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "numbers");
    retrieved_item = cJSON_GetArrayItem(retrieved_arr, 1);
    retrieved_val1 = cJSON_GetNumberValue(retrieved_item);
    retrieved_val2 = cJSON_GetNumberValue(cJSON_GetArrayItem(retrieved_arr, 2));

    // step 5: Validate
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)add_ok4;
    (void)retrieved_val1;
    (void)retrieved_val2;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}