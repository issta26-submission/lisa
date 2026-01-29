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
//<ID> 1497
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *item_false = (cJSON *)0;
    cJSON *item_null = (cJSON *)0;
    cJSON *retrieved_null = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_false = 0;
    cJSON_bool added_null = 0;
    cJSON_bool is_null_flag = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    item_false = cJSON_CreateFalse();
    item_null = cJSON_CreateNull();

    // step 3: Configure
    added_false = cJSON_AddItemToObject(root, "enabled", item_false);
    added_null = cJSON_AddItemToObject(root, "optional", item_null);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    retrieved_null = cJSON_GetObjectItem(root, "optional");
    is_null_flag = cJSON_IsNull(retrieved_null);

    // step 5: Validate
    validation_score = (int)(printed != (char *)0) + (int)added_false + (int)added_null + (int)is_null_flag;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_null;
    // API sequence test completed successfully
    return 66;
}