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
//<ID> 516
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *int_arr = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *got_arr = (cJSON *)0;
    cJSON *got_str = (cJSON *)0;
    const char *retrieved_cstr = (const char *)0;
    int arr_size = 0;
    cJSON_bool add_ok = 0;
    int accumulator = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    int numbers[3] = {10, 20, 30};
    int_arr = cJSON_CreateIntArray(numbers, 3);
    str_item = cJSON_CreateString("hello");

    // step 3: Configure
    add_ok = cJSON_AddItemToObject(root, "numbers", int_arr);
    add_ok = cJSON_AddItemToObject(root, "greeting", str_item);

    // step 4: Operate
    got_arr = cJSON_GetObjectItem(root, "numbers");
    got_str = cJSON_GetObjectItem(root, "greeting");
    arr_size = cJSON_GetArraySize(got_arr);
    retrieved_cstr = cJSON_GetStringValue(got_str);

    // step 5: Validate
    accumulator = arr_size + (int)retrieved_cstr[0];
    (void)add_ok;
    (void)accumulator;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}