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
//<ID> 1532
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *primary = NULL;
    cJSON *container = NULL;
    cJSON *dup_of_primary = NULL;
    cJSON *container_copy = NULL;
    cJSON *retrieved = NULL;
    double retrieved_val = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    primary = cJSON_CreateNumber(123.456);
    cJSON_AddItemToObject(root, "primary", primary);
    container = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "container", container);
    cJSON_AddItemToObject(container, "inner", cJSON_CreateNumber(7.0));

    // step 3: Operate / Validate
    dup_of_primary = cJSON_Duplicate(primary, 0);
    cJSON_AddItemToObject(container, "dup_of_primary", dup_of_primary);
    container_copy = cJSON_Duplicate(container, 1);
    cJSON_AddItemToObject(root, "container_copy", container_copy);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "primary");
    retrieved = cJSON_GetObjectItem(container, "dup_of_primary");
    retrieved_val = cJSON_GetNumberValue(retrieved);
    cJSON_AddItemToObject(root, "incremented", cJSON_CreateNumber(retrieved_val + 1.0));

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}