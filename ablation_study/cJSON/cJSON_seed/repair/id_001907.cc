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
//<ID> 1907
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
    cJSON *name_item = cJSON_AddStringToObject(meta, "name", "fuzzer");
    cJSON *active_item = cJSON_AddFalseToObject(meta, "active");

    // step 2: Configure
    cJSON_bool has_active = cJSON_HasObjectItem(meta, "active");
    cJSON *got_active = cJSON_GetObjectItem(meta, "active");
    cJSON_bool active_is_bool = cJSON_IsBool(got_active);
    cJSON *got_name = cJSON_GetObjectItem(meta, "name");
    char *name_val = cJSON_GetStringValue(got_name);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (has_active % 10));
    buffer[2] = (char)('0' + (active_is_bool % 10));
    buffer[3] = name_val ? name_val[0] : 'X';
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}