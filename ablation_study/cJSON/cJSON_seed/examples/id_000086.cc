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
//<ID> 86
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(root, "name", name);

    // step 2: Configure
    cJSON *dup_name = cJSON_Duplicate(name, 0);
    cJSON_AddItemToObject(root, "name_copy", dup_name);

    // step 3: Operate & Validate
    cJSON_bool has_name = cJSON_HasObjectItem(root, "name");
    cJSON *presence = cJSON_CreateString(has_name ? "present" : "absent");
    cJSON_AddItemToObject(root, "presence", presence);
    char *buffer = (char *)cJSON_malloc(4);
    memset(buffer, 0, 4);
    buffer[0] = (char)('0' + (char)has_name);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}