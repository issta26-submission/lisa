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
//<ID> 1635
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
    cJSON *nested = NULL;
    cJSON *ref_item = NULL;
    cJSON *null_item = NULL;
    cJSON_bool has_ref = 0;
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_AddObjectToObject(root, "child");
    nested = cJSON_CreateObject();
    cJSON_AddItemToObject(child, "nested", nested);
    ref_item = cJSON_CreateString("referenced_string");
    cJSON_AddItemReferenceToObject(root, "ref", ref_item);
    null_item = cJSON_AddNullToObject(root, "maybe");

    // step 3: Operate / Validate
    has_ref = cJSON_HasObjectItem(root, "ref");
    cJSON_AddNumberToObject(child, "has_ref", (double)has_ref);
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}