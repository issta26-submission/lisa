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
//<ID> 1491
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON *flag = cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON *desc = cJSON_CreateString("short");
    cJSON_AddItemToObject(root, "description", desc);

    // step 2: Configure
    char *set_ret = cJSON_SetValuestring(name, "renamed");
    (void)set_ret;
    char *set_ret2 = cJSON_SetValuestring(desc, "extended description");
    (void)set_ret2;

    // step 3: Operate and Validate
    char *name_str = cJSON_GetStringValue(name);
    char *desc_str = cJSON_GetStringValue(desc);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = name_str ? name_str[0] : 'x';
    scratch[1] = desc_str ? desc_str[0] : 'y';
    scratch[2] = out_len ? out[0] : ' ';
    scratch[3] = (char)('0' + (int)cJSON_IsTrue(flag));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}