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
//<ID> 1514
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child_false = (cJSON *)0;
    cJSON *child_str = (cJSON *)0;
    cJSON *retrieved_false = (cJSON *)0;
    cJSON *retrieved_str = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_false = 0;
    cJSON_bool added_str = 0;
    cJSON_bool is_false = 0;
    const char *strval = (const char *)0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child_false = cJSON_CreateFalse();
    child_str = cJSON_CreateString("hello");

    // step 3: Configure
    added_false = cJSON_AddItemToObject(root, "flag", child_false);
    added_str = cJSON_AddItemToObject(root, "greeting", child_str);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    retrieved_false = cJSON_GetObjectItem(root, "flag");
    retrieved_str = cJSON_GetObjectItem(root, "greeting");
    is_false = cJSON_IsFalse(retrieved_false);
    strval = cJSON_GetStringValue(retrieved_str);

    // step 5: Validate
    validation_score = (int)(printed != (char *)0) + (int)added_false + (int)added_str + (int)(retrieved_false != (cJSON *)0) + (int)is_false + (int)(strval != (const char *)0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_str;
    (void)retrieved_false;
    (void)strval;
    // API sequence test completed successfully
    return 66;
}