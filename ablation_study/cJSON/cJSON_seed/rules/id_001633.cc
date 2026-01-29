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
//<ID> 1633
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
    cJSON *maybe_null = NULL;
    cJSON_bool has_maybe = 0;
    cJSON_bool ref_added = 0;
    cJSON_bool print_ok = 0;
    char buffer[256];

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_AddObjectToObject(root, "child");
    maybe_null = cJSON_AddNullToObject(root, "maybe");
    shared = cJSON_CreateObject();
    cJSON_AddItemToObject(shared, "val", cJSON_CreateNumber(42.0));
    ref_added = cJSON_AddItemReferenceToObject(root, "shared_ref", shared);

    // step 3: Operate / Validate
    has_maybe = cJSON_HasObjectItem(root, "maybe");
    print_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)child;
    (void)maybe_null;
    (void)has_maybe;
    (void)ref_added;
    (void)print_ok;
    (void)buffer;
    return 66;
}