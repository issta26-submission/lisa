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
//<ID> 833
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *root2 = NULL;
    cJSON *name = NULL;
    cJSON *age = NULL;
    cJSON *name2 = NULL;
    cJSON *age2 = NULL;
    char *printed = NULL;
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool added_root_name = 0;
    cJSON_bool added_root_age = 0;
    cJSON_bool prealloc_ok = 0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name = cJSON_CreateString("Alice");
    age = cJSON_CreateNumber(30.0);
    added_root_name = cJSON_AddItemToObject(root, "name", name);
    added_root_age = cJSON_AddItemToObject(root, "age", age);

    root2 = cJSON_CreateObject();
    name2 = cJSON_CreateString("Alice");
    age2 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToObject(root2, "name", name2);
    cJSON_AddItemToObject(root2, "age", age2);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    prealloc_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    equal = cJSON_Compare(root, root2, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root2);
    cJSON_Delete(root);
    memset(buffer, 0, sizeof(buffer));
    (void)added_root_name;
    (void)added_root_age;
    (void)prealloc_ok;
    (void)equal;

    // API sequence test completed successfully
    return 66;
}