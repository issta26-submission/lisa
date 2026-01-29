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
//<ID> 597
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("tester");
    cJSON_bool add_name_ok = cJSON_AddItemToObject(meta, "name", name);
    cJSON_bool add_meta_ok = cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *answer_num = cJSON_CreateNumber(42.0);
    cJSON_bool add_answer_ok = cJSON_AddItemToObject(root, "answer", answer_num);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool has_meta = cJSON_HasObjectItem(parsed, "meta");
    cJSON *parsed_meta = cJSON_GetObjectItem(parsed, "meta");
    cJSON *parsed_name = cJSON_GetObjectItem(parsed_meta, "name");
    const char *name_str = cJSON_GetStringValue(parsed_name);
    cJSON_bool invalid_check = cJSON_IsInvalid(cJSON_GetObjectItem(parsed, "missing_key"));

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;add_meta=%d;add_name=%d;add_answer=%d;has_meta=%d;name=%s;invalid=%d",
            version ? version : "null",
            (int)add_meta_ok,
            (int)add_name_ok,
            (int)add_answer_ok,
            (int)has_meta,
            name_str ? name_str : "null",
            (int)invalid_check);
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}