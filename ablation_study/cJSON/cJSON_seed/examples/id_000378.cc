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
//<ID> 378
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *txt = "hello world";
    cJSON *root = cJSON_CreateObject();
    cJSON *s = cJSON_CreateString(txt);
    cJSON_AddItemToObject(root, "greeting", s);
    cJSON *n = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(root, "value", n);
    cJSON *maybe_missing = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe_missing", maybe_missing);

    // step 2: Configure & Operate
    cJSON *dup_s = cJSON_Duplicate(s, 1);
    cJSON_AddItemToObject(root, "greeting_copy", dup_s);
    double num_val = cJSON_GetNumberValue(n);
    char *meta_buf = (char *)cJSON_malloc(96);
    memset(meta_buf, 0, 96);
    sprintf(meta_buf, "number=%.2f;is_null=%d", num_val, (int)cJSON_IsNull(maybe_missing));
    cJSON *meta = cJSON_CreateString(meta_buf);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_free(meta_buf);

    // step 3: Validate
    char *unfmt = cJSON_PrintUnformatted(root);
    cJSON *snapshot = cJSON_CreateString(unfmt);
    cJSON_AddItemToObject(root, "snapshot", snapshot);
    cJSON_free(unfmt);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}