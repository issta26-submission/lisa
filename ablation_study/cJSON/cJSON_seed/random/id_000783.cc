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
//<ID> 783
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
    cJSON *num = (cJSON *)0;
    cJSON *retrieved_child = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    char *printed = (char *)0;
    double extracted_value = 0.0;
    cJSON_bool added_child = 0;
    cJSON_bool added_num = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    num = cJSON_CreateNumber(42.5);

    // step 3: Configure
    added_num = cJSON_AddItemToObject(child, "value", num);
    added_child = cJSON_AddItemToObject(root, "child", child);

    // step 4: Operate
    retrieved_child = cJSON_GetObjectItem(root, "child");
    retrieved_num = cJSON_GetObjectItemCaseSensitive(retrieved_child, "value");
    extracted_value = cJSON_GetNumberValue(retrieved_num);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (retrieved_child != (cJSON *)0) + (retrieved_num != (cJSON *)0) + (int)cJSON_IsNumber(retrieved_num) + (printed != (char *)0) + (int)(extracted_value == 42.5) + (int)cJSON_IsObject(root) + (int)added_child + (int)added_num;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}