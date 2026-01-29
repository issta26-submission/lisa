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
//<ID> 832
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
    cJSON *age_item = NULL;
    cJSON *dup_root = NULL;
    cJSON *dup_name = NULL;
    char *printed = NULL;
    cJSON_bool names_equal = 0;
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("Alice");
    cJSON_AddItemToObject(root, "name", name_item);
    age_item = cJSON_CreateNumber(30.0);
    cJSON_AddItemToObject(root, "age", age_item);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    dup_root = cJSON_Duplicate(root, 1);
    dup_name = cJSON_GetObjectItem(dup_root, "name");
    names_equal = cJSON_Compare(name_item, dup_name, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    memset(buffer, 0, sizeof(buffer));

    // API sequence test completed successfully
    (void)names_equal;
    return 66;
}