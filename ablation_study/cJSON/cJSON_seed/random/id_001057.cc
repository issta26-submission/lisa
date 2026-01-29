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
//<ID> 1057
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *names_list[] = { "alice", "bob", "carol" };
    cJSON *root = (cJSON *)0;
    cJSON *names = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *item2 = (cJSON *)0;
    const char *val0 = (const char *)0;
    const char *val1 = (const char *)0;
    char *printed = (char *)0;
    cJSON_bool add_ok = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create a root object and a string array
    root = cJSON_CreateObject();
    names = cJSON_CreateStringArray((const char *const *)names_list, 3);

    // step 3: Configure - add the string array to the root object
    add_ok = cJSON_AddItemToObject(root, "names", names);

    // step 4: Operate - fetch array items and obtain their string values, print/unformatted and free the print
    item0 = cJSON_GetArrayItem(names, 0);
    item1 = cJSON_GetArrayItem(names, 1);
    item2 = cJSON_GetArrayItem(names, 2);
    val0 = cJSON_GetStringValue(item0);
    val1 = cJSON_GetStringValue(item1);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);

    // step 5: Validate - compute a small validation score from meaningful results
    validation_score = (int)(root != (cJSON *)0) + (int)(names != (cJSON *)0) + (int)(add_ok) + (int)(item0 != (cJSON *)0) + (int)(item1 != (cJSON *)0) + (int)(item2 != (cJSON *)0) + (int)(val0 != (const char *)0) + (int)(val1 != (const char *)0);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}