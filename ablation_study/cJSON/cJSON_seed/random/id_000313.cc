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
//<ID> 313
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
    cJSON *greeting = (cJSON *)0;
    cJSON *retrieved_child = (cJSON *)0;
    cJSON *retrieved_greeting = (cJSON *)0;
    cJSON *retrieved_flag = (cJSON *)0;
    const char *greeting_text = (const char *)0;
    cJSON_bool flag_true = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child_obj = cJSON_CreateObject();

    // step 3: Configure
    greeting = cJSON_CreateString("hello, world");
    cJSON_AddItemToObject(child_obj, "greeting", greeting);
    cJSON_AddItemToObject(root, "child", child_obj);
    cJSON_AddTrueToObject(root, "flag");

    // step 4: Operate
    retrieved_child = cJSON_GetObjectItem(root, "child");
    retrieved_greeting = cJSON_GetObjectItem(retrieved_child, "greeting");
    greeting_text = cJSON_GetStringValue(retrieved_greeting);
    retrieved_flag = cJSON_GetObjectItem(root, "flag");
    flag_true = cJSON_IsTrue(retrieved_flag);

    // step 5: Validate
    (void)greeting_text;
    (void)flag_true;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}