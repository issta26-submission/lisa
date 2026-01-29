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
//<ID> 538
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
    cJSON *str_item = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_raw = (cJSON *)0;
    cJSON *retrieved_str = (cJSON *)0;
    double numbers[3] = {1.5, 2.5, 3.5};
    char *str_val = (char *)0;
    cJSON_bool is_raw = 0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateDoubleArray(numbers, 3);
    str_item = cJSON_CreateString("hello world");
    raw_item = cJSON_CreateRaw("{\"note\":\"raw payload\"}");

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(arr, str_item);
    add_ok2 = cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "meta", raw_item);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "values");
    retrieved_raw = cJSON_GetObjectItem(root, "meta");
    retrieved_str = cJSON_GetArrayItem(retrieved_arr, 3);
    str_val = cJSON_GetStringValue(retrieved_str);
    is_raw = cJSON_IsRaw(retrieved_raw);

    // step 5: Validate
    (void)str_val;
    (void)is_raw;
    (void)add_ok1;
    (void)add_ok2;
    (void)retrieved_arr;
    (void)retrieved_raw;
    (void)retrieved_str;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}