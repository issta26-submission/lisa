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
//<ID> 838
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *name = NULL;
    cJSON *age = NULL;
    cJSON *dup_root = NULL;
    char *printed = NULL;
    cJSON_bool added_name = 0;
    cJSON_bool added_age = 0;
    cJSON_bool equal = 0;
    char scratch[64];
    memset(scratch, 0, sizeof(scratch));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name = cJSON_CreateString("Alice");
    added_name = cJSON_AddItemToObject(root, "name", name);
    age = cJSON_CreateNumber(30.0);
    added_age = cJSON_AddItemToObject(root, "age", age);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    dup_root = cJSON_Duplicate(root, 1);
    equal = cJSON_Compare(root, dup_root, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    memset(scratch, 0, sizeof(scratch));

    // API sequence test completed successfully
    (void)added_name;
    (void)added_age;
    (void)equal;
    return 66;
}