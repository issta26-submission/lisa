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
//<ID> 782
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child_obj = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *retrieved_child = (cJSON *)0;
    cJSON *retrieved_value = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_child = 0;
    cJSON_bool added_root_num = 0;
    double retrieved_number = 0.0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child_obj = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(3.14);

    // step 3: Configure
    added_child = cJSON_AddItemToObject(root, "child", child_obj);
    added_root_num = cJSON_AddItemToObject(child_obj, "value", num_item);
    (void)cJSON_AddItemToObject(root, "num_root", cJSON_CreateNumber(42.0));

    // step 4: Operate
    retrieved_child = cJSON_GetObjectItem(root, "child");
    retrieved_value = cJSON_GetObjectItemCaseSensitive(retrieved_child, "value");
    retrieved_number = cJSON_GetNumberValue(retrieved_value);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    validation_score = (retrieved_child != (cJSON *)0) + (retrieved_value != (cJSON *)0) + (printed != (char *)0) + (int)cJSON_IsNumber(retrieved_value) + (int)(retrieved_number == 3.14) + (int)added_child + (int)added_root_num;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}