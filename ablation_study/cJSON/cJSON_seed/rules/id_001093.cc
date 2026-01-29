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
//<ID> 1093
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *s1 = NULL;
    cJSON *s2 = NULL;
    cJSON *first = NULL;
    char *printed = NULL;
    char *got_str = NULL;
    int size = 0;
    cJSON_bool add_res1 = 0;
    cJSON_bool add_res2 = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObjectCS(root, "list", arr);
    s1 = cJSON_CreateString("alpha");
    s2 = cJSON_CreateString("beta");
    add_res1 = cJSON_AddItemReferenceToArray(arr, s1);
    add_res2 = cJSON_AddItemReferenceToArray(arr, s2);

    // step 3: Operate / Validate
    size = cJSON_GetArraySize(arr);
    first = cJSON_GetArrayItem(arr, 0);
    got_str = cJSON_GetStringValue(first);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}