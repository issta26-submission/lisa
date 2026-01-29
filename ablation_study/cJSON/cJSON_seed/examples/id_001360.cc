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
//<ID> 1360
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(3.1415);
    cJSON *msg = cJSON_CreateString("hello cJSON");
    cJSON *child = cJSON_CreateObject();
    cJSON *inner = cJSON_CreateNumber(7.0);

    // step 2: Configure
    cJSON_AddItemToObject(root, "pi", num);
    cJSON_AddItemToObject(root, "msg", msg);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "inner", inner);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 3: Operate & Validate
    cJSON *num_item = cJSON_GetObjectItem(root, "pi");
    cJSON *msg_item = cJSON_GetObjectItem(root, "msg");
    cJSON *child_item = cJSON_GetObjectItem(root, "child");
    const char *s = cJSON_GetStringValue(msg_item);
    double pi_val = cJSON_GetNumberValue(num_item);
    double set_ret = cJSON_SetNumberHelper(inner, 9.25);
    double inner_val = cJSON_GetNumberValue(inner);
    cJSON_bool is_obj = cJSON_IsObject(child_item);
    (void)s;
    (void)pi_val;
    (void)set_ret;
    (void)inner_val;
    (void)is_obj;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}