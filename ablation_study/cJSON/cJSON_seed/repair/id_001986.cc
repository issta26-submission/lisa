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
//<ID> 1986
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("child_name");
    cJSON *count = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(child, "name", name);
    cJSON_AddItemToObject(child, "count", count);
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "ok", flag);
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", child_ref);
    cJSON *status = cJSON_CreateString("running");
    cJSON_AddItemToObject(root, "status", status);

    // step 3: Operate and Validate
    cJSON *got_child = cJSON_GetObjectItem(root, "child");
    cJSON *got_name = cJSON_GetObjectItem(got_child, "name");
    char *name_val = cJSON_GetStringValue(got_name);
    cJSON *got_count = cJSON_GetObjectItem(got_child, "count");
    double count_val = cJSON_GetNumberValue(got_count);
    cJSON_bool flag_is_false = cJSON_IsFalse(flag);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    buffer[0] = (char)('0' + (flag_is_false % 10));
    buffer[1] = (char)('0' + ((int)count_val % 10));
    buffer[2] = '\0';
    (void)name_val;
    (void)count_val;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}