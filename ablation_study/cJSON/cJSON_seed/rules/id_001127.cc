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
//<ID> 1127
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *parsed = NULL;
    cJSON *flag_item = NULL;
    cJSON *replacement = NULL;
    cJSON *false_container = NULL;
    cJSON *false_item = NULL;
    char *tmpbuf = NULL;
    const char *parse_end = NULL;
    const char *json = "{\"id\":1,\"flag\":false,\"name\":\"x\"}";
    const char *strs[2] = { "one", "two" };
    size_t json_len = sizeof("{\"id\":1,\"flag\":false,\"name\":\"x\"}") - 1;

    // step 2: Setup / Configure
    tmpbuf = (char *)cJSON_malloc(16);
    memset(tmpbuf, 0, 16);
    false_item = cJSON_CreateFalse();
    false_container = cJSON_CreateObject();
    cJSON_AddItemToObject(false_container, "placeholder", false_item);
    parsed = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);
    flag_item = cJSON_GetObjectItem(parsed, "flag");
    replacement = cJSON_CreateStringArray(strs, 2);

    // step 3: Operate / Validate
    cJSON_ReplaceItemViaPointer(parsed, flag_item, replacement);

    // step 4: Cleanup
    cJSON_free(tmpbuf);
    cJSON_Delete(false_container);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}