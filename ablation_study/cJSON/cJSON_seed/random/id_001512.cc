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
//<ID> 1512
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
    cJSON *item_name = (cJSON *)0;
    cJSON *retrieved_false = (cJSON *)0;
    cJSON *retrieved_name = (cJSON *)0;
    char *printed = (char *)0;
    const char *name_value = (const char *)0;
    cJSON_bool added_false = 0;
    cJSON_bool added_name = 0;
    cJSON_bool is_false = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    item_false = cJSON_CreateFalse();
    item_name = cJSON_CreateString("example");

    // step 3: Configure
    added_false = cJSON_AddItemToObject(root, "flag", item_false);
    added_name = cJSON_AddItemToObject(root, "name", item_name);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    retrieved_false = cJSON_GetObjectItem(root, "flag");
    retrieved_name = cJSON_GetObjectItem(root, "name");
    name_value = cJSON_GetStringValue(retrieved_name);
    is_false = cJSON_IsFalse(retrieved_false);

    // step 5: Validate
    validation_score = (int)(printed != (char *)0) + (int)added_false + (int)added_name + (int)(retrieved_name != (cJSON *)0) + (int)(name_value != (const char *)0) + (int)is_false;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_false;
    (void)is_false;
    // API sequence test completed successfully
    return 66;
}