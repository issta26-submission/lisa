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
//<ID> 147
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
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON *empty = cJSON_CreateNull();
    cJSON_AddItemToObject(meta, "empty", empty);
    cJSON *payload = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "payload", payload);
    cJSON_AddNumberToObject(payload, "count", 1.0);

    // step 3: Operate & Validate
    cJSON *retrieved_meta = cJSON_GetObjectItem(root, "meta");
    cJSON_bool has_meta = cJSON_HasObjectItem(root, "meta");
    cJSON_bool meta_is_obj = cJSON_IsObject(retrieved_meta);
    cJSON *has_meta_flag = cJSON_CreateBool(has_meta);
    cJSON_AddItemToObject(root, "has_meta", has_meta_flag);
    cJSON_AddNumberToObject(payload, "meta_is_obj", (double)meta_is_obj);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}