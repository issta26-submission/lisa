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
//<ID> 1649
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
    cJSON *a1 = (cJSON *)0;
    cJSON *a2 = (cJSON *)0;
    cJSON *a3 = (cJSON *)0;
    cJSON *detached_ptr = (cJSON *)0;
    cJSON *detached_idx = (cJSON *)0;
    char *json_unformatted = (char *)0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_a1 = 0;
    cJSON_bool added_a2 = 0;
    cJSON_bool added_a3 = 0;
    cJSON_bool added_det1 = 0;
    cJSON_bool added_det2 = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    a1 = cJSON_CreateString("one");
    a2 = cJSON_CreateString("two");
    a3 = cJSON_CreateString("three");

    // step 3: Configure
    added_a1 = cJSON_AddItemToArray(arr, a1);
    added_a2 = cJSON_AddItemToArray(arr, a2);
    added_a3 = cJSON_AddItemToArray(arr, a3);
    added_arr = cJSON_AddItemToObject(root, "numbers", arr);

    // step 4: Operate
    detached_ptr = cJSON_DetachItemViaPointer(arr, a2);
    detached_idx = cJSON_DetachItemFromArray(arr, 0);
    added_det1 = cJSON_AddItemToObject(root, "detached_ptr", detached_ptr);
    added_det2 = cJSON_AddItemToObject(root, "detached_idx", detached_idx);
    json_unformatted = cJSON_PrintUnformatted(root);
    cJSON_Minify(json_unformatted);

    // step 5: Validate
    validation = (int)(json_unformatted != (char *)0)
               + (int)(detached_ptr != (cJSON *)0)
               + (int)(detached_idx != (cJSON *)0)
               + (int)added_arr
               + (int)added_det1
               + (int)added_det2;

    // step 6: Cleanup
    cJSON_free(json_unformatted);
    cJSON_Delete(root);
    (void)validation;
    (void)a1;
    (void)a2;
    (void)a3;
    (void)added_a1;
    (void)added_a2;
    (void)added_a3;
    // API sequence test completed successfully
    return 66;
}