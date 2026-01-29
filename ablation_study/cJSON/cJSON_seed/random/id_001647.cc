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
//<ID> 1647
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
    cJSON *detached_via_ptr = (cJSON *)0;
    cJSON *detached_from_array = (cJSON *)0;
    char *json_unformatted = (char *)0;
    cJSON_bool added_array = 0;
    cJSON_bool added_second = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str1 = cJSON_CreateString("first");
    str2 = cJSON_CreateString("second");

    // step 3: Configure
    added_array = cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, str1);
    added_second = cJSON_AddItemToObject(root, "extra", str2);

    // step 4: Operate
    detached_via_ptr = cJSON_DetachItemViaPointer(root, str2);
    detached_from_array = cJSON_DetachItemFromArray(arr, 0);
    json_unformatted = cJSON_PrintUnformatted(root);
    cJSON_Minify(json_unformatted);

    // step 5: Validate
    validation_score = (int)(root != (cJSON *)0)
        + (int)added_array
        + (int)added_second
        + (int)(detached_via_ptr != (cJSON *)0)
        + (int)(detached_from_array != (cJSON *)0)
        + (int)(json_unformatted != (char *)0);

    // step 6: Cleanup
    cJSON_free(json_unformatted);
    cJSON_Delete(detached_via_ptr);
    cJSON_Delete(detached_from_array);
    cJSON_Delete(root);
    (void)validation_score;
    (void)str1;
    (void)str2;
    (void)arr;
    (void)added_array;
    (void)added_second;
    // API sequence test completed successfully
    return 66;
}