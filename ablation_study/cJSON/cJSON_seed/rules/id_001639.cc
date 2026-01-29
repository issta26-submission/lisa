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
//<ID> 1639
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *child = NULL;
    cJSON *shared = NULL;
    cJSON *null_item = NULL;
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool had_ref = 0;
    cJSON_bool printed_ok = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_AddObjectToObject(root, "child");
    shared = cJSON_CreateObject();
    cJSON_AddItemToObject(child, "shared", shared);
    null_item = cJSON_AddNullToObject(shared, "maybe");
    cJSON_AddItemReferenceToObject(root, "shared_ref", shared);

    // step 3: Operate / Validate
    had_ref = cJSON_HasObjectItem(root, "shared_ref");
    printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), had_ref);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}