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
//<ID> 1517
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
    cJSON *name_item = cJSON_AddStringToObject(root, "name", "initial");
    cJSON *num_item = cJSON_AddNumberToObject(root, "value", 42.5);
    cJSON *bool_item = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "enabled", bool_item);
    cJSON *ref_str = cJSON_CreateString("ref_initial");
    cJSON_AddItemToArray(items, ref_str);
    cJSON *extra_str = cJSON_CreateString("extra");
    cJSON_AddItemToArray(items, extra_str);

    // step 2: Configure
    char *set_ret = cJSON_SetValuestring(name_item, "tester");

    // step 3: Operate
    const char *name_val = cJSON_GetStringValue(name_item);
    double num_val = cJSON_GetNumberValue(num_item);
    cJSON_bool enabled = cJSON_IsTrue(bool_item);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    size_t buf_len = out_len + 32;
    char *scratch = (char *)cJSON_malloc(buf_len);
    memset(scratch, 0, buf_len);
    scratch[0] = name_val ? name_val[0] : 'x';
    scratch[1] = (char)('0' + ((int)num_val % 10));
    scratch[2] = out_len ? out[0] : ' ';
    scratch[3] = (char)('0' + (int)enabled);
    scratch[4] = set_ret ? set_ret[0] : 'y';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}