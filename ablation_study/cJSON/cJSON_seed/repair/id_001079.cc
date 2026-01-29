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
//<ID> 1079
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *parsed = cJSON_Parse("{\"refname\":\"parsed\",\"value\":7}");
    cJSON_AddItemReferenceToObject(root, "external", parsed);

    // step 2: Configure
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, s1);
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, s2);
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(items, num);

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(items);
    cJSON *first = cJSON_GetArrayItem(items, 0);
    char *first_val = cJSON_GetStringValue(first);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = first_val[0];
    char *printed = cJSON_Print(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_DetachItemViaPointer(root, parsed);
    cJSON_Delete(parsed);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}