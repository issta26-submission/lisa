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
//<ID> 1553
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
    cJSON *orig = NULL;
    cJSON *second = NULL;
    cJSON *first = NULL;
    cJSON *repl = NULL;
    cJSON *retrieved = NULL;
    cJSON *dup = NULL;
    cJSON *bufstr = NULL;
    void *buffer = NULL;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "array", arr);
    orig = cJSON_CreateString("initial");
    cJSON_AddItemToArray(arr, orig);
    second = cJSON_CreateString("second");
    cJSON_AddItemToArray(arr, second);
    cJSON_AddStringToObject(root, "label", "root_label");
    buffer = cJSON_malloc(16);
    memset(buffer, 0, 16);
    ((char *)buffer)[0] = 'b';
    ((char *)buffer)[1] = 'u';
    ((char *)buffer)[2] = 'f';
    ((char *)buffer)[3] = '0';
    ((char *)buffer)[4] = '\0';
    bufstr = cJSON_CreateString((const char *)buffer);
    cJSON_AddItemToObject(root, "from_buffer", bufstr);

    // step 3: Operate / Validate
    first = cJSON_GetArrayItem(arr, 0);
    repl = cJSON_CreateString("replaced");
    cJSON_ReplaceItemViaPointer(arr, first, repl);
    retrieved = cJSON_GetArrayItem(arr, 0);
    dup = cJSON_Duplicate(repl, 1);
    equal = cJSON_Compare(retrieved, dup, 1);
    (void)equal;

    // step 4: Cleanup
    cJSON_Delete(dup);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}