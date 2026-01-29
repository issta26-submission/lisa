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
//<ID> 1471
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *count_node = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(meta, "count", count_node);
    cJSON *msg_node = cJSON_CreateString("hello-world");
    cJSON_AddItemToObject(root, "message", msg_node);

    // step 2: Configure
    cJSON *flag_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(meta, "enabled", flag_true);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(meta, "disabled", flag_false);
    double updated_count = cJSON_SetNumberHelper(count_node, 202.25);

    // step 3: Operate and Validate
    const char *msg_text = cJSON_GetStringValue(msg_node);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 8);
    memset(scratch, 0, out_len + 8);
    scratch[0] = out_len ? out[0] : ' ';
    scratch[1] = msg_text ? msg_text[0] : ' ';
    scratch[2] = (char)('0' + ((int)updated_count % 10));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}