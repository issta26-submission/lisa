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
//<ID> 659
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "meta", meta);

    // step 2: Configure
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObjectCS(meta, "enabled", flag);
    cJSON *ver_major = cJSON_AddNumberToObject(meta, "version_major", 1.0);
    cJSON *ver_minor = cJSON_AddNumberToObject(meta, "version_minor", 2.0);

    // step 3: Operate and Validate
    cJSON_bool has_meta = cJSON_HasObjectItem(root, "meta");
    cJSON_AddBoolToObject(root, "has_meta", has_meta);
    cJSON *meta_copy = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToObjectCS(root, "meta_copy", meta_copy);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)printed;
    cJSON_AddStringToObject(root, "snapshot", buffer);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}