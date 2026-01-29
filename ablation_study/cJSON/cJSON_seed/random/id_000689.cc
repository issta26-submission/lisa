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
//<ID> 689
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
    cJSON *obj_item = (cJSON *)0;
    cJSON *arr_item0 = (cJSON *)0;
    cJSON *arr_item1 = (cJSON *)0;
    cJSON *str_in_obj0 = (cJSON *)0;
    cJSON *str_in_obj1 = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_obj_meta = 0;
    cJSON_bool added_str0 = 0;
    cJSON_bool added_str1 = 0;
    int size_before = 0;
    int size_after = 0;
    int presence_count = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "items");
    obj_item = cJSON_CreateObject();
    str_in_obj0 = cJSON_CreateString("hello");
    str_in_obj1 = cJSON_CreateString("world");
    arr_item0 = cJSON_CreateString("first");
    arr_item1 = cJSON_CreateString("second");

    // step 3: Configure
    added_str0 = cJSON_AddItemToObject(obj_item, "greeting0", str_in_obj0);
    added_str1 = cJSON_AddItemToObject(obj_item, "greeting1", str_in_obj1);
    added_obj_meta = cJSON_AddItemToObject(root, "meta", obj_item);
    cJSON_AddItemToArray(arr, arr_item0);
    cJSON_AddItemToArray(arr, arr_item1);

    // step 4: Operate
    size_before = cJSON_GetArraySize(arr);
    printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    size_after = cJSON_GetArraySize(arr);

    // step 5: Validate
    presence_count = (printed != (char *)0) + (arr != (cJSON *)0) + (obj_item != (cJSON *)0) + (size_after - size_before);
    (void)added_obj_meta;
    (void)added_str0;
    (void)added_str1;
    (void)size_before;
    (void)size_after;
    (void)presence_count;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}