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
//<ID> 184
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "empty", null_item);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    size_t buf_len = printed_len + 8;
    char *buf = (char *)cJSON_malloc(buf_len);
    memset(buf, ' ', buf_len);
    size_t offset = 3;
    memcpy(buf + offset, printed, printed_len);
    buf[offset + printed_len] = '\0';
    cJSON_Minify(buf);

    // step 3: Operate and Validate
    cJSON *parsed = cJSON_Parse(buf);
    cJSON *retrieved = cJSON_GetObjectItem(parsed, "empty");
    cJSON_bool retrieved_is_null = cJSON_IsNull(retrieved);
    int summary = (root ? root->type : 0) + (retrieved ? retrieved->type : 0) + (int)retrieved_is_null;
    (void)summary;

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}