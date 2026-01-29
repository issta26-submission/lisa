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
//<ID> 418
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *flag_enabled = cJSON_CreateBool(1);
    cJSON *flag_inner = cJSON_CreateBool(0);
    cJSON_bool added_enabled = 0;
    cJSON_bool added_meta = 0;
    cJSON_bool added_inner = 0;
    char *printed = NULL;
    cJSON *parsed = NULL;
    cJSON *retr_enabled = NULL;
    cJSON *retr_meta = NULL;
    cJSON *retr_inner = NULL;
    cJSON_bool invalid_parsed = 0;
    cJSON_bool invalid_enabled = 0;

    // step 2: Setup / Configure
    added_enabled = cJSON_AddItemToObject(root, "enabled", flag_enabled);
    added_meta = cJSON_AddItemToObject(root, "meta", meta);
    added_inner = cJSON_AddItemToObject(meta, "inner", flag_inner);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    retr_enabled = cJSON_GetObjectItem(parsed, "enabled");
    retr_meta = cJSON_GetObjectItem(parsed, "meta");
    retr_inner = cJSON_GetObjectItem(retr_meta, "inner");
    invalid_parsed = cJSON_IsInvalid(parsed);
    invalid_enabled = cJSON_IsInvalid(retr_enabled);
    buffer[0] = (char)('0' + (int)invalid_parsed);
    buffer[1] = (char)('0' + (int)invalid_enabled);
    buffer[2] = (char)('0' + (int)added_enabled);
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}