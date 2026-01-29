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
//<ID> 1519
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
    cJSON *enabled_bool = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "enabled", enabled_bool);
    cJSON *name_str = cJSON_AddStringToObject(meta, "name", "fuzz_tester");
    cJSON *version_num = cJSON_AddNumberToObject(meta, "version", 2.71);
    cJSON *items_arr = cJSON_AddArrayToObject(root, "items");

    // step 2: Configure
    cJSON *it1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items_arr, it1);
    cJSON *it2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items_arr, it2);
    cJSON *num_elem = cJSON_CreateNumber(100.0);
    cJSON_AddItemToArray(items_arr, num_elem);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    const char *name_val = cJSON_GetStringValue(name_str);
    double version_val = cJSON_GetNumberValue(version_num);
    cJSON_bool is_enabled = cJSON_IsTrue(enabled_bool);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = name_val ? name_val[0] : 'x';
    scratch[1] = (char)('0' + ((int)version_val % 10));
    scratch[2] = (char)('0' + (int)is_enabled);
    scratch[3] = out_len ? out[0] : ' ';
    scratch[4] = (char)('A');

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}