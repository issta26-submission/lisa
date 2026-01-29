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
//<ID> 637
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *scratch = (char *)cJSON_malloc(512);
    memset(scratch, 0, 512);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *obj0 = cJSON_CreateObject();
    cJSON *obj1 = cJSON_CreateObject();
    cJSON *inner = cJSON_CreateObject();
    cJSON *retrieved = NULL;
    size_t printed_len = 0;
    size_t to_copy = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, obj0);
    cJSON_AddItemToArray(arr, obj1);
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON_AddItemToObject(obj0, "inner", inner);

    // step 3: Operate / Validate
    retrieved = cJSON_GetArrayItem(arr, 0);
    cJSON_AddItemToObject(retrieved, "note", cJSON_CreateString("first"));
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 511 ? printed_len : 511;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}