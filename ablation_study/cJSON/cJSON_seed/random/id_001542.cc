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
//<ID> 1542
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
    cJSON *str_item = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *note_ptr = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *array_elem = (cJSON *)0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_raw = 0;
    cJSON_bool is_raw = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("first");
    raw_item = cJSON_CreateRaw("RAW_CONTENT");

    // step 3: Configure
    added_arr = cJSON_AddItemToObject(root, "list", arr);
    added_raw = cJSON_AddItemToArray(arr, raw_item);
    note_ptr = cJSON_AddStringToObject(root, "note", "includes_raw");

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "list");
    array_elem = cJSON_GetArrayItem(retrieved_arr, 0);
    is_raw = cJSON_IsRaw(array_elem);

    // step 5: Validate
    validation_score = (int)(root != (cJSON *)0)
        + (int)added_arr
        + (int)added_raw
        + (int)(note_ptr != (cJSON *)0)
        + (int)(retrieved_arr != (cJSON *)0)
        + (int)(array_elem != (cJSON *)0)
        + (int)is_raw;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)str_item;
    (void)array_elem;
    // API sequence test completed successfully
    return 66;
}