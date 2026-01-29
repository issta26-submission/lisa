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
//<ID> 1528
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
    cJSON *num_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *retrieved_false = (cJSON *)0;
    cJSON *retrieved_total = (cJSON *)0;
    cJSON *added_number_obj = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_to_array = 0;
    cJSON_bool added_arr_to_root = 0;
    cJSON_bool added_false = 0;
    cJSON_bool is_false = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(3.1415);
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    added_to_array = cJSON_AddItemToArray(arr, num_item);
    added_arr_to_root = cJSON_AddItemToObject(root, "items", arr);
    added_false = cJSON_AddItemToObject(root, "active", false_item);
    added_number_obj = cJSON_AddNumberToObject(root, "total", 100.0);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    retrieved_false = cJSON_GetObjectItem(root, "active");
    retrieved_total = cJSON_GetObjectItem(root, "total");
    is_false = cJSON_IsFalse(retrieved_false);

    // step 5: Validate
    validation_score = (int)(printed != (char *)0) + (int)added_to_array + (int)added_arr_to_root + (int)added_false + (int)(added_number_obj != (cJSON *)0) + (int)is_false + (int)(retrieved_total != (cJSON *)0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_total;
    (void)retrieved_false;
    (void)added_number_obj;
    // API sequence test completed successfully
    return 66;
}