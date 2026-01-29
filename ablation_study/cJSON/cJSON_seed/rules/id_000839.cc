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
//<ID> 839
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *name_item = NULL;
    cJSON *num_item = NULL;
    cJSON *dup_root = NULL;
    char *printed = NULL;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", name_item);
    num_item = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "value", num_item);

    // step 3: Operate / Validate
    dup_root = cJSON_Duplicate(root, 1);
    printed = cJSON_PrintUnformatted(dup_root);
    equal = cJSON_Compare(root, dup_root, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)equal;
    return 66;
}