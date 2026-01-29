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
//<ID> 1055
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *strings[] = {"one", "two", "three"};
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *item2 = (cJSON *)0;
    char *printed = (char *)0;
    const char *val0 = (const char *)0;
    const char *val1 = (const char *)0;
    const char *val2 = (const char *)0;
    cJSON_bool add_ok = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create a string array and an object root
    arr = cJSON_CreateStringArray((const char *const *)strings, 3);
    root = cJSON_CreateObject();

    // step 3: Configure - attach the string array to the root object
    add_ok = cJSON_AddItemToObject(root, "list", arr);

    // step 4: Operate - fetch array items and read their string values, produce an unformatted print and free it
    item0 = cJSON_GetArrayItem(arr, 0);
    item1 = cJSON_GetArrayItem(arr, 1);
    item2 = cJSON_GetArrayItem(arr, 2);
    val0 = cJSON_GetStringValue(item0);
    val1 = cJSON_GetStringValue(item1);
    val2 = cJSON_GetStringValue(item2);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);

    // step 5: Validate - compute a small validation score using the results
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(item0 != (cJSON *)0) + (int)(item1 != (cJSON *)0) + (int)(item2 != (cJSON *)0) + (int)(val0 != (const char *)0) + (int)(val1 != (const char *)0) + (int)(val2 != (const char *)0) + (int)(add_ok);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees the attached array and items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}