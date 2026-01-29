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
//<ID> 1362
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *msg = cJSON_CreateString("initial message");

    // step 2: Configure
    cJSON_AddItemToObject(root, "number", num);
    cJSON_AddItemToObject(root, "message", msg);
    cJSON *enabled = cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON_SetNumberHelper(num, 123.456);

    // step 3: Operate & Validate
    cJSON *num_item = cJSON_GetObjectItem(root, "number");
    cJSON *msg_item = cJSON_GetObjectItem(root, "message");
    double num_val = cJSON_GetNumberValue(num_item);
    const char *msg_val = cJSON_GetStringValue(msg_item);
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON *half = cJSON_CreateNumber(num_val / 2.0);
    cJSON_AddItemToObject(root, "half", half);
    cJSON *msg_copy = cJSON_CreateString(msg_val);
    cJSON_AddItemToObject(root, "msg_copy", msg_copy);
    cJSON *is_object_num = cJSON_CreateNumber((double)root_is_obj);
    cJSON_AddItemToObject(root, "is_object", is_object_num);
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 0);
    cJSON *printed = cJSON_CreateString(buffer);
    cJSON_AddItemToObject(root, "printed", printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    (void)enabled;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}