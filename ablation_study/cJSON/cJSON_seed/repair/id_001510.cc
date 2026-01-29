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
//<ID> 1510
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *item0 = cJSON_CreateString("first");
    cJSON_AddItemToArray(items, item0);
    cJSON *flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "enabled", flag);
    cJSON *num = cJSON_AddNumberToObject(root, "pi", 3.1415);
    cJSON *msg = cJSON_AddStringToObject(root, "msg", "ready");

    // step 2: Configure
    char *set_ret = cJSON_SetValuestring(item0, "first-updated");
    double num_val = cJSON_GetNumberValue(num);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    size_t buf_len = out_len + 16;
    char *scratch = (char *)cJSON_malloc(buf_len);
    memset(scratch, 0, buf_len);
    scratch[0] = cJSON_GetStringValue(msg) ? cJSON_GetStringValue(msg)[0] : 'x';
    scratch[1] = (char)('0' + ((int)num_val % 10));
    scratch[2] = out_len ? out[0] : ' ';
    scratch[3] = (char)('0' + (int)cJSON_IsTrue(flag));
    scratch[4] = set_ret ? set_ret[0] : 'z';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}