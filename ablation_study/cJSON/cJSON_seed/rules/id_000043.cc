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
//<ID> 43
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.5);
    cJSON *s = cJSON_CreateString("hello");
    cJSON *dup = NULL;
    cJSON *first = NULL;
    char *buffer = NULL;
    int size = 0;
    cJSON_bool cmp = 0;

    // step 2: Setup
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "greeting", s);

    // step 3: Core operations / Validate
    size = cJSON_GetArraySize(arr);
    first = cJSON_GetArrayItem(arr, 0);
    dup = cJSON_Duplicate(first, 1);
    cmp = cJSON_Compare(first, dup, 1);
    cJSON_AddBoolToObject(root, "first_equals_dup", cmp);
    cJSON_AddNumberToObject(root, "count", (double)size);
    buffer = cJSON_PrintBuffered(root, 128, 1);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}