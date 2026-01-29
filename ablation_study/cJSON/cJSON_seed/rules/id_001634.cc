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
//<ID> 1634
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
    cJSON *orig_item = NULL;
    cJSON_bool has_config = 0;
    cJSON_bool printed_ok = 0;
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    config = cJSON_AddObjectToObject(root, "config");
    orig_item = cJSON_CreateString("referenced_value");
    cJSON_AddItemReferenceToObject(config, "ref", orig_item);
    cJSON_AddNullToObject(config, "maybe_null");

    // step 3: Operate / Validate
    has_config = cJSON_HasObjectItem(root, "config");
    printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}