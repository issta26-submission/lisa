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
//<ID> 1499
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
    cJSON *counter = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(root, "count", counter);
    cJSON *flag = cJSON_AddBoolToObject(root, "active", 1);

    // step 2: Configure
    char *set_ret = cJSON_SetValuestring(name, "tester");

    // step 3: Operate and Validate
    char *name_str = cJSON_GetStringValue(name);
    double count_val = cJSON_GetNumberValue(counter);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    size_t buf_len = out_len + 32;
    char *scratch = (char *)cJSON_malloc(buf_len);
    memset(scratch, 0, buf_len);
    scratch[0] = name_str ? name_str[0] : 'x';
    scratch[1] = (char)('0' + ((int)count_val % 10));
    scratch[2] = out_len ? out[0] : ' ';
    scratch[3] = (char)('0' + (int)cJSON_IsTrue(flag));
    scratch[4] = set_ret ? set_ret[0] : 'y';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}