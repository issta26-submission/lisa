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
//<ID> 1607
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *pi_item = (cJSON *)0;
    cJSON *dup_root = (cJSON *)0;
    cJSON *retrieved_pi = (cJSON *)0;
    cJSON_bool added_pi = 0;
    cJSON_bool added_child = 0;
    cJSON_bool dup_is_object = 0;
    double pi_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    pi_item = cJSON_CreateNumber(123.456);

    // step 3: Configure
    added_pi = cJSON_AddItemToObjectCS(root, "pi", pi_item);
    added_child = cJSON_AddItemToObjectCS(root, "child", child);
    dup_root = cJSON_Duplicate(root, 1);

    // step 4: Operate
    dup_is_object = cJSON_IsObject(dup_root);
    retrieved_pi = cJSON_GetObjectItem(dup_root, "pi");
    pi_value = cJSON_GetNumberValue(retrieved_pi);

    // step 5: Validate
    validation_score = (int)added_pi
        + (int)added_child
        + (int)dup_is_object
        + (int)(dup_root != (cJSON *)0)
        + (int)(pi_value > 100.0);

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dup_root);
    (void)validation_score;
    (void)retrieved_pi;
    (void)pi_value;
    // API sequence test completed successfully
    return 66;
}