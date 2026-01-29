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
//<ID> 1516
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *direct = cJSON_AddStringToObject(root, "direct_str", "directly_added");
    cJSON *version_item = cJSON_AddNumberToObject(root, "version", 1.23);
    cJSON *name_str = cJSON_CreateString("initial_name");
    cJSON_AddItemToObject(root, "name", name_str);
    cJSON *flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "ok", flag);
    cJSON *arr = cJSON_AddArrayToObject(root, "items");
    cJSON *aelem1 = cJSON_CreateString("elem1");
    cJSON_AddItemToArray(arr, aelem1);
    cJSON *aelem2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, aelem2);

    // step 2: Configure
    char *set_ret = cJSON_SetValuestring(name_str, "renamed");

    // step 3: Operate and Validate
    const char *name_val = cJSON_GetStringValue(name_str);
    double version_val = cJSON_GetNumberValue(version_item);
    cJSON_bool is_ok = cJSON_IsTrue(flag);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    size_t buf_len = out_len + 16;
    char *scratch = (char *)cJSON_malloc(buf_len);
    memset(scratch, 0, buf_len);
    scratch[0] = name_val ? name_val[0] : 'x';
    scratch[1] = (char)('0' + (int)((int)version_val % 10));
    scratch[2] = is_ok ? '1' : '0';
    scratch[3] = set_ret ? set_ret[0] : 'y';
    scratch[4] = out_len ? out[0] : ' ';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}