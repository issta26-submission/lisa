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
//<ID> 230
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
    cJSON *item1 = (cJSON *)0;
    cJSON *item2 = (cJSON *)0;
    cJSON *single = (cJSON *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    cJSON_bool is_arr = (cJSON_bool)0;
    cJSON_bool equal_items = (cJSON_bool)0;
    char *out_text = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure
    item1 = cJSON_CreateString("alpha");
    item2 = cJSON_CreateString("alpha");
    add_ok1 = cJSON_AddItemToArray(arr, item1);
    add_ok2 = cJSON_AddItemToArray(arr, item2);
    single = cJSON_CreateString("single_value");
    cJSON_AddItemToObject(root, "single", single);

    // step 4: Operate
    is_arr = cJSON_IsArray(arr);
    cJSON_AddBoolToObject(root, "items_is_array", is_arr);
    cJSON_AddBoolToObject(root, "added_first", add_ok1);
    cJSON_AddBoolToObject(root, "added_second", add_ok2);

    // step 5: Validate
    equal_items = cJSON_Compare(item1, item2, (cJSON_bool)1);
    cJSON_AddBoolToObject(root, "first_two_equal", equal_items);
    out_text = cJSON_PrintUnformatted(root);
    cJSON_free(out_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}