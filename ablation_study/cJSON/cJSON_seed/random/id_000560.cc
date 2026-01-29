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
//<ID> 560
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
    cJSON *num_in_array1 = (cJSON *)0;
    cJSON *num_in_array2 = (cJSON *)0;
    cJSON *num_in_object = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    char *json_str = (char *)0;
    double retrieved_value = 0.0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_in_array1 = cJSON_CreateNumber(7.0);
    num_in_array2 = cJSON_CreateNumber(13.5);

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(arr, num_in_array1);
    add_ok2 = cJSON_AddItemToArray(arr, num_in_array2);
    add_ok3 = cJSON_AddItemToObject(root, "numbers", arr);
    num_in_object = cJSON_AddNumberToObject(root, "pi", 3.14159);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "numbers");
    retrieved_num = cJSON_GetObjectItem(root, "pi");
    json_str = cJSON_PrintUnformatted(root);
    retrieved_value = cJSON_GetNumberValue(retrieved_num);

    // step 5: Validate
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)retrieved_arr;
    (void)num_in_object;
    (void)retrieved_value;
    (void)json_str;

    // step 6: Cleanup
    if (json_str) { cJSON_free(json_str); }
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}