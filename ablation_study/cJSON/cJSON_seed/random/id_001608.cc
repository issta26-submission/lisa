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
//<ID> 1608
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *inner = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *dup_inner = (cJSON *)0;
    cJSON *retrieved_dup = (cJSON *)0;
    cJSON *retrieved_value = (cJSON *)0;
    cJSON_bool is_obj = 0;
    double extracted_number = 0.0;
    cJSON_bool added_value = 0;
    cJSON_bool added_orig = 0;
    cJSON_bool added_dup = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    inner = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(3.1415);

    // step 3: Configure
    added_value = cJSON_AddItemToObjectCS(inner, "value", num_item);
    dup_inner = cJSON_Duplicate(inner, 1);
    added_orig = cJSON_AddItemToObjectCS(root, "orig", inner);
    added_dup = cJSON_AddItemToObjectCS(root, "dup", dup_inner);

    // step 4: Operate
    retrieved_dup = cJSON_GetObjectItem(root, "dup");
    is_obj = cJSON_IsObject(retrieved_dup);
    retrieved_value = cJSON_GetObjectItem(retrieved_dup, "value");
    extracted_number = cJSON_GetNumberValue(retrieved_value);

    // step 5: Validate
    validation_score = (int)added_value
        + (int)added_orig
        + (int)added_dup
        + (int)(retrieved_dup != (cJSON *)0)
        + (int)is_obj
        + (int)(retrieved_value != (cJSON *)0)
        + (int)(extracted_number == 3.1415);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)extracted_number;
    (void)retrieved_value;
    (void)retrieved_dup;
    // API sequence test completed successfully
    return 66;
}