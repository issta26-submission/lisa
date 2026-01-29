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
//<ID> 1506
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
    cJSON *count = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(root, "count", count);
    cJSON *flag = cJSON_AddFalseToObject(root, "active");

    // step 2: Configure
    cJSON *note = cJSON_CreateString("initialized");
    cJSON_AddItemToObject(meta, "note", note);
    cJSON_bool ref_added = cJSON_AddItemReferenceToObject(root, "active_ref", flag);
    (void)ref_added;
    cJSON *label = cJSON_CreateString("payload");
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate and Validate
    cJSON *got_flag = cJSON_GetObjectItem(root, "active");
    cJSON_bool is_false_orig = cJSON_IsFalse(got_flag);
    cJSON *got_flag_ref = cJSON_GetObjectItem(root, "active_ref");
    cJSON_bool is_false_ref = cJSON_IsFalse(got_flag_ref);
    cJSON *false_count = cJSON_CreateNumber((double)(is_false_orig + is_false_ref));
    cJSON_AddItemToObject(meta, "false_count", false_count);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? strlen(out) : (size_t)0;
    size_t buf_len = out_len + 16;
    char *scratch = (char *)cJSON_malloc(buf_len);
    memset(scratch, 0, buf_len);
    scratch[0] = is_false_orig ? '1' : '0';
    scratch[1] = is_false_ref ? '1' : '0';
    scratch[2] = out_len ? out[0] : ' ';
    scratch[3] = (char)('0' + (int)false_count->valueint);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}