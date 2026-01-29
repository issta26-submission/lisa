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
//<ID> 295
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"name\":\"Alice\",\"active\":true,\"count\":3}";
    cJSON *root = cJSON_Parse(json);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddTrueToObject(meta, "updated");
    cJSON_AddStringToObject(meta, "note", "initial");
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    const char *orig_name = cJSON_GetStringValue(name_item);
    cJSON_AddStringToObject(root, "prev_name", orig_name);
    cJSON *new_name = cJSON_CreateString("Bob");
    cJSON_ReplaceItemViaPointer(root, name_item, new_name);

    // step 3: Operate and Validate
    cJSON *meta_item = cJSON_GetObjectItem(root, "meta");
    double flags = (double)cJSON_IsObject(meta_item) + 2.0 * (double)cJSON_IsTrue(cJSON_GetObjectItem(meta_item, "updated"));
    cJSON_AddNumberToObject(root, "flags", flags);
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON_AddNumberToObject(root, "count_doubled", count_val * 2.0);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}