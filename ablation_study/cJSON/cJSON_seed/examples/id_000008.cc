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
//<ID> 8
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *child = nullptr;
    cJSON *str_item = nullptr;
    char *serialized = nullptr;

    // step 2: Setup
    child = cJSON_AddObjectToObject(root, "config");

    // step 3: Core operations (create items and add to objects)
    str_item = cJSON_CreateString("example");
    cJSON_AddItemToObjectCS(root, "name", str_item);
    cJSON_AddNumberToObject(child, "version", 1.23);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(root, "name");
    serialized = cJSON_PrintUnformatted(root);
    cJSON_free(serialized);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}