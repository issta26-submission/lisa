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
//<ID> 808
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *meta = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *int_array = (cJSON *)0;
    cJSON *retrieved_meta = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    cJSON *retrieved_ints = (cJSON *)0;
    cJSON *retrieved_int_elem = (cJSON *)0;
    cJSON *retrieved_false = (cJSON *)0;
    cJSON_bool is_arr = 0;
    cJSON_bool is_false = 0;
    int validation_score = 0;
    double num_value = 0.0;
    int ints[] = {10, 20, 30, 40};

    // step 2: Initialize
    root = cJSON_CreateObject();
    meta = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(7.5);
    false_item = cJSON_CreateFalse();
    int_array = cJSON_CreateIntArray(ints, 4);

    // step 3: Configure
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "num", num_item);
    cJSON_AddItemToObject(root, "ints", int_array);
    cJSON_AddItemToObject(root, "enabled", false_item);

    // step 4: Operate
    retrieved_meta = cJSON_GetObjectItem(root, "meta");
    retrieved_num = cJSON_GetObjectItemCaseSensitive(retrieved_meta, "num");
    retrieved_ints = cJSON_GetObjectItem(root, "ints");
    retrieved_int_elem = cJSON_GetArrayItem(retrieved_ints, 2);
    retrieved_false = cJSON_GetObjectItem(root, "enabled");
    is_arr = cJSON_IsArray(retrieved_ints);
    is_false = cJSON_IsFalse(retrieved_false);
    num_value = cJSON_GetNumberValue(retrieved_num);

    // step 5: Validate
    validation_score = (retrieved_meta != (cJSON *)0) + (retrieved_num != (cJSON *)0) + (retrieved_ints != (cJSON *)0) + (retrieved_int_elem != (cJSON *)0) + (retrieved_false != (cJSON *)0) + (int)is_arr + (int)is_false + (num_value == 7.5);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}