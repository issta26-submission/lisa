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
//<ID> 1510
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *retrieved_false = (cJSON *)0;
    cJSON *retrieved_str = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_false = 0;
    cJSON_bool added_str = 0;
    cJSON_bool is_false = 0;
    const char *str_val = (const char *)0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    false_item = cJSON_CreateFalse();
    str_item = cJSON_CreateString("hello world");

    // step 3: Configure
    added_false = cJSON_AddItemToObject(root, "flag", false_item);
    added_str = cJSON_AddItemToObject(root, "greeting", str_item);

    // step 4: Operate
    retrieved_false = cJSON_GetObjectItem(root, "flag");
    retrieved_str = cJSON_GetObjectItem(root, "greeting");
    is_false = cJSON_IsFalse(retrieved_false);
    str_val = cJSON_GetStringValue(retrieved_str);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (int)(root != (cJSON *)0) + (int)(false_item != (cJSON *)0) + (int)(str_item != (cJSON *)0)
        + (int)added_false + (int)added_str + (int)(retrieved_false != (cJSON *)0) + (int)is_false
        + (int)(str_val != (const char *)0) + (int)(printed != (char *)0);

    // step 6: Cleanup
    if (printed) { cJSON_free((void *)printed); } /* harmless conditional only for safe free */
    cJSON_Delete(root);
    (void)retrieved_str;
    (void)retrieved_false;
    (void)str_val;
    (void)validation_score;
    // API sequence test completed successfully
    return 66;
}