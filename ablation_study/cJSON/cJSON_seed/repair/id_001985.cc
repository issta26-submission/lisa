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
//<ID> 1985
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
    cJSON_AddItemToObject(root, "child", child);
    cJSON *name = cJSON_CreateString("device42");
    cJSON_AddItemToObject(child, "name", name);
    cJSON *count = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(child, "count", count);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(child, "active", flag);

    // step 2: Configure
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", child_ref);
    cJSON *note = cJSON_CreateString("configured");
    cJSON_AddItemToObject(root, "note", note);

    // step 3: Operate and Validate
    const char *name_val = cJSON_GetStringValue(name);
    double count_val = cJSON_GetNumberValue(count);
    cJSON_bool is_false = cJSON_IsFalse(flag);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 0);
    buffer[0] = (char)('0' + ((int)count_val % 10));
    buffer[1] = (char)('0' + (printed % 10));
    buffer[2] = (char)('0' + (is_false % 10));
    buffer[3] = '\0';
    (void)name_val;
    (void)count_val;
    (void)printed;
    (void)is_false;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}