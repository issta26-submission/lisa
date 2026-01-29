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
//<ID> 1474
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_AddObjectToObject(root, "info");
    cJSON *items = cJSON_AddArrayToObject(info, "items");
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(info, "name", name);
    cJSON_AddItemToArray(items, cJSON_CreateTrue());
    cJSON_AddItemToArray(items, cJSON_CreateFalse());
    cJSON *num = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(info, "value", num);

    // step 2: Configure
    double new_val = cJSON_SetNumberHelper(num, 123.5);
    char *name_str = cJSON_GetStringValue(name);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = name_str[0];
    scratch[1] = (char)('0' + ((int)new_val % 10));
    scratch[2] = 'K';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}