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
//<ID> 1642
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
    cJSON *str1 = (cJSON *)0;
    cJSON *str2 = (cJSON *)0;
    cJSON *num = (cJSON *)0;
    cJSON *detached_via_pointer = (cJSON *)0;
    cJSON *detached_from_array = (cJSON *)0;
    char *json_buffer = (char *)0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str1 = cJSON_CreateString("alpha");
    str2 = cJSON_CreateString("beta");
    num = cJSON_CreateNumber(42.0);

    // step 3: Configure
    add_ok1 = cJSON_AddItemToObject(root, "first", str1);
    add_ok2 = cJSON_AddItemToObject(root, "num", num);
    add_ok3 = cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, str2);

    // step 4: Operate
    json_buffer = cJSON_PrintUnformatted(root);
    cJSON_Minify(json_buffer);
    detached_via_pointer = cJSON_DetachItemViaPointer(root, str1);
    detached_from_array = cJSON_DetachItemFromArray(arr, 0);

    // step 5: Validate
    validation_score = (int)(json_buffer != (char *)0)
        + (int)(detached_via_pointer != (cJSON *)0)
        + (int)(detached_from_array != (cJSON *)0)
        + (int)(cJSON_GetArraySize(arr) == 0)
        + (int)(cJSON_GetObjectItem(root, "first") == (cJSON *)0);

    // step 6: Cleanup
    cJSON_free(json_buffer);
    cJSON_Delete(detached_via_pointer);
    cJSON_Delete(detached_from_array);
    cJSON_Delete(root);

    (void)validation_score;
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;

    // API sequence test completed successfully
    return 66;
}