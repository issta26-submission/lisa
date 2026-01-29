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
//<ID> 1518
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
    cJSON_AddStringToObject(meta, "name", "tester");
    cJSON *alias = cJSON_CreateString("alias_v1");
    cJSON_AddItemToObject(meta, "alias", alias);
    cJSON *flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "active", flag);
    cJSON_AddNumberToObject(root, "count", 42.5);
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);

    // step 2: Configure
    char *set_ret = cJSON_SetValuestring(alias, "alias_v2");

    // step 3: Operate and Validate
    cJSON *got_name = cJSON_GetObjectItem(meta, "name");
    cJSON *got_alias = cJSON_GetObjectItem(meta, "alias");
    cJSON *got_count = cJSON_GetObjectItem(root, "count");
    const char *name_str = cJSON_GetStringValue(got_name);
    const char *alias_str = cJSON_GetStringValue(got_alias);
    double count_val = cJSON_GetNumberValue(got_count);
    cJSON_bool active_bool = cJSON_IsTrue(cJSON_GetObjectItem(root, "active"));
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    size_t buf_len = out_len + 16;
    char *scratch = (char *)cJSON_malloc(buf_len);
    memset(scratch, 0, buf_len);
    scratch[0] = name_str ? name_str[0] : 'x';
    scratch[1] = alias_str ? alias_str[0] : 'y';
    scratch[2] = (char)('0' + ((int)count_val % 10));
    scratch[3] = active_bool ? '1' : '0';
    scratch[4] = out_len ? out[0] : ' ';
    scratch[5] = set_ret ? set_ret[0] : 'z';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}