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
//<ID> 317
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *got_msg = (cJSON *)0;
    const char *msg_val = (const char *)0;
    cJSON_bool add_result = (cJSON_bool)0;
    cJSON_bool is_true = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("hello from cJSON");

    // step 3: Configure
    add_result = cJSON_AddItemToObject(root, "message", str_item);
    true_item = cJSON_AddTrueToObject(root, "success");

    // step 4: Operate
    got_msg = cJSON_GetObjectItem(root, "message");
    msg_val = cJSON_GetStringValue(got_msg);
    is_true = cJSON_IsTrue(cJSON_GetObjectItem(root, "success"));

    // step 5: Validate
    cJSON_AddItemToObject(root, "message_copy", cJSON_CreateString(msg_val ? msg_val : "null"));
    cJSON_AddItemToObject(root, "success_copy", cJSON_CreateString(is_true ? "true" : "false"));

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}