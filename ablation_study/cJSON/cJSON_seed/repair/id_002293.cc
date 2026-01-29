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
//<ID> 2293
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flags = cJSON_CreateArray();
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToArray(flags, flag_false);
    cJSON_AddItemToObject(root, "flags", flags);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "id", "device-42");
    cJSON_AddNumberToObject(meta, "version", 2.0);
    cJSON *extras = cJSON_CreateArray();
    cJSON_AddItemToArray(extras, cJSON_CreateFalse());
    cJSON_AddItemToObject(root, "extras", extras);

    // step 3: Operate
    cJSON_bool has_flags = cJSON_HasObjectItem(root, "flags");
    cJSON_AddItemToObject(meta, "has_flags", cJSON_CreateBool(has_flags));

    // step 4: Validate and Cleanup
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (has_flags ? '1' : '0');
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}