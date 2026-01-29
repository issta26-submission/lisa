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
//<ID> 1667
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
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *replaced_item = (cJSON *)0;
    cJSON *new_value = (cJSON *)0;
    cJSON *got_value = (cJSON *)0;
    char *json_unformatted = (char *)0;
    cJSON_bool replaced = 0;
    int array_size = 0;
    double retrieved_number = 0.0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(1.5);
    num2 = cJSON_CreateNumber(2.5);
    replaced_item = cJSON_CreateNumber(10.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "value", replaced_item);

    // step 4: Operate
    array_size = cJSON_GetArraySize(arr);
    new_value = cJSON_CreateNumber(99.5);
    replaced = cJSON_ReplaceItemInObject(root, "value", new_value);
    got_value = cJSON_GetObjectItem(root, "value");
    retrieved_number = cJSON_GetNumberValue(got_value);
    json_unformatted = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation = (int)(json_unformatted != (char *)0)
               + (int)(array_size == 2)
               + (int)replaced
               + (int)(retrieved_number == 99.5);
    (void)validation;

    // step 6: Cleanup
    cJSON_free(json_unformatted);
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)num1;
    (void)num2;
    (void)replaced_item;
    (void)new_value;
    (void)got_value;
    (void)replaced;
    (void)array_size;
    (void)retrieved_number;
    // API sequence test completed successfully
    return 66;
}