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
//<ID> 738
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *str_item = NULL;
    cJSON *num_item = NULL;
    cJSON *temp_item = NULL;
    cJSON *detached = NULL;
    cJSON *retrieved_list = NULL;
    int validation_score = 0;
    cJSON_bool added_list = 0;
    cJSON_bool added_temp = 0;
    cJSON_bool added_str_to_array = 0;
    cJSON_bool added_num_to_array = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("alpha");
    num_item = cJSON_CreateNumber(42.0);
    temp_item = cJSON_CreateString("temporary");

    // step 3: Configure
    added_list = cJSON_AddItemToObject(root, "list", arr);
    added_str_to_array = cJSON_AddItemToArray(arr, str_item);
    added_num_to_array = cJSON_AddItemToArray(arr, num_item);
    added_temp = cJSON_AddItemToObject(root, "temp", temp_item);

    // step 4: Operate
    retrieved_list = cJSON_GetObjectItem(root, "list");
    detached = cJSON_DetachItemFromObject(root, "temp");

    // step 5: Validate
    validation_score = (retrieved_list != NULL)
                     + (detached != NULL)
                     + (int)added_list
                     + (int)added_str_to_array
                     + (int)added_num_to_array
                     + (int)added_temp;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}