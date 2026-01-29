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
//<ID> 332
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
    cJSON *true_item = (cJSON *)0;
    cJSON *meta = (cJSON *)0;
    cJSON *got_arr = (cJSON *)0;
    cJSON *got_first = (cJSON *)0;
    cJSON *got_second = (cJSON *)0;
    cJSON *dup = (cJSON *)0;
    char *got_str = (char *)0;
    cJSON_bool first_is_string = (cJSON_bool)0;
    cJSON_bool meta_is_object = (cJSON_bool)0;
    cJSON_bool add_ok = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    meta = cJSON_CreateObject();

    // step 3: Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "meta", meta);
    str_item = cJSON_CreateString("alpha");
    add_ok = cJSON_AddItemToArray(arr, str_item);
    true_item = cJSON_CreateTrue();
    add_ok2 = cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToObject(meta, "description", cJSON_CreateString("sample"));

    // step 4: Operate
    got_arr = cJSON_GetObjectItem(root, "items");
    got_first = cJSON_GetArrayItem(got_arr, 0);
    got_second = cJSON_GetArrayItem(got_arr, 1);
    got_str = cJSON_GetStringValue(got_first);
    first_is_string = cJSON_IsString(got_first);
    meta_is_object = cJSON_IsObject(meta);
    dup = cJSON_Duplicate(root, (cJSON_bool)1);

    // step 5: Validate
    (void)got_second;
    (void)add_ok;
    (void)add_ok2;
    (void)got_str;
    (void)first_is_string;
    (void)meta_is_object;
    (void)cJSON_Compare(root, dup, (cJSON_bool)0);

    // step 6: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}