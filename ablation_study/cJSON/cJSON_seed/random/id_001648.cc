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
//<ID> 1648
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
    cJSON *detached_item = (cJSON *)0;
    cJSON *detached_arr = (cJSON *)0;
    char json_buf[] = " { \"alpha\" : [ 10, 20 ] } ";
    cJSON_bool added_arr = 0;
    cJSON_bool added_s1 = 0;
    cJSON_bool added_s2 = 0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("one");
    s2 = cJSON_CreateString("two");

    // step 3: Configure
    added_arr = cJSON_AddItemToObject(root, "numbers", arr);
    added_s1 = cJSON_AddItemToArray(arr, s1);
    added_s2 = cJSON_AddItemToArray(arr, s2);

    // step 4: Operate
    cJSON_Minify(json_buf);
    detached_item = cJSON_DetachItemFromArray(arr, 0);
    detached_arr = cJSON_DetachItemViaPointer(root, arr);
    arr = detached_arr;

    // step 5: Validate
    arr_size = cJSON_GetArraySize(arr);
    validation_score = (int)added_arr + (int)added_s1 + (int)added_s2 + (int)(arr_size == 1) + (int)(detached_item != (cJSON *)0) + (int)(detached_arr != (cJSON *)0) + (int)(json_buf[0] != 0);

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(arr);
    cJSON_Delete(detached_item);
    (void)validation_score;
    (void)s1;
    (void)s2;
    (void)added_arr;
    (void)added_s1;
    (void)added_s2;
    // API sequence test completed successfully
    return 66;
}