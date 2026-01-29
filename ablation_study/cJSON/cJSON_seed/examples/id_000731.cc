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
//<ID> 731
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    const char *raw = "{\"root\":{\"items\":[{\"name\":\"alpha\",\"id\":1},{\"name\":\"beta\",\"id\":2}]}}";
    cJSON *parsed = cJSON_Parse(raw);
    cJSON *root = cJSON_GetObjectItem(parsed, "root");
    cJSON *items = cJSON_GetObjectItem(root, "items");

    // step 2: Configure
    cJSON *extra_array = cJSON_CreateArray();
    cJSON *ref_gamma = cJSON_CreateStringReference("gamma");
    cJSON *str_delta = cJSON_CreateString("delta");
    cJSON_AddItemToArray(extra_array, ref_gamma);
    cJSON_AddItemToArray(extra_array, str_delta);
    cJSON_AddItemToObject(root, "extra", extra_array);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(parsed);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *reparsed = cJSON_Parse(buffer);
    cJSON *re_root = cJSON_GetObjectItem(reparsed, "root");
    cJSON *re_extra = cJSON_GetObjectItem(re_root, "extra");
    cJSON *first_extra = cJSON_GetArrayItem(re_extra, 0);
    const char *first_val = cJSON_GetStringValue(first_extra);
    (void)first_val;
    (void)items;
    (void)version;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(reparsed);

    // API sequence test completed successfully
    return 66;
}