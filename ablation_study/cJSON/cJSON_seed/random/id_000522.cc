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
//<ID> 522
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
    cJSON *s1 = (cJSON *)0;
    cJSON *s2 = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    cJSON *added_str = (cJSON *)0;
    cJSON *got_name = (cJSON *)0;
    cJSON *got_items = (cJSON *)0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    cJSON_bool add_ok4 = 0;
    cJSON_bool is_bool = 0;
    char *name_val = (char *)0;
    int arr_size = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("alpha");
    s2 = cJSON_CreateString("beta");
    bool_item = cJSON_CreateTrue();

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(arr, s1);
    add_ok2 = cJSON_AddItemToArray(arr, s2);
    add_ok3 = cJSON_AddItemToArray(arr, bool_item);

    // step 4: Operate
    add_ok4 = cJSON_AddItemToObject(root, "items", arr);
    added_str = cJSON_AddStringToObject(root, "type", "collection");
    is_bool = cJSON_IsBool(bool_item);

    // step 5: Validate
    got_name = cJSON_GetObjectItem(root, "type");
    name_val = cJSON_GetStringValue(got_name);
    got_items = cJSON_GetObjectItem(root, "items");
    arr_size = cJSON_GetArraySize(got_items);
    (void)added_str;
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)add_ok4;
    (void)is_bool;
    (void)name_val;
    (void)arr_size;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}