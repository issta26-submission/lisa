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
//<ID> 571
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = " { \"name\":\"tester\", \"meta\":{ \"id\": 123, \"valid\": true }, \"list\": [\"a\",\"b\"] } ";
    size_t raw_len = strlen(raw_json) + 1;
    char *mutable_json = (char *)cJSON_malloc(raw_len);
    memset(mutable_json, 0, raw_len);
    strcpy(mutable_json, raw_json);
    cJSON_Minify(mutable_json);
    cJSON *root = cJSON_Parse(mutable_json);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *meta_item = cJSON_GetObjectItem(root, "meta");
    cJSON *id_item = cJSON_GetObjectItem(meta_item, "id");
    char *name_str = cJSON_GetStringValue(name_item);
    cJSON_AddStringToObject(root, "status", "ok");
    cJSON *status_item = cJSON_GetObjectItem(root, "status");

    // step 3: Operate
    cJSON *meta_dup = cJSON_Duplicate(meta_item, 1);
    cJSON_bool equal_meta = cJSON_Compare(meta_item, meta_dup, 1);
    cJSON_bool id_invalid = cJSON_IsInvalid(id_item);
    char *status_str = cJSON_GetStringValue(status_item);

    // step 4: Validate & Cleanup
    char *printed = cJSON_Print(root);
    size_t printed_len = strlen(printed);
    size_t scratch_len = printed_len + 16;
    char *scratch = (char *)cJSON_malloc(scratch_len);
    memset(scratch, 0, scratch_len);
    strcpy(scratch, printed);
    (void)name_str;
    (void)status_str;
    (void)equal_meta;
    (void)id_invalid;
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(meta_dup);
    cJSON_Delete(root);
    cJSON_free(mutable_json);
    // API sequence test completed successfully
    return 66;
}