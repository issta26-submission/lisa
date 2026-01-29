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
//<ID> 153
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);

    // step 2: Configure
    cJSON *name = cJSON_CreateString("example_name");
    cJSON_AddItemToObjectCS(config, "name", name);
    cJSON *greeting = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON_AddNumberToObject(config, "version", 1.0);

    // step 3: Operate & Validate
    cJSON *retrieved_name_item = cJSON_GetObjectItem(config, "name");
    const char *retrieved_name = cJSON_GetStringValue(retrieved_name_item);
    cJSON *copied_name = cJSON_CreateString(retrieved_name);
    cJSON_AddItemToObject(root, "copied_name", copied_name);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}