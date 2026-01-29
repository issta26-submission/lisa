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
//<ID> 1832
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = " { \"meta\" : { \"version\":\"1.0\" }, \"items\": [ {\"id\":1}, {\"id\":2}, {\"id\":3} ] } ";
    size_t raw_len = strlen(raw_json);
    char *jsonbuf = (char *)cJSON_malloc(raw_len + 1);
    memset(jsonbuf, 0, raw_len + 1);
    strcpy(jsonbuf, raw_json);
    cJSON_Minify(jsonbuf);
    cJSON *root = cJSON_Parse(jsonbuf);
    cJSON *dup_root = cJSON_Duplicate(root, 1);

    // step 2: Configure
    cJSON *extra = cJSON_AddObjectToObject(dup_root, "extra");
    cJSON *meta_in_dup = cJSON_AddObjectToObject(dup_root, "meta_copy");
    cJSON_AddNumberToObject(extra, "added_value", 99.5);
    cJSON_AddStringToObject(meta_in_dup, "copied_from", "root_meta");

    // step 3: Operate and Validate
    cJSON *items = cJSON_GetObjectItem(root, "items");
    int size_before = cJSON_GetArraySize(items);
    char *printed_before = cJSON_PrintUnformatted(root);
    cJSON_DeleteItemFromArray(items, 1);
    int size_after = cJSON_GetArraySize(items);
    char *printed_after = cJSON_PrintUnformatted(root);
    char *report = (char *)cJSON_malloc(128);
    memset(report, 0, 128);
    report[0] = printed_before ? printed_before[0] : 'B';
    report[1] = printed_after ? printed_after[0] : 'A';
    report[2] = (char)('0' + (size_before % 10));
    report[3] = (char)('0' + (size_after % 10));
    (void)report; (void)meta_in_dup; (void)extra;

    // step 4: Cleanup
    cJSON_free(printed_before);
    cJSON_free(printed_after);
    cJSON_free(report);
    cJSON_free(jsonbuf);
    cJSON_Delete(root);
    cJSON_Delete(dup_root);
    return 66;
    // API sequence test completed successfully
}