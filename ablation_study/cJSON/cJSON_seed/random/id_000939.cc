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
//<ID> 939
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "outer_array";
    const char *label_key = "label";
    const char *label_val = "compact";
    const char *inner_item_str = "hello";
    cJSON *root = (cJSON *)0;
    cJSON *outerArr = (cJSON *)0;
    cJSON *innerArr = (cJSON *)0;
    cJSON *innerStr = (cJSON *)0;
    cJSON *arrRef = (cJSON *)0;
    cJSON *addedLabel = (cJSON *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    char *json_str = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create root and nested structures
    root = cJSON_CreateObject();
    outerArr = cJSON_CreateArray();
    innerArr = cJSON_CreateArray();
    innerStr = cJSON_CreateString(inner_item_str);

    // step 3: Configure - populate inner array and create an array reference
    add_ok1 = cJSON_AddItemToArray(innerArr, innerStr);
    arrRef = cJSON_CreateArrayReference(innerArr);
    add_ok2 = cJSON_AddItemToArray(outerArr, arrRef);
    cJSON_AddItemToObject(root, arr_key, outerArr);

    // step 4: Operate - add a string to the root using required API
    addedLabel = cJSON_AddStringToObject(root, label_key, label_val);

    // step 5: Validate / Serialize - print unformatted then minify in-place and compute a score
    json_str = cJSON_PrintUnformatted(root);
    cJSON_Minify(json_str);
    validation_score = (int)(root != (cJSON *)0) + (int)add_ok1 + (int)add_ok2 + (int)(arrRef != (cJSON *)0) + (int)(addedLabel != (cJSON *)0) + (int)(json_str != (char *)0);
    (void)validation_score;

    // step 6: Cleanup
    if (json_str) { cJSON_free(json_str); }
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}