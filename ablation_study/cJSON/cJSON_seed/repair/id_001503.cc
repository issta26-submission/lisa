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
//<ID> 1503
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
    cJSON *name = cJSON_CreateString("original");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON *counter = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "count", counter);
    cJSON *flag = cJSON_AddFalseToObject(root, "active");

    // step 2: Configure
    cJSON *extracted = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "extracted", extracted);
    cJSON_AddItemReferenceToObject(extracted, "count_ref", counter);
    cJSON_AddItemReferenceToObject(root, "name_ref", name);

    // step 3: Operate and Validate
    cJSON *flag_item = cJSON_GetObjectItem(root, "active");
    cJSON *name_ref = cJSON_GetObjectItem(root, "name_ref");
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    cJSON_bool is_false = cJSON_IsFalse(flag_item);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    size_t buf_len = out_len + 16;
    char *scratch = (char *)cJSON_malloc(buf_len);
    memset(scratch, 0, buf_len);
    const char *name_str = cJSON_GetStringValue(name_ref);
    double count_val = cJSON_GetNumberValue(count_item);
    scratch[0] = name_str ? name_str[0] : 'x';
    scratch[1] = (char)('0' + ((int)count_val % 10));
    scratch[2] = (char)('0' + (int)is_false);
    scratch[3] = out_len ? out[0] : ' ';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}