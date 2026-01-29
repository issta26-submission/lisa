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
//<ID> 1994
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "info", info);
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(info, "name", name);
    cJSON *flag = cJSON_AddFalseToObject(root, "enabled");
    cJSON *null_item = cJSON_AddNullToObject(root, "nothing");

    // step 2: Configure
    const char *ver = cJSON_Version();
    cJSON_AddStringToObject(root, "version", ver);
    char *newval = cJSON_SetValuestring(name, "configured");

    // step 3: Operate and Validate
    char *label = cJSON_GetStringValue(name);
    cJSON_bool is_false = cJSON_IsFalse(flag);
    cJSON_bool is_null = cJSON_IsNull(null_item);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    buffer[0] = label ? label[0] : 'x';
    buffer[1] = (char)('0' + (int)is_false);
    buffer[2] = (char)('0' + (int)is_null);
    buffer[3] = ver ? ver[0] : 'v';
    buffer[4] = (char)('0' + (int)printed_ok);
    buffer[5] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}