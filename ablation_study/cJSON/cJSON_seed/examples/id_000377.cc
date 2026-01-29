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
//<ID> 377
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *msg = "hello_world";
    cJSON *root = cJSON_CreateObject();
    cJSON *gstr = cJSON_CreateString(msg);
    cJSON *gdup = cJSON_Duplicate(gstr, 1);
    cJSON_AddItemToObject(root, "greeting", gstr);
    cJSON_AddItemToObject(root, "greeting_copy", gdup);
    cJSON *num = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", num);
    cJSON *maybe_missing = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe_missing", maybe_missing);

    // step 2: Configure
    double pi_val = cJSON_GetNumberValue(num);
    char *meta_buf = (char *)cJSON_malloc(64);
    memset(meta_buf, 0, 64);
    sprintf(meta_buf, "pi=%.5f", pi_val);
    cJSON *meta = cJSON_CreateString(meta_buf);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_free(meta_buf);

    // step 3: Operate & Validate
    cJSON_bool was_null = cJSON_IsNull(maybe_missing);
    char status_buf[32];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "null=%d", (int)was_null);
    cJSON *status = cJSON_CreateString(status_buf);
    cJSON_AddItemToObject(root, "status", status);
    cJSON *pi_item = cJSON_GetObjectItem(root, "pi");
    double pi_again = cJSON_GetNumberValue(pi_item);
    cJSON *pi_plus_one = cJSON_CreateNumber(pi_again + 1.0);
    cJSON_AddItemToObject(root, "pi_plus_one", pi_plus_one);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapstr = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapstr);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}