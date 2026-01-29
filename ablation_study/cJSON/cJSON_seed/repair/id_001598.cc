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
//<ID> 1598
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON_AddNumberToObject(settings, "threshold", 2.718);
    cJSON *limit_item = cJSON_CreateNumber(100.0);
    cJSON_AddItemToObject(settings, "limit", limit_item);
    cJSON *label = cJSON_CreateString("deviceX");
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Configure
    cJSON *shared_tag = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(root, "tag", shared_tag);
    cJSON_bool ref_added = cJSON_AddItemReferenceToObject(root, "tag_ref", shared_tag);
    (void)ref_added;

    // step 3: Operate and Validate
    cJSON *settings_ref = cJSON_GetObjectItemCaseSensitive(root, "settings");
    cJSON *threshold_item = cJSON_GetObjectItemCaseSensitive(settings_ref, "threshold");
    double thr_val = cJSON_GetNumberValue(threshold_item);
    double quad_val = thr_val * 4.0;
    cJSON_AddNumberToObject(root, "quad_threshold", quad_val);
    cJSON *tag_ref = cJSON_GetObjectItemCaseSensitive(root, "tag_ref");
    const char *tag_str = cJSON_GetStringValue(tag_ref);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 8);
    memset(scratch, 0, out_len + 8);
    scratch[0] = tag_str ? tag_str[0] : 'x';
    scratch[1] = (char)('0' + (int)(thr_val > 0.0));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}