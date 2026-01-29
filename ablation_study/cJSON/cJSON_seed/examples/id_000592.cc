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
//<ID> 592
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("tester");
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON *meta = cJSON_CreateObject();
    cJSON *id_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(meta, "id", id_item);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *tags = cJSON_CreateArray();
    cJSON_AddItemToArray(tags, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(tags, cJSON_CreateString("beta"));
    cJSON_AddItemToObject(root, "tags", tags);
    cJSON_bool add_result = cJSON_AddItemToObject(root, "extra", cJSON_CreateString("value"));

    // step 3: Operate & Validate
    cJSON_bool has_name = cJSON_HasObjectItem(root, "name");
    cJSON *name_from_root = cJSON_GetObjectItem(root, "name");
    const char *name_str = cJSON_GetStringValue(name_from_root);
    cJSON *invalid_item = (cJSON*)cJSON_malloc(sizeof(cJSON));
    memset(invalid_item, 0, sizeof(cJSON));
    cJSON_bool is_invalid = cJSON_IsInvalid(invalid_item);
    char *snapshot = cJSON_PrintUnformatted(root);
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;has_name=%d;is_invalid=%d;name=%s;add_ok=%d",
            version ? version : "null",
            (int)has_name,
            (int)is_invalid,
            name_str ? name_str : "null",
            (int)add_result);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_free(invalid_item);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}