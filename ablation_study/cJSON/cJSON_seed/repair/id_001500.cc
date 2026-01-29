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
//<ID> 1500
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *shared_name = cJSON_CreateString("shared_name");
    cJSON_AddItemToObject(config, "name", shared_name);
    cJSON *enabled_flag = cJSON_AddFalseToObject(config, "enabled");
    cJSON_AddItemReferenceToObject(root, "name_ref", shared_name);

    // step 2: Configure
    cJSON *got_flag = cJSON_GetObjectItem(config, "enabled");
    int flag_was_false = (int)cJSON_IsFalse(got_flag);
    cJSON *flag_number = cJSON_CreateNumber((double)flag_was_false);
    cJSON_AddItemToObject(root, "flag_numeric", flag_number);

    // step 3: Operate and Validate
    cJSON *retrieved_num = cJSON_GetObjectItem(root, "flag_numeric");
    double numeric_val = cJSON_GetNumberValue(retrieved_num);
    cJSON *retrieved_name_ref = cJSON_GetObjectItem(root, "name_ref");
    const char *name_str = cJSON_GetStringValue(retrieved_name_ref);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    size_t buf_len = out_len + 16;
    char *scratch = (char *)cJSON_malloc(buf_len);
    memset(scratch, 0, buf_len);
    scratch[0] = (char)('0' + (int)numeric_val);
    scratch[1] = name_str ? name_str[0] : 'x';
    scratch[2] = out_len ? out[0] : ' ';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}