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
//<ID> 1507
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

    // step 2: Configure
    cJSON *flag = cJSON_AddFalseToObject(root, "active");
    cJSON *ref_item = cJSON_CreateString("referenced");
    cJSON_bool ref_added = cJSON_AddItemReferenceToObject(meta, "ref", ref_item);

    // step 3: Operate and Validate
    cJSON *got_flag = cJSON_GetObjectItem(root, "active");
    cJSON_bool was_false = cJSON_IsFalse(got_flag);
    double prev_count = cJSON_GetNumberValue(counter);
    double updated_count = cJSON_SetNumberHelper(counter, prev_count + (double)was_false);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    size_t buf_len = out_len + 16;
    char *scratch = (char *)cJSON_malloc(buf_len);
    memset(scratch, 0, buf_len);
    scratch[0] = (char)('0' + (int)was_false);
    scratch[1] = ref_added ? 'R' : 'r';
    scratch[2] = out_len ? out[0] : 'X';
    (void)updated_count;
    (void)scratch;

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}