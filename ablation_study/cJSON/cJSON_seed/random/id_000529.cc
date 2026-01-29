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
//<ID> 529
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
    cJSON *flag_true = (cJSON *)0;
    cJSON *flag_false = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *first_flag = (cJSON *)0;
    const char *title_val = (const char *)0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    cJSON_bool is_flag_true = 0;
    cJSON_bool is_flag_false = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    flag_true = cJSON_CreateTrue();
    flag_false = cJSON_CreateFalse();

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(arr, flag_true);
    add_ok2 = cJSON_AddItemToArray(arr, flag_false);
    add_ok3 = cJSON_AddItemToObject(root, "flags", arr);
    cJSON_AddStringToObject(root, "title", "demo");

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "flags");
    first_flag = cJSON_GetArrayItem(retrieved_arr, 0);
    is_flag_true = cJSON_IsBool(first_flag);
    is_flag_false = cJSON_IsBool(flag_false);
    title_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "title"));

    // step 5: Validate
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)is_flag_true;
    (void)is_flag_false;
    (void)title_val;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}