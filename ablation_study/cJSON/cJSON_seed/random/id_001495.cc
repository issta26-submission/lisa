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
//<ID> 1495
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *null_node = (cJSON *)0;
    cJSON *false_node = (cJSON *)0;
    cJSON *retrieved_null = (cJSON *)0;
    cJSON *retrieved_false = (cJSON *)0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    null_node = cJSON_CreateNull();
    false_node = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToObject(root, "nil", null_node);
    cJSON_AddItemToObject(root, "flag", false_node);

    // step 4: Operate
    retrieved_null = cJSON_GetObjectItem(root, "nil");
    retrieved_false = cJSON_GetObjectItem(root, "flag");
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (int)(printed != (char *)0) + (int)cJSON_IsNull(retrieved_null) + (int)cJSON_IsFalse(retrieved_false);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_null;
    (void)retrieved_false;
    // API sequence test completed successfully
    return 66;
}