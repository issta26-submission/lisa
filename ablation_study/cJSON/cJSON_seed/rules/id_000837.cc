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
//<ID> 837
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
    cJSON *name1 = NULL;
    cJSON *age1 = NULL;
    cJSON *name2 = NULL;
    cJSON *age2 = NULL;
    char *printed = NULL;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name1 = cJSON_CreateString("Alice");
    cJSON_AddItemToObject(root, "name", name1);
    age1 = cJSON_CreateNumber(30);
    cJSON_AddItemToObject(root, "age", age1);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    root2 = cJSON_CreateObject();
    name2 = cJSON_CreateString("Alice");
    cJSON_AddItemToObject(root2, "name", name2);
    age2 = cJSON_CreateNumber(30);
    cJSON_AddItemToObject(root2, "age", age2);
    equal = cJSON_Compare(root, root2, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root2);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)equal;
    return 66;
}