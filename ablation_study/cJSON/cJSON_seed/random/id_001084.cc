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
//<ID> 1084
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
    cJSON *str = (cJSON *)0;
    cJSON *dup_root = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *fetched_str = (cJSON *)0;
    char *printed = (char *)0;
    const char *str_value = (const char *)0;
    cJSON_bool add_ok_arr = (cJSON_bool)0;
    cJSON_bool add_ok_str = (cJSON_bool)0;
    cJSON_bool is_null_arr = (cJSON_bool)0;
    int validation_score = 0;
    float numbers[3] = { 1.0f, 2.0f, 3.0f };

    // step 2: Setup - create root object, a float array and a string
    root = cJSON_CreateObject();
    arr = cJSON_CreateFloatArray(numbers, 3);
    str = cJSON_CreateString("hello");

    // step 3: Configure - attach array and string to the root object
    add_ok_arr = cJSON_AddItemToObject(root, "floats", arr);
    add_ok_str = cJSON_AddItemToObject(root, "greeting", str);

    // step 4: Operate - fetch items and duplicate the root (deep copy)
    fetched_arr = cJSON_GetObjectItem(root, "floats");
    fetched_str = cJSON_GetObjectItem(root, "greeting");
    dup_root = cJSON_Duplicate(root, (cJSON_bool)1);

    // step 5: Validate - use cJSON_IsNull, serialize duplicated tree and derive a score
    is_null_arr = cJSON_IsNull(fetched_arr);
    printed = cJSON_PrintUnformatted(dup_root);
    cJSON_free((void *)printed);
    str_value = cJSON_GetStringValue(fetched_str);
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(str != (cJSON *)0) + (int)(add_ok_arr) + (int)(add_ok_str) + (int)(!is_null_arr) + (int)(dup_root != (cJSON *)0) + (int)(str_value != (const char *)0);
    (void)validation_score;

    // step 6: Cleanup - delete duplicated tree and original root
    cJSON_Delete(dup_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}