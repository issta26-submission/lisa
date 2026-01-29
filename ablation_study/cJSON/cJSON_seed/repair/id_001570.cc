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
//<ID> 1570
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
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON *bool_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(child, "num", num_item);
    cJSON_AddItemToObject(child, "flag", bool_item);
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON *fetched_child = cJSON_GetObjectItem(root, "child");
    cJSON_bool is_obj = cJSON_IsObject(fetched_child);
    cJSON *fetched_num_before = cJSON_GetObjectItem(fetched_child, "num");
    cJSON *new_num = cJSON_CreateNumber(123.456);
    cJSON_ReplaceItemInObject(fetched_child, "num", new_num);

    // step 3: Operate and Validate
    cJSON *num_after = cJSON_GetObjectItem(fetched_child, "num");
    double val = cJSON_GetNumberValue(num_after);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 8);
    memset(scratch, 0, out_len + 8);
    scratch[0] = (char)('0' + (int)is_obj);
    scratch[1] = out_len ? out[0] : ' ';
    scratch[2] = (char)('0' + (int)val);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}