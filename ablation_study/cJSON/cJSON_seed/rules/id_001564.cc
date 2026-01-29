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
//<ID> 1564
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
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    cJSON *first = NULL;
    cJSON *trueflag = NULL;
    char *out = NULL;
    char *dup = NULL;
    const char *first_val = NULL;
    cJSON_bool first_is_string = 0;
    size_t len = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    item0 = cJSON_CreateString("alpha");
    item1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);
    trueflag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", trueflag);

    // step 3: Operate / Validate
    first = cJSON_GetArrayItem(arr, 0);
    first_is_string = cJSON_IsString(first);
    first_val = cJSON_GetStringValue(first);
    len = strlen(first_val) + 1;
    dup = (char *)cJSON_malloc(len);
    memset(dup, 0, len);
    memcpy(dup, first_val, len);
    cJSON_AddStringToObject(root, "copied", dup);
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(dup);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}