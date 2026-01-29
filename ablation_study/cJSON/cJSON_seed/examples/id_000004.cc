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
//<ID> 4
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *version_node = cJSON_AddNumberToObject(root, "version", 1.23);

    // step 2: Configure
    cJSON *greeting = cJSON_CreateString("hello world");
    cJSON_AddItemToObjectCS(config, "greeting", greeting);
    cJSON *temp = cJSON_CreateString("temporary");
    cJSON_AddItemToObjectCS(root, "temp", temp);

    // step 3: Operate / Validate
    cJSON_DeleteItemFromObject(root, "temp");
    char *out_unformatted = cJSON_PrintUnformatted(root);
    cJSON *duplicate = cJSON_Duplicate(root, 1);
    char *out_formatted = cJSON_Print(duplicate);
    cJSON_free(out_unformatted);
    cJSON_free(out_formatted);
    cJSON_Delete(duplicate);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}