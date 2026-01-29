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
//<ID> 1632
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
    cJSON *shared_string = NULL;
    cJSON *inner_obj = NULL;
    cJSON *null_item = NULL;
    cJSON_bool has_child = 0;
    cJSON_bool printed_ok = 0;
    char buffer[256];
    int buflen = (int)sizeof(buffer);

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_AddObjectToObject(root, "child");
    inner_obj = cJSON_AddObjectToObject(child, "inner_object");
    shared_string = cJSON_CreateString("shared_value");
    cJSON_AddItemToObject(inner_obj, "shared", shared_string);
    cJSON_AddItemReferenceToObject(root, "child_ref", child);
    null_item = cJSON_AddNullToObject(root, "maybe");

    // step 3: Operate / Validate
    has_child = cJSON_HasObjectItem(root, "child");
    memset(buffer, 0, sizeof(buffer));
    printed_ok = cJSON_PrintPreallocated(root, buffer, buflen, 1);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}