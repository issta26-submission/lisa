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
//<ID> 1365
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
    cJSON_AddItemToObject(root, "num", num);
    cJSON_AddItemToObject(root, "msg", msg);
    cJSON *enabled_item = cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON_bool is_obj = cJSON_IsObject(root);
    cJSON *isobj_item = cJSON_AddBoolToObject(root, "is_object", is_obj);

    // step 2: Configure
    cJSON_SetNumberHelper(num, 2.71828);
    double current = cJSON_GetNumberValue(num);
    const char *text = cJSON_GetStringValue(msg);
    double doubled = current * 2.0;
    cJSON *doubled_item = cJSON_CreateNumber(doubled);
    cJSON_AddItemToObject(root, "doubled", doubled_item);
    cJSON *copy_msg = cJSON_CreateString(text);
    cJSON_AddItemToObject(root, "msg_copy", copy_msg);

    // step 3: Operate & Validate
    char *printed = cJSON_PrintUnformatted(root);
    double read_back = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "doubled"));
    const char *read_msg = cJSON_GetStringValue(cJSON_GetObjectItem(root, "msg_copy"));
    cJSON *verified_num = cJSON_CreateNumber(read_back);
    cJSON_AddItemToObject(root, "verified", verified_num);
    cJSON *verified_msg = cJSON_CreateString(read_msg);
    cJSON_AddItemToObject(root, "verified_msg", verified_msg);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}