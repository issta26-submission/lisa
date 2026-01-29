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
//<ID> 1998
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
    cJSON *name_item = cJSON_CreateString("initial");
    cJSON_AddItemToObject(child, "name", name_item);

    // step 2: Configure
    cJSON *flag_false = cJSON_AddFalseToObject(root, "ok");
    cJSON *maybe_null = cJSON_AddNullToObject(child, "maybe");
    const char *version = cJSON_Version();
    cJSON_AddStringToObject(root, "version", version);
    char *prev_name = cJSON_SetValuestring(name_item, "updated");

    // step 3: Operate and Validate
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    cJSON *got_child = cJSON_GetObjectItem(root, "child");
    cJSON *got_name = cJSON_GetObjectItem(got_child, "name");
    const char *name_val = cJSON_GetStringValue(got_name);
    cJSON_bool is_false = cJSON_IsFalse(flag_false);
    buffer[0] = (char)('0' + (printed_ok % 10));
    buffer[1] = (char)('0' + ((int)is_false % 10));
    buffer[2] = name_val ? name_val[0] : 'x';
    buffer[3] = version && version[0] ? version[0] : 'v';
    buffer[4] = prev_name && prev_name[0] ? prev_name[0] : 'p';
    buffer[5] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}