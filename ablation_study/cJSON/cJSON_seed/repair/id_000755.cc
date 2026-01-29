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
//<ID> 755
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "info", info);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *raw = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON *nul = cJSON_CreateNull();
    cJSON_AddItemToObject(info, "raw_data", raw);
    cJSON_AddItemToObject(info, "optional", nul);
    cJSON_AddStringToObject(meta, "version", "1.2");
    cJSON_AddNumberToObject(meta, "revision", 7.0);

    // step 3: Operate and Validate
    cJSON *info_ref = cJSON_GetObjectItem(root, "info");
    cJSON *raw_ref = cJSON_GetObjectItem(info_ref, "raw_data");
    cJSON *optional_ref = cJSON_GetObjectItem(info_ref, "optional");
    char *raw_text = cJSON_GetStringValue(raw_ref);
    cJSON *raw_as_string = cJSON_CreateString(raw_text);
    cJSON_AddItemToObject(root, "raw_as_string", raw_as_string);
    cJSON *meta_ref = cJSON_GetObjectItem(root, "meta");
    cJSON *ver_ref = cJSON_GetObjectItem(meta_ref, "version");
    char *ver_text = cJSON_GetStringValue(ver_ref);
    cJSON *ver_copy = cJSON_CreateString(ver_text);
    cJSON_AddItemToObject(root, "version_copy", ver_copy);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}