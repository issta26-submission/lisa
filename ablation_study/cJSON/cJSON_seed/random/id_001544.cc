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
//<ID> 1544
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
    cJSON *item_alpha = (cJSON *)0;
    cJSON *item_beta = (cJSON *)0;
    cJSON *item_gamma = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *added_title_ptr = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *array_elem = (cJSON *)0;
    cJSON *retrieved_raw = (cJSON *)0;
    cJSON_bool added_alpha = 0;
    cJSON_bool added_beta = 0;
    cJSON_bool added_gamma = 0;
    cJSON_bool added_arr_to_obj = 0;
    cJSON_bool is_raw = 0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item_alpha = cJSON_CreateString("alpha");
    item_beta = cJSON_CreateString("beta");
    item_gamma = cJSON_CreateString("gamma");
    raw_item = cJSON_CreateRaw("<<RAW_PAYLOAD>>");

    // step 3: Configure
    added_alpha = cJSON_AddItemToArray(arr, item_alpha);
    added_beta = cJSON_AddItemToArray(arr, item_beta);
    added_gamma = cJSON_AddItemToArray(arr, item_gamma);
    added_arr_to_obj = cJSON_AddItemToObject(root, "items", arr);
    added_title_ptr = cJSON_AddStringToObject(root, "title", "example");
    cJSON_AddItemToObject(root, "payload", raw_item);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "items");
    array_elem = cJSON_GetArrayItem(retrieved_arr, 1);
    retrieved_raw = cJSON_GetObjectItem(root, "payload");
    is_raw = cJSON_IsRaw(retrieved_raw);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (int)added_alpha
        + (int)added_beta
        + (int)added_gamma
        + (int)added_arr_to_obj
        + (int)(added_title_ptr != (cJSON *)0)
        + (int)(array_elem != (cJSON *)0)
        + (int)is_raw
        + (int)(printed != (char *)0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_arr;
    (void)retrieved_raw;
    (void)item_alpha;
    (void)item_beta;
    (void)item_gamma;
    (void)raw_item;
    (void)added_title_ptr;
    // API sequence test completed successfully
    return 66;
}