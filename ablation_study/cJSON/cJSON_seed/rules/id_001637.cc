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
//<ID> 1637
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *shared = NULL;
    cJSON *group = NULL;
    cJSON *num = NULL;
    cJSON_bool has_maybe = 0;
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    shared = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "shared", shared);
    num = cJSON_AddNumberToObject(shared, "value", 42.0);
    group = cJSON_AddObjectToObject(root, "group");

    // step 3: Operate / Validate
    cJSON_AddItemReferenceToObject(group, "shared_ref", shared);
    cJSON_AddNullToObject(root, "maybe");
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    has_maybe = cJSON_HasObjectItem(root, "maybe");

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}