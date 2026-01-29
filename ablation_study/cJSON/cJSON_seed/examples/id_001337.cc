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
//<ID> 1337
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
    cJSON *flag = cJSON_CreateFalse();
    cJSON *name = cJSON_CreateString("example");
    cJSON *version = cJSON_CreateNumber(1.23);

    // step 2: Configure
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_AddItemToObject(meta, "name", name);
    cJSON_AddItemToObject(meta, "version", version);
    cJSON *raw_static = cJSON_CreateRaw("{\"raw\":\"value\"}");
    cJSON_AddItemToObject(root, "raw", raw_static);

    // step 3: Operate & Validate
    cJSON_bool has_flag = cJSON_HasObjectItem(root, "flag");
    (void)has_flag;
    cJSON *meta_item = cJSON_GetObjectItem(root, "meta");
    cJSON *name_item = cJSON_GetObjectItem(meta_item, "name");
    char *name_value = cJSON_GetStringValue(name_item);
    cJSON *raw_from_name = cJSON_CreateRaw(name_value);
    cJSON_AddItemToObject(root, "raw_name", raw_from_name);
    int buf_len = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}