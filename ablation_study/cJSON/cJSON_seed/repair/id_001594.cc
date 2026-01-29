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
//<ID> 1594
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    cJSON *greeting = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(child, "greeting", greeting);
    cJSON *base = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(child, "base", base);
    cJSON *temp_num = cJSON_CreateNumber(5.0);
    cJSON_AddItemReferenceToObject(root, "five_ref", temp_num);

    // step 2: Configure
    cJSON *child_ref = cJSON_GetObjectItemCaseSensitive(root, "child");
    cJSON *base_ref = cJSON_GetObjectItemCaseSensitive(child_ref, "base");
    double base_val = cJSON_GetNumberValue(base_ref);
    double temp_val = cJSON_GetNumberValue(temp_num);

    // step 3: Operate and Validate
    double derived_val = base_val * 2.0 + temp_val;
    cJSON *derived_item = cJSON_AddNumberToObject(root, "derived", derived_val);
    cJSON *got_derived = cJSON_GetObjectItemCaseSensitive(root, "derived");
    double got_val = cJSON_GetNumberValue(got_derived);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = (char)('0' + (int)(got_val > 0.0));
    scratch[1] = (char)('0' + (int)(base_val > 0.0));
    scratch[2] = (char)('0' + (int)(temp_val > 0.0));
    scratch[3] = (char)('0' + (int)(derived_item != NULL));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}