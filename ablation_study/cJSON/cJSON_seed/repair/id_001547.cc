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
//<ID> 1547
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddTrueToObject(root, "is_valid");
    cJSON_AddBoolToObject(root, "enabled", 1);
    float sample_floats[3];
    sample_floats[0] = 1.1f;
    sample_floats[1] = 2.2f;
    sample_floats[2] = 3.3f;
    cJSON *float_array = cJSON_CreateFloatArray(sample_floats, 3);
    cJSON_AddItemToObject(root, "floats", float_array);

    // step 2: Configure
    cJSON *threshold = cJSON_CreateNumber(7.5);
    cJSON_AddItemToObject(root, "threshold", threshold);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddBoolToObject(meta, "active", 0);

    // step 3: Operate and Validate
    cJSON *thr_item = cJSON_GetObjectItemCaseSensitive(root, "threshold");
    double thr_val = cJSON_GetNumberValue(thr_item);
    cJSON *thr_plus = cJSON_CreateNumber(thr_val + 2.25);
    cJSON_AddItemToObject(root, "threshold_plus", thr_plus);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = out_len ? out[0] : 'x';
    scratch[1] = (char)('0' + (int)thr_val);
    scratch[2] = (char)('#');

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}