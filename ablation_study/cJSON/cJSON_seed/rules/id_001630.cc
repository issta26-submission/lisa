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
//<ID> 1630
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *config = NULL;
    cJSON *shared = NULL;
    cJSON *null_item = NULL;
    char buffer[256];
    cJSON_bool has_ref = 0;
    cJSON_bool printed = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    shared = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "shared", shared);
    config = cJSON_AddObjectToObject(root, "config");
    null_item = cJSON_AddNullToObject(config, "maybe");

    // step 3: Operate / Validate
    cJSON_AddItemReferenceToObject(root, "shared_ref", shared);
    has_ref = cJSON_HasObjectItem(root, "shared_ref");
    memset(buffer, 0, sizeof(buffer));
    printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}