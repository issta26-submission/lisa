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
//<ID> 1070
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
    cJSON *child = cJSON_CreateObject();
    cJSON_AddNumberToObject(child, "value", 42.0);
    cJSON_AddStringToObject(child, "name", "child_one");

    // step 2: Configure
    cJSON *parsed = cJSON_Parse("{\"parsed\":true,\"count\":7}");
    cJSON_AddItemToArray(items, parsed);
    cJSON *s = cJSON_CreateString("hello");
    cJSON_AddItemToArray(items, s);
    cJSON_AddItemReferenceToObject(root, "child_ref", child);
    cJSON_AddStringToObject(root, "title", "cJSON Test");

    // step 3: Operate and Validate
    char *out = cJSON_Print(root);
    int count = cJSON_GetArraySize(items);
    void *scratch = cJSON_malloc(16);
    memset(scratch, 0, 16);
    ((char *)scratch)[0] = (char)('0' + (count & 0xF));
    ((char *)scratch)[1] = (char)('0' + (char)cJSON_IsObject(root));
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}