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
//<ID> 1509
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flags = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "flags", flags);
    cJSON *created_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(flags, "created_false", created_false);
    cJSON *direct_false = cJSON_AddFalseToObject(flags, "direct_false");
    cJSON *holder = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "holder", holder);

    // step 2: Configure
    cJSON_AddItemReferenceToObject(root, "ref_to_created_false", created_false);
    cJSON_AddItemReferenceToObject(holder, "ref_to_direct_false", direct_false);

    // step 3: Operate and Validate
    cJSON *ref_item = cJSON_GetObjectItem(root, "ref_to_created_false");
    cJSON *dir_ref_item = cJSON_GetObjectItem(holder, "ref_to_direct_false");
    cJSON_bool is_ref_false = cJSON_IsFalse(ref_item);
    cJSON_bool is_dir_false = cJSON_IsFalse(dir_ref_item);
    cJSON *num1 = cJSON_CreateNumber((double)is_ref_false);
    cJSON *num2 = cJSON_CreateNumber((double)is_dir_false);
    cJSON_AddItemToObject(root, "ref_false_numeric", num1);
    cJSON_AddItemToObject(root, "dir_false_numeric", num2);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 8);
    memset(scratch, 0, out_len + 8);
    scratch[0] = (char)('0' + (int)is_ref_false);
    scratch[1] = (char)('0' + (int)is_dir_false);
    scratch[2] = out_len ? out[0] : ' ';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}