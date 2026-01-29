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
//<ID> 1495
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
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 2: Configure
    char *prev_name = cJSON_SetValuestring(name, "updated");
    (void)prev_name;
    cJSON *count = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(root, "count", count);

    // step 3: Operate and Validate
    const char *name_str = cJSON_GetStringValue(name);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = name_str ? name_str[0] : 'x';
    scratch[1] = (char)('0' + ((int)cJSON_GetNumberValue(count) % 10));
    scratch[2] = out_len ? out[0] : ' ';
    double recorded_count = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
    (void)recorded_count;

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}