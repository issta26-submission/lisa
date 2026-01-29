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
//<ID> 305
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *enabled = cJSON_CreateTrue();
    cJSON *name = cJSON_CreateString("example");
    cJSON *dup = NULL;
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON_bool is_bool = 0;
    char *name_val = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "enabled", enabled);
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddNumberToObject(root, "version", 1.0);
    cJSON_AddStringToObject(root, "status", "active");

    // step 3: Operate / Validate
    cJSON *retr_bool = cJSON_GetObjectItem(root, "enabled");
    cJSON *retr_name = cJSON_GetObjectItem(root, "name");
    is_bool = cJSON_IsBool(retr_bool);
    name_val = cJSON_GetStringValue(retr_name);
    dup = cJSON_Duplicate(root, 1);
    printed = cJSON_PrintUnformatted(dup);
    buffer[0] = name_val ? name_val[0] : '\0';
    buffer[1] = printed ? printed[0] : '\0';
    (void)is_bool;
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}