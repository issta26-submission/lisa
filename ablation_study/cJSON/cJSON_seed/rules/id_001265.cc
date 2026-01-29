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
//<ID> 1265
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char raw_fragment[] = "{\"embedded\":123}";
    cJSON *root = NULL;
    cJSON *meta = NULL;
    cJSON *raw_item = NULL;
    cJSON *ex_meta = NULL;
    cJSON *ex_raw = NULL;
    char *printed = NULL;
    char buffer[256];
    cJSON_bool prealloc_result = 0;
    const char *raw_str = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    meta = cJSON_AddObjectToObject(root, "meta");
    raw_item = cJSON_CreateRaw(raw_fragment);
    cJSON_AddItemToObjectCS(meta, "raw_field", raw_item);
    cJSON_AddTrueToObject(root, "ok");

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    memset(buffer, 0, sizeof(buffer));
    prealloc_result = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    ex_meta = cJSON_GetObjectItem(root, "meta");
    ex_raw = cJSON_GetObjectItem(ex_meta, "raw_field");
    raw_str = cJSON_GetStringValue(ex_raw);
    (void)raw_str;
    (void)prealloc_result;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}