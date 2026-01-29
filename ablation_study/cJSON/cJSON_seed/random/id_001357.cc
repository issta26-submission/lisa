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
//<ID> 1357
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *dup = (cJSON *)0;
    cJSON *dup_str = (cJSON *)0;
    cJSON *dup_num = (cJSON *)0;
    char *retrieved_str = (char *)0;
    double retrieved_num = 0.0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    num = cJSON_CreateNumber(42.5);

    // step 3: Configure
    cJSON_AddItemToObject(root, "value", num);
    str_item = cJSON_AddStringToObject(root, "desc", "The answer to everything");

    // step 4: Operate
    dup = cJSON_Duplicate(root, 1);
    dup_str = cJSON_GetObjectItem(dup, "desc");
    dup_num = cJSON_GetObjectItem(dup, "value");
    retrieved_str = cJSON_GetStringValue(dup_str);
    retrieved_num = cJSON_GetNumberValue(dup_num);

    // step 5: Validate
    validation_score = (int)(dup != (cJSON *)0) + (int)(retrieved_str != (char *)0) + (int)(retrieved_num == 42.5);

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dup);

    // API sequence test completed successfully
    (void)validation_score;
    (void)retrieved_str;
    (void)retrieved_num;
    return 66;
}