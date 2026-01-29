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
//<ID> 1498
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *false_node = (cJSON *)0;
    cJSON *null_node = (cJSON *)0;
    cJSON *found_flag = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool null_check = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    false_node = cJSON_CreateFalse();
    null_node = cJSON_CreateNull();

    // step 3: Configure
    cJSON_AddItemToObject(root, "enabled", false_node);
    cJSON_AddItemToObject(root, "missing", null_node);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    null_check = cJSON_IsNull(null_node);
    found_flag = cJSON_GetObjectItem(root, "enabled");

    // step 5: Validate
    validation_score = (int)(printed != (char *)0) + (int)null_check + (int)(found_flag != (cJSON *)0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)found_flag;
    (void)null_node;
    (void)false_node;
    // API sequence test completed successfully
    return 66;
}