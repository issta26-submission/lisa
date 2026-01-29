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
//<ID> 1836
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
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *detached_item = (cJSON *)0;
    cJSON *retrieved_direct = (cJSON *)0;
    double direct_value = 0.0;
    char json_with_spaces[] = " { \"x\": 1 , \"y\" : 2 } ";
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(10.5);
    num2 = cJSON_CreateNumber(20.75);

    // step 3: Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddNumberToObject(root, "direct", 42.0);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "values");
    detached_item = cJSON_DetachItemFromArray(retrieved_arr, 0);
    retrieved_direct = cJSON_GetObjectItem(root, "direct");
    direct_value = cJSON_GetNumberValue(retrieved_direct);
    cJSON_Minify(json_with_spaces);

    // step 5: Validate
    validation = (int)(retrieved_arr != (cJSON *)0) + (int)(detached_item != (cJSON *)0) + (int)(retrieved_direct != (cJSON *)0) + (int)(direct_value == 42.0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(detached_item);
    cJSON_Delete(root);
    (void)num1;
    (void)num2;
    (void)arr;
    (void)retrieved_arr;
    (void)retrieved_direct;
    (void)direct_value;
    (void)json_with_spaces;
    // API sequence test completed successfully
    return 66;
}