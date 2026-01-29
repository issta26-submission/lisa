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
//<ID> 1537
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
    cJSON *child = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *retrieved_child = (cJSON *)0;
    cJSON *retrieved_str = (cJSON *)0;
    const char *name_val = (const char *)0;
    cJSON_bool added_num1 = 0;
    cJSON_bool added_num2 = 0;
    cJSON_bool added_numbers = 0;
    cJSON_bool added_child = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    child = cJSON_CreateObject();
    num1 = cJSON_CreateNumber(10.0);
    num2 = cJSON_CreateNumber(20.0);
    str_item = cJSON_CreateString("hello");

    // step 3: Configure
    added_num1 = cJSON_AddItemToArray(arr, num1);
    added_num2 = cJSON_AddItemToArray(arr, num2);
    added_numbers = cJSON_AddItemToObject(root, "numbers", arr);
    added_child = cJSON_AddItemToObject(child, "greeting", str_item);
    cJSON_AddItemToObject(root, "meta", child);

    // step 4: Operate
    retrieved_child = cJSON_GetObjectItem(root, "meta");
    retrieved_str = cJSON_GetObjectItem(retrieved_child, "greeting");
    name_val = cJSON_GetStringValue(retrieved_str);

    // step 5: Validate
    validation_score = (int)added_num1 + (int)added_num2 + (int)added_numbers + (int)added_child + (int)(retrieved_child != (cJSON *)0) + (int)(retrieved_str != (cJSON *)0) + (int)(name_val != (const char *)0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)name_val;
    (void)retrieved_str;
    (void)retrieved_child;
    (void)num1;
    (void)num2;
    (void)str_item;
    (void)child;
    (void)arr;
    // API sequence test completed successfully
    return 66;
}