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
//<ID> 1523
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *parsed = NULL;
    cJSON *arr = NULL;
    cJSON *first_elem = NULL;
    cJSON *meta = NULL;
    cJSON *source = NULL;
    const char *parse_end = NULL;
    const char *source_val = NULL;
    const char *version = NULL;
    char json[] = "{\"data\":[{\"id\":1},{\"id\":2}], \"meta\": {\"source\":\"unit\"}}";
    size_t len = sizeof(json) - 1;
    char prebuf[64];
    memset(prebuf, 0, sizeof(prebuf));

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLengthOpts(json, len, &parse_end, 1);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "imported", parsed);
    arr = cJSON_GetObjectItem(parsed, "data");
    first_elem = cJSON_GetArrayItem(arr, 0);

    // step 3: Operate / Validate
    int count = cJSON_GetArraySize(arr);
    meta = cJSON_GetObjectItem(parsed, "meta");
    source = cJSON_GetObjectItem(meta, "source");
    source_val = cJSON_GetStringValue(source);
    version = cJSON_Version();
    cJSON_AddItemToObject(root, "cjson_version", cJSON_CreateStringReference(version));
    (void)count;
    (void)first_elem;
    (void)source_val;
    (void)prebuf;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}