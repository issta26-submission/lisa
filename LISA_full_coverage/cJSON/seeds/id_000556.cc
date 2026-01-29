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
//<ID> 556
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = " { \"name\": \"test\", \"count\": 42, \"active\": false } ";
    size_t raw_len = strlen(raw_json) + 1;
    char *minified = (char *)cJSON_malloc(raw_len);
    memcpy(minified, raw_json, raw_len);
    cJSON_Minify(minified);
    cJSON *parsed = cJSON_Parse(minified);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "parsed", parsed);

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(100.0);
    double new_number = cJSON_SetNumberHelper(num, 987.654);
    cJSON_AddItemToObject(root, "num", num);
    cJSON_AddTrueToObject(root, "enabled");
    cJSON *dup_parsed = cJSON_Duplicate(parsed, 1);
    cJSON_AddItemToObject(root, "parsed_dup", dup_parsed);

    // step 3: Operate
    char *snapshot = cJSON_Print(root);
    size_t snaplen = snapshot ? strlen(snapshot) : 0;
    size_t buf_len = snaplen + 64;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);

    // step 4: Validate & Cleanup
    double extracted = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "num"));
    (void)new_number;
    (void)extracted;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_free(minified);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}