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
//<ID> 1056
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddStringToObject(root, "title", "example");
    cJSON *first = cJSON_CreateString("one");
    cJSON_AddItemToArray(items, first);
    cJSON *second = cJSON_CreateString("two");
    cJSON_AddItemToArray(items, second);

    // step 2: Configure
    cJSON_AddStringToObject(root, "status", "ok");
    cJSON *dup = cJSON_Duplicate(root, 1);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(dup);
    cJSON *got_title = cJSON_GetObjectItem(root, "title");
    const char *title_val = cJSON_GetStringValue(got_title);
    int count = cJSON_GetArraySize(items);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = title_val[0];
    ((char *)scratch)[1] = printed[0];
    ((char *)scratch)[2] = (char)('0' + (count % 10));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}