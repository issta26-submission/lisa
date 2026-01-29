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
//<ID> 552
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
    cJSON *num3 = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *retrieved_item0 = (cJSON *)0;
    cJSON *retrieved_item2 = (cJSON *)0;
    double numbers[3] = {10.0, 20.5, 30.75};
    double read0 = 0.0;
    double read2 = 0.0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    cJSON_bool add_ok4 = 0;
    cJSON_bool add_ok5 = 0;
    cJSON_bool is_false = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(numbers[0]);
    num2 = cJSON_CreateNumber(numbers[1]);
    num3 = cJSON_CreateNumber(numbers[2]);
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(arr, num1);
    add_ok2 = cJSON_AddItemToArray(arr, num2);
    add_ok3 = cJSON_AddItemToArray(arr, num3);
    add_ok4 = cJSON_AddItemToObject(root, "numbers", arr);
    add_ok5 = cJSON_AddItemToObject(root, "valid", false_item);

    // step 4: Operate
    retrieved_item0 = cJSON_GetArrayItem(arr, 0);
    retrieved_item2 = cJSON_GetArrayItem(arr, 2);

    // step 5: Validate
    read0 = cJSON_GetNumberValue(retrieved_item0);
    read2 = cJSON_GetNumberValue(retrieved_item2);
    is_false = cJSON_IsFalse(cJSON_GetObjectItem(root, "valid"));
    (void)read0;
    (void)read2;
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)add_ok4;
    (void)add_ok5;
    (void)is_false;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}