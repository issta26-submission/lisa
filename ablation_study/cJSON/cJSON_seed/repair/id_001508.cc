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
//<ID> 1508
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
    cJSON *flag = cJSON_AddFalseToObject(root, "active");
    cJSON *ref_item = cJSON_CreateString("refdata");
    cJSON_AddItemReferenceToObject(root, "ref", ref_item);

    // step 2: Configure
    char *set_ret = cJSON_SetValuestring(name, "tester");

    // step 3: Operate and Validate
    cJSON *meta_lookup = cJSON_GetObjectItem(root, "meta");
    cJSON *name_lookup = cJSON_GetObjectItem(meta_lookup, "name");
    const char *name_str = cJSON_GetStringValue(name_lookup);
    cJSON *count_lookup = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(count_lookup);
    cJSON *flag_lookup = cJSON_GetObjectItem(root, "active");
    cJSON_bool is_false = cJSON_IsFalse(flag_lookup);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    size_t buf_len = out_len + 16;
    char *scratch = (char *)cJSON_malloc(buf_len);
    memset(scratch, 0, buf_len);
    scratch[0] = name_str ? name_str[0] : 'x';
    scratch[1] = (char)('0' + ((int)count_val % 10));
    scratch[2] = is_false ? 'F' : 'T';
    scratch[3] = set_ret ? set_ret[0] : 'z';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}