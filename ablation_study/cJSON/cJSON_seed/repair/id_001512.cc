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
//<ID> 1512
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "count", 42.0);
    cJSON *status = cJSON_AddStringToObject(root, "status", "ok");
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *elem = cJSON_CreateString("elem");
    cJSON_AddItemToArray(items, elem);
    cJSON *flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "enabled", flag);

    // step 2: Configure
    char *set_ret = cJSON_SetValuestring(elem, "elem_two");

    // step 3: Operate and Validate
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    const char *status_str = cJSON_GetStringValue(status);
    cJSON_bool is_enabled = cJSON_IsTrue(flag);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = (char)('0' + (int)count_val % 10);
    scratch[1] = status_str ? status_str[0] : 'x';
    scratch[2] = set_ret ? set_ret[0] : 'y';
    scratch[3] = (char)('0' + (int)is_enabled);
    scratch[4] = out_len ? out[0] : ' ';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}