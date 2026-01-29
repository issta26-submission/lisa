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
//<ID> 1150
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *items = (cJSON *)0;
    cJSON *s1 = (cJSON *)0;
    cJSON *s2 = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *fetched_items = (cJSON *)0;
    cJSON *fetched_elem1 = (cJSON *)0;
    cJSON_bool is_str = 0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object and an array, attach array to root
    root = cJSON_CreateObject();
    items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 3: Configure - create string items and add them to the array; add a raw JSON member to root
    s1 = cJSON_CreateString("alpha");
    s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, s2);
    raw_item = cJSON_AddRawToObject(root, "raw", "{\"k\":123}");

    // step 4: Operate - retrieve array and second element, check its type using cJSON_IsString
    fetched_items = cJSON_GetObjectItem(root, "items");
    fetched_elem1 = cJSON_GetArrayItem(fetched_items, 1);
    is_str = cJSON_IsString(fetched_elem1);

    // step 5: Validate - serialize for round-trip use and compute a simple validation score
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);
    validation_score = (int)(fetched_items != (cJSON *)0) + (int)(fetched_elem1 != (cJSON *)0) + (int)is_str + (int)(raw_item != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}