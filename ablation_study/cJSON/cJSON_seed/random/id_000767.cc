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
//<ID> 767
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *str1 = (cJSON *)0;
    cJSON *str2 = (cJSON *)0;
    cJSON *copied = (cJSON *)0;
    cJSON *got_item = (cJSON *)0;
    char *got_str = (char *)0;
    cJSON_bool is_root_obj = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str1 = cJSON_CreateString("alpha");
    str2 = cJSON_CreateString("beta");

    // step 3: Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "sequence", arr);
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, str2);

    // step 4: Operate
    got_item = cJSON_GetArrayItem(arr, 1);
    got_str = cJSON_GetStringValue(got_item);
    copied = cJSON_CreateString(got_str);
    cJSON_AddItemToObject(root, "copied_second", copied);
    is_root_obj = cJSON_IsObject(root);

    // step 5: Validate
    validation_score = (root != (cJSON *)0) + (child != (cJSON *)0) + (arr != (cJSON *)0) + (got_item != (cJSON *)0) + (got_str != (char *)0) + (is_root_obj != 0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}