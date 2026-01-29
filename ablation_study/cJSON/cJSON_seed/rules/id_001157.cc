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
//<ID> 1157
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *parsed = NULL;
    cJSON *root = NULL;
    cJSON *dup_parsed = NULL;
    cJSON *added_str = NULL;
    cJSON *msg = NULL;
    const char *msg_val = NULL;
    const char *parse_end = NULL;
    const char raw_json[] = "{\n  \"array\": [ 1 , 2 , null ],\n  \"message\": \" hello world \"\n}";
    size_t raw_len = (size_t)(sizeof(raw_json) - 1);
    char *mutable_json = (char *)cJSON_malloc(raw_len + 1);
    memset(mutable_json, 0, raw_len + 1);
    memcpy(mutable_json, raw_json, raw_len);

    // step 2: Setup / Configure
    cJSON_Minify(mutable_json);
    parsed = cJSON_ParseWithLengthOpts(mutable_json, raw_len, &parse_end, 1);
    root = cJSON_CreateObject();
    added_str = cJSON_CreateString("injected_value");
    cJSON_AddItemToObject(root, "injected_key", added_str);
    cJSON_AddNullToObject(root, "explicit_null");
    dup_parsed = cJSON_Duplicate(parsed, 1);
    cJSON_AddItemToObject(root, "source_copy", dup_parsed);

    // step 3: Operate / Validate
    msg = cJSON_GetObjectItem(parsed, "message");
    msg_val = cJSON_GetStringValue(msg);
    cJSON_AddItemToObject(root, "message_copy", cJSON_CreateString(msg_val));

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    cJSON_free(mutable_json);
    // API sequence test completed successfully
    return 66;
}