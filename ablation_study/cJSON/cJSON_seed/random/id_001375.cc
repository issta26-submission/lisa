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
//<ID> 1375
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
    cJSON *child = (cJSON *)0;
    cJSON *found_arr = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *found_child = (cJSON *)0;
    cJSON *found_name = (cJSON *)0;
    char *json_str = (char *)0;
    int is_array_flag = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("one");
    s2 = cJSON_CreateString("two");
    child = cJSON_CreateObject();

    // step 3: Configure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddStringToObject(child, "name", "childname");
    cJSON_AddItemToObject(root, "child", child);

    // step 4: Operate
    found_arr = cJSON_GetObjectItem(root, "items");
    is_array_flag = (int)cJSON_IsArray(found_arr);
    item0 = cJSON_GetArrayItem(found_arr, 0);
    item1 = cJSON_GetArrayItem(found_arr, 1);
    found_child = cJSON_GetObjectItem(root, "child");
    found_name = cJSON_GetObjectItem(found_child, "name");
    json_str = cJSON_PrintUnformatted(root);
    cJSON_Minify(json_str);

    // step 5: Validate
    validation_score = (int)(root != (cJSON *)0) + is_array_flag + (int)(item0 != (cJSON *)0) + (int)(item1 != (cJSON *)0) + (int)(found_name != (cJSON *)0) + (int)(json_str != (char *)0);

    // step 6: Cleanup
    cJSON_free(json_str);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)found_arr;
    (void)found_child;
    (void)found_name;
    (void)item0;
    (void)item1;
    return 66;
}