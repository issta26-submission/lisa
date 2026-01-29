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
//<ID> 801
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *raw_json = "{\"flag\":true,\"num\":7}";
    cJSON *parsed = cJSON_Parse(raw_json);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "count", 0.0);

    // step 2: Configure
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddRawToObject(root, "raw", raw_json);

    // step 3: Operate & Validate
    cJSON *retrieved = cJSON_GetObjectItemCaseSensitive(root, "parsed");
    cJSON *flag_item = cJSON_GetObjectItemCaseSensitive(retrieved, "flag");
    cJSON_bool flag_true = cJSON_IsTrue(flag_item);
    cJSON *count_item = cJSON_GetObjectItemCaseSensitive(meta, "count");
    cJSON_SetNumberHelper(count_item, (double)flag_true);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}