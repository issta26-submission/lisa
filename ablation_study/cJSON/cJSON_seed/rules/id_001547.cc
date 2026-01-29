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
//<ID> 1547
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *names = NULL;
    cJSON *s1 = NULL;
    cJSON *s2 = NULL;
    cJSON *names_ref_dup = NULL;
    cJSON *first = NULL;
    cJSON *first_copy = NULL;
    char *out = NULL;
    const char *first_val = NULL;
    int names_count = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    names = cJSON_AddArrayToObject(root, "names");
    s1 = cJSON_CreateString("Alice");
    s2 = cJSON_CreateString("Bob");
    cJSON_AddItemToArray(names, s1);
    cJSON_AddItemToArray(names, s2);
    // create a duplicate of the array for the "names_ref" property to avoid
    // sharing the same child pointer (which can lead to double-free on delete)
    names_ref_dup = cJSON_Duplicate(names, 1);
    cJSON_AddItemToObject(root, "names_ref", names_ref_dup);
    cJSON_AddTrueToObject(root, "active");

    // step 3: Operate / Validate
    names_count = cJSON_GetArraySize(names);
    first = cJSON_GetArrayItem(names, 0);
    first_val = cJSON_GetStringValue(first);
    first_copy = cJSON_CreateString(first_val);
    cJSON_AddItemToObject(root, "first_name", first_copy);
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
}