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
//<ID> 1580
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *child = NULL;
    cJSON *arr = NULL;
    cJSON *str0 = NULL;
    cJSON *num1 = NULL;
    cJSON *nested = NULL;
    cJSON *ref_to_child = NULL;
    cJSON *first = NULL;
    cJSON *copied = NULL;
    char *out = NULL;
    const char *first_val = NULL;
    double num_val = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str0 = cJSON_CreateString("greeting");
    num1 = cJSON_CreateNumber(123.0);
    cJSON_AddItemToArray(arr, str0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToObject(child, "items", arr);
    cJSON_AddItemToObject(root, "child", child);
    nested = cJSON_AddObjectToObject(root, "nested");
    cJSON_AddRawToObject(root, "raw", "{\"enabled\":false}");
    ref_to_child = cJSON_Duplicate(child, 1); /* deep-copy instead of reference to avoid double-parenting */
    cJSON_AddItemToObject(nested, "child_ref", ref_to_child);

    // step 3: Operate / Validate
    first = cJSON_GetArrayItem(arr, 0);
    first_val = cJSON_GetStringValue(first);
    copied = cJSON_CreateString(first_val);
    cJSON_AddItemToObject(root, "first_copy", copied);
    num_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    (void)num_val; /* keep variable referenced to avoid unused-variable warnings */

    return 66;
}