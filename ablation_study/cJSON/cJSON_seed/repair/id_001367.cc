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
//<ID> 1367
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num = cJSON_CreateNumber(123.456);
    cJSON *raw = cJSON_CreateRaw("{\"raw\":true}");
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToArray(arr, raw);
    cJSON_AddItemToObject(root, "payload", arr);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "kind", "example");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *ref = cJSON_CreateStringReference("refstr");
    cJSON_AddItemToArray(arr, ref);

    // step 3: Operate and Validate
    double num_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    char *raw_val = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 1));
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    char *buf = (char *)scratch;
    buf[0] = (char)('0' + ((int)num_val % 10));
    buf[1] = raw_val ? raw_val[0] : 0;
    buf[2] = out ? out[0] : 0;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}