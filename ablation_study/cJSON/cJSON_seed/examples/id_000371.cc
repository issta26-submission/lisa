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
//<ID> 371
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON *greeting_dup = cJSON_Duplicate(greeting, 1);
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON_AddItemToObject(root, "greeting_copy", greeting_dup);
    cJSON *num = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", num);
    cJSON *maybe_missing = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe_missing", maybe_missing);

    // step 2: Configure
    double pi_val = cJSON_GetNumberValue(num);
    double doubled = pi_val * 2.0;
    cJSON *doubled_num = cJSON_CreateNumber(doubled);
    cJSON_AddItemToObject(root, "pi_doubled", doubled_num);

    // step 3: Operate & Validate
    cJSON_bool was_null = cJSON_IsNull(maybe_missing);
    char *meta_buf = (char *)cJSON_malloc(64);
    memset(meta_buf, 0, 64);
    sprintf(meta_buf, "doubled=%.5f|null=%d", doubled, (int)was_null);
    cJSON *meta = cJSON_CreateString(meta_buf);
    cJSON_AddItemToObject(root, "meta", meta);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapstr = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapstr);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(meta_buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}