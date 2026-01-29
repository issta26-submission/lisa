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
//<ID> 1579
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
    cJSON *pi_item = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(child, "pi", pi_item);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 2: Configure
    cJSON *child_ref = cJSON_GetObjectItem(root, "child");
    cJSON_bool is_obj = cJSON_IsObject(child_ref);
    cJSON *pi_ref = cJSON_GetObjectItem(child_ref, "pi");
    double pi_val = cJSON_GetNumberValue(pi_ref);
    cJSON *double_pi = cJSON_CreateNumber(pi_val * 2.0);
    cJSON_AddItemToObject(root, "double_pi", double_pi);

    // step 3: Operate and Validate
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(dup, root, 1);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 16);
    memset(scratch, 0, out_len + 16);
    scratch[0] = (char)('0' + (int)is_obj);
    scratch[1] = out_len ? out[0] : ' ';
    scratch[2] = (char)('0' + (int)equal);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}