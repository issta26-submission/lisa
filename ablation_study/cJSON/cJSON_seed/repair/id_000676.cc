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
//<ID> 676
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *body = cJSON_AddObjectToObject(root, "body");
    cJSON *meta = cJSON_AddObjectToObject(body, "meta");
    cJSON_AddStringToObject(meta, "name", "example");
    cJSON_AddNumberToObject(meta, "version", 1.23);

    // step 2: Configure
    cJSON *meta_copy = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToObject(root, "meta_copy", meta_copy);
    char *buf = (char *)cJSON_malloc(256);
    memset(buf, 0, 256);
    cJSON_PrintPreallocated(root, buf, 256, 1);
    cJSON_AddStringToObject(root, "snapshot", buf);

    // step 3: Operate and Validate
    cJSON *name_item = cJSON_GetObjectItem(meta, "name");
    cJSON_bool name_is_str = cJSON_IsString(name_item);
    cJSON_AddBoolToObject(meta, "name_is_string", name_is_str);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}