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
//<ID> 367
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json_text = "{\"alpha\": 1, \"beta\": {\"x\": 10}}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *child = cJSON_CreateObject();
    cJSON *ptr_added = NULL;
    cJSON *ptr_childnum = NULL;
    char *printed = NULL;
    cJSON_bool add_result = 0;

    // step 2: Setup / Configure
    ptr_added = cJSON_AddNumberToObject(root, "added", 123.456);
    ptr_childnum = cJSON_AddNumberToObject(child, "childnum", 7.5);
    add_result = cJSON_AddItemToObject(root, "extra", child);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "alpha");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}