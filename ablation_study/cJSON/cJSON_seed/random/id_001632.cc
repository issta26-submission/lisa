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
//<ID> 1632
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
    cJSON *num_item = (cJSON *)0;
    cJSON_bool added_arr_to_root = 0;
    cJSON_bool added_s2 = 0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("alpha");
    s2 = cJSON_CreateString("hello");

    // step 3: Configure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, cJSON_CreateString("beta"));
    added_arr_to_root = cJSON_AddItemToObject(root, "letters", arr);
    num_item = cJSON_AddNumberToObject(root, "count", (double)cJSON_GetArraySize(arr));
    added_s2 = cJSON_AddItemToObject(root, "greet", s2);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    validation_score = (int)added_arr_to_root
        + (int)added_s2
        + (int)cJSON_IsString(s1)
        + (int)cJSON_IsString(s2)
        + (int)(num_item != (cJSON *)0)
        + (int)(arr_size == 2);

    // step 5: Validate
    (void)validation_score;
    (void)arr_size;
    (void)num_item;

    // step 6: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}