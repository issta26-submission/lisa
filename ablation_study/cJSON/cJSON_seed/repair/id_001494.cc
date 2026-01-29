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
//<ID> 1494
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
    cJSON *name = cJSON_CreateString("initial_name");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON *active = cJSON_AddBoolToObject(root, "active", 1);

    // step 2: Configure
    char *set_res = cJSON_SetValuestring(name, "tester_v2");
    const char *name_str = cJSON_GetStringValue(name);
    cJSON *note = cJSON_CreateString("note1");
    cJSON_AddItemToObject(meta, "note", note);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = name_str ? name_str[0] : 'x';
    scratch[1] = (char)('0' + (int)cJSON_IsTrue(active));
    scratch[2] = out_len ? out[0] : ' ';
    scratch[3] = set_res ? set_res[0] : 'y';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}