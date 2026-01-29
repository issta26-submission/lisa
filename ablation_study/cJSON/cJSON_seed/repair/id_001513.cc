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
//<ID> 1513
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

    // step 2: Configure
    cJSON *ver = cJSON_AddStringToObject(info, "version", "1.2.3");
    cJSON *count = cJSON_AddNumberToObject(info, "count", 42.0);
    cJSON *flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "enabled", flag);
    cJSON *arr = cJSON_AddArrayToObject(root, "items");
    cJSON *s = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, s);
    cJSON *n = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(arr, n);

    // step 3: Operate and Validate
    const char *name_val = cJSON_GetStringValue(s);
    double count_val = cJSON_GetNumberValue(count);
    cJSON_bool is_enabled = cJSON_IsTrue(flag);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = name_val ? name_val[0] : 'x';
    scratch[1] = (char)('0' + ((int)count_val % 10));
    scratch[2] = out_len ? out[0] : ' ';
    scratch[3] = (char)('0' + (int)is_enabled);
    scratch[4] = ver ? (ver->string ? ver->string[0] : 'v') : 'v';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}