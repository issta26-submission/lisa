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
//<ID> 1636
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
    cJSON *s3 = (cJSON *)0;
    cJSON_bool added_s1 = 0;
    cJSON_bool added_s2 = 0;
    cJSON_bool added_s3 = 0;
    cJSON_bool added_arr_to_root = 0;
    cJSON_bool added_count = 0;
    cJSON *count_item = (cJSON *)0; // helper to receive returned item pointer
    cJSON_bool is_s1_string = 0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("one");
    s2 = cJSON_CreateString("two");
    s3 = cJSON_CreateString("three");

    // step 3: Configure
    added_s1 = cJSON_AddItemToArray(arr, s1);
    added_s2 = cJSON_AddItemToArray(arr, s2);
    added_s3 = cJSON_AddItemToArray(arr, s3);
    added_arr_to_root = cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    count_item = cJSON_AddNumberToObject(root, "count", (double)arr_size);
    added_count = (count_item != (cJSON *)0) ? 1 : 0;
    is_s1_string = cJSON_IsString(s1);
    validation_score = (int)added_s1 + (int)added_s2 + (int)added_s3 + (int)added_arr_to_root + (int)added_count + (int)is_s1_string + arr_size;

    // step 5: Cleanup
    cJSON_Delete(root);
    (void)validation_score;

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}