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
//<ID> 589
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{ \"user\" : \"alice\", \"age\" : 30, \"meta\" : { \"active\" : true } }";
    size_t raw_len = strlen(raw_json) + 1;
    char *mutable_json = (char *)cJSON_malloc(raw_len);
    memset(mutable_json, 0, raw_len);
    strcpy(mutable_json, raw_json);
    cJSON_Minify(mutable_json);
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(mutable_json, raw_len, &parse_end, 1);

    // step 2: Configure
    cJSON *user_item = cJSON_GetObjectItemCaseSensitive(root, "user");
    cJSON *meta_item = cJSON_GetObjectItem(root, "meta");
    cJSON *active_item = cJSON_GetObjectItem(meta_item, "active");
    char *user_str = cJSON_GetStringValue(user_item);
    cJSON *status_item = cJSON_CreateString("verified");
    cJSON_AddItemToObject(root, "status", status_item);

    // step 3: Operate
    char *buffered = cJSON_PrintBuffered(root, 128, 1);
    int prelen = 256;
    char *prebuf = (char *)cJSON_malloc(prelen);
    memset(prebuf, 0, prelen);
    cJSON_bool pre_ok = cJSON_PrintPreallocated(root, prebuf, prelen, 1);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "age");

    // step 4: Validate & Cleanup
    cJSON *status_after = cJSON_GetObjectItem(root, "status");
    char *status_str = cJSON_GetStringValue(status_after);
    (void)user_str;
    (void)status_str;
    (void)active_item;
    (void)pre_ok;
    cJSON_free(buffered);
    cJSON_free(prebuf);
    cJSON_Delete(root);
    cJSON_free(mutable_json);
    // API sequence test completed successfully
    return 66;
}