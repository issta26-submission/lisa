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
//<ID> 834
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root1 = NULL;
    cJSON *root2 = NULL;
    cJSON *name1 = NULL;
    cJSON *name2 = NULL;
    cJSON *age1 = NULL;
    cJSON *age2 = NULL;
    char *printed = NULL;
    char buffer[128];
    cJSON_bool names_equal = 0;
    cJSON_bool ages_equal = 0;
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root1 = cJSON_CreateObject();
    name1 = cJSON_CreateString("Alice");
    cJSON_AddItemToObject(root1, "name", name1);
    age1 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToObject(root1, "age", age1);

    root2 = cJSON_CreateObject();
    name2 = cJSON_CreateString("Alice");
    cJSON_AddItemToObject(root2, "name", name2);
    age2 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToObject(root2, "age", age2);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root1);
    names_equal = cJSON_Compare(name1, name2, 1);
    ages_equal = cJSON_Compare(age1, age2, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root2);
    cJSON_Delete(root1);
    memset(buffer, 0, sizeof(buffer));

    // API sequence test completed successfully
    (void)names_equal;
    (void)ages_equal;
    return 66;
}